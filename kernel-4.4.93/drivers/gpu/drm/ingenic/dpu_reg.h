#ifndef _DPU_REG_H_
#define _DPU_REG_H_
#include <linux/bitops.h>
#include "ingenic_drv.h"

#define         DPU_BASE	 	0xb3050000

/*-------------------------------------------------------------------------------
 *			      DPU Register Offset
 * -----------------------------------------------------------------------------*/


/* RW	32	0x0000_0000	frame descriptor's address*/
#define DC_FRM_CFG_ADDR		        (0x0000)
/* -W	32	0x0000_0000	frame descriptor's control*/
#define DC_FRM_CFG_CTRL		        (0x0004)
/* RW	32	0x0000_0000	DC control*/
#define DC_CTRL				(0x2000)
/* RW	32	0x0000_0000	DC status*/
#define DC_ST				(0x2004)
/* -W	32	0x0000_0000	DC clear status*/
#define DC_CLR_ST			(0x2008)
/* RW	32	0x0000_0000	DC interrupt mask*/
#define DC_INTC				(0x200c)
/* R-	32	0x0000_0000	For debug*/
#define DC_INT_FLAG			(0x2010)
/* RW	32	0x0000_0000	Common configure register*/
#define DC_COM_CONFIG		        (0x2014)
/* RW	32	0x0181-4060	Priority config*/
#define DC_PCFG_RD_CTRL		        (0x2018)
/* RW	32	0x0181-4060	OFIFO Priority level threshold config*/
#define DC_OFIFO_PCFG		        (0x2020)
/* R-	32	0x0000_0000	For debug*/
#define DC_FRM_DES			(0x2100)
/* R-	32	0x0000_0000	For debug*/
#define DC_LAY0_DES			(0x2104)
/* R-	32	0x0000_0000	For debug*/
#define DC_LAY1_DES			(0x2108)
/* R-	32	0x0000_0000	For debug*/
#define DC_LAY2_DES			(0x210c)
/* R-	32	0x0000_0000	For debug*/
#define DC_LAY3_DES			(0x2110)
/* R-	32	0x0000_0000	For debug*/
#define DC_RDMA_DES			(0x2114)
/* R-	32	0x0000_0000	frame descriptor's current site*/
#define DC_FRM_CHAIN_SITE		(0x2200)
/* R-	32	0x0000_0000	rdma descriptor's current site*/
#define DC_RDMA_CHAIN_SITE		(0x2204)
/* R-	32	0x0000_0000	layer0's current site*/
#define DC_LAY0_SITE			(0x3100)
/* R-	32	0x0000_0000	layer1's current site*/
#define DC_LAY1_SITE			(0x3104)
/* R-	32	0x0000_0000	layer2's current site*/
#define DC_LAY2_SITE			(0x3108)
/* R-	32	0x0000_0000	layer3's current site*/
#define DC_LAY3_SITE			(0x310c)
/* R-	32	0x0000_0000	rdma's current site*/
#define DC_RDMA_SITE			(0x3110)
/* R-	32	0x0000_0000	wdma's current site*/
#define DC_WDMA_SITE			(0x221c)
/* RW	32	0x0000_0000	TLB Global Control*/
#define DC_TLB_GLBC			(0x3000)
/* RW	32	0x0000_0000	TLB table entry address*/
#define DC_TLB_TLBA			(0x3010)
/* RW	32	0x0000_0000	TLB trigger control*/
#define DC_TLB_TLBC			(0x3020)
/* R-	32	0x0000_0000	VPN probe 0*/
#define DC_TLB0_VPN			(0x3030)
/* R-	32	0x0000_0000	VPN probe 1*/
#define DC_TLB1_VPN			(0x3034)
/* R-	32	0x0000_0000	VPN probe 2*/
#define DC_TLB2_VPN			(0x3038)
/* R-	32	0x0000_0000	VPN probe 3*/
#define DC_TLB3_VPN			(0x303c)
/* RW	32	0x0000_0000	TLB table entry verification*/
#define DC_TLB_TLBV			(0x3040)
/* R-	32	0x0000_0000	TLB status*/
#define DC_TLB_STAT			(0x3050)
/* RW   32      0x066204a8	CscMultYRv*/
#define DC_CSC_MULT_YRV			(0x3200)
/* RW   32      0x03410190      CscMultGuGv*/
#define DC_CSC_MULT_GUGV		(0x3204)
/* RW   32      0x00000812      CscMultBu*/
#define DC_CSC_MULT_BU			(0x3208)
/* RW   32      0x00800000      CscSubYUV*/
#define DC_CSC_SUB_YUV			(0x320c)
/* RW	32	0x0000_0000	display common configure*/
#define DC_DISP_COM			(0x8000)
/* RW	32	0x0000_0000	TFT HSYNC*/
#define DC_TFT_HSYNC		        (0x9000)
/* RW	32	0x0000_0000	TFT VSYNC*/
#define DC_TFT_VSYNC		        (0x9004)
/* RW	32	0x0000_0000	TFT HDE*/
#define DC_TFT_HDE			(0x9008)
/* RW	32	0x0000_0000	TFT VDE*/
#define DC_TFT_VDE			(0x900C)
/* RW	32	0x0000_0000	TFT configure*/
#define DC_TFT_CFG			(0x9010)
/* RW	32	0x0000_0000	TFT status*/
#define DC_TFT_ST			(0x9014)
/* RW	32	0x0000_0000	SLCD configure*/
#define DC_SLCD_CFG			(0xA000)
/* RW	32	0x0000_0000	SLCD WR's duty*/
#define DC_SLCD_WR_DUTY		        (0xA004)
/* RW	32	0x0000_0000	SLCD timing*/
#define DC_SLCD_TIMING		        (0xA008)
/* RW	32	0x0000_0000	frame size*/
#define DC_SLCD_FRM_SIZE	        (0xA00C)
/* RW	32	0x0000_0000	slow time*/
#define DC_SLCD_SLOW_TIME	        (0xA010)
/* RW	32	0x0000_0000	SLCD command*/
#define DC_SLCD_CMD			(0xA014)
/* RW	32	0x0000_0000	SLCD status*/
#define DC_SLCD_ST			(0xA018)


