/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * drivers/dsp/common/imgdts.h
 *
 * Copyright (c) 2007-2025 Allwinnertech Co., Ltd.
 * Author: wujiayi <wujiayi@allwinnertech.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details
 *
 */

#ifndef __IMGDTS_H
#define __IMGDTS_H

#define DTS_CLOSE (0)
#define DTS_OPEN  (1)

struct dts_uart_pin_msg_t {
	unsigned int port;
	unsigned int port_num;
	unsigned int mul_sel;
};

struct dts_uart_msg_t {
	unsigned int status;
	unsigned int uart_port;
	struct dts_uart_pin_msg_t uart_pin_msg[2];
};

struct dts_gpio_int_t {
	unsigned int gpio_a;
	unsigned int gpio_b;
	unsigned int gpio_c;
	unsigned int gpio_d;
	unsigned int gpio_e;
	unsigned int gpio_f;
	unsigned int gpio_g;
};

/* dts msg about dsp */
struct dts_msg_t {
	/* dsp status */
	unsigned int dsp_status;
	/* uart */
	struct dts_uart_msg_t uart_msg;
	/* gpio int */
	struct dts_gpio_int_t gpio_int;
};

#endif

