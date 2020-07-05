#include <errno.h>
#include <malloc.h>
#include <linux/mtd/partitions.h>
#include "../jz_sfc_common.h"
#include "nand_common.h"
#include <ubi_uboot.h>

#define MXIC_DEVICES_NUM         3
#define MXIC_CMD_GET_ECC	0x7c
#define THOLD	    4
#define TSETUP	    4
#define TSHSL_R	    100
#define TSHSL_W	    100

#define TRD	    70
#define TRD_14AC    25
#define TPP	    600
#define TBE	    4

static struct jz_sfcnand_device *mxic_nand;

static struct jz_sfcnand_base_param mxic_param[MXIC_DEVICES_NUM] = {
	[0] = {
	/*MX35LF1GE4AB*/
		.pagesize = 2 * 1024,
		.blocksize = 2 * 1024 * 64,
		.oobsize = 64,
		.flashsize = 2 * 1024 * 64 * 1024,

		.tHOLD  = THOLD,
		.tSETUP = TSETUP,
		.tSHSL_R = TSHSL_R,
		.tSHSL_W = TSHSL_W,

		.tRD = TRD,
		.tPP = TPP,
		.tBE = TBE,

		.plane_select = 0,
		.ecc_max = 0x4,
		.need_quad = 1,
	},
	[1] = {
	/*MX35LF2GE4AB*/
		.pagesize = 2 * 1024,
		.blocksize = 2 * 1024 * 64,
		.oobsize = 64,
		.flashsize = 2 * 1024 * 64 * 2048,

		.tHOLD  = THOLD,
		.tSETUP = TSETUP,
		.tSHSL_R = TSHSL_R,
		.tSHSL_W = TSHSL_W,

		.tRD = TRD,
		.tPP = TPP,
		.tBE = TBE,

		.plane_select = 1,
		.ecc_max = 0x4,
		.need_quad = 1,
	},
	[2] = {
	/*MX35LF2G14AC*/
		.pagesize = 2 * 1024,
		.blocksize = 2 * 1024 * 64,
		.oobsize = 64,
		.flashsize = 2 * 1024 * 64 * 2048,

		.tHOLD  = THOLD,
		.tSETUP = TSETUP,
		.tSHSL_R = TSHSL_R,
		.tSHSL_W = TSHSL_W,

		.tRD = TRD_14AC,
		.tPP = TPP,
		.tBE = TBE,

		.plane_select = 1,
		.ecc_max = 0,
		.need_quad = 1,
	},

};

static struct device_id_struct device_id[MXIC_DEVICES_NUM] = {
	DEVICE_ID_STRUCT(0x12, "MX35LF1GE4AB", &mxic_param[0]),
	DEVICE_ID_STRUCT(0x22, "MX35LF2GE4AB", &mxic_param[1]),
	DEVICE_ID_STRUCT(0x20, "MX35LF2G14AC", &mxic_param[2]),
};


static cdt_params_t *mxic_get_cdt_params(struct sfc_flash *flash, uint8_t device_id)
{
	CDT_PARAMS_INIT(mxic_nand->cdt_params);

	/* ecc status read */
	CMD_INFO(mxic_nand->cdt_params.ecc_r, MXIC_CMD_GET_ECC, 8, 0, TM_STD_SPI);

	switch(device_id) {
	    case 0x12:
	    case 0x22:
	    case 0x20:
		    break;
	    default:
		    pr_err("device_id err, please check your  device id: device_id = 0x%02x\n", device_id);
		    return NULL;
	}

	return &mxic_nand->cdt_params;
}


static int32_t get_ecc_value(struct sfc_flash *flash)
{
	struct sfc_cdt_xfer xfer;
	uint32_t buf = 0;
	int8_t count = 5;

try_read_again:
	memset(&xfer, 0, sizeof(xfer));

	/*set index*/
	xfer.cmd_index = NAND_ECC_STATUS_READ;

	/* set addr */
	xfer.columnaddr = 0;

	/* set transfer config */
	xfer.dataen = ENABLE;
	xfer.config.datalen = 1;
	xfer.config.data_dir = GLB_TRAN_DIR_READ;
	xfer.config.ops_mode = CPU_OPS;
	xfer.config.buf = (uint8_t *)&buf;

	if(sfc_sync_cdt(flash->sfc, &xfer) && count--){
		goto try_read_again;
	}

	if(count < 0)
		return -EIO;

	return buf & 0xf;
}


static inline int deal_ecc_status(struct sfc_flash *flash, uint8_t device_id, uint8_t ecc_status)
{
	int ret = 0;

	switch(device_id) {
		case 0x12:
		case 0x22:
			switch((ecc_status >> 0x4) & 0x3) {
				case 0x2:
					ret = -EBADMSG;
					break;
				case 0x1:
					if((ret = get_ecc_value(flash)) > 0x4)
						ret = -EBADMSG;
					break;
				case 0x0:
					ret = 0;
					break;
				default:
					pr_err("it is flash Unknown state, device_id: 0x%02x\n", device_id);
					ret = -EIO;
			}
			break;

		case 0x20:
			ret = 0;
			break;
		default:
			pr_err("device_id err, it maybe don`t support this device, check your device id: device_id = 0x%02x\n", device_id);
			ret = -EIO;
	}
	return ret;
}

static int mxic_nand_init(void) {

	mxic_nand = kzalloc(sizeof(*mxic_nand), GFP_KERNEL);
	if(!mxic_nand) {
		pr_err("alloc mxic_nand struct fail\n");
		return -ENOMEM;
	}

	mxic_nand->id_manufactory = 0xC2;
	mxic_nand->id_device_list = device_id;
	mxic_nand->id_device_count = MXIC_DEVICES_NUM;

	mxic_nand->ops.get_cdt_params = mxic_get_cdt_params;
	mxic_nand->ops.deal_ecc_status = deal_ecc_status;

	/* use private get feature interface, please define it in this document */
	mxic_nand->ops.get_feature = NULL;

	return jz_sfcnand_register(mxic_nand);
}

SPINAND_MOUDLE_INIT(mxic_nand_init);
