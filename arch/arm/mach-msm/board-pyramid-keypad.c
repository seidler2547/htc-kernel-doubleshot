/* arch/arm/mach-msm/board-pyramid-keypad.c
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
#include <linux/keyreset.h>
#include <mach/gpio.h>
#include "board-htc8x60.h"

/* Macros assume PMIC GPIOs start at 0 */
#define PM8058_GPIO_BASE			NR_MSM_GPIOS
#define PM8058_GPIO_PM_TO_SYS(pm_gpio)		(pm_gpio + PM8058_GPIO_BASE)
#define PM8058_GPIO_SYS_TO_PM(sys_gpio)		(sys_gpio - PM8058_GPIO_BASE)
#define PM8058_MPP_BASE			(PM8058_GPIO_BASE + PM8058_GPIOS)
#define PM8058_MPP_PM_TO_SYS(pm_gpio)		(pm_gpio + PM8058_MPP_BASE)
#define PM8058_MPP_SYS_TO_PM(sys_gpio)		(sys_gpio - PM8058_MPP_BASE)
#define PM8058_IRQ_BASE				(NR_MSM_IRQS + NR_GPIO_IRQS)

#define PM8901_GPIO_BASE			(PM8058_GPIO_BASE + \
						PM8058_GPIOS + PM8058_MPPS)
#define PM8901_GPIO_PM_TO_SYS(pm_gpio)		(pm_gpio + PM8901_GPIO_BASE)
#define PM8901_GPIO_SYS_TO_PM(sys_gpio)		(sys_gpio - PM901_GPIO_BASE)
#define PM8901_IRQ_BASE				(PM8058_IRQ_BASE + \
						NR_PMIC8058_IRQS)
static char *keycaps = "--qwerty";
#undef MODULE_PARAM_PREFIX
#define MODULE_PARAM_PREFIX "board_pyramid."
module_param_named(keycaps, keycaps, charp, 0);

static struct gpio_event_direct_entry pyramid_keypad_input_map[] = {
	{
		.gpio = HTC8X60_GPIO_KEY_POWER,
		.code = KEY_POWER,
	},
	{
		.gpio = PM8058_GPIO_PM_TO_SYS(PYRAMID_VOL_UP),
		.code = KEY_VOLUMEUP,
	},
	{
		.gpio = PM8058_GPIO_PM_TO_SYS(PYRAMID_VOL_DN),
		.code = KEY_VOLUMEDOWN,
	},
};

static void pyramid_gpio_event_input_init(void)
{
	gpio_tlmm_config(GPIO_CFG(HTC8X60_GPIO_KEY_POWER, 0, GPIO_CFG_INPUT,
				GPIO_CFG_PULL_UP, GPIO_CFG_4MA), GPIO_CFG_ENABLE);

	enable_irq_wake(MSM_GPIO_TO_INT(HTC8X60_GPIO_KEY_POWER));
}

static struct gpio_event_input_info pyramid_keypad_input_info = {
	.info.func = gpio_event_input_func,
	.info.no_suspend = true,
	.flags = GPIOEDF_PRINT_KEYS,
	.type = EV_KEY,
#if BITS_PER_LONG != 64 && !defined(CONFIG_KTIME_SCALAR)
	.debounce_time.tv.nsec = 5 * NSEC_PER_MSEC,
# else
	.debounce_time.tv64 = 5 * NSEC_PER_MSEC,
# endif
	.keymap = pyramid_keypad_input_map,
	.keymap_size = ARRAY_SIZE(pyramid_keypad_input_map),
};

static struct gpio_event_info *pyramid_keypad_info[] = {
	&pyramid_keypad_input_info.info,
};

static int pyramid_gpio_keypad_power(
		const struct gpio_event_platform_data *pdata, bool on)
{
	return 0;
}

static struct gpio_event_platform_data pyramid_keypad_data = {
	.name = "pyramid-keypad",
	.info = pyramid_keypad_info,
	.info_count = ARRAY_SIZE(pyramid_keypad_info),
	.power = pyramid_gpio_keypad_power,
};

static struct platform_device pyramid_keypad_input_device = {
	.name = GPIO_EVENT_DEV_NAME,
	.id = 0,
	.dev		= {
		.platform_data	= &pyramid_keypad_data,
	},
};

static struct keyreset_platform_data pyramid_reset_keys_pdata = {
	.keys_down = {
		KEY_POWER,
		KEY_VOLUMEDOWN,
		KEY_VOLUMEUP,
		0
	},
};

struct platform_device pyramid_reset_keys_device = {
	.name = KEYRESET_NAME,
	.dev.platform_data = &pyramid_reset_keys_pdata,
};

void __init htc8x60_init_keypad(void)
{
	if (platform_device_register(&pyramid_reset_keys_device))
		printk(KERN_WARNING "%s: register reset key fail\n", __func__);

	pyramid_gpio_event_input_init();
	platform_device_register(&pyramid_keypad_input_device);
}
