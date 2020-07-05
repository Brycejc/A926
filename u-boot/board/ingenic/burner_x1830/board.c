/*
 * Ingenic x1830 setup code
 *
 * Copyright (c) 2017 Ingenic Semiconductor Co.,Ltd
 * Author: Zoro <ykli@ingenic.cn>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <nand.h>
#include <net.h>
#include <netdev.h>
#include <asm/gpio.h>
#include <asm/arch/cpm.h>
#include <asm/arch/mmc.h>
#include <asm/jz_uart.h>
#include <asm/arch/clk.h>

#ifndef CONFIG_SPL_BUILD
DECLARE_GLOBAL_DATA_PTR;
struct global_info ginfo __attribute__ ((section(".data")));
extern struct jz_uart *uart;
#endif

struct cgu_clk_src cgu_clk_src[] = {
	{HELIX, MPLL},
	{MACPHY, MPLL},
	{MSC, APLL},
	{SSI, MPLL},
	{CIM, VPLL},
	{ISP, MPLL},
	{I2S, VPLL}, //i2s使用VPLL
	{SRC_EOF,SRC_EOF}
};

#ifdef CONFIG_SYS_NAND_SELF_INIT
void board_nand_init(void)
{
	    return 0;
}
#endif

int board_early_init_f(void)
{
#ifndef CONFIG_SPL_BUILD
	burner_param_info();
	uart = (struct jz_uart *)(UART0_BASE + gd->arch.gi->uart_idx * 0x1000);
#endif
	return 0;
}

#ifdef CONFIG_USB_GADGET
int jz_udc_probe(void);
void board_usb_init(void)
{
	printf("USB_udc_probe\n");
	jz_udc_probe();
}
#endif /* CONFIG_USB_GADGET */

#ifdef CONFIG_MMC
int board_mmc_init(bd_t *bd)
{
	jz_mmc_init();
	return 0;
}
#endif

/* U-Boot common routines */
int checkboard(void)
{
	puts("Board: burner_x1830 (Ingenic XBurst T30 SoC)\n");
	return 0;
}

#ifdef CONFIG_SPL_BUILD

void spl_board_init(void)
{
}

#endif /* CONFIG_SPL_BUILD */
