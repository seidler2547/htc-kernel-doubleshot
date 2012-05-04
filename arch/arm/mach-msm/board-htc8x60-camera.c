/*
 * Camera file for HTC MSM8x60 devices
 */
#include <asm/mach-types.h>
#include <linux/gpio.h>
#include <linux/htc_flashlight.h>
#include <linux/i2c.h>
#include <linux/mfd/pmic8901.h>
#include <linux/spi/spi.h>
#include <mach/board.h>
#include <mach/board_htc.h>
#include <mach/msm_bus_board.h>
#include <mach/gpiomux.h>
#include "board-htc8x60.h"
#include "devices-msm8x60.h"
#include "devices.h"

unsigned engid;

#ifdef CONFIG_S5K3H1GX
static uint32_t camera_off_gpio_table[] = {
	GPIO_CFG(137, 0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),	/* CAM1_RST# */
	GPIO_CFG(138, 0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),	/* CAM2_RST# */
	GPIO_CFG(140, 0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),	/* CAM2_PWDN */
	GPIO_CFG(32, 0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA),	/* CAM_MCLK */
	GPIO_CFG(HTC8X60_CAM_I2C_SDA, 0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_4MA),	/* CAM_I2C_SDA */
	GPIO_CFG(HTC8X60_CAM_I2C_SCL, 0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_8MA),	/* CAM_I2C_SCL */
	GPIO_CFG(141, 0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),	/* CAM_SEL */
	GPIO_CFG(10, 0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),	/* CAM_CAM1_ID */
};

static uint32_t camera_on_gpio_table_workaround[] = {
	GPIO_CFG(HTC8X60_CAM_I2C_SDA, 0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_4MA),	/* CAM_I2C_SDA */
	GPIO_CFG(HTC8X60_CAM_I2C_SCL, 0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_8MA),	/* CAM_I2C_SCL */
};

static uint32_t camera_on_gpio_table[] = {
	GPIO_CFG(HTC8X60_CAM_I2C_SDA, 1, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_4MA),	/* CAM_I2C_SDA */
	GPIO_CFG(HTC8X60_CAM_I2C_SCL, 1, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_8MA),	/* CAM_I2C_SCL */
	GPIO_CFG(32, 1, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA),	/* CAM_MCLK */
	GPIO_CFG(137, 0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),	/* CAM1_RST# */
	GPIO_CFG(138, 0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),	/* CAM2_RST# */
	GPIO_CFG(140, 0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),	/* CAM2_PWDN */
	GPIO_CFG(141, 0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),	/* CAM_SEL */
	GPIO_CFG(10, 0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),	/* CAM_CAM1_ID */
};
#endif

