/*
 * Copyright (C) 2016 Ingenic Semiconductor Co., Ltd.
 * Author: cli <chen.li@ingenic.com>
 *
 * X1800 Clock (kernel.4.4)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <linux/clk-provider.h>
#include <dt-bindings/clock/ingenic-x1800.h>
#include <linux/syscore_ops.h>
#include <linux/slab.h>
#include <linux/clk.h>
#include <linux/of_address.h>
#include <linux/mfd/syscon.h>
#include <soc/cpm.h>
#include <linux/delay.h>
#include <linux/regmap.h>

#include <linux/module.h>

#include "clk-comm.h"
#include "clk-pll-v2.h"
#include "clk-cpccr.h"
#include "clk-cgu.h"
#include "clk-gate.h"
#include "clk-rtc.h"

#define assert(expr)                                            \
    do {                                                        \
        if (!(expr)) {                                          \
            panic("Assertion failed! %s, %s, %s, line %d\n",    \
                  #expr, __FILE__, __func__, __LINE__);         \
        }                                                       \
    } while (0)

static const char *clk_name[NR_CLKS] = {
	[CLK_EXT] = "ext",
	[CLK_RTC_EXT] = "rtc_ext",
	[CLK_PLL_APLL] = "apll",
	[CLK_PLL_MPLL] = "mpll",
	[CLK_MUX_SCLKA] = "sclka_mux",
	[CLK_MUX_CPLL] = "cpu_mux",
	[CLK_MUX_H0PLL] = "ahb0_mux",
	[CLK_MUX_H2PLL] = "ahb2_mux",
	[CLK_RATE_CPUCLK] = "cpu",
	[CLK_RATE_L2CCLK] = "l2cache",
	[CLK_RATE_H0CLK] = "ahb0",
	[CLK_RATE_H2CLK] = "ahb2",
	[CLK_RATE_PCLK] = "pclk",
	[CLK_CGU_DDR] = "cgu_ddr",
	[CLK_CGU_VPU] = "cgu_vpu",
	[CLK_CGU_MAC] = "cgu_mac",
	[CLK_CGU_ISP] = "cgu_isp",
	[CLK_CGU_MSC0] = "cgu_msc0",
	[CLK_CGU_MSC1] = "cgu_msc1",
	[CLK_CGU_MSCMUX] = "cgu_mscmux",
	[CLK_CGU_SFC] = "cgu_sfc",
	[CLK_CGU_SSI] = "cgu_ssi",
	[CLK_CGU_CIM] = "cgu_cim",
	[CLK_CGU_I2S] = "cgu_i2s",
	[CLK_CGU_ISP] = "cgu_isp",
	[CLK_GATE_EFUSE] = "gate_efuse",
	[CLK_GATE_SFC  ] = "gate_sfc",
	[CLK_GATE_OTG  ] = "gate_otg",
	[CLK_GATE_MSC0 ] = "gate_msc0",
	[CLK_GATE_MSC1 ] = "gate_msc1",
	[CLK_GATE_I2C0 ] = "gate_i2c0",
	[CLK_GATE_I2C1 ] = "gate_i2c1",
	[CLK_GATE_I2C2 ] = "gate_i2c2",
	[CLK_GATE_I2C3 ] = "gate_i2c3",
	[CLK_GATE_AIC  ] = "gate_aic",
	[CLK_GATE_DMIC ] = "gate_dmic",
	[CLK_GATE_SADC ] = "gate_sadc",
	[CLK_GATE_UART0] = "gate_uart0",
	[CLK_GATE_UART1] = "gate_uart1",
	[CLK_GATE_UART2] = "gate_uart2",
	[CLK_GATE_TCU  ] = "gate_tcu",
	[CLK_GATE_SSI0  ] = "gate_ssi0",
	[CLK_GATE_SYS_OST  ] = "gate_ost",
	[CLK_GATE_PDMA ] = "gate_pdma",
	[CLK_GATE_ISP  ] = "gate_isp",
	[CLK_GATE_AES  ] = "gate_aes",
	[CLK_GATE_MAC  ] = "gate_mac",
	[CLK_GATE_RTC  ] = "gate_rtc",
	[CLK_GATE_APB0 ] = "gate_apb",
	[CLK_GATE_AHB0 ] = "gate_ahb0",
	[CLK_GATE_CPU  ] = "gate_cpu",
	[CLK_GATE_DDR  ] = "gate_ddr",
	[CLK_GATE_VPU0] = "gate_vpu0",
	[CLK_GATE_IPU] = "gate_ipu",
	[CLK_GATE_SCLKA] = "sclka",
	[CLK_GATE_USBPHY] = "gate_usbphy",
	[CLK_GATE_SCLKABUS] = "sclka_bus",
	[CLK_RTC] = "rtc",
	[CLK_CGU_USB] = "cgu_usb",
};

/********************************************************************************
 *	PLL
 ********************************************************************************/
/*PLL HWDESC*/
static const s8 pll_od_encode[4] = {1, 2, 4, 8};
static struct ingenic_pll_hwdesc apll_hwdesc = \
	PLL_DESC(CPM_CPAPCR, 20, 12, 14, 6, 11, 3, 8, 3, 0, 3, 2);