/*-------------------------------------------------------------------------------
 *			DPU Registers Bits Field Define
 * -----------------------------------------------------------------------------*/


/* Frame descriptor control(DC_FRM_CFG_CTRL) bit field define */

/* Frame descriptor DMA start */
#define	DC_FRM_START			BIT(0)

/* Control configure(DC_CTRL) bit field define */

/* Write 1 to start TFT transfer. */
#define DC_TFT_START			BIT(6)
/* Write 1 to start SLCD transfer. */
#define DC_SLCD_START			BIT(5)
/* General stop the composer channel. Assure integrity of the current frame */
#define DC_GEN_STP_CMP			BIT(3)
/* Reset the counter of FRM_DES, LAYx_DES and RDMA_DES */
#define DC_DES_CNT_RST			BIT(2)
/* Quick stop the composer channel. */
#define DC_QCK_STP_CMP			BIT(0)

/* Status Register(DC_ST) bit field define */

/* Composer cannot meet the writeback frame rate*/
#define DC_CMP_W_SLOW			BIT(31)
/*  composer end of each frame. */
#define DC_CMP_END			BIT(23)
/* One frame display end */
#define DC_DISP_END			BIT(17)
/* layer1's block DMA end. */
#define DC_BDMA1_END			BIT(11)
/* layer0's block DMA end. */
#define DC_BDMA0_END			BIT(10)
/* all layer's block DMA end. */
#define DC_BDMA_END			BIT(9)
/* TFT jump into under-run mode. */
#define DC_TFT_UNDR			BIT(8)
/* Display channel general stop */
#define DC_STOP_DISP_ACK		BIT(6)
/* composer direct output channel is working. */
#define DC_FRM_WORKING			BIT(4)
/* one frame start reading. */
#define DC_ST_FRM_START			BIT(2)
/* one frame read end. */
#define DC_FRM_END			BIT(1)
/* display controller is working. */
#define DC_WORKING			BIT(0)

/* Clear status Register(DC_CSR) bit field define */

/* Clear CMP_W_SLOW*/
#define DC_CLR_CMP_W_SLOW		BIT(31)
/* clear CMP_END */
#define DC_CLR_CMP_END			BIT(23)
/* clear DISP_END */
#define DC_CLR_DISP_END			BIT(17)
/* clear BDMA1_END */
#define DC_CLR_BDMA1_END		BIT(11)
/* clear BDMA0_END */
#define DC_CLR_BDMA0_END		BIT(10)
/* clear BDMA_END */
#define DC_CLR_BDMA_END			BIT(9)
/* clear TFT_UNDR */
#define DC_CLR_TFT_UNDR			BIT(8)
/* clear STOP_DISP_ACK */
#define DC_CLR_STOP_DISP_ACK		BIT(6)
/* clear FRM_START. */
#define DC_CLR_FRM_START		BIT(2)
/* clear FRM_END */
#define DC_CLR_FRM_END			BIT(1)

/* INTC register(DC_INTC) bit field define */

/* Mask of CMP_W_SLOW*/
#define DC_CWS_MSK			BIT(31)
/* mask of CMP_END */
#define DC_EOC_MSK			BIT(23)
/* mask of DISP_END */
#define DC_EOD_MSK			BIT(17)
/* mask of BDMA1_END */
#define DC_EOB1_MSK			BIT(11)
/* mask of BDMA0_END */
#define DC_EOB0_MSK			BIT(10)
/* mask of BDMA_END */
#define DC_EOB_MSK			BIT(9)
/* mask of TFT_UNDR */
#define DC_UOT_MSK			BIT(8)
/* mask of STOP_DISP_ACK */
#define DC_SDA_MSK			BIT(6)
/* mask of FRM_START. */
#define DC_SOF_MSK			BIT(2)
/* mask of FRM_END */
#define DC_EOF_MSK			BIT(1)

/* DC interrupt flag(DC_INT_FLAG) */

/* Interrupt of CMP_W_SLOW*/
#define DC_INT_CWS			BIT(31)
/* Interrupt of CMP_END */
#define DC_INT_EOC			BIT(23)
/* Interrupt of DISP_END */
#define DC_INT_EOD			BIT(17)
/* Interrupt of BDMA1_END */
#define DC_INT_EOB1			BIT(11)
/* Interrupt of BDMA0_END */
#define DC_INT_EOB0			BIT(10)
/* Interrupt of BDMA_END */
#define DC_INT_EOB			BIT(9)
/* Interrupt of TFT_UNDR */
#define DC_INT_UOT			BIT(8)
/* Interrupt of STOP_DISP_ACK */
#define DC_INT_SDA			BIT(6)
/* Interrupt of FRM_START. */
#define DC_INT_SOF			BIT(2)
/* Interrupt of FRM_END */
#define DC_INT_EOF			BIT(1)

