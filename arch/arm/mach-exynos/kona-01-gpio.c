/*
 *  linux/arch/arm/mach-exynos/midas-gpio.c
 *
 * Copyright (c) 2010 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com/
 *
 * EXYNOS - GPIO setting in set board
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

/* Kona Gpio config history

 * 2013. 02. 02 : jaecheol kim (jc22.kim@samsung.com)
    => KONA 3g : GT-N5100_REV0.8_IF_PMIC_0201_1700.pdf
 */

#include <linux/gpio.h>
#include <linux/serial_core.h>
#include <plat/devs.h>
#include <plat/gpio-cfg.h>
#include <plat/regs-serial.h>
#include <mach/gpio-midas.h>
#include <plat/cpu.h>
#include <mach/pmu.h>

struct gpio_init_data {
	uint num;
	uint cfg;
	uint val;
	uint pud;
	uint drv;
};

extern int s3c_gpio_slp_cfgpin(unsigned int pin, unsigned int config);
extern int s3c_gpio_slp_setpull_updown(unsigned int pin, unsigned int config);

#ifdef CONFIG_MIDAS_COMMON
/*
 * P4NOTE GPIO Init Table
 */
static struct gpio_init_data kona_init_gpios[] = {

#if defined(CONFIG_MACH_KONA_EUR_LTE)
	{EXYNOS4_GPA1(0), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1},/* 3G : AP_RXD LTE : NC */
	{EXYNOS4_GPA1(1), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* 3G : AP_TXD LTE : NC */
#endif

	{EXYNOS4_GPA1(4), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1},	/* NC */
	{EXYNOS4_GPA1(5), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1},	/* NC */

	{EXYNOS4_GPD0(2), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1},
	{EXYNOS4_GPD0(3), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1},
	{EXYNOS4_GPD1(0), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1},
	{EXYNOS4_GPD1(1), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1},
	{EXYNOS4_GPD1(2), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* GSENSE_SDA_1.8V */
	{EXYNOS4_GPD1(3), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* GSENSE_SCL_1.8V */

	{EXYNOS4_GPF0(4), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1},
	{EXYNOS4_GPF0(5), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1},

#if defined(CONFIG_QC_MODEM)
	{EXYNOS4_GPF3(0), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* BUCK2_SEL */
	{EXYNOS4_GPF3(1), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* BUCK3_SEL */
	{EXYNOS4_GPF3(2), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* BUCK4_SEL */
#endif

	{EXYNOS4_GPL2(4), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* MOTOR_EN -> NC */
	{EXYNOS4_GPL2(5), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* ACCESSORY_EN -> NC */
	{EXYNOS4_GPL2(6), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* UART_SEL -> NC */

	{EXYNOS4212_GPM4(0), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
			S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1},	/* ADC_I2C_SCL -> NC */
	{EXYNOS4212_GPM4(1), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
			S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1},	/* ADC_I2C_SDA -> NC */
	{EXYNOS4212_GPM4(6), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
			S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1},	/* USB_OTG_EN -> NC */

	{EXYNOS4_GPX0(0), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* ADC_INT */
	{EXYNOS4_GPX0(1), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* ADC_IC_INT */
#if defined(CONFIG_SEC_MODEM) || defined(CONFIG_MACH_KONA_EUR_LTE)
	{EXYNOS4_GPX0(2), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* PS_ALS_INT */
#else
	{EXYNOS4_GPX0(2), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* EAR_SEND_END_AP */
#endif
	{EXYNOS4_GPX0(3), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* COVER_DET */
	{EXYNOS4_GPX0(4), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* DOCK_INT -> NC*/
#if !defined(CONFIG_QC_MODEM)
	{EXYNOS4_GPX0(5), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* REMOTE_SENSE_IRQ -> NC */
	{EXYNOS4_GPX0(6), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* CHG_INT -> NC*/
#endif
	{EXYNOS4_GPX0(7), S3C_GPIO_SFN(0xF), S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* AP_PMIC_IRQ */

#if !defined(CONFIG_MACH_KONA_EUR_LTE) && !defined(CONFIG_MACH_KONA_EUR_WIFI) && !defined(CONFIG_MACH_KONALTE_USA_ATT) && \
	!defined(CONFIG_MACH_KONA_KOR_WIFI) && !defined(CONFIG_TARGET_LOCALE_CHN) 
	{EXYNOS4_GPX1(2), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1},
#endif

	{EXYNOS4_GPX1(3), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* ACCESSORY_INT -> NC */

	{EXYNOS4_GPX1(5), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* OVP_FLAG */
#if defined(CONFIG_SEC_MODEM) || defined(CONFIG_QC_MODEM)
	{EXYNOS4_GPX1(7), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* SIM_DETECT */
#endif

#if !defined(CONFIG_QC_MODEM) || defined(CONFIG_MACH_KONA_EUR_LTE)
	{EXYNOS4_GPX2(0), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* BUCK3_SEL */
	{EXYNOS4_GPX2(1), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* BUCK4_SEL */
#endif
	{EXYNOS4_GPX2(3), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* FUEL_ALERT */
	{EXYNOS4_GPX2(4), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1},
	{EXYNOS4_GPX2(6), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* BT_HOST_WAKEUP */
	{EXYNOS4_GPX2(7), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* nPower */

	{EXYNOS4_GPX3(0), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* IF_CON_SENSE -> PMIC_JIGONB */
#if defined(CONFIG_QC_MODEM)
	{EXYNOS4_GPX3(1), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
#else
	{EXYNOS4_GPX3(1), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* BT_WAKE */
#endif
#if defined(CONFIG_SEC_MODEM)
	{EXYNOS4_GPX3(2), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* CP_PMU_RST */
#endif
	{EXYNOS4_GPX3(5), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* V_ACCESSORY_5V -> NC*/

	{EXYNOS4_GPK1(0), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPK1(1), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPK1(2), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */

	{EXYNOS4_GPK3(1), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* WLAN_SDIO_CMD */
	{EXYNOS4_GPK3(3), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* WLAN_SDIO_D(0) */
	{EXYNOS4_GPK3(4), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* WLAN_SDIO_D(1) */
	{EXYNOS4_GPK3(5), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* WLAN_SDIO_D(2) */
	{EXYNOS4_GPK3(6), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* WLAN_SDIO_D(3) */

	{EXYNOS4_GPY0(0), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPY0(1), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPY0(4), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* USB_SEL0 -> NC */
	{EXYNOS4_GPY0(5), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* USB_SEL1 -> NC */
	{EXYNOS4_GPY1(0), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPY1(1), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPY1(2), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPY1(3), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */

	{EXYNOS4212_GPJ1(3), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* CAM_MCLK */
	{EXYNOS4212_GPM2(2), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* VTCAM_MCLK */

#if defined(CONFIG_QC_MODEM)
	{EXYNOS4212_GPM3(4), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1},
#endif

#if defined(CONFIG_QC_MODEM)
	/* GPIO_AP2MDM_PMIC_RESET_N */
	{EXYNOS4_GPL0(0),
	 S3C_GPIO_OUTPUT, S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_UP, S5P_GPIO_DRVSTR_LV4},
#endif

#if defined(CONFIG_MACH_KONA_EUR_LTE)
	{EXYNOS4_GPY2(1), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
#endif
};

/*
 * kona GPIO Sleep Table
 */
static unsigned int kona_sleep_gpio_table[][3] = {
	{EXYNOS4_GPA0(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE}, /* BT_UART_RXD */
	{EXYNOS4_GPA0(1),  S3C_GPIO_SLP_OUT0, S3C_GPIO_PULL_NONE}, /* BT_UART_TXD */
	{EXYNOS4_GPA0(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE}, /* BT_UART_CTS */
	{EXYNOS4_GPA0(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_UP},  /* BT_UART_RTS */
#if !defined(CONFIG_QC_MODEM)
	{EXYNOS4_GPA0(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_UP}, /* GPS_UART_RXD */
	{EXYNOS4_GPA0(5),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_UP}, /* GPS_UART_TXD */
	{EXYNOS4_GPA0(6),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* GPS_UART_CTS */
	{EXYNOS4_GPA0(7),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},   /* GPS_UART_RTS */
#else
	{EXYNOS4_GPA0(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* AP_RXD */
	{EXYNOS4_GPA0(5),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* AP_TXD */
	{EXYNOS4_GPA0(6),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPA0(7),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
#endif

	{EXYNOS4_GPA1(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* 3G : AP_RXD LTE : NC */
	{EXYNOS4_GPA1(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* 3G : AP_TXD LTE : NC */
	{EXYNOS4_GPA1(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* TSP_SDA_1.8V */
	{EXYNOS4_GPA1(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* TSP_SCL_1.8V */
	{EXYNOS4_GPA1(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC*/
	{EXYNOS4_GPA1(5),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */

	{EXYNOS4_GPB(0),   S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPB(1),   S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPB(2),   S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPB(3),   S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPB(4),   S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPB(5),   S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPB(6),   S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPB(7),   S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */

	{EXYNOS4_GPC0(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* REC_PCM_CLK(NC) */
	{EXYNOS4_GPC0(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* LCD_EN */
	{EXYNOS4_GPC0(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* REC_PCM_SYNC(NC) */
	{EXYNOS4_GPC0(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* REC_PCM_IN(NC) */
	{EXYNOS4_GPC0(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* REC_PCM_OUT(NC) */

	{EXYNOS4_GPC1(0), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_UP}, /* PEN_PDCT */
	{EXYNOS4_GPC1(1),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* PEN_LDO_EN */

	{EXYNOS4_GPC1(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_UP}, /* PEN_IRQ_1.8V */
	{EXYNOS4_GPC1(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_UP}, /* PEN_SDA_1.8V */
	{EXYNOS4_GPC1(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_UP}, /* PEN_SCL_1.8V */

	{EXYNOS4_GPD0(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPD0(1),  S3C_GPIO_SLP_OUT0, S3C_GPIO_PULL_NONE}, /* LED_BACKLIGHT_PWM */
	{EXYNOS4_GPD0(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPD0(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},

	{EXYNOS4_GPD1(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* 3M_SDA_1.8V */
	{EXYNOS4_GPD1(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* 3M_SCL_1.8V */
	{EXYNOS4_GPD1(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPD1(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},

	{EXYNOS4_GPF0(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* LCD_HSYNC */
	{EXYNOS4_GPF0(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* LCD_VSYNC */
	{EXYNOS4_GPF0(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* LCD_DE */
	{EXYNOS4_GPF0(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* LCD_PCLK */
	{EXYNOS4_GPF0(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* LCD_FREQ_SCL */
	{EXYNOS4_GPF0(5),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* LCD_FREQ_SDA */
	{EXYNOS4_GPF0(6),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPF0(7),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */

	{EXYNOS4_GPF1(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */

#if defined(CONFIG_QC_MODEM)
	{EXYNOS4_GPF1(1), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* MDM2AP_HSIC_READY */
#else
	{EXYNOS4_GPF1(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
#endif
	{EXYNOS4_GPF1(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
#if defined(CONFIG_QC_MODEM)
	{EXYNOS4_GPF1(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE}, /* TSP_VENDOR1 */
#else
	{EXYNOS4_GPF1(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
#endif
	{EXYNOS4_GPF1(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPF1(5),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
#if defined(CONFIG_QC_MODEM)
	{EXYNOS4_GPF1(6), S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* AP2MDM_STATUS */
#else
	{EXYNOS4_GPF1(6),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE}, /* TSP_VENDOR1 */
#endif
	{EXYNOS4_GPF1(7),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE}, /* TSP_VENDOR2 */

	{EXYNOS4_GPF2(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPF2(1),  S3C_GPIO_SLP_OUT0, S3C_GPIO_PULL_NONE}, /* VT_CAM_nSTBY */

#if defined(CONFIG_QC_MODEM)
	{EXYNOS4_GPF2(2),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* MDM2AP_HSIC_PWR_ACTIVE */
	{EXYNOS4_GPF2(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* WCN_PRIORITY */
	{EXYNOS4_GPF2(4),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* MDM_LTE_FRAME_SYNC */
#else
	{EXYNOS4_GPF2(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPF2(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPF2(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
#endif

	{EXYNOS4_GPF2(5),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
#if defined(CONFIG_QC_MODEM)
	{EXYNOS4_GPF2(6),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* BT_WAKE */
#else
	{EXYNOS4_GPF2(6),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
#endif	
	{EXYNOS4_GPF2(7),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */

#if defined(CONFIG_QC_MODEM)
	{EXYNOS4_GPF3(0),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* BUCK2_SEL */
	{EXYNOS4_GPF3(1),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* BUCK3_SEL */
	{EXYNOS4_GPF3(2),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* BUCK4_SEL */
	{EXYNOS4_GPF3(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */	
#else
	{EXYNOS4_GPF3(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPF3(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPF3(2),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* BUCK3_SEL */
	{EXYNOS4_GPF3(3),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* BUCK4_SEL */
#endif

	{EXYNOS4_GPF3(4),  S3C_GPIO_SLP_OUT0, S3C_GPIO_PULL_NONE}, /* MHL_RST */
	{EXYNOS4_GPF3(5),  S3C_GPIO_SLP_OUT0, S3C_GPIO_PULL_NONE}, /* MHL_INT */

	/* Exynos4212 specific gpio */
	{EXYNOS4212_GPJ0(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* CAM_PCLK */
#if defined(CONFIG_QC_MODEM)
	{EXYNOS4212_GPJ0(1),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_DOWN}, /*AP2MDM_ERR_FATAL*/
#else
	{EXYNOS4212_GPJ0(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
#endif
	{EXYNOS4212_GPJ0(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPJ0(3),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* SUB_MICBIAS_EN */
	{EXYNOS4212_GPJ0(4),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* EAR_SND_SEL */
	{EXYNOS4212_GPJ0(5),  S3C_GPIO_SLP_OUT0, S3C_GPIO_PULL_NONE}, /* CAM_EN2 */
	{EXYNOS4212_GPJ0(6),  S3C_GPIO_SLP_OUT0, S3C_GPIO_PULL_NONE}, /* CAM_EN1 */
	{EXYNOS4212_GPJ0(7),  S3C_GPIO_SLP_OUT0, S3C_GPIO_PULL_NONE}, /* 5M_nSTBY */

	{EXYNOS4212_GPJ1(0),  S3C_GPIO_SLP_OUT0, S3C_GPIO_PULL_NONE}, /* VT_CAM_nRST */
#if defined(CONFIG_SEC_MODEM) || defined(CONFIG_MACH_KONA_EUR_LTE)
	{EXYNOS4212_GPJ1(1),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* PS_ALS_EN */
#else
	{EXYNOS4212_GPJ1(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
#endif

	{EXYNOS4212_GPJ1(2),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* VPS_SOUND_EN */
	{EXYNOS4212_GPJ1(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* CAM_MCLK */
	{EXYNOS4212_GPJ1(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* MLH_WAKE_UP */

	{EXYNOS4_GPK0(0),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* NAND_CLK */
	{EXYNOS4_GPK0(1),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* NAND_CMD */
	{EXYNOS4_GPK0(2),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* eMMC_EN */
	{EXYNOS4_GPK0(3),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* NAND_D(0) */
	{EXYNOS4_GPK0(4),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* NAND_D(1) */
	{EXYNOS4_GPK0(5),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* NAND_D(2) */
	{EXYNOS4_GPK0(6),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* NAND_D(3) */

	{EXYNOS4_GPK1(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPK1(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPK1(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPK1(3),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* NAND_D(4) */
	{EXYNOS4_GPK1(4),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* NAND_D(5) */
	{EXYNOS4_GPK1(5),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* NAND_D(6) */
	{EXYNOS4_GPK1(6),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* NAND_D(7) */

	{EXYNOS4_GPK2(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPK2(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPK2(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPK2(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPK2(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPK2(5),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPK2(6),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},

	{EXYNOS4_GPK3(0),  S3C_GPIO_SLP_OUT0,  S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPK3(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPK3(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPK3(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPK3(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPK3(5),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPK3(6),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},

#if defined(CONFIG_SEC_MODEM)
	{EXYNOS4_GPL0(0),  S3C_GPIO_SLP_OUT0, S3C_GPIO_PULL_NONE}, /* ACTIVE_STATE_HSIC */
#elif defined(CONFIG_QC_MODEM)
	{EXYNOS4_GPL0(0),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_UP}, /* AP2MDM_PMIC_RESET_N */
#else
	{EXYNOS4_GPL0(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
#endif
	{EXYNOS4_GPL0(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE}, /* PS_ALS_SCL_1.8V */
	{EXYNOS4_GPL0(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE}, /* PS_ALS_SDA_1.8V */
	{EXYNOS4_GPL0(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC(IRDA_CONTROL) */
	{EXYNOS4_GPL0(4),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* HDMI_EN */
	{EXYNOS4_GPL0(6),  S3C_GPIO_SLP_PREV,  S3C_GPIO_PULL_NONE}, /* BT_EN */

#if defined(CONFIG_SEC_MODEM)
	{EXYNOS4_GPL1(0),  S3C_GPIO_SLP_OUT0, S3C_GPIO_PULL_NONE}, /* PDA_ACTIVE */
#else
	{EXYNOS4_GPL1(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
#endif
	{EXYNOS4_GPL1(1),  S3C_GPIO_SLP_OUT0, S3C_GPIO_PULL_NONE}, /* 5M_nRST */

	{EXYNOS4_GPL2(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPL2(1),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* IRDA_EN */

#if defined(CONFIG_QC_MODEM)
	{EXYNOS4_GPL2(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
#else
	{EXYNOS4_GPL2(2),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* GPS_EN */
#endif
	{EXYNOS4_GPL2(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPL2(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* MOTOR_EN -> NC */

#if defined(CONFIG_SEC_MODEM) || defined(CONFIG_QC_MODEM)
	{EXYNOS4_GPL2(5),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* AP2MDM_PON_RESET_N  , CP_ON*/
#else
	{EXYNOS4_GPL2(5),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
#endif

	{EXYNOS4_GPL2(6),  S3C_GPIO_SLP_INPUT,  S3C_GPIO_PULL_DOWN}, /*ACCESSORY_EN -> NC */
	{EXYNOS4_GPL2(7),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* UART_SEL -> NC */

#if defined(CONFIG_SEC_MODEM) || defined(CONFIG_QC_MODEM)
	{EXYNOS4212_GPM0(0),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* MICBIAS_EN */
#else
	{EXYNOS4212_GPM0(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* MICBIAS_EN */
#endif
	{EXYNOS4212_GPM0(1),  S3C_GPIO_SLP_OUT0, S3C_GPIO_PULL_NONE}, /* LED_BACKLIGHT_RESET */
	{EXYNOS4212_GPM0(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPM0(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPM0(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPM0(5),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* 2TOUCH_EN */
	{EXYNOS4212_GPM0(6),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE}, /* IRDA_IRQ */
	{EXYNOS4212_GPM0(7),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */

	{EXYNOS4212_GPM1(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE}, /* MOTOR_I2C_SDA */
	{EXYNOS4212_GPM1(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE}, /* MOTOR_I2C_SCL */
	{EXYNOS4212_GPM1(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE}, /* HW_REV0 */
	{EXYNOS4212_GPM1(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE}, /* HW_REV1 */
	{EXYNOS4212_GPM1(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE}, /* HW_REV2 */
	{EXYNOS4212_GPM1(5),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE}, /* HW_REV3 */
	{EXYNOS4212_GPM1(6),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */

	{EXYNOS4212_GPM2(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE}, /* IF_PMIC_SDA */
	{EXYNOS4212_GPM2(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE}, /* IF_PMIC_SCL */
	{EXYNOS4212_GPM2(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* VT_CAM_MCLK */
	{EXYNOS4212_GPM2(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE}, /* TSP_INT */
	 /*  SUSPEND_REQUEST_HSIC for 3G, AP2MDM_WAKEUP for LTE, NC */
	{EXYNOS4212_GPM2(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},

	{EXYNOS4212_GPM3(0),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* PMIC_DVS1 */
	{EXYNOS4212_GPM3(1),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* PMIC_DVS2 */
	{EXYNOS4212_GPM3(2),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* PMIC_DVS3 */
#if defined(CONFIG_SEC_MODEM)
	{EXYNOS4212_GPM3(3),  S3C_GPIO_SLP_OUT1, S3C_GPIO_PULL_NONE}, /* RESET_REQ_N */
	{EXYNOS4212_GPM3(4),  S3C_GPIO_SLP_OUT0, S3C_GPIO_PULL_DOWN}, /* AP_DUMP_INT */
#elif defined(CONFIG_QC_MODEM)
	{EXYNOS4212_GPM3(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* AP2MDM_SOFT_RESET */
	{EXYNOS4212_GPM3(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
#else
	{EXYNOS4212_GPM3(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPM3(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
#endif
	{EXYNOS4212_GPM3(5),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* WLAN_EN */
#if defined(CONFIG_SEC_MODEM)
	{EXYNOS4212_GPM3(6),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE}, /* CP_DUMP_INT */
#else
	{EXYNOS4212_GPM3(6),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
#endif
	{EXYNOS4212_GPM3(7),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */

	{EXYNOS4212_GPM4(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* ADC_I2C_SCL_1.8V -> NC */
	{EXYNOS4212_GPM4(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* ADC_I2C_SDA_1.8V -> NC */
	{EXYNOS4212_GPM4(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPM4(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPM4(4),  S3C_GPIO_SLP_PREV,  S3C_GPIO_PULL_NONE}, /* CODEC_LDO_EN */
	{EXYNOS4212_GPM4(5),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPM4(6),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC(USB_OTG_EN) */
	{EXYNOS4212_GPM4(7),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */

	{EXYNOS4_GPY0(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* IRDA_SDA */
	{EXYNOS4_GPY0(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* IRDA_SCL */
	{EXYNOS4_GPY0(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE}, /* FUEL_SDA_1.8V */
	{EXYNOS4_GPY0(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE}, /* FUEL_SDA_1.8V */
	{EXYNOS4_GPY0(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY0(5),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */

	{EXYNOS4_GPY1(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY1(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY1(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY1(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* PEN_FWE1_1.8V */
	{EXYNOS4_GPY2(0),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* TF_EN */
	{EXYNOS4_GPY2(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY2(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE}, /* MHL_SDA_1.8V */
	{EXYNOS4_GPY2(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE}, /* MHL_SCL_1.8V */
	{EXYNOS4_GPY2(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE}, /* MSENSE_SDA_1.8V */
	{EXYNOS4_GPY2(5),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE}, /* MSENSE_SCL_1.8V */

	{EXYNOS4_GPY3(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY3(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY3(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY3(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY3(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY3(5),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY3(6),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY3(7),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */

	{EXYNOS4_GPY4(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY4(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY4(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY4(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY4(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY4(5),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY4(6),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY4(7),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */

	{EXYNOS4_GPY5(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY5(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY5(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY5(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY5(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY5(5),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY5(6),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY5(7),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */

	{EXYNOS4_GPY6(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY6(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY6(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},  /* NC */
	{EXYNOS4_GPY6(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY6(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY6(5),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY6(6),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY6(7),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */

	{EXYNOS4_GPZ(0),   S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* MM_I2S_CLK */
	{EXYNOS4_GPZ(1),   S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPZ(2),   S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* MM_I2S_SYNC */
	{EXYNOS4_GPZ(3),   S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* MM_I2S_CLK */
	{EXYNOS4_GPZ(4),   S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* MM_I2S_CLK */
	{EXYNOS4_GPZ(5),   S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPZ(6),   S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */


	{EXYNOS4212_GPV0(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV0(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV0(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV0(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV0(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV0(5),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV0(6),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV0(7),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},

	{EXYNOS4212_GPV1(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV1(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV1(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV1(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV1(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV1(5),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV1(6),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV1(7),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},

	{EXYNOS4212_GPV2(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV2(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV2(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV2(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV2(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV2(5),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV2(6),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV2(7),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},

	{EXYNOS4212_GPV3(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV3(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV3(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV3(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV3(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV3(5),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV3(6),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV3(7),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},

	{EXYNOS4212_GPV4(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV4(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
}; /* kona_sleep_gpio_table */

struct kona_sleep_table {
	unsigned int (*ptr)[3];
	int size;
};

#define GPIO_TABLE(_ptr) \
	{.ptr = _ptr, \
	.size = ARRAY_SIZE(_ptr)} \

 #define GPIO_TABLE_NULL \
	{.ptr = NULL, \
	.size = 0} \

static struct kona_sleep_table kona_sleep_table[] = {
	GPIO_TABLE(kona_sleep_gpio_table),		/* Rev0.8(0x0) */
	GPIO_TABLE_NULL,
	GPIO_TABLE_NULL,
	GPIO_TABLE_NULL,
	GPIO_TABLE_NULL,
	GPIO_TABLE_NULL,
	GPIO_TABLE_NULL,
	GPIO_TABLE_NULL,
	GPIO_TABLE_NULL,
	GPIO_TABLE_NULL,
	GPIO_TABLE_NULL,
	GPIO_TABLE_NULL,
	GPIO_TABLE_NULL,
	GPIO_TABLE_NULL,
	GPIO_TABLE_NULL,
};
#endif /* CONFIG_MIDAS_COMMON */

static void config_sleep_gpio_table(int array_size,
				    unsigned int (*gpio_table)[3])
{
	u32 i, gpio;

	for (i = 0; i < array_size; i++) {
		gpio = gpio_table[i][0];
		s3c_gpio_slp_cfgpin(gpio, gpio_table[i][1]);
		s3c_gpio_slp_setpull_updown(gpio, gpio_table[i][2]);
	}
}

#ifdef CONFIG_MIDAS_COMMON
void kona_config_sleep_gpio_table(void)
{
	int i;
	int index = min(ARRAY_SIZE(kona_sleep_table), system_rev + 1);

	for (i = 0; i < index; i++) {
		if (kona_sleep_table[i].ptr == NULL)
			continue;

		config_sleep_gpio_table(kona_sleep_table[i].size,
				kona_sleep_table[i].ptr);
	}
}
#endif

/* To save power consumption, gpio pin set before enterling sleep */
void midas_config_sleep_gpio_table(void)
{
	kona_config_sleep_gpio_table();
}

/* Intialize gpio set in midas board */
void midas_config_gpio_table(void)
{
	u32 i, gpio;

	printk(KERN_DEBUG "%s\n", __func__);

	for (i = 0; i < ARRAY_SIZE(kona_init_gpios); i++) {
		gpio = kona_init_gpios[i].num;
		if (gpio <= EXYNOS4212_GPV4(1)) {
			s3c_gpio_cfgpin(gpio, kona_init_gpios[i].cfg);
			s3c_gpio_setpull(gpio, kona_init_gpios[i].pud);

			if (kona_init_gpios[i].val != S3C_GPIO_SETPIN_NONE)
				gpio_set_value(gpio, kona_init_gpios[i].val);

			s5p_gpio_set_drvstr(gpio, kona_init_gpios[i].drv);
		}
	}
}