static struct ingenic_pll_hwdesc mpll_hwdesc = \
	PLL_DESC(CPM_CPMPCR, 20, 12, 14, 6, 11, 3, 8, 3, 0, 3, 2);


/********************************************************************************
 *	CPCCR
 ********************************************************************************/
/*CPCCR PARENTS*/
static const int sclk_a_p[] = { DUMMY_STOP, CLK_EXT, CLK_PLL_APLL, DUMMY_UNKOWN };
static const int cpccr_p[] = { DUMMY_STOP, CLK_MUX_SCLKA, CLK_PLL_MPLL, DUMMY_UNKOWN };

/*CPCCR HWDESC*/
#define INDEX_CPCCR_HWDESC(_id)  ((_id) - CLK_ID_CPCCR)
static struct ingenic_cpccr_hwdesc cpccr_hwdesc[] = {
	[INDEX_CPCCR_HWDESC(CLK_MUX_SCLKA)] = CPCCR_MUX_RODESC(CPM_CPCCR, 30, 0x3),	/*sclk a*/
	[INDEX_CPCCR_HWDESC(CLK_MUX_CPLL)] = CPCCR_MUX_RODESC(CPM_CPCCR, 28, 0x3),	/*cpll*/
	[INDEX_CPCCR_HWDESC(CLK_MUX_H0PLL)] = CPCCR_MUX_RODESC(CPM_CPCCR, 26, 0x3),	/*h0pll*/
	[INDEX_CPCCR_HWDESC(CLK_MUX_H2PLL)] = CPCCR_MUX_RODESC(CPM_CPCCR, 24, 0x3),	/*h2pll*/
	[INDEX_CPCCR_HWDESC(CLK_RATE_PCLK)] = CPCCR_RATE_RODESC(CPM_CPCCR, 16, 0xf),	/*pdiv*/
	[INDEX_CPCCR_HWDESC(CLK_RATE_H2CLK)] = CPCCR_RATE_RODESC(CPM_CPCCR, 12, 0xf),	/*h2div*/
	[INDEX_CPCCR_HWDESC(CLK_RATE_H0CLK)] = CPCCR_RATE_RODESC(CPM_CPCCR, 8, 0xf),	/*h0div*/
	[INDEX_CPCCR_HWDESC(CLK_RATE_L2CCLK)] = CPCCR_RATE_RODESC(CPM_CPCCR, 4, 0xf),	/*l2cdiv*/
	[INDEX_CPCCR_HWDESC(CLK_RATE_CPUCLK)] = CPCCR_RATE_RODESC(CPM_CPCCR, 0, 0xf),	/*cdiv*/
};
#define X1800_CPCCR_HWDESC(_id) &cpccr_hwdesc[INDEX_CPCCR_HWDESC((_id))]

/********************************************************************************
 *	cgu
 ********************************************************************************/
/*CGU PARENTS*/
/*mac, lcd pixel, msc, cim mclk, sfc*/
static const int cgu_ddr[] = { DUMMY_STOP, CLK_MUX_SCLKA, CLK_PLL_MPLL, DUMMY_UNKOWN };
static const int cgu_sel_grp0[] = { CLK_MUX_SCLKA, CLK_PLL_MPLL };
static const int const cgu_ssi[] = { CLK_EXT, CLK_CGU_SFC};
static const int const cgu_usb[] = { CLK_EXT, CLK_EXT, CLK_MUX_SCLKA, CLK_PLL_MPLL };

/*FIXCLK*/
static const u8 usb_cs_ext[] = {0x0, 0x1};
static const u8 ssi_cs_ext[] = {0x0};