/* Common configure register(DC_COM_CONFIG) bit field define */

/* The max length of the block DMA's burst. */
#define LAYER1_CLKGATE_EN		BIT(17)
#define LAYER0_CLKGATE_EN		BIT(16)
#define DC_BURST_LEN_BDMA_LBIT		(2)
#define DC_BURST_LEN_BDMA_HBIT		(3)
#define DC_BURST_LEN_BDMA_MASK	\
	GENMASK(DC_BURST_LEN_BDMA_HBIT, DC_BURST_LEN_BDMA_LBIT)
#define DC_BURST_LEN_BDMA_4		(0) << DC_BURST_LEN_BDMA_LBIT
#define DC_BURST_LEN_BDMA_8		(1) << DC_BURST_LEN_BDMA_LBIT
#define DC_BURST_LEN_BDMA_16		(2) << DC_BURST_LEN_BDMA_LBIT
#define DC_BURST_LEN_BDMA_32		(3) << DC_BURST_LEN_BDMA_LBIT

/* keep default value */
#define	DC_OUT_SEL			BIT(1)

/* PCFG_RD_CTRL register */

/*  QoS value */
#define DC_ARQOS_VAL_LBIT		(1)
#define DC_ARQOS_VAL_HBIT		(2)
#define DC_ARQOS_VAL_MASK   \
	GENMASK(DC_ARQOS_VAL_HBIT, DC_ARQOS_VAL_LBIT)
#define DC_ARQOS_VAL_0			(0) << DC_ARQOS_VAL_LBIT
#define DC_ARQOS_VAL_1			(1) << DC_ARQOS_VAL_LBIT
#define DC_ARQOS_VAL_2			(2) << DC_ARQOS_VAL_LBIT
#define DC_ARQOS_VAL_3			(3) << DC_ARQOS_VAL_LBIT
/*  QoS ctrl */
#define DC_ARQOS_CTRL			BIT(0)

/* PCFG register(OFIFO's PCFG Register) bit field define */
#define DC_PCFG0_LBIT			(0)
#define DC_PCFG0_HBIT			(8)
#define DC_PCFG0_MASK	      \
	GENMASK(DC_PCFG0_HBIT, DC_PCFG0_LBIT)

#define DC_PCFG1_LBIT			(9)
#define DC_PCFG1_HBIT			(17)
#define DC_PCFG1_MASK	      \
	GENMASK(DC_PCFG1_HBIT, DC_PCFG1_LBIT)

#define DC_PCFG2_LBIT			(18)
#define DC_PCFG2_HBIT			(26)
#define DC_PCFG2_MASK	       \
	GENMASK(DC_PCFG2_HBIT, DC_PCFG2_LBIT)

/* CscMultYRv */

#define DC_CSC_MULT_Y_LBIT		(0)
#define DC_CSC_MULT_Y_HBIT		(10)
#define DC_CSC_MULT_Y_MASK        \
	GENMASK(DC_CSC_MULT_Y_HBIT, DC_CSC_MULT_Y_LBIT)
#define DC_CSC_MULT_Y_MD0		(0x400) << DC_CSC_MULT_Y_LBIT
#define DC_CSC_MULT_Y_MD1		(0x4a8) << DC_CSC_MULT_Y_LBIT
#define DC_CSC_MULT_Y_MD2		(0x400) << DC_CSC_MULT_Y_LBIT
#define DC_CSC_MULT_Y_MD3		(0x47c) << DC_CSC_MULT_Y_LBIT

#define DC_CSC_MULT_RV_LBIT		(16)
#define DC_CSC_MULT_RV_HBIT		(26)
#define DC_CSC_MULT_RV_MASK        \
	GENMASK(DC_CSC_MULT_RV_HBIT, DC_CSC_MULT_RV_LBIT)
#define DC_CSC_MULT_RV_MD0		(0x48f) << DC_CSC_MULT_RV_LBIT
#define DC_CSC_MULT_RV_MD1		(0x662) << DC_CSC_MULT_RV_LBIT
#define DC_CSC_MULT_RV_MD2		(0x59c) << DC_CSC_MULT_RV_LBIT
#define DC_CSC_MULT_RV_MD3		(0x57c) << DC_CSC_MULT_RV_LBIT

/* CscMultGuGv */

#define DC_CSC_MULT_GU_LBIT		(0)
#define DC_CSC_MULT_GU_HBIT		(8)
#define DC_CSC_MULT_GU_MASK        \
	GENMASK(DC_CSC_MULT_GU_HBIT, DC_CSC_MULT_GU_LBIT)
#define DC_CSC_MULT_GU_MD0		(0x193) << DC_CSC_MULT_GU_LBIT
#define DC_CSC_MULT_GU_MD1		(0x190) << DC_CSC_MULT_GU_LBIT
#define DC_CSC_MULT_GU_MD2		(0x160) << DC_CSC_MULT_GU_LBIT
#define DC_CSC_MULT_GU_MD3		(0x15a) << DC_CSC_MULT_GU_LBIT

#define DC_CSC_MULT_GV_LBIT		(16)
#define DC_CSC_MULT_GV_HBIT		(25)
#define DC_CSC_MULT_GV_MASK        \
	GENMASK(DC_CSC_MULT_GV_HBIT, DC_CSC_MULT_GV_LBIT)
