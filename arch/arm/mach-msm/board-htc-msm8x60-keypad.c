/* arch/arm/mach-msm/board-msm8x60-keypad.c
 *
 * Copyright (C) 2008 Google, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/platform_device.h>
#include <linux/gpio_event.h>
#include <linux/gpio.h>
#include <mach/gpio.h>
#include "board-htc-msm8x60.h"

static struct gpio_event_direct_entry msm8x60_keypad_switch_map[] = {
	{ MSM8X60_GPIO_KEY_POWER, 	KEY_POWER		},
	{ MSM8X60_GPIO_KEY_VOL_UP,	KEY_VOLUMEUP		},
	{ MSM8X60_GPIO_KEY_VOL_DOWN,	KEY_VOLUMEDOWN		},
	{ MSM8X60_GPIO_KEY_CAM_STEP1,	KEY_HP			},
	{ MSM8X60_GPIO_KEY_CAM_STEP2,	KEY_CAMERA		},
};

static void msm8x60_gpio_event_input_init(void)
{
	gpio_tlmm_config(GPIO_CFG(MSM8X60_GPIO_KEY_POWER, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_UP,
				GPIO_CFG_2MA), GPIO_CFG_ENABLE);
	gpio_tlmm_config(GPIO_CFG(MSM8X60_GPIO_KEY_VOL_UP, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_UP,
				GPIO_CFG_2MA), GPIO_CFG_ENABLE);
	gpio_tlmm_config(GPIO_CFG(MSM8X60_GPIO_KEY_VOL_DOWN, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_UP,
				GPIO_CFG_2MA), GPIO_CFG_ENABLE);
	gpio_tlmm_config(GPIO_CFG(MSM8X60_GPIO_KEY_CAM_STEP1, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_UP,
				GPIO_CFG_2MA), GPIO_CFG_ENABLE);
	gpio_tlmm_config(GPIO_CFG(MSM8X60_GPIO_KEY_CAM_STEP2, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_UP,
				GPIO_CFG_2MA), GPIO_CFG_ENABLE);

	enable_irq_wake(MSM_GPIO_TO_INT(MSM8X60_GPIO_KEY_VOL_UP));
	enable_irq_wake(MSM_GPIO_TO_INT(MSM8X60_GPIO_KEY_VOL_DOWN));
	enable_irq_wake(MSM_GPIO_TO_INT(MSM8X60_GPIO_KEY_POWER));
};

static struct gpio_event_input_info msm8x60_keypad_switch_info = {
	.info.func = gpio_event_input_func,
	.info.no_suspend = true,
	.flags = 0,
	.type = EV_KEY,
	.keymap = msm8x60_keypad_switch_map,
	.keymap_size = ARRAY_SIZE(msm8x60_keypad_switch_map)
};

static struct gpio_event_info *msm8x60_keypad_info[] = {
	&msm8x60_keypad_switch_info.info,
};

static int msm8x60_gpio_keypad_power(
		const struct gpio_event_platform_data *pdata, bool on)
{
	return 0;
};

static struct gpio_event_platform_data msm8x60_keypad_data = {
	.name = "msm8x60-keypad",
	.info = msm8x60_keypad_info,
	.info_count = ARRAY_SIZE(msm8x60_keypad_info),
	.power = msm8x60_gpio_keypad_power,
};

static struct platform_device msm8x60_gpio_keypad_device = {
	.name = GPIO_EVENT_DEV_NAME,
	.id = 1,
	.dev        = {
		.platform_data  = &msm8x60_keypad_data,
	},
};

static struct platform_device *msm8x60_input_devices[] __initdata = {
	&msm8x60_gpio_keypad_device,
};

void __init htc_msm8x60_init_keypad(void)
{
	msm8x60_gpio_event_input_init();
	platform_add_devices(msm8x60_input_devices, ARRAY_SIZE(msm8x60_input_devices));
};