/*CGU HWDESC*/
#define INDEX_CGU_HWDESC(_id) ((_id) - CLK_ID_DIV)
static struct ingenic_cgu_hwdesc cgu_hwdesc[] = {
	/*reg, sel, selmsk, ce, busy ,stop, cdr, cdrmsk, parenttable, step*/
	[INDEX_CGU_HWDESC(CLK_CGU_DDR)] = CGU_DESC(CPM_DDRCDR, 30, 0x3, 29, 28, 27, 0, 0xf, 1),		/*ddr*/		/*FIXME ddr now is ro mode*/
	[INDEX_CGU_HWDESC(CLK_CGU_MAC)] = CGU_DESC(CPM_MACCDR, 31, 0x1, 29, 28, 27, 0, 0xff, 1),	/*mac*/
	[INDEX_CGU_HWDESC(CLK_CGU_ISP)]	= CGU_DESC(CPM_ISPCDR, 31, 0x1, 29, 27, 26, 0, 0xff, 1),	/*isp*/
	[INDEX_CGU_HWDESC(CLK_CGU_MSCMUX)] = CGU_DESC(CPM_MSC0CDR, 31, 0x1, -1, -1, -1, 0, 0, 1),	 /*mscmux*/
	[INDEX_CGU_HWDESC(CLK_CGU_MSC0)] = CGU_DESC(CPM_MSC0CDR, -1, 0, 29, 28, 27, 0, 0xff, 2),	 /*msc0*/
	[INDEX_CGU_HWDESC(CLK_CGU_MSC1)] = CGU_DESC(CPM_MSC1CDR, -1, 0, 29, 28, 27, 0, 0xff, 2),	 /*msc1*/
	[INDEX_CGU_HWDESC(CLK_CGU_SFC)] = CGU_DESC(CPM_SFCCDR, 31, 0x1, 29, 28, 27, 0, 0xff, 1),	/*sfc*/
	[INDEX_CGU_HWDESC(CLK_CGU_CIM)] = CGU_DESC(CPM_CIMCDR, 31, 0x1, 29, 28, 27, 0, 0xff, 1),	/*cim mclk*/
	[INDEX_CGU_HWDESC(CLK_CGU_I2S)] = CGU_DESC(CPM_I2SCDR, 31, 0x1, 29, -1, -1, 0, 0, 1),  		/*i2s*/
	[INDEX_CGU_HWDESC(CLK_CGU_SSI)] = CGU_DESC_WITH_FIXCLK(CPM_SFCCDR, 30, 0x1, -1, -1, -1, 0, 0x0, 2, ssi_cs_ext), /*ssi*/
	[INDEX_CGU_HWDESC(CLK_CGU_USB)] = CGU_DESC_WITH_FIXCLK(CPM_USBCDR, 30, 0x3, 29, 28, 27, 0, 0xff, 1, usb_cs_ext),/*usb*/
};
#define X1800_CGU_HWDESC(_id) &cgu_hwdesc[INDEX_CGU_HWDESC((_id))]

static int ingenic_ssi_determine_rate(struct clk_hw *hw, struct clk_rate_request *req)
{

	struct ingenic_clk *iclk = to_ingenic_clk(hw);
	struct ingenic_cgu_hwdesc *hwdesc = iclk->hwdesc;
	unsigned long rate;
	struct clk_hw *clk_hw;

	clk_hw = clk_hw_get_parent_by_index(hw, hwdesc->fixclk[0]);
	rate = clk_hw_get_rate(clk_hw);
	if (rate == req->rate) {
		req->best_parent_rate = rate;
		req->best_parent_hw = clk_hw;
		return 0;
	}
	clk_hw = clk_hw_get_parent_by_index(hw, 1);
	req->best_parent_rate =req->rate * hwdesc->div_step;
	req->best_parent_hw = clk_hw;
	req->rate = DIV_ROUND_UP(req->best_parent_rate, hwdesc->div_step);
	return 0;
}

static const struct clk_ops ingenic_ssi_mux_ops = {
	.set_parent = ingenic_cgu_set_parent,
	.get_parent = ingenic_cgu_get_parent,
	.recalc_rate = ingenic_cgu_recalc_rate,
	.determine_rate = ingenic_ssi_determine_rate,
#ifdef CONFIG_INGENIC_CLK_DEBUG_FS
	.debug_init	= ingenic_cgu_debug_init,
#endif
};

static unsigned long ingenic_audio_recalc_rate(struct clk_hw *hw, unsigned long parent_rate)
{
	struct ingenic_clk *iclk = to_ingenic_clk(hw);
	struct ingenic_cgu_hwdesc *hwdesc = iclk->hwdesc;
	unsigned long rate, flags;
	uint32_t tmp;
	unsigned int m, n;

	CLK_LOCK(iclk, flags);

	tmp = clkhw_readl(iclk, hwdesc->regoff);
	m = (tmp >> 13) & 0x1ff;
	n = tmp & 0x1fff;

	assert(n > 0);

	rate = DIV_ROUND_UP_ULL((u64)parent_rate * m, n);

	CLK_UNLOCK(iclk, flags);
	return rate;
}

static int ingenic_audio_set_rate(struct clk_hw *hw, unsigned long rate,
		unsigned long parent_rate)
{

	struct ingenic_clk *iclk = to_ingenic_clk(hw);
	struct ingenic_cgu_hwdesc *hwdesc = iclk->hwdesc;
	uint32_t xcdr;
	unsigned long flags;

	unsigned int m = 1, n = 0;

	CLK_LOCK(iclk, flags);
	xcdr = clkhw_readl(iclk, hwdesc->regoff) & 0xf0000000;
	n = DIV_ROUND_UP_ULL((u64)parent_rate * m, rate);
	assert(n > 0);
	xcdr |= (m << 13) | (n << 0);
	clkhw_writel(iclk, hwdesc->regoff, xcdr);
	CLK_UNLOCK(iclk, flags);
	return 0;
}

static long ingenic_audio_round_rate(struct clk_hw *hw, unsigned long rate,
					unsigned long *parent_rate)
{
	unsigned int m  = 1, n ;
	n = DIV_ROUND_UP_ULL((u64)(*parent_rate) * m, rate);
	if (n > 0x1fff) n = 0x1fff;
	rate = DIV_ROUND_UP_ULL((u64)(*parent_rate)*m, n);
	return rate;
}