#define DC_CSC_MULT_GV_MD0		(0x253) << DC_CSC_MULT_GV_LBIT
#define DC_CSC_MULT_GV_MD1		(0x341) << DC_CSC_MULT_GV_LBIT
#define DC_CSC_MULT_GV_MD2		(0x2db) << DC_CSC_MULT_GV_LBIT
#define DC_CSC_MULT_GV_MD3		(0x2cb) << DC_CSC_MULT_GV_LBIT

/* CscMultBu */

#define DC_CSC_MULT_BU_LBIT		(0)
#define DC_CSC_MULT_BU_HBIT		(11)
#define DC_CSC_MULT_BU_MASK        \
	GENMASK(DC_CSC_MULT_BU_HBIT, DC_CSC_MULT_BU_LBIT)
#define DC_CSC_MULT_BU_MD0		(0x820) << DC_CSC_MULT_BU_LBIT
#define DC_CSC_MULT_BU_MD1		(0x812) << DC_CSC_MULT_BU_LBIT
#define DC_CSC_MULT_BU_MD2		(0x717) << DC_CSC_MULT_BU_LBIT
#define DC_CSC_MULT_BU_MD3		(0x6ee) << DC_CSC_MULT_BU_LBIT

/* CscSubYUV */

#define DC_CSC_SUB_Y_LBIT		(0)
#define DC_CSC_SUB_Y_HBIT		(4)
#define DC_CSC_SUB_Y_MASK        \
	GENMASK(DC_CSC_SUB_Y_HBIT, DC_CSC_SUB_Y_LBIT)
#define DC_CSC_SUB_Y_MD0		(0x00) << DC_CSC_SUB_Y_LBIT
#define DC_CSC_SUB_Y_MD1		(0x10) << DC_CSC_SUB_Y_LBIT
#define DC_CSC_SUB_Y_MD2		(0x00) << DC_CSC_SUB_Y_LBIT
#define DC_CSC_SUB_Y_MD3		(0x10) << DC_CSC_SUB_Y_LBIT

#define DC_CSC_SUB_UV_LBIT		(16)
#define DC_CSC_SUB_UV_HBIT		(23)
#define DC_CSC_SUB_UV_MASK        \
	GENMASK(DC_CSC_SUB_UV_HBIT, DC_CSC_SUB_UV_LBIT)
#define DC_CSC_SUB_UV_MD0		(0x00) << DC_CSC_SUB_UV_LBIT
#define DC_CSC_SUB_UV_MD1		(0x80) << DC_CSC_SUB_UV_LBIT
#define DC_CSC_SUB_UV_MD2		(0x80) << DC_CSC_SUB_UV_LBIT
#define DC_CSC_SUB_UV_MD3		(0x80) << DC_CSC_SUB_UV_LBIT

/* Common(DISP_COM) bit field define */

/* Dither drop bit. */
#define DC_DP_DITHER_DW_LBIT		(16)
#define DC_DP_DITHER_DW_HBIT		(21)
#define DC_DP_DITHER_DW_MASK        \
	GENMASK(DC_DP_DITHER_DW_HBIT, DC_DP_DITHER_DW_LBIT)
#define DC_DP_DITHER_DW_BLUE_LBIT       (16)
#define DC_DP_DITHER_DW_GREEN_LBIT      (18)
#define DC_DP_DITHER_DW_RED_LBIT        (20)
#define DC_DP_DITHER_DROP_0_BITS	(0)
#define DC_DP_DITHER_DROP_2_BITS	(1)
#define DC_DP_DITHER_DROP_3_BITS	(2)
#define DC_DP_DITHER_DROP_4_BITS	(3)
#define	DC_RGB888_TO_RGB565_DITHER	(0b100110) << DC_DP_DITHER_DW_LBIT
#define	DC_RGB888_TO_RGB666_DITHER	(0b010101) << DC_DP_DITHER_DW_LBIT
/* Dither Clk gate enable */
#define DC_DITHER_CLKGATE_EN		BIT(4)
/* SLCD Clk gate enable */
#define DC_SLCD_CLKGATE_EN			BIT(3)
/* TFT Clk gate enable */
#define DC_TFT_CLKGATE_EN			BIT(2)
/* Dither enable. 1:dither enable 0:dither disable */
#define DC_DP_DITHER_EN			BIT(1)
/* Display interfaces select. 0:TFT 1:SLCD */
#define DC_DP_IF_SEL			GENMASK(1, 0)
#define	DC_DISP_COM_SLCD		BIT(1)
#define DC_DISP_COM_TFT			BIT(0)

/* TIMING_HSYNC(TFT_TIMING_HSYNC) bit field define */

/* HSYNC pulse start point(base on dot clock cycle) */
#define DC_HPS_LBIT			(16)
#define DC_HPS_HBIT			(27)
#define DC_HPS_MASK           \
	GENMASK(DC_HPS_HBIT, DC_HPS_LBIT)
/* HSYNC pulse end point(base on dot clock cycle) */
#define DC_HPE_LBIT			(0)
#define DC_HPE_HBIT			(11)
#define DC_HPE_MASK           \
	GENMASK(DC_HPE_HBIT, DC_HPE_LBIT)

/* TIMING_VSYNC(TFT_TIMING_VSYNC) bit field define */

/* VSYNC pulse start point(base on dot clock cycle) */
#define DC_VPS_LBIT			(16)
#define DC_VPS_HBIT			(27)
#define DC_VPS_MASK           \
	GENMASK(DC_VPS_HBIT, DC_VPS_LBIT)