#ifdef CONFIG_QS_S5K4E1
static uint32_t camera_off_gpio_table_liteon[] = {
	GPIO_CFG(HTC8X60_CAM_I2C_SDA, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA),/*i2c*/
	GPIO_CFG(HTC8X60_CAM_I2C_SCL, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA),/*i2c*/
	GPIO_CFG(HTC8X60_SP3D_MCLK, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),/*MCLK*/
	GPIO_CFG(106, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),/*sharp INT*/
	GPIO_CFG(HTC8X60_SP3D_SPI_DO, 0, GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(HTC8X60_SP3D_SPI_DI, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
	GPIO_CFG(HTC8X60_SP3D_SPI_CS, 0, GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(HTC8X60_SP3D_SPI_CLK, 0, GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(HTC8X60_S5K4E1_VCM_PD, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(HTC8X60_S5K4E1_INTB, 0, GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(HTC8X60_S5K4E1_PD, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(HTC8X60_WEBCAM_RST, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),/*camera reset*/
	GPIO_CFG(HTC8X60_WEBCAM_STB, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),/*camera standby*/
	GPIO_CFG(HTC8X60_CAM_SEL, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),/*camera switch*/
};

static uint32_t camera_on_gpio_table_liteon[] = {
	GPIO_CFG(HTC8X60_CAM_I2C_SDA, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA),
	GPIO_CFG(HTC8X60_CAM_I2C_SCL, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA),
	GPIO_CFG(HTC8X60_SP3D_MCLK, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_6MA),/*MCLK*/
	GPIO_CFG(106, 0, GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(HTC8X60_SP3D_SPI_DO,  0, GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(HTC8X60_SP3D_SPI_DI,  0, GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(HTC8X60_SP3D_SPI_CS,  0, GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(HTC8X60_SP3D_SPI_CLK, 0, GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(HTC8X60_S5K4E1_VCM_PD, 0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_8MA),
	GPIO_CFG(HTC8X60_S5K4E1_INTB, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_8MA),
	GPIO_CFG(HTC8X60_S5K4E1_PD, 0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_8MA),
	GPIO_CFG(HTC8X60_WEBCAM_RST, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(HTC8X60_WEBCAM_STB, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(HTC8X60_CAM_SEL, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
};
#endif

#ifdef CONFIG_SP3D
static uint32_t camera_off_gpio_table_sp3d[] = {
	GPIO_CFG(HTC8X60_CAM_I2C_SDA, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA),/*i2c*/
	GPIO_CFG(HTC8X60_CAM_I2C_SCL, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA),/*i2c*/
	GPIO_CFG(HTC8X60_SP3D_MCLK, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),/*MCLK*/
	GPIO_CFG(106, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),/*sharp INT*/
	GPIO_CFG(HTC8X60_SP3D_SPI_DO, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(HTC8X60_SP3D_SPI_DI, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
	GPIO_CFG(HTC8X60_SP3D_SPI_CS, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(HTC8X60_SP3D_SPI_CLK, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(HTC8X60_SP3D_GATE, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(HTC8X60_SP3D_CORE_GATE, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(HTC8X60_SP3D_SYS_RST, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(HTC8X60_SP3D_PDX, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(HTC8X60_WEBCAM_RST, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),/*camera reset*/
	GPIO_CFG(HTC8X60_WEBCAM_STB, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),/*camera standby*/
	GPIO_CFG(HTC8X60_CAM_SEL, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),/*camera switch*/
};

static uint32_t camera_on_gpio_table_sp3d[] = {
	GPIO_CFG(HTC8X60_CAM_I2C_SDA, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA),
	GPIO_CFG(HTC8X60_CAM_I2C_SCL, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA),
	GPIO_CFG(HTC8X60_SP3D_MCLK, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_6MA),/*MCLK*/
	GPIO_CFG(106, 0, GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(HTC8X60_SP3D_SPI_DO,  1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(HTC8X60_SP3D_SPI_DI,  1, GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(HTC8X60_SP3D_SPI_CS,  1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(HTC8X60_SP3D_SPI_CLK, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(HTC8X60_SP3D_GATE, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(HTC8X60_SP3D_CORE_GATE, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(HTC8X60_SP3D_SYS_RST, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(HTC8X60_SP3D_PDX, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(HTC8X60_WEBCAM_RST, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(HTC8X60_WEBCAM_STB, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(HTC8X60_CAM_SEL, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
};

static uint32_t sp3d_spi_gpio[] = {
	/* or this? the i/o direction and up/down are much more correct */
	GPIO_CFG(HTC8X60_SP3D_SPI_DO,  1, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_8MA),
	GPIO_CFG(HTC8X60_SP3D_SPI_DI,  1, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_8MA),
	GPIO_CFG(HTC8X60_SP3D_SPI_CS,  1, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_8MA),
	GPIO_CFG(HTC8X60_SP3D_SPI_CLK, 1, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_8MA),
};
#endif

static int camera_sensor_power_enable(char *power, unsigned volt)
{
	struct regulator *sensor_power;
	int rc;

	if (power == NULL)
		return -ENODEV;

	sensor_power = regulator_get(NULL, power);
	if (IS_ERR(sensor_power)) {
		pr_err("[Camera]%s: Unable to get %s\n", __func__, power);
		return -ENODEV;
	}
	rc = regulator_set_voltage(sensor_power, volt, volt);
	if (rc) {
		pr_err("[Camera]%s: unable to set %s voltage to %d rc:%d\n",
			__func__, power, volt, rc);
		regulator_put(sensor_power);
		return -ENODEV;
	}
	rc = regulator_enable(sensor_power);
	if (rc) {
		pr_err("[Camera]%s: Enable regulator %s failed\n", __func__, power);
		regulator_put(sensor_power);
		return -ENODEV;
	}
	regulator_put(sensor_power);
	return rc;
}

#if defined(CONFIG_MACH_SHOOTER) || defined(CONFIG_MACH_SHOOTER_U)
static int camera_sensor_power_enable_8901(char *power){
	struct regulator *sensor_power;
	int rc;
	pr_info("%s %s",__func__,power);
	if (power == NULL)
		return -ENODEV;

	sensor_power = regulator_get(NULL, power);
	if (IS_ERR(sensor_power)) {
		pr_err("[Camera]%s: Unable to get %s\n", __func__, power);
		return -ENODEV;
	}
	rc = regulator_enable(sensor_power);
	if (rc) {
		pr_err("[Camera]%s: Enable regulator %s failed\n", __func__, power);
		regulator_put(sensor_power);
		return -ENODEV;
	}
	regulator_put(sensor_power);
	return rc;
}
#endif

static int camera_sensor_power_disable(char *power)
{
	struct regulator *sensor_power;
	int rc;
	if (power == NULL)
		return -ENODEV;

	sensor_power = regulator_get(NULL, power);
	if (IS_ERR(sensor_power)) {
		pr_err("[Camera]%s: Unable to get %s\n", __func__, power);
		return -ENODEV;
	}
	rc = regulator_disable(sensor_power);
	if (rc) {
		pr_err("[Camera]%s: Enable regulator %s failed\n", __func__, power);
		regulator_put(sensor_power);
		return -ENODEV;
	}
	regulator_put(sensor_power);
	return rc;
}

#ifdef CONFIG_MACH_PYRAMID
static int htc8x60_pyramid_vreg_off(void)
{
	int rc;
	pr_info("[Camera]%s\n", __func__);
	/* main / 2nd camera digital power */
	rc = camera_sensor_power_disable("8058_l9");
	pr_info("[Camera]sensor_power disable(\"8058_l9\") == %d\n", rc);

	/* main / 2nd camera analog power */
	rc = camera_sensor_power_disable("8058_l15");
	pr_info("[Camera]sensor_power_disable(\"8058_l15\") == %d\n", rc);

	/* IO power off */
	rc = camera_sensor_power_disable("8058_l12");
	pr_info("[Camera]sensor_power_disable(\"8058_l12\") == %d\n", rc);

	/* main camera VCM power */
	rc = camera_sensor_power_disable("8058_l10");
	pr_info("[Camera]sensor_power_disable(\"8058_l10\") == %d\n", rc);

	mdelay(20);

	return rc;
}

static int htc8x60_pyramid_vreg_on(void)
{
	static int first_run = 1;
	int rc;
	pr_info("[Camera]%s\n", __func__);
	/* Work-around for PYD power issue */
	if (first_run == 1) {
		first_run = 0;

		msm8x60_config_gpio_table(camera_on_gpio_table_workaround,
					ARRAY_SIZE(camera_on_gpio_table_workaround));

		mdelay(10);

		/* main camera VCM power */
		rc = camera_sensor_power_enable("8058_l10", 2850000);
		pr_info("[Camera]sensor_power_enable(\"8058_l10\", 2850) == %d\n", rc);
		/*IO*/
		rc = camera_sensor_power_enable("8058_l12", 1800000);
		pr_info("[Camera]sensor_power_enable(\"8058_l12\", 1800) == %d\n", rc);
		udelay(50);
		/* main / 2nd camera analog power */
		rc = camera_sensor_power_enable("8058_l15", 2800000);
		pr_info("[Camera]sensor_power_enable(\"8058_l15\", 2850) == %d\n", rc);
		udelay(50);
		/* main / 2nd camera digital power */
		rc = camera_sensor_power_enable("8058_l9", 1800000);
		pr_info("[Camera]sensor_power_enable(\"8058_l9\", 1800) == %d\n", rc);

		mdelay(20);
		pr_info("[Camera]call htc8x60_pyramid_vreg_off() at first boot up !!!\n");
		htc8x60_pyramid_vreg_off();
	}

	/* main camera VCM power */
	rc = camera_sensor_power_enable("8058_l10", 2850000);
	pr_info("[Camera]sensor_power_enable(\"8058_l10\", 2850) == %d\n", rc);
	/*IO*/
	rc = camera_sensor_power_enable("8058_l12", 1800000);
	pr_info("[Camera]sensor_power_enable(\"8058_l12\", 1800) == %d\n", rc);
	udelay(50);
	/* main / 2nd camera analog power */
	rc = camera_sensor_power_enable("8058_l15", 2800000);
	pr_info("[Camera]sensor_power_enable(\"8058_l15\", 2850) == %d\n", rc);
	udelay(50);
	/* main / 2nd camera digital power */
	rc = camera_sensor_power_enable("8058_l9", 1800000);
	pr_info("[Camera]sensor_power_enable(\"8058_l9\", 1800) == %d\n", rc);

	mdelay(1);

	return rc;
}
#endif

#ifdef CONFIG_SP3D
static int htc8x60_sp3d_vreg_on(void)
{
	int rc;
	pr_info("[Camera]%s\n", __func__);
	/* VDDIO*/
	if(system_rev == 2 && engid >= 3){/*VERSION A*/
		rc = camera_sensor_power_enable_8901("8901_lvs2");
		pr_info("[Camera]sensor_power_enable(\"8901_lvs2\", 1800) == %d\n", rc);
		udelay(26);
		/*DVDD18 */
		rc = camera_sensor_power_enable_8901("8901_lvs3");
		pr_info("[Camera]sensor_power_enable(\"8901_lvs3\", 1800) == %d\n", rc);
	}else{
		rc = camera_sensor_power_enable("8058_l8", 1800000);
		pr_info("[Camera]sensor_power_enable(\"8058_l8\", 1800) == %d\n", rc);
		udelay(26);
		/*DVDD18 */
		rc = camera_sensor_power_enable("8058_l9", 1800000);
		pr_info("[Camera]sensor_power_enable(\"8058_l9\", 1800) == %d\n", rc);
	}

	gpio_set_value(HTC8X60_SP3D_CORE_GATE, 1); // CORE GATE
	gpio_set_value(HTC8X60_SP3D_SYS_RST, 1); // RST
	gpio_set_value(HTC8X60_SP3D_PDX, 1); // PDX
	gpio_set_value(HTC8X60_SP3D_GATE, 1); // GATE
	/* main camera AVDD */
	rc = camera_sensor_power_enable("8058_l15", 2800000);
	pr_info("[Camera]sensor_power_enable(\"8058_l15\", 2800) == %d\n", rc);
	/* main camera MVDD */
	rc = camera_sensor_power_enable("8058_l10", 2800000);
	pr_info("[Camera]sensor_power_enable(\"8058_l10\", 2800) == %d\n", rc);
	gpio_tlmm_config(sp3d_spi_gpio[0], GPIO_CFG_ENABLE);
	gpio_tlmm_config(sp3d_spi_gpio[1], GPIO_CFG_ENABLE);
	gpio_tlmm_config(sp3d_spi_gpio[2], GPIO_CFG_ENABLE);
	gpio_tlmm_config(sp3d_spi_gpio[3], GPIO_CFG_ENABLE);
	return rc;
}

static int htc8x60_sp3d_vreg_off(void)
{
	int rc;
	pr_info("[Camera]%s\n", __func__);
	gpio_set_value(HTC8X60_SP3D_PDX, 0); // PDX
	/* main camera MVDD */
	rc = camera_sensor_power_disable("8058_l10");
	udelay(10);

	if (!(engid == 7)) {	//according to logic Jason Kao, only tutn off l15 when sharp
	/* main camera AVDD */
		rc = camera_sensor_power_disable("8058_l15");
		udelay(10);
	}
	if(system_rev == 2 && engid >= 3){
		/* main camera DVDD18 */
		rc = camera_sensor_power_disable("8901_lvs3");
		gpio_set_value(HTC8X60_SP3D_SYS_RST, 0); // RST
		gpio_set_value(HTC8X60_SP3D_CORE_GATE, 0); // CORE GATE
		gpio_set_value(HTC8X60_SP3D_GATE, 0); // GATE
		/*VDDIO*/
		rc = camera_sensor_power_disable("8901_lvs2");
	}else{
		/* main camera DVDD18 */
		rc = camera_sensor_power_disable("8058_l9");
		gpio_set_value(HTC8X60_SP3D_SYS_RST, 0); // RST
		gpio_set_value(HTC8X60_SP3D_CORE_GATE, 0); // CORE GATE
		gpio_set_value(HTC8X60_SP3D_GATE, 0); // GATE
		/*VDDIO*/
		rc = camera_sensor_power_disable("8058_l8");
	}

	return rc;
}
#endif

#ifdef CONFIG_QS_S5K4E1
static int htc8x60_qs_s5k4e1_vreg_on(void)
{
	int rc;
	pr_info("[Camera]%s\n", __func__);
	mdelay(50);

	rc = camera_sensor_power_enable("8058_l15", 2800000);
	pr_info("[Camera]sensor_power_enable(\"8058_l15\", 1800) == %d\n", rc);
	udelay(50);

	if(system_rev >= 2 && engid >= 3){/*VERSION A*/
		/*IO*//*This is switch power*/
		rc = camera_sensor_power_enable_8901("8901_lvs3");
		pr_info("[Camera]sensor_power_enable(\"8901_lvs3\", 1800) == %d\n", rc);
		mdelay(1);
		
		rc = camera_sensor_power_enable_8901("8901_lvs2");
		pr_info("[Camera]sensor_power_enable(\"8901_lvs2\", 1800) == %d\n", rc);
	}else {
		rc = camera_sensor_power_enable("8058_l9", 1800000);
		pr_info("[Camera]sensor_power_enable(\"8058_l9\", 1800) == %d\n", rc);
		/* VDDIO*/
		rc = camera_sensor_power_enable("8058_l8", 1800000);
		pr_info("[Camera]sensor_power_enable(\"8058_l8\", 1800) == %d\n", rc);
	}
	udelay(50);

	/* main camera AVDD */
	rc = camera_sensor_power_enable("8058_l10", 2800000);
	pr_info("[Camera]sensor_power_enable(\"8058_l10\", 2800) == %d\n", rc);
	udelay(50);

	gpio_set_value(HTC8X60_S5K4E1_INTB, 1);
	gpio_set_value(HTC8X60_S5K4E1_PD, 1);
	gpio_set_value(HTC8X60_S5K4E1_VCM_PD, 1);

	return rc;
}

static int htc8x60_qs_s5k4e1_vreg_off(void)
{
	int rc;
	pr_info("[Camera]%s\n", __func__);
	gpio_set_value(HTC8X60_S5K4E1_INTB, 0); // interrupt
	gpio_set_value(HTC8X60_S5K4E1_VCM_PD, 0); // PDX
	gpio_set_value(HTC8X60_S5K4E1_PD, 0); // RST

	/* main camera AVDD */
	rc = camera_sensor_power_disable("8058_l10");
	udelay(50);
	
	/*VDDIO*/
	if(system_rev >= 2 && engid >= 3){/*VERSION A*/
		rc = camera_sensor_power_disable("8901_lvs2");
		/*This is swich power*/
		rc = camera_sensor_power_disable("8901_lvs3");
		pr_info("[Camera]sensor_power_enable(\"8901_lvs3\", 1800) == %d\n", rc);
	}else{
		/*This is swich power*/
		rc = camera_sensor_power_disable("8058_l9");
		pr_info("[Camera]sensor_power_disable(\"8058_l9\") == %d\n", rc);
		rc = camera_sensor_power_disable("8058_l8");
		pr_info("[Camera]sensor_power_disable(\"8058_l8\") == %d\n", rc);
	}

	//according to logic Jason Kao, do not turn off l15 to avoid current leakage
	if (!(engid == 7)) 
	{
		rc = camera_sensor_power_disable("8058_l15");
		udelay(50);
	}
	return rc;
}
#endif

#ifdef CONFIG_S5K6AAFX
static int htc8x60_s5k6aafx_vreg_on(void)
{
	int rc;
	pr_info("[Camera]%s\n", __func__);
	/* main / 2nd camera analog power */
	rc = camera_sensor_power_enable("8058_l3", 2850000);
	pr_info("[Camera]sensor_power_enable(\"8058_l3\", 2850) == %d\n", rc);
	mdelay(5);
	/* main / 2nd camera digital power */
	if(system_rev == 2 && engid >= 3){
		rc = camera_sensor_power_enable_8901("8901_lvs2");
		pr_info("[Camera]sensor_power_enable(\"8901_lvs2\", 1800) == %d\n", rc);
		mdelay(5);
		/*IO*/
		rc = camera_sensor_power_enable_8901("8901_lvs3");
		pr_info("[Camera]sensor_power_enable(\"8901_lvs3\", 1800) == %d\n", rc);
		mdelay(1);
		
	}else{
		rc = camera_sensor_power_enable("8058_l8", 1800000);
		pr_info("[Camera]sensor_power_enable(\"8058_l8\", 1800) == %d\n", rc);
		mdelay(5);
		/*IO*/
		rc = camera_sensor_power_enable("8058_l9", 1800000);
		pr_info("[Camera]sensor_power_enable(\"8058_l9\", 1800) == %d\n", rc);
		mdelay(1);
	}
	return rc;
}

static int htc8x60_s5k6aafx_vreg_off(void)
{
	int rc;
	pr_info("[Camera]%s\n", __func__);
	/* IO power off */
	if(system_rev == 2 && engid >= 3){
		rc = camera_sensor_power_disable("8901_lvs3");
		pr_info("[Camera]sensor_power_disable(\"8901_lvs3\") == %d\n", rc);
		mdelay(1);

		/* main / 2nd camera digital power */
		rc = camera_sensor_power_disable("8901_lvs2");
		pr_info("[Camera]sensor_power_disable(\"8901_lvs2\") == %d\n", rc);
	}else{
		rc = camera_sensor_power_disable("8058_l9");
		pr_info("[Camera]sensor_power_disable(\"8058_l9\") == %d\n", rc);
		mdelay(1);

		/* main / 2nd camera digital power */
		rc = camera_sensor_power_disable("8058_l8");
		pr_info("[Camera]sensor_power_disable(\"8058_l8\") == %d\n", rc);
	}
	mdelay(1);
	/* main / 2nd camera analog power */
	rc = camera_sensor_power_disable("8058_l3");
	pr_info("[Camera]sensor_power_disable(\"8058_l3\") == %d\n", rc);
	return rc;
}
#endif

static void htc8x60_maincam_clk_switch(void)
{
	pr_info("[Camera]Doing clk switch (Main Cam)\n");
	gpio_set_value(HTC8X60_CAM_SEL, 0);
}

static void htc8x60_seccam_clk_switch(void)
{
	pr_info("[Camera]Doing clk switch (2nd Cam)\n");
	gpio_set_value(HTC8X60_CAM_SEL, 1);
}

static int htc8x60_config_camera_on_gpios(void)
{
#if defined(CONFIG_MACH_SHOOTER) || defined(CONFIG_MACH_SHOOTER_U)
	if (engid == 7) {
		msm8x60_config_gpio_table(camera_on_gpio_table_liteon,
					ARRAY_SIZE(camera_on_gpio_table_liteon));
	} else {
		msm8x60_config_gpio_table(camera_on_gpio_table_sp3d,
					ARRAY_SIZE(camera_on_gpio_table_sp3d));
	}
#else
	msm8x60_config_gpio_table(camera_on_gpio_table,
				ARRAY_SIZE(camera_on_gpio_table));
#endif
	return 0;
}

static void htc8x60_config_camera_off_gpios(void)
{
#if defined(CONFIG_MACH_SHOOTER) || defined(CONFIG_MACH_SHOOTER_U)
	if (engid == 7) {
		msm8x60_config_gpio_table(camera_off_gpio_table_liteon,
			ARRAY_SIZE(camera_off_gpio_table_liteon));
	} else {
		msm8x60_config_gpio_table(camera_off_gpio_table_sp3d,
			ARRAY_SIZE(camera_off_gpio_table_sp3d));
	}
	gpio_set_value(HTC8X60_SP3D_SPI_DO, 0);
	gpio_set_value(HTC8X60_SP3D_SPI_CS, 0);
	gpio_set_value(HTC8X60_SP3D_SPI_CLK, 0);
	gpio_set_value(HTC8X60_SP3D_MCLK, 0);
	gpio_set_value(HTC8X60_CAM_SEL, 0);
#else
	msm8x60_config_gpio_table(camera_off_gpio_table,
				ARRAY_SIZE(camera_off_gpio_table));
#endif
}

static struct msm_camera_device_platform_data msm_camera_device_data = {
	.camera_gpio_on  = htc8x60_config_camera_on_gpios,
	.camera_gpio_off = htc8x60_config_camera_off_gpios,
	.ioext.csiphy = 0x04800000,
	.ioext.csisz  = 0x00000400,
	.ioext.csiirq = CSI_0_IRQ,
	.ioclk.mclk_clk_rate = 24000000,
	.ioclk.vfe_clk_rate  = 228570000,
};

static struct msm_camera_device_platform_data msm_camera_device_data_web_cam = {
	.camera_gpio_on  = htc8x60_config_camera_on_gpios,
	.camera_gpio_off = htc8x60_config_camera_off_gpios,
	.ioext.csiphy = 0x04900000,
	.ioext.csisz  = 0x00000400,
	.ioext.csiirq = CSI_1_IRQ,
	.ioclk.mclk_clk_rate = 24000000,
	.ioclk.vfe_clk_rate  = 228570000,
};

static struct resource msm_camera_resources[] = {
	{
		.start	= 0x04500000,
		.end	= 0x04500000 + SZ_1M - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= VFE_IRQ,
		.end	= VFE_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
};

#ifdef CONFIG_MACH_PYRAMID
int aat1271_flashlight_control(int mode);
static int flashlight_control(int mode)
{
#ifdef CONFIG_FLASHLIGHT_AAT1271
	return aat1271_flashlight_control(mode);
#else
	return 0;
#endif
}
#endif /* CONFIG_MACH_PYRAMID */

static struct msm_camera_sensor_flash_src msm_flash_src = {
#ifdef CONFIG_MACH_PYRAMID
	.flash_sr_type			= MSM_CAMERA_FLASH_SRC_CURRENT_DRIVER,
	.camera_flash			= flashlight_control,
#else
	.flash_sr_type			= MSM_CAMERA_FLASH_SRC_PWM,
	._fsrc.pwm_src.freq		= 1000,
	._fsrc.pwm_src.max_load		= 300,
	._fsrc.pwm_src.low_load		= 30,
	._fsrc.pwm_src.high_load	= 100,
	._fsrc.pwm_src.channel		= 7,
#endif
};

static struct camera_flash_cfg msm_camera_sensor_flash_cfg = {
#ifdef CONFIG_MACH_MSM_PYRAMID
	.low_temp_limit	= 5,
	.low_cap_limit	= 30,
#else
	.low_temp_limit	= 5,
	.low_cap_limit	= 5,
#endif
};

#ifdef CONFIG_S5K3H1GX
static struct msm_camera_sensor_flash_data flash_s5k3h1gx = {
	.flash_type		= MSM_CAMERA_FLASH_LED,
	.flash_src		= &msm_flash_src,
};

static struct msm_camera_sensor_info msm_camera_sensor_s5k3h1gx_data = {
	.sensor_name		= "s5k3h1gx",
	.sensor_reset		= 137, /* Main Cam RST */
	.sensor_pwd		= 137, /* Main Cam PWD */
	.vcm_pwd		= 58, /* VCM_PD */
	.vcm_enable		= 0,
	.camera_power_on	= htc8x60_pyramid_vreg_on,
	.camera_power_off	= htc8x60_pyramid_vreg_off,
	.camera_clk_switch	= htc8x60_maincam_clk_switch,
	.pdata			= &msm_camera_device_data,
	.resource		= msm_camera_resources,
	.num_resources		= ARRAY_SIZE(msm_camera_resources),
	.flash_data		= &flash_s5k3h1gx,
	.flash_cfg		= &msm_camera_sensor_flash_cfg,
	.power_down_disable	= false, /* true: disable pwd down function */
	.mirror_mode		= 1,
	.cam_select_pin		= HTC8X60_CAM_SEL,
	.csi_if			= 1,
	.dev_node		= 0
};

static struct platform_device msm_camera_sensor_s5k3h1gx = {
	.name	= "msm_camera_s5k3h1gx",
	.dev	= {
		.platform_data = &msm_camera_sensor_s5k3h1gx_data,
	},
};
#endif

#ifdef CONFIG_MT9V113
static struct msm_camera_sensor_flash_data flash_mt9v113 = {
	.flash_type	= MSM_CAMERA_FLASH_NONE,
};

static struct msm_camera_sensor_info msm_camera_sensor_mt9v113_data = {
	.sensor_name		= "mt9v113",
	.sensor_reset		= 138, /* 2nd Cam RST */
	.sensor_pwd		= 140, /* 2nd Cam PWD */
	.camera_clk_switch	= htc8x60_seccam_clk_switch,
	.camera_power_on	= htc8x60_pyramid_vreg_on,
	.camera_power_off	= htc8x60_pyramid_vreg_off,
	.pdata			= &msm_camera_device_data_web_cam,
	.resource		= msm_camera_resources,
	.num_resources		= ARRAY_SIZE(msm_camera_resources),
	.flash_data		= &flash_mt9v113,
	.power_down_disable	= false, /* true: disable pwd down function */
	.mirror_mode		= 1,
	.cam_select_pin		= HTC8X60_CAM_SEL,
	.csi_if			= 1,
	.dev_node		= 1
};

/* HTC named this msm_camera_webcam, not sure why */
static struct platform_device msm_camera_sensor_webcam = {
	.name	= "msm_camera_webcam",
	.dev	= {
		.platform_data = &msm_camera_sensor_mt9v113_data,
	}
};
#endif

#ifdef CONFIG_SP3D
static struct spi_board_info sp3d_spi_board_info[] __initdata = {
	{
		.modalias	= "sp3d_spi",
		.mode		= SPI_MODE_3,
		.bus_num	= 1,
		.chip_select	= 0,
		.max_speed_hz	= 15060000,
	}
};

static struct msm_camera_sensor_flash_data flash_sp3d = {
	.flash_type		= MSM_CAMERA_FLASH_LED,
	.flash_src		= &msm_flash_src
};

static struct msm_camera_sensor_info msm_camera_sensor_sp3d_data = {
	.sensor_name	= "sp3d",
	.vcm_enable		= 0,
	.camera_power_on = htc8x60_sp3d_vreg_on,
	.camera_power_off = htc8x60_sp3d_vreg_off,
	.camera_clk_switch = htc8x60_maincam_clk_switch,
	.pdata			= &msm_camera_device_data,
	.resource		= msm_camera_resources,
	.num_resources	= ARRAY_SIZE(msm_camera_resources),
	.flash_data		= &flash_sp3d,
	.flash_cfg = &msm_camera_sensor_flash_cfg,
	.stereo_low_cap_limit = 15,
	.mirror_mode = 0,
	.csi_if		= 1,
	.dev_node	= 0
};

struct platform_device msm_camera_sensor_sp3d = {
	.name	= "msm_camera_sp3d",
	.dev	= {
		.platform_data = &msm_camera_sensor_sp3d_data,
	},
};
#endif

#ifdef CONFIG_QS_S5K4E1
static char eeprom_data[864];
static struct msm_camera_sensor_flash_data flash_qs_s5k4e1 = {
	.flash_type		= MSM_CAMERA_FLASH_LED,
	.flash_src		= &msm_flash_src
};

static struct msm_camera_sensor_info msm_camera_sensor_qs_s5k4e1_data = {
	.sensor_name	= "qs_s5k4e1",
	.sensor_reset	= HTC8X60_S5K4E1_PD,
	.vcm_enable		= 0,
	.camera_power_on = htc8x60_qs_s5k4e1_vreg_on,
	.camera_power_off = htc8x60_qs_s5k4e1_vreg_off,
	.camera_clk_switch = htc8x60_maincam_clk_switch,
	.pdata			= &msm_camera_device_data,
	.resource		= msm_camera_resources,
	.num_resources	= ARRAY_SIZE(msm_camera_resources),
	.flash_data		= &flash_qs_s5k4e1,
	.flash_cfg = &msm_camera_sensor_flash_cfg,
	.stereo_low_cap_limit = 15,
	.csi_if			= 1,
	.dev_node		= 0,
	.eeprom_data		= eeprom_data,
};

struct platform_device msm_camera_sensor_qs_s5k4e1 = {
	.name	= "msm_camera_qs_s5k4e1",
	.dev	= {
		.platform_data = &msm_camera_sensor_qs_s5k4e1_data,
	},
};
#endif

#ifdef CONFIG_S5K6AAFX
static struct msm_camera_sensor_flash_data flash_s5k6aafx = {
	.flash_type		= MSM_CAMERA_FLASH_NONE,
};

static struct msm_camera_sensor_info msm_camera_sensor_s5k6aafx_data = {
	.sensor_name	= "s5k6aafx",
	.sensor_reset	= HTC8X60_WEBCAM_RST,/*2nd Cam RST*/
	.sensor_pwd		= HTC8X60_WEBCAM_STB,/*2nd Cam PWD*/
	.vcm_enable		= 0,
	.camera_power_on = htc8x60_s5k6aafx_vreg_on,
	.camera_power_off = htc8x60_s5k6aafx_vreg_off,
	.camera_clk_switch = htc8x60_seccam_clk_switch,
	.pdata			= &msm_camera_device_data_web_cam,
	.resource		= msm_camera_resources,
	.num_resources	= ARRAY_SIZE(msm_camera_resources),
	.flash_data             = &flash_s5k6aafx,
	.mirror_mode = 0,
	.csi_if		= 1,
	.dev_node	= 1,
};

struct platform_device msm_camera_sensor_s5k6aafx = {
	.name	= "msm_camera_s5k6aafx",
	.dev	= {
		.platform_data = &msm_camera_sensor_s5k6aafx_data,
	},
};
#endif

void __init htc8x60_init_cam(unsigned eid)
{
	engid = eid;

#ifdef CONFIG_S5K6AAFX
	platform_device_register(&msm_camera_sensor_s5k6aafx);
#endif
#ifdef CONFIG_SP3D
	spi_register_board_info(sp3d_spi_board_info,
			ARRAY_SIZE(sp3d_spi_board_info));
	platform_device_register(&msm_camera_sensor_sp3d);
#endif
#ifdef CONFIG_QS_S5K4E1
	platform_device_register(&msm_camera_sensor_qs_s5k4e1);
#endif
#ifdef CONFIG_S5K3H1GX
	platform_device_register(&msm_camera_sensor_s5k3h1gx);
#endif
#ifdef CONFIG_MT9V113
	platform_device_register(&msm_camera_sensor_webcam);
#endif
}