static int ingenic_audio_enable(struct clk_hw *hw)
{
	struct ingenic_clk *iclk = to_ingenic_clk(hw);
	struct ingenic_cgu_hwdesc *hwdesc = iclk->hwdesc;
	uint32_t xcdr;
	unsigned long flags;

	CLK_LOCK(iclk, flags);

	xcdr = clkhw_readl(iclk, hwdesc->regoff);

	xcdr |= BIT(hwdesc->bit_ce);

	clkhw_writel(iclk, hwdesc->regoff, xcdr);

	CLK_UNLOCK(iclk, flags);
	return 0;
}

static void ingenic_audio_disable(struct clk_hw *hw)
{
	struct ingenic_clk *iclk = to_ingenic_clk(hw);
	struct ingenic_cgu_hwdesc *hwdesc = iclk->hwdesc;
	uint32_t xcdr;
	unsigned long flags;

	CLK_LOCK(iclk, flags);

	xcdr = clkhw_readl(iclk, hwdesc->regoff);

	xcdr &= ~BIT(hwdesc->bit_ce);

	clkhw_writel(iclk, hwdesc->regoff, xcdr);

	CLK_UNLOCK(iclk, flags);
	return;
}

static const struct clk_ops ingenic_audio_ops = {
	.set_parent = ingenic_cgu_set_parent,
	.get_parent = ingenic_cgu_get_parent,
	.recalc_rate = ingenic_audio_recalc_rate,
	.round_rate = ingenic_audio_round_rate,
	.set_rate = ingenic_audio_set_rate,
	.enable = ingenic_audio_enable,
	.disable = ingenic_audio_disable,
};

static const struct clk_ops ingenic_msc_mux_ops = {
	.set_parent = ingenic_cgu_set_parent,
	.get_parent = ingenic_cgu_get_parent,
};

static const struct clk_ops ingenic_msc_ops = {
	.enable = ingenic_cgu_enable,
	.disable = ingenic_cgu_disable,
	.is_enabled = ingenic_cgu_is_enabled,
	.recalc_rate = ingenic_cgu_recalc_rate,
	.determine_rate = ingenic_cgu_determine_rate,
	.set_rate = ingenic_cgu_set_rate,
#ifdef CONFIG_INGENIC_CLK_DEBUG_FS
	.debug_init	= ingenic_cgu_debug_init,
#endif
};

/********************************************************************************
 *	GATE
 ********************************************************************************/
#define INDEX_GATE_HWDESC(_id)  ((_id) - CLK_ID_GATE)
static struct ingenic_gate_hwdesc gate_hwdesc[] = {
	[INDEX_GATE_HWDESC(CLK_GATE_EFUSE)	] =	GATE_DESC(CPM_CLKGR, 1),
	[INDEX_GATE_HWDESC(CLK_GATE_OTG)	] =	GATE_DESC(CPM_CLKGR, 3),
	[INDEX_GATE_HWDESC(CLK_GATE_MSC0)	] =	GATE_DESC(CPM_CLKGR, 4),
	[INDEX_GATE_HWDESC(CLK_GATE_MSC1)	] =	GATE_DESC(CPM_CLKGR, 5),
	[INDEX_GATE_HWDESC(CLK_GATE_SSI0)	] =	GATE_DESC(CPM_CLKGR, 6),
	[INDEX_GATE_HWDESC(CLK_GATE_I2C0)	] =	GATE_DESC(CPM_CLKGR, 7),
	[INDEX_GATE_HWDESC(CLK_GATE_I2C1)	] =	GATE_DESC(CPM_CLKGR, 8),
	[INDEX_GATE_HWDESC(CLK_GATE_I2C2)	] =	GATE_DESC(CPM_CLKGR, 9),
	[INDEX_GATE_HWDESC(CLK_GATE_I2C3)	] =	GATE_DESC(CPM_CLKGR, 10),
	[INDEX_GATE_HWDESC(CLK_GATE_AIC)	] =	GATE_DESC(CPM_CLKGR, 11),
	[INDEX_GATE_HWDESC(CLK_GATE_DMIC)	] =	GATE_DESC(CPM_CLKGR, 12),
	[INDEX_GATE_HWDESC(CLK_GATE_SADC)	] =	GATE_DESC(CPM_CLKGR, 13),
	[INDEX_GATE_HWDESC(CLK_GATE_UART0)	] =	GATE_DESC(CPM_CLKGR, 14),
	[INDEX_GATE_HWDESC(CLK_GATE_UART1)	] =	GATE_DESC(CPM_CLKGR, 15),
	[INDEX_GATE_HWDESC(CLK_GATE_UART2)	] =	GATE_DESC(CPM_CLKGR, 16),
	[INDEX_GATE_HWDESC(CLK_GATE_SFC)	] =	GATE_DESC(CPM_CLKGR, 20),
	[INDEX_GATE_HWDESC(CLK_GATE_PDMA)	] =	GATE_DESC(CPM_CLKGR, 21),
	[INDEX_GATE_HWDESC(CLK_GATE_ISP)	] =	GATE_DESC(CPM_CLKGR, 23),
	[INDEX_GATE_HWDESC(CLK_GATE_DES)	] =	GATE_DESC(CPM_CLKGR, 28),
	[INDEX_GATE_HWDESC(CLK_GATE_RTC)	] =	GATE_DESC(CPM_CLKGR, 29),
	[INDEX_GATE_HWDESC(CLK_GATE_TCU)	] =	GATE_DESC(CPM_CLKGR, 30),
	[INDEX_GATE_HWDESC(CLK_GATE_DDR)	] =	GATE_DESC(CPM_CLKGR, 31),
	[INDEX_GATE_HWDESC(CLK_GATE_VPU0)	] =	GATE_DESC(CPM_CLKGR1, 0),
	[INDEX_GATE_HWDESC(CLK_GATE_IPU)	] =	GATE_DESC(CPM_CLKGR1, 2),
	[INDEX_GATE_HWDESC(CLK_GATE_MAC)	] =	GATE_DESC(CPM_CLKGR1, 4),
	[INDEX_GATE_HWDESC(CLK_GATE_AES)	] =	GATE_DESC(CPM_CLKGR1, 5),
	[INDEX_GATE_HWDESC(CLK_GATE_AHB0)	] =	GATE_DESC(CPM_CLKGR1, 10),
	[INDEX_GATE_HWDESC(CLK_GATE_SYS_OST)	] =	GATE_DESC(CPM_CLKGR1, 11),
	[INDEX_GATE_HWDESC(CLK_GATE_APB0)	] =	GATE_DESC(CPM_CLKGR1, 14),
	[INDEX_GATE_HWDESC(CLK_GATE_CPU)	] =	GATE_DESC(CPM_CLKGR1, 15),
	[INDEX_GATE_HWDESC(CLK_GATE_SCLKA)	] =	GATE_DESC(CPM_CPCCR, 23),
	[INDEX_GATE_HWDESC(CLK_GATE_USBPHY)	] =	GATE_DESC(CPM_OPCR, 23),
	[INDEX_GATE_HWDESC(CLK_GATE_SCLKABUS)	] =	GATE_DESC(CPM_OPCR, 28),
};