/* VSYNC pulse end point(base on dot clock cycle) */
#define DC_VPE_LBIT			(0)
#define DC_VPE_HBIT			(11)
#define DC_VPE_MASK           \
	GENMASK(DC_VPE_HBIT, DC_VPE_LBIT)

/* TIMING_HDE(TFT_TIMING_HDE) bit field define */

/* Horizontal display area start point (base on dot clock cycle).
 * And it is also the DE pulse start point in horizontal.
 * It indicate the end point of vertical back porch.*/
#define DC_HDS_LBIT			(16)
#define DC_HDS_HBIT			(27)
#define DC_HDS_MASK              \
	GENMASK(HDE_HDS_HBIT, DC_HDS_LBIT)
/* Horizontal display area end point (base on dot clock cycle).
 * And it is also the DE pulse end point in horizontal.
 * It indicate the end point of horizontal front porch.*/
#define DC_HDE_LBIT			(0)
#define DC_HDE_HBIT			(11)
#define DC_HDE_MASK              \
	GENMASK(DC_HDE_HBIT, DC_HDE_LBIT)

/* TIMING_VDE(TFT_TIMING_VDE) bit field define */

/* Vertical display area start point(base on dot clock cycle).
 * And it is also the DE pulse start point in vertical.
 * It indicate the end point of vertical back porch. */
#define DC_VDS_LBIT			(16)
#define DC_VDS_HBIT			(27)
#define DC_VDS_MASK               \
	GENMASK(VDE_VDS_HBIT, DC_VDS_LBIT)
/*  Vertical display area end point(base on dot clock cycle).
 *  And it is also the DE pulse end point in vertical.
 *  It indicate the start point of vertical front porch. */
#define DC_VDE_LBIT			(0)
#define DC_VDE_HBIT			(11)
#define DC_VDE_MASK               \
	GENMASK(DC_VDE_HBIT, DC_VDE_LBIT)

/* TRANS_CONFIG(TFT_TRAN_CFG) bit field define */

/* Inverse the polarity of the DOT_CLK */
#define	DC_PIX_CLK_INV			BIT(10)
/* Default level of DE(invalid level) */
#define	DC_DE_DL			BIT(9)
/* Default level of HSYNC and VSYNC(invalid level) */
#define DC_SYNC_DL			BIT(8)
/* Output color mode of even lines(2,4,6...) */
#define DC_COLOR_EVEN_LBIT		(19)
#define DC_COLOR_EVEN_HBIT		(21)
#define DC_COLOR_EVEN_MASK        \
	GENMASK(DC_COLOR_EVEN_HBIT, DC_COLOR_EVEN_LBIT)
#define DC_EVEN_RGB             	(0b000) << DC_COLOR_EVEN_LBIT
#define DC_EVEN_RBG             	(0b001) << DC_COLOR_EVEN_LBIT
#define DC_EVEN_BGR             	(0b010) << DC_COLOR_EVEN_LBIT
#define DC_EVEN_BRG             	(0b011) << DC_COLOR_EVEN_LBIT
#define DC_EVEN_GBR             	(0b100) << DC_COLOR_EVEN_LBIT
#define DC_EVEN_GRB             	(0b101) << DC_COLOR_EVEN_LBIT
/* Output color mode of odd lines(1, 3, 5...) */
#define DC_COLOR_ODD_LBIT		(16)
#define DC_COLOR_ODD_HBIT		(18)
#define DC_COLOR_ODD_MASK          \
	GENMASK(DC_COLOR_ODD_HBIT, DC_COLOR_ODD_LBIT)
#define DC_ODD_RGB			(0b000) << DC_COLOR_ODD_LBIT
#define DC_ODD_RBG			(0b001) << DC_COLOR_ODD_LBIT
#define DC_ODD_BGR			(0b010) << DC_COLOR_ODD_LBIT
#define DC_ODD_BRG			(0b011) << DC_COLOR_ODD_LBIT
#define DC_ODD_GBR			(0b100) << DC_COLOR_ODD_LBIT
#define DC_ODD_GRB			(0b101) << DC_COLOR_ODD_LBIT
/* 0x:0x: 24bits parallel mode
 * 10: 8bits serial mode without dummy(RGB)
 * 11: 8bits serial mode with dummy(RGBD)*/
#define DC_MODE_LBIT			(0)
#define DC_MODE_HBIT		        (2)
#define DC_MODE_MASK               \
	GENMASK(DC_MODE_HBIT, DC_MODE_LBIT)
#define DC_MODE_PARALLEL_888		(0b000) << DC_MODE_LBIT
#define DC_MODE_PARALLEL_666		(0b001) << DC_MODE_LBIT
#define DC_MODE_PARALLEL_565		(0b010) << DC_MODE_LBIT
#define DC_MODE_SERIAL_8BIT_RGB	        (0b100) << DC_MODE_LBIT
#define DC_MODE_SERIAL_8BIT_RGBD	(0b101) << DC_MODE_LBIT

/* STATUS(TFT_ST) bit field define */

/* 0: TFT is not working 1: TFT is working */
#define	DC_TFT_WORKING			BIT(1)
/* Indicate the controller jump into recovery state,
 * and starting trans one frame 0 to panel.
 * 0: TFT is working order 1: TFT is in UNDER mode.*/
#define	DC_TFT_UNDER			BIT(0)

/* SLCD_CONFIG(SLCD_PANEL_CFG) bit field define */

