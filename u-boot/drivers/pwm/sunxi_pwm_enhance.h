/*
 * drivers/pwm/pwm-sunxi-enhance.h
 *
 * Allwinnertech pulse-width-modulation controller driver
 *
 * Copyright (C) 2018 AllWinner
 *
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */
#ifndef __PWM_SUNXI_NEW__H__
#define __PWM_SUNXI_NEW__H__

#define PWM_PIER	(0x0000)
#define PWM_PISR	(0x0004)
#define	PWM_CIER	(0x0010)
#define	PWM_CISR	(0x0014)
#define PWM_PCCR01	(0x0020)
#define PWM_PCCR23	(0x0024)
#define PWM_PCCR45	(0x0028)
#define PWM_PCCR67	(0x002c)
#define PWM_PDZCR01	(0x0030)
#define PWM_PDZCR23	(0x0034)
#define	PWM_PDZCR45	(0x0038)
#define	PWM_PDZCR67	(0x003c)
#define PWM_PER		(0x0040)
#define PWM_CER		(0x0044)

#define PWM_PCR_BASE	(0x0060 + 0x0000)
#define PWM_PPR_BASE	(0x0060 + 0x0004)
#define PWM_PCNTR_BASE	(0x0060 + 0x0008)
#define PWM_CCR_BASE	(0x0060 + 0x000c)
#define	PWM_CRLR_BASE	(0x0060 + 0x0010)
#define PWM_CFLR_BASE	(0x0060 + 0x0014)
#define PWM_PCCR8	(0x0300)

#define PWM_ACT_STA_SHIFT	0x8
#define PWM_ACT_STA_WIDTH	0x1
#define PWM_CLK_SRC_SHIFT	0x7
#define PWM_CLK_SRC_WIDTH	0x2
#define PWM_DIV_M_SHIFT		0x0
#define PWM_DIV_M_WIDTH		0x4
#define PWM_PRESCAL_SHIFT	0x0
#define PWM_PRESCAL_WIDTH	0x8
#define PWM_ACT_CYCLES_SHIFT	0x0
#define PWM_ACT_CYCLES_WIDTH	0x10
#define PWM_PERIOD_CYCLES_SHIFT	0x10
#define PWM_PERIOD_CYCLES_WIDTH	0x10
#define PWM_DZ_EN_SHIFT		0x0
#define PWM_DZ_EN_WIDTH		0x1
#define PWM_PDZINTV_SHIFT	0x8
#define PWM_PDZINTV_WIDTH	0x8
#define PWM_BYPASS_WIDTH	0x1
#define PWM_CLK_GATING_SHIFT	0x4
#define PWM_CLK_GATING_WIDTH	0x1

#define PWM_CCU_OFFSET		0x7ac
#define PWM_RCM_OFFSET		0x13c
#endif