#define X1800_GATE_HWDESC(_id)	\
	 &gate_hwdesc[INDEX_GATE_HWDESC(_id)]

/*RTC*/
struct ingenic_rtc_hwdesc rtc_hwdesc = {
	.regoff = CPM_OPCR,
	.bit_cs	= 2,
	.div = 512,
};
static const int rtc_mux[] = { CLK_EXT, CLK_RTC_EXT };

/********************************************************************************
 *	X1800 CLK
 ********************************************************************************/
/*Note: parent num > 1, _pids is parent clock id's array*/
#define X1800_CLK_MUX(_id, _pids, _ops, _flags, _phwdesc)	\
		CLK_INIT_DATA_LOCKED(_id, _ops, _pids, ARRAY_SIZE(_pids), _flags, _phwdesc)

/*Note: parent num == 1, _pid is parent clock id*/
#define X1800_CLK(_id, _pid, _ops, _flags, _phwdesc)	\
	CLK_INIT_DATA_LOCKED(_id, _ops, _pid, 1, _flags, _phwdesc)

static const struct ingenic_clk_init __initdata x1800_clk_init_data[] = {
	/*pll*/
	X1800_CLK(CLK_PLL_APLL, CLK_EXT, &ingenic_pll_v2_ro_ops, CLK_IGNORE_UNUSED, &apll_hwdesc),
	X1800_CLK(CLK_PLL_MPLL, CLK_EXT, &ingenic_pll_v2_ro_ops, CLK_IGNORE_UNUSED, &mpll_hwdesc),
	/*cpccr*/
	X1800_CLK_MUX(CLK_MUX_SCLKA, sclk_a_p, &ingenic_cpccr_mux_ro_ops, 0, X1800_CPCCR_HWDESC(CLK_MUX_SCLKA)),
	X1800_CLK_MUX(CLK_MUX_CPLL, cpccr_p, &ingenic_cpccr_mux_ro_ops, 0, X1800_CPCCR_HWDESC(CLK_MUX_CPLL)),
	X1800_CLK_MUX(CLK_MUX_H0PLL, cpccr_p, &ingenic_cpccr_mux_ro_ops, 0, X1800_CPCCR_HWDESC(CLK_MUX_H0PLL)),
	X1800_CLK_MUX(CLK_MUX_H2PLL, cpccr_p, &ingenic_cpccr_mux_ro_ops, 0, X1800_CPCCR_HWDESC(CLK_MUX_H2PLL)),
	X1800_CLK(CLK_RATE_PCLK, CLK_MUX_H2PLL, &ingenic_cpccr_rate_ro_ops,0, X1800_CPCCR_HWDESC(CLK_RATE_PCLK)),
	X1800_CLK(CLK_RATE_H2CLK, CLK_MUX_H2PLL, &ingenic_cpccr_rate_ro_ops,0, X1800_CPCCR_HWDESC(CLK_RATE_H2CLK)),
	X1800_CLK(CLK_RATE_H0CLK, CLK_MUX_H0PLL, &ingenic_cpccr_rate_ro_ops,0, X1800_CPCCR_HWDESC(CLK_RATE_H0CLK)),
	X1800_CLK(CLK_RATE_CPUCLK, CLK_MUX_CPLL, &ingenic_cpccr_rate_ro_ops,0, X1800_CPCCR_HWDESC(CLK_RATE_CPUCLK)),
	X1800_CLK(CLK_RATE_L2CCLK, CLK_MUX_CPLL, &ingenic_cpccr_rate_ro_ops,0, X1800_CPCCR_HWDESC(CLK_RATE_L2CCLK)),
	/*cgu*/
	X1800_CLK_MUX(CLK_CGU_DDR, cgu_ddr, &ingenic_cgu_ops, CLK_IGNORE_UNUSED, X1800_CGU_HWDESC(CLK_CGU_DDR)),
	X1800_CLK_MUX(CLK_CGU_MAC, cgu_sel_grp0, &ingenic_cgu_ops, CLK_SET_PARENT_GATE, X1800_CGU_HWDESC(CLK_CGU_MAC)),
	X1800_CLK_MUX(CLK_CGU_ISP, cgu_sel_grp0, &ingenic_cgu_ops, CLK_SET_PARENT_GATE, X1800_CGU_HWDESC(CLK_CGU_ISP)),
	X1800_CLK_MUX(CLK_CGU_MSCMUX, cgu_sel_grp0, &ingenic_msc_mux_ops, CLK_SET_PARENT_GATE, X1800_CGU_HWDESC(CLK_CGU_MSCMUX)),
	X1800_CLK(CLK_CGU_MSC0, CLK_CGU_MSCMUX, &ingenic_msc_ops, 0, X1800_CGU_HWDESC(CLK_CGU_MSC0)),
	X1800_CLK(CLK_CGU_MSC1, CLK_CGU_MSCMUX, &ingenic_msc_ops, 0, X1800_CGU_HWDESC(CLK_CGU_MSC1)),
	X1800_CLK_MUX(CLK_CGU_SFC, cgu_sel_grp0, &ingenic_cgu_ops, CLK_SET_PARENT_GATE, X1800_CGU_HWDESC(CLK_CGU_SFC)),
	X1800_CLK_MUX(CLK_CGU_CIM, cgu_sel_grp0, &ingenic_cgu_ops, CLK_SET_PARENT_GATE, X1800_CGU_HWDESC(CLK_CGU_CIM)),
	X1800_CLK_MUX(CLK_CGU_SSI, cgu_ssi, &ingenic_ssi_mux_ops, CLK_SET_RATE_PARENT, X1800_CGU_HWDESC(CLK_CGU_SSI)),
	X1800_CLK_MUX(CLK_CGU_I2S, cgu_sel_grp0, &ingenic_audio_ops, 0, X1800_CGU_HWDESC(CLK_CGU_I2S)),
	X1800_CLK_MUX(CLK_CGU_USB, cgu_usb, &ingenic_cgu_ops, CLK_SET_PARENT_GATE, X1800_CGU_HWDESC(CLK_CGU_USB)),


	/*gate*/
	X1800_CLK(CLK_GATE_EFUSE, CLK_RATE_H2CLK, &ingenic_gate_ops, 0,
		      X1800_GATE_HWDESC(CLK_GATE_EFUSE)),
	X1800_CLK(CLK_GATE_SFC  , CLK_RATE_H2CLK, &ingenic_gate_ops, 0,
		      X1800_GATE_HWDESC(CLK_GATE_SFC)),
	X1800_CLK(CLK_GATE_OTG  , CLK_RATE_H2CLK, &ingenic_gate_ops, 0,
		      X1800_GATE_HWDESC(CLK_GATE_OTG)),
	X1800_CLK(CLK_GATE_MSC0 , CLK_RATE_H2CLK, &ingenic_gate_ops, 0,
		      X1800_GATE_HWDESC(CLK_GATE_MSC0)),
	X1800_CLK(CLK_GATE_MSC1 , CLK_RATE_H2CLK, &ingenic_gate_ops, 0,
		      X1800_GATE_HWDESC(CLK_GATE_MSC1)),
	X1800_CLK(CLK_GATE_I2C0 , CLK_RATE_PCLK, &ingenic_gate_ops, 0,
		      X1800_GATE_HWDESC(CLK_GATE_I2C0)),
	X1800_CLK(CLK_GATE_I2C1 , CLK_RATE_PCLK, &ingenic_gate_ops, 0,
		      X1800_GATE_HWDESC(CLK_GATE_I2C1)),
	X1800_CLK(CLK_GATE_I2C2 , CLK_RATE_PCLK, &ingenic_gate_ops, 0,
		      X1800_GATE_HWDESC(CLK_GATE_I2C2)),
	X1800_CLK(CLK_GATE_I2C3 , CLK_RATE_PCLK, &ingenic_gate_ops, 0,
		      X1800_GATE_HWDESC(CLK_GATE_I2C3)),
	X1800_CLK(CLK_GATE_AIC  , CLK_RATE_PCLK, &ingenic_gate_ops, 0,
		      X1800_GATE_HWDESC(CLK_GATE_AIC)),
	X1800_CLK(CLK_GATE_VPU0 , CLK_RATE_H0CLK, &ingenic_gate_ops, 0,
		      X1800_GATE_HWDESC(CLK_GATE_VPU0)),
	X1800_CLK(CLK_GATE_SADC , CLK_RATE_PCLK, &ingenic_gate_ops, 0,
		      X1800_GATE_HWDESC(CLK_GATE_SADC)),
	X1800_CLK(CLK_GATE_UART0, CLK_EXT, &ingenic_gate_ops, 0,
		      X1800_GATE_HWDESC(CLK_GATE_UART0)),
	X1800_CLK(CLK_GATE_UART1, CLK_EXT, &ingenic_gate_ops, 0,
		      X1800_GATE_HWDESC(CLK_GATE_UART1)),
	X1800_CLK(CLK_GATE_UART2, CLK_EXT, &ingenic_gate_ops, 0,
		      X1800_GATE_HWDESC(CLK_GATE_UART2)),
	X1800_CLK(CLK_GATE_DMIC , CLK_RATE_PCLK, &ingenic_gate_ops, 0,
		      X1800_GATE_HWDESC(CLK_GATE_DMIC)),
	X1800_CLK(CLK_GATE_TCU  , CLK_RATE_PCLK, &ingenic_gate_ops, 0,
		      X1800_GATE_HWDESC(CLK_GATE_TCU)),
	X1800_CLK(CLK_GATE_SSI0  , CLK_RATE_PCLK,&ingenic_gate_ops, 0,
		      X1800_GATE_HWDESC(CLK_GATE_SSI0)),
	X1800_CLK(CLK_GATE_SYS_OST  , CLK_RATE_L2CCLK, &ingenic_gate_ops, 0,
		      X1800_GATE_HWDESC(CLK_GATE_SYS_OST)),
	X1800_CLK(CLK_GATE_PDMA , CLK_RATE_H2CLK, &ingenic_gate_ops, 0,
		      X1800_GATE_HWDESC(CLK_GATE_PDMA)),
	X1800_CLK(CLK_GATE_IPU  , CLK_RATE_H0CLK, &ingenic_gate_ops, 0,
		      X1800_GATE_HWDESC(CLK_GATE_IPU)),
	X1800_CLK(CLK_GATE_ISP  , CLK_RATE_H0CLK, &ingenic_gate_ops, 0,
		      X1800_GATE_HWDESC(CLK_GATE_ISP)),
	X1800_CLK(CLK_GATE_AES  , CLK_RATE_H2CLK, &ingenic_gate_ops, 0,
		      X1800_GATE_HWDESC(CLK_GATE_AES)),
	X1800_CLK(CLK_GATE_MAC  , CLK_RATE_H2CLK, &ingenic_gate_ops, 0,
		      X1800_GATE_HWDESC(CLK_GATE_MAC)),
	X1800_CLK(CLK_GATE_RTC  , CLK_RATE_PCLK, &ingenic_gate_ops, 0,
		      X1800_GATE_HWDESC(CLK_GATE_RTC)),
	X1800_CLK(CLK_GATE_APB0 , CLK_RATE_PCLK, &ingenic_gate_ops, CLK_IGNORE_UNUSED,
		      X1800_GATE_HWDESC(CLK_GATE_APB0)),
	X1800_CLK(CLK_GATE_AHB0 , CLK_RATE_H0CLK, &ingenic_gate_ops, CLK_IGNORE_UNUSED,
		      X1800_GATE_HWDESC(CLK_GATE_AHB0)),
	X1800_CLK(CLK_GATE_CPU  , CLK_RATE_CPUCLK, &ingenic_gate_ops, CLK_IGNORE_UNUSED,
		      X1800_GATE_HWDESC(CLK_GATE_CPU)),
	X1800_CLK(CLK_GATE_DDR  , CLK_RATE_H0CLK, &ingenic_gate_ops, CLK_IGNORE_UNUSED,
		      X1800_GATE_HWDESC(CLK_GATE_DDR)),
	X1800_CLK(CLK_GATE_SCLKA, CLK_MUX_SCLKA, &ingenic_gate_ops, CLK_IGNORE_UNUSED,
		      X1800_GATE_HWDESC(CLK_GATE_SCLKA)),
	X1800_CLK(CLK_GATE_USBPHY, CLK_EXT, &ingenic_gate_ops, 0,
		      X1800_GATE_HWDESC(CLK_GATE_USBPHY)),
	X1800_CLK(CLK_GATE_SCLKABUS, CLK_MUX_SCLKA,  &ingenic_gate_ops, CLK_IGNORE_UNUSED,
			X1800_GATE_HWDESC(CLK_GATE_SCLKABUS)),
	X1800_CLK_MUX(CLK_RTC, rtc_mux, &ingenic_rtc_ops, 0, &rtc_hwdesc),
};