/* Frame refresh mode
 * 0: single frame mode(default) 1: continuous frame mode */
#define	DC_FRM_MD			BIT(31)
/* Anti-jitter for RDY.
 * 0: sample RDY with 1 pixclk cycle(default)
 * 1: sample RDY with 3 pixclk cycles for anti-jitter.*/
#define	DC_RDY_ANTI_JIT			BIT(27)
/* Whether the pixel data or command need conversion
 * 0: no use conversion, such as parameter of command(default)
 * 1: need conversion, such as pixel data*/
#define	DC_FMT_EN			BIT(26)
/*  001: typeA(6800) 010: typeB(8080)(default) 100: typeC(SPI) */
#define DC_DBI_TYPE_LBIT		(23)
#define DC_DBI_TYPE_HBIT		(25)
#define DC_DBI_TYPE_MASK        \
	GENMASK(DC_DBI_TYPE_HBIT, DC_DBI_TYPE_LBIT)
#define	DC_DBI_TYPE_A_6800		(0b001) << DC_DBI_TYPE_LBIT
#define	DC_DBI_TYPE_B_8080		(0b010) << DC_DBI_TYPE_LBIT
#define DC_DBI_TYPE_C_SPI_3             (0b100) << DC_DBI_TYPE_LBIT
#define DC_DBI_TYPE_C_SPI_4             (0b101) << DC_DBI_TYPE_LBIT
/* Color format.
 * 00: 565  01: 666 10: 888(default) */
#define DC_DATA_FMT_LBIT		(21)
#define DC_DATA_FMT_HBIT		(22)
#define DC_DATA_FMT_MASK        \
	GENMASK(DC_DATA_FMT_HBIT, DC_DATA_FMT_LBIT)
#define DC_DATA_FMT_565			(0b00) << DC_DATA_FMT_LBIT
#define DC_DATA_FMT_444			(0b01) << DC_DATA_FMT_LBIT
#define DC_DATA_FMT_666			(0b01) << DC_DATA_FMT_LBIT
#define DC_DATA_FMT_888			(0b10) << DC_DATA_FMT_LBIT
/*  Anti-jitter for TE.
 *  0: sample TE with 1 pixclk cycle;
 *  1: sample TE with 3 pixclk cycles for anti-jitter(default).*/
#define	DC_TE_ANTI_JIT	                BIT(20)
/* The active edge of TE.
 * 0: the front edge(default) 1: the back edge*/
#define	DC_TE_MD			BIT(19)
/* 0: do not wait TE, send pix_data after SLCD_START;
 * 1: wait TE, and then send pix_data.(default)*/
#define	DC_TE_SWITCH	        	BIT(18)
/* 0: do not wait RDY, send command or data immediately;(default)
 * 1: wait RDY, and then send command or data. */
#define	DC_RDY_SWITCH			BIT(17)
/* CS control enable.
 * 0: The CS pin is controlled by GPIO.(default)
 * 1: The CS pin is controlled by Display Controller. */
#define	DC_CS_EN			BIT(16)
/* The CS's default polarity
 * 0: the default(invalid) level is low;
 * 1: the default(invalid) level is high;(default) */
#define	DC_CS_DP			BIT(11)
/* The RDY's default polarity
 * 0: the default(invalid) level is low;
 * 1: the default(invalid) level is high;(default) */
#define	DC_RDY_DP			BIT(10)
/* DC's definition.
 * 0: Command DC = low, Data DC = high(default)
 * 1: Command DC = high, Data DC = low */
#define	DC_DC_MD			BIT(9)
/* WR's default polarity.
 * 0: drive at posedge, and sample at negedge;
 * 1: drive at negedge, and sample at posedge.(default) */
#define	DC_WR_DP			BIT(8)
/* SPI_CLK Polarity.
 * 0: Active edge is Falling(default)
 * 1: Active edge is Rising Used for serial transfer mode. */
#define	DC_CLKPLY			BIT(7)
/* The TE' default polarity
 * 0: the default(invalid) level is low;(default)
 * 1: the default(invalid) level is high; */
#define	DC_TE_DP			BIT(6)
/* Panel's data width. It's also used in MIPI DSI command mod
 * 000: 8bits 001: 9bits 010: 16bits 011: 18bits 100: 24bits(default)  */
#define DC_DWIDTH_LBIT			(3)
#define DC_DWIDTH_HBIT			(5)
#define DC_DWIDTH_MASK        \
	GENMASK(DC_DWIDTH_HBIT, DC_DWIDTH_LBIT)
#define	DC_DWIDTH_8BITS			(0b000) << DC_DWIDTH_LBIT
#define	DC_DWIDTH_9BITS			(0b001) << DC_DWIDTH_LBIT
#define	DC_DWIDTH_16BITS		(0b010) << DC_DWIDTH_LBIT
#define	DC_DWIDTH_18BITS		(0b011) << DC_DWIDTH_LBIT
#define	DC_DWIDTH_24BITS		(0b100) << DC_DWIDTH_LBIT
/* The width of panel's command and it's parameters
 * 000: 8bits(default) 001: 9bits 010: 16bits 011: 18bits 100: 24bits */
#define DC_CWIDTH_LBIT			(0)
#define DC_CWIDTH_HBIT			(2)
#define DC_CWIDTH_MASK        \
	GENMASK(DC_CWIDTH_HBIT, DC_CWIDTH_LBIT)
#define	DC_CWIDTH_8BITS			(0b000) << DC_CWIDTH_LBIT
#define	DC_CWIDTH_9BITS			(0b001) << DC_CWIDTH_LBIT
#define	DC_CWIDTH_16BITS		(0b010) << DC_CWIDTH_LBIT
#define	DC_CWIDTH_18BITS		(0b011) << DC_CWIDTH_LBIT
#define	DC_CWIDTH_24BITS		(0b100) << DC_CWIDTH_LBIT

/* SLCD_WR_DUTY(SLCD_WR_DUTY) bit field define */

/* WR's high time period when transmitting data
 * (base on MIPI DBI Type B, and this is low time at Type A) */
#define DC_DSTIME_LBIT			(24)
#define DC_DSTIME_HBIT			(31)
#define DC_DSTIME_MASK        \
	GENMASK(DC_DSTIME_HBIT, DC_DSTIME_LBIT)
/* WR's low time period when transmitting data
 * (base on MIPI DBI Type B, and this is high time at Type A) */
#define DC_DDTIME_LBIT			(16)
#define DC_DDTIME_HBIT			(23)
#define DC_DDTIME_MASK        \
	GENMASK(DC_DDTIME_HBIT, DC_DDTIME_LBIT)
/* WR's high time period when transmitting command
 * (base on MIPI DBI Type B, and this is low time at Type A) */
#define DC_CSTIME_LBIT			(8)
#define DC_CSTIME_HBIT			(15)
#define DC_CSTIME_MASK        \
	GENMASK(DC_CSTIME_HBIT, DC_CSTIME_LBIT)
/* WR's low time period when transmitting command
 * (base on MIPI DBI Type B, and this is high time at Type A) */
#define DC_CDTIME_LBIT			(0)
#define DC_CDTIME_HBIT			(7)
#define DC_CDTIME_MASK        \
	GENMASK(DC_CDTIME_HBIT, DC_CDTIME_LBIT)

/* SLCD_TIMING(SLCD_TIMING) bit field define */
#define DC_TCH_LBIT			(24)
#define DC_TCH_HBIT			(31)
#define DC_TCH_MASK        \
	GENMASK(DC_TCH_HBIT, DC_TCH_LBIT)
#define DC_TCS_LBIT			(16)
#define DC_TCS_HBIT			(23)
#define DC_TCS_MASK        \
	GENMASK(DC_TCS_HBIT, DC_TCS_LBIT)
#define DC_TAH_LBIT			(8)
#define DC_TAH_HBIT			(15)
#define DC_TAH_MASK        \
	GENMASK(DC_TAH_HBIT, DC_TAH_LBIT)
#define DC_TAS_LBIT			(0)
#define DC_TAS_HBIT			(7)
#define DC_TAS_MASK        \
	GENMASK(DC_TAS_HBIT, DC_TAS_LBIT)

/* SLCD_FRM_SIZE(SLCD_FRM_SIZE) bit field define */

/* Line number of each frame. */
#define DC_SLCD_FRM_V_SIZE_LBIT		(16)
#define DC_SLCD_FRM_V_SIZE_HBIT		(31)
#define DC_SLCD_FRM_V_SIZE_MASK        \
	GENMASK(DC_SLCD_FRM_V_SIZE_HBIT, DC_SLCD_FRM_V_SIZE_LBIT)
/* Pixel number of each line. */
#define DC_SLCD_FRM_H_SIZE_LBIT		(0)
#define DC_SLCD_FRM_H_SIZE_HBIT		(15)
#define DC_SLCD_FRM_H_SIZE_MASK        \
	GENMASK(DC_SLCD_FRM_H_SIZE_HBIT, DC_SLCD_FRM_H_SIZE_LBIT)

/* SLCD_SLOW_TIME(SLCD_SLOW_TIME) bit field define */

/* SLCD will delay some period of pixclk before transmitting data,
 * after TE鈥檚 relative edge. */
#define DC_SLOW_TIME_LBIT			(0)
#define DC_SLOW_TIME_HBIT			(15)
#define DC_SLOW_TIME_MASK         \
	GENMASK(DC_SLOW_TIME_HBIT, DC_SLOW_TIME_LBIT)

/* SLCD_REG_IF(SLCD_REG_IF) bit field define */

/* It is used to decide the meanings of the DATA/CMD/PARAMETER.
 * 00: pix_data; 01: parameter; 1x: command(default). */
#define DC_SLCD_CMD_FLAG_LBIT		(30)
#define DC_SLCD_CMD_FLAG_HBIT		(31)
#define DC_SLCD_CMD_FLAG_MASK       \
	GENMASK(DC_SLCD_CMD_FLAG_HBIT, DC_SLCD_CMD_FLAG_LBIT)
#define DC_SLCD_CMD_FLAG_DATA	        (0b00) << DC_SLCD_CMD_FLAG_LBIT
#define DC_SLCD_CMD_FLAG_PRM            (0b01) << DC_SLCD_CMD_FLAG_LBIT
#define DC_SLCD_CMD_FLAG_CMD	        (0b10) << DC_SLCD_CMD_FLAG_LBIT
/* Indicate this is the last content, the hardware will disable the CS */
#define	DC_SLCD_CMD_END		        BIT(29)
/* content */
#define DC_SLCD_CMD_CONTENT_LBIT	(0)
#define DC_SLCD_CMD_CONTENT_HBIT	(23)
#define DC_SLCD_CMD_CONTENT_MASK     \
	GENMASK(DC_SLCD_CMD_CONTENT_HBIT, DC_SLCD_CMD_CONTENT_LBIT)