static int x1800_clk_suspend(void)
{
	return 0;
}

static void x1800_clk_resume(void)
{
	return;
}

static struct syscore_ops x1800_syscore_ops = {
	.suspend = x1800_clk_suspend,
	.resume = x1800_clk_resume,
};
#define RECOVERY_SIGNATURE	(0x001a1a)
#define SOFTBURN_SIGNATURE	(0x005353)
#define REBOOT_SIGNATURE	(0x003535)
#define UNMSAK_SIGNATURE	(0x7c0000)//do not use these bits
static struct regmap *clk_regmap;
void ingenic_recovery_sign(void)
{
	unsigned cpsppr;
	if (IS_ERR_OR_NULL(clk_regmap))
		return;
	pr_info("ingenic recovery sign\n");
	do {
		regmap_read(clk_regmap, CPM_CPPSR, &cpsppr);
		if (cpsppr == RECOVERY_SIGNATURE)
			break;
		regmap_write(clk_regmap, CPM_CPSPPR, 0x5a5a);
		regmap_write(clk_regmap, CPM_CPPSR, RECOVERY_SIGNATURE);
		regmap_write(clk_regmap, CPM_CPSPPR, 0);
		udelay(100);
	} while(1);
	regmap_write(clk_regmap, CPM_RSR, 0);
	return;
}
EXPORT_SYMBOL_GPL(ingenic_recovery_sign);