/* SLCD_STATUS(SLCD_ST) bit field define */

/* Indicate whether the SLCD DBI interface controller is working or not.
 * 0: idle 1: busy */
#define DC_SLCD_ST_BUSY			BIT(0)

enum {
	LAYER_CFG_FORMAT_RGB555 = 0,
	LAYER_CFG_FORMAT_ARGB1555 = 1,
	LAYER_CFG_FORMAT_RGB565	= 2,
	LAYER_CFG_FORMAT_RGB888	= 4,
	LAYER_CFG_FORMAT_ARGB8888 = 5,
	LAYER_CFG_FORMAT_NV12 = 8,
	LAYER_CFG_FORMAT_NV21 = 9,
	LAYER_CFG_FORMAT_YUV422 = 10,
	LAYER_CFG_FORMAT_TILE_H264 = 12,
};

enum {
	LAYER_CFG_COLOR_RGB = 0,
	LAYER_CFG_COLOR_RBG = 1,
	LAYER_CFG_COLOR_GRB = 2,
	LAYER_CFG_COLOR_GBR = 3,
	LAYER_CFG_COLOR_BRG = 4,
	LAYER_CFG_COLOR_BGR = 5,
};

typedef union frm_size {
	uint32_t d32;
	struct {
		uint32_t width:12;
	        uint32_t reserve12_15:4;
	        uint32_t height:12;
	        uint32_t reserve28_31:4;
	}b;
} frm_size_t;

typedef union frm_ctrl {
	uint32_t d32;
	struct {
		uint32_t stop:1;
		uint32_t bit1_keep0:1;
		uint32_t bit2_keep1:1;
		uint32_t bit3_4_keep0:2;
		uint32_t reserve5_31:27;
	}b;
} frm_ctrl_t;

typedef union lay_cfg_en {
	uint32_t d32;
	struct {
		uint32_t bit0_3_keep0:4;
		uint32_t lay0_en:1;
		uint32_t lay1_en:1;
		uint32_t bit6_7_keep0:2;
		uint32_t lay0_z_order:2;
		uint32_t lay1_z_order:2;
		uint32_t bit12_15_keep0:4;
		uint32_t reserve16_31:16;
	}b;
} lay_cfg_en_t;

typedef union cmp_irq_ctrl {
	uint32_t d32;
	struct {
		uint32_t reserve_0:1;
		uint32_t eof_msk:1;
		uint32_t sof_msk:1;
		uint32_t reserve3_14:12;
		uint32_t bit15_keep0:1;
		uint32_t reserve_16:1;
		uint32_t eod_msk:1;
		uint32_t reserve16_31:14;
	}b;
} cmp_irq_ctrl_t;

typedef union lay_size {
	uint32_t d32;
	struct {
		uint32_t width:12;
	        uint32_t reserve12_15:4;
	        uint32_t height:12;
	        uint32_t reserve28_31:4;
	}b;
} lay_size_t;

typedef union lay_cfg {
	uint32_t d32;
	struct {
		uint32_t g_alpha:8;
		uint32_t reserve8_9:2;
		uint32_t color:3;
		uint32_t g_alpha_en:1;
		uint32_t domain_multi:1;
		uint32_t reserve14_15:1;
		uint32_t format:4;
		uint32_t reserve22_31:12;
	}b;
} lay_cfg_t;

typedef union lay_scale {
	uint32_t d32;
	struct {
		uint32_t target_width:12;
	        uint32_t reserve12_15:4;
	        uint32_t target_height:12;
	        uint32_t reserve28_31:4;
	}b;
} lay_scale_t;

typedef union lay_pos {
	uint32_t d32;
	struct {
		uint32_t x_pos:12;
	        uint32_t reserve12_15:4;
	        uint32_t y_pos:12;
	        uint32_t reserve28_31:4;
	}b;
} lay_pos_t;

#define FRAME_CTRL_DEFAULT_SET  (0x04)
struct ingenicfb_framedesc {
	uint32_t	   FrameNextCfgAddr;
	frm_size_t	   FrameSize;
	frm_ctrl_t	   FrameCtrl;
	uint32_t	   WritebackAddr;
	uint32_t	   WritebackStride;
	uint32_t	   Layer0CfgAddr;
	uint32_t	   Layer1CfgAddr;
	uint32_t 	   Layer2CfgAddr;
	uint32_t	   Layer3CfgAddr;
	lay_cfg_en_t       LayCfgEn;
	cmp_irq_ctrl_t	   InterruptControl;
};

struct ingenicfb_layerdesc {
	lay_size_t	LayerSize;
	lay_cfg_t	LayerCfg;
	uint32_t	LayerBufferAddr;
	lay_scale_t	LayerScale;
	uint32_t	layer_reserve0;
	uint32_t	LayerScratch;
	lay_pos_t	LayerPos;
	uint32_t	layer_reserve1;
	uint32_t	layer_reserve2;
	uint32_t	LayerStride;
};

/*
 * Helpers:
 */

static inline unsigned long reg_read(struct drm_device *dev, u32 reg)
{
	struct ingenic_drm_private *priv = dev->dev_private;
	return ioread32(priv->mmio + reg);
}

static inline void reg_write(struct drm_device *dev, u32 reg, u32 data)
{
	struct ingenic_drm_private *priv = dev->dev_private;
	iowrite32(data, priv->mmio + reg);
}
#endif /* _DPU_REG_H_ */