void ingenic_reboot_sign(void)
{
	if (IS_ERR_OR_NULL(clk_regmap))
		return;
	pr_info("ingenic reboot sign\n");
	regmap_write(clk_regmap, CPM_CPSPPR, 0x5a5a);
	regmap_write(clk_regmap, CPM_CPPSR, REBOOT_SIGNATURE);
	regmap_write(clk_regmap, CPM_CPSPPR, 0);
	regmap_write(clk_regmap, CPM_RSR, 0);
	udelay(100);
	return;
}
EXPORT_SYMBOL_GPL(ingenic_reboot_sign);

void ingenic_softburn_sign(void)
{
	unsigned int cpsppr;
	if (IS_ERR_OR_NULL(clk_regmap))
		return;
	pr_info("ingenic recovery sign\n");
	do {
		regmap_read(clk_regmap, CPM_CPPSR, &cpsppr);
		if (cpsppr == SOFTBURN_SIGNATURE)
			break;
		regmap_write(clk_regmap, CPM_CPSPPR, 0x5a5a);
		regmap_write(clk_regmap, CPM_CPPSR, SOFTBURN_SIGNATURE);
		regmap_write(clk_regmap, CPM_CPSPPR, 0);
		udelay(100);
	} while(1);
	return;
}
EXPORT_SYMBOL_GPL(ingenic_softburn_sign);

static void __init x1800_clk_init(struct device_node *np)
{
	struct ingenic_clk_provide *ctx = kzalloc(sizeof(struct ingenic_clk_provide), GFP_ATOMIC);

	ctx->regbase = of_io_request_and_map(np, 0, "cpm");
	if (!ctx->regbase)
		return;

	ctx->np = np;
	ctx->data.clks = (struct clk**)kzalloc(sizeof(struct clk*) * NR_CLKS, GFP_ATOMIC);
	ctx->data.clk_num = NR_CLKS;
	ctx->clk_name = clk_name;
	clk_regmap = ctx->pm_regmap = syscon_node_to_regmap(np);
	if (IS_ERR(ctx->pm_regmap)) {
		pr_err("Cannot find regmap for %s: %ld\n", np->full_name,
				PTR_ERR(ctx->pm_regmap));
		return;
	}
	ingenic_fixclk_register(ctx, clk_name[CLK_EXT], CLK_EXT);

	ingenic_fixclk_register(ctx, clk_name[CLK_RTC_EXT], CLK_RTC_EXT);

	ingenic_clks_register(ctx, x1800_clk_init_data, ARRAY_SIZE(x1800_clk_init_data));

	register_syscore_ops(&x1800_syscore_ops);

	if (of_clk_add_provider(np, of_clk_src_onecell_get, &ctx->data))
		panic("could not register clk provider\n");

	{
		char *name[] = {"cpu", "l2cache", "ahb0", "ahb2" , "pclk"};
		unsigned long rate[] = {0, 0, 0, 0, 0};
		int i;
		struct clk *clk;
		for (i = 0; i < 5; i++)  {
			clk = clk_get(NULL, name[i]);
			if (!IS_ERR(clk))
				rate[i] = clk_get_rate(clk);
			clk_put(clk);
			rate[i] /= 1000000;
		}
		printk("CPU:[%dM]L2CACHE:[%dM]AHB0:[%dM]AHB2:[%dM]PCLK:[%dM]\n",
				(int)rate[0], (int)rate[1], (int)rate[2],
				(int)rate[3], (int)rate[4]);
	}
	{
		char *name[] = {"apll", "mpll"};
		unsigned long rate[] = {0, 0};
		struct clk *clk;
		int i;
		for (i = 0; i < 2; i++)  {
			clk = clk_get(NULL, name[i]);
			if (!IS_ERR(clk))
				rate[i] = clk_get_rate(clk);
			clk_put(clk);
		}
		printk("APLL:[%ld], MPLL:[%ld]\n", rate[0], rate[1]);
	}

	return;
}
CLK_OF_DECLARE(x1800_clk, "ingenic,x1800-clocks", x1800_clk_init);
