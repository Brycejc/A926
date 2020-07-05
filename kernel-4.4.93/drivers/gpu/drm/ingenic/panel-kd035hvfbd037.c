#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/of_gpio.h>
#include <drm/drmP.h>
#include <drm/drm_crtc.h>
#include <drm/drm_mipi_dsi.h>
#include <drm/drm_panel.h>

#include <video/display_timing.h>
#include <video/videomode.h>
#include "ingenic_drv.h"

struct board_gpio {
	short gpio;
	short active_level;
};

struct panel_desc {
	const struct drm_display_mode *modes;
	unsigned int num_modes;
	const struct display_timing *timings;
	unsigned int num_timings;

	unsigned int bpc;

	struct {
		unsigned int width;
		unsigned int height;
	} size;

	/**
	 * @prepare: the time (in milliseconds) that it takes for the panel to
	 *           become ready and start receiving video data
	 * @enable: the time (in milliseconds) that it takes for the panel to
	 *          display the first valid frame after starting to receive
	 *          video data
	 * @disable: the time (in milliseconds) that it takes for the panel to
	 *           turn the display off (no content is visible)
	 * @unprepare: the time (in milliseconds) that it takes for the panel
	 *             to power itself down completely
	 */
	struct {
		unsigned int prepare;
		unsigned int enable;
		unsigned int disable;
		unsigned int unprepare;
	} delay;

	u32 bus_format;
};


struct panel_dev {
	struct drm_panel base;
	/* ingenic frame buffer */
	struct device *dev;
	struct lcd_panel *panel;

	struct backlight_device *backlight;
	int power;
	const struct panel_desc *desc;

	struct regulator *vcc;
	struct board_gpio cs;
	struct board_gpio rst;
	struct board_gpio pwm;
	struct board_gpio vdd_en;
};

static struct smart_lcd_data_table kd035hvfbd037_data_table[] = {
/* LCD init code */
	{SMART_CONFIG_UDELAY, 12000},
	{SMART_CONFIG_CMD    , 0xff},   //Command 2 Enable
	{SMART_CONFIG_PRM    , 0x48},
	{SMART_CONFIG_PRM   , 0x02},
	{SMART_CONFIG_PRM   , 0x01},

	{SMART_CONFIG_CMD    , 0x00},
	{SMART_CONFIG_PRM   , 0x80},
	{SMART_CONFIG_CMD    , 0xff},  //ORISE Command Enable
	{SMART_CONFIG_PRM   , 0x48},
	{SMART_CONFIG_PRM   , 0x02},

	{SMART_CONFIG_CMD    , 0x00},
	{SMART_CONFIG_PRM   , 0x90},
	{SMART_CONFIG_CMD    , 0xFF},  //MPU 16bit setting
	{SMART_CONFIG_PRM   , 0x01},	//02-16BIT MCU,01-8BIT MCU

	{SMART_CONFIG_CMD    , 0x00},
	{SMART_CONFIG_PRM   , 0x93},
	{SMART_CONFIG_CMD    , 0xFF},  //SW MPU enable
	{SMART_CONFIG_PRM   , 0x20},

	{SMART_CONFIG_CMD    , 0x00},
	{SMART_CONFIG_PRM   , 0x00},
	{SMART_CONFIG_CMD    , 0x51},    //Wright Display brightness
	{SMART_CONFIG_PRM   , 0xf0},

	{SMART_CONFIG_CMD    , 0x00},
	{SMART_CONFIG_PRM   , 0x00},
	{SMART_CONFIG_CMD    , 0x53},   // Wright CTRL Display
	{SMART_CONFIG_PRM   , 0x24},

	{SMART_CONFIG_CMD    , 0x00},
	{SMART_CONFIG_PRM  , 0xb1},
	{SMART_CONFIG_CMD    , 0xc5},   //VSEL setting
	{SMART_CONFIG_PRM   , 0x00},

	{SMART_CONFIG_CMD    , 0x00},
	{SMART_CONFIG_PRM   , 0xB0},
	{SMART_CONFIG_CMD    , 0xc4},   //Gate Timing control
	{SMART_CONFIG_PRM   , 0x02},
	{SMART_CONFIG_PRM   , 0x08},
	{SMART_CONFIG_PRM   , 0x05},
	{SMART_CONFIG_PRM   , 0x00},

	{SMART_CONFIG_CMD    , 0x00},
	{SMART_CONFIG_PRM   , 0x90},
	{SMART_CONFIG_CMD    , 0xc0},   //TCON MCLK Shift Control
	{SMART_CONFIG_PRM   , 0x00},
	{SMART_CONFIG_PRM   , 0x0f},
	{SMART_CONFIG_PRM   , 0x00},
	{SMART_CONFIG_PRM   , 0x15},
	{SMART_CONFIG_PRM   , 0x00},
	{SMART_CONFIG_PRM   , 0x17},

	{SMART_CONFIG_CMD    , 0x00},
	{SMART_CONFIG_PRM   , 0x82},
	{SMART_CONFIG_CMD    , 0xc5},  //Adjust pump phase
	{SMART_CONFIG_PRM   , 0x01},

	{SMART_CONFIG_CMD    , 0x00},
	{SMART_CONFIG_PRM   , 0x90},
	{SMART_CONFIG_CMD    , 0xc5},  //Adjust pump phase
	{SMART_CONFIG_PRM   , 0x47},

	{SMART_CONFIG_CMD    , 0x00},
	{SMART_CONFIG_PRM   , 0x00},
	{SMART_CONFIG_CMD    , 0xd8},  //GVDD/NGVDD Setting
	{SMART_CONFIG_PRM   , 0x58},  //58,17V
	{SMART_CONFIG_PRM   , 0x58},  //58

	{SMART_CONFIG_CMD    , 0x00},
	{SMART_CONFIG_PRM   , 0x00},
	{SMART_CONFIG_CMD    , 0xd9},  //VCOM Setting
	{SMART_CONFIG_PRM   , 0xb0},  //

	{SMART_CONFIG_CMD    , 0x00},
	{SMART_CONFIG_PRM   , 0x91},
	{SMART_CONFIG_CMD    , 0xb3},  //Display setting
	{SMART_CONFIG_PRM   , 0xC0},
	{SMART_CONFIG_PRM   , 0x25},

	{SMART_CONFIG_CMD    , 0x00},
	{SMART_CONFIG_PRM   , 0x81},
	{SMART_CONFIG_CMD    , 0xC1}, //Osillator Adjustment:70Hz
	{SMART_CONFIG_PRM   , 0x77},

	{SMART_CONFIG_CMD    , 0x00},
	{SMART_CONFIG_PRM   , 0x00},
	{SMART_CONFIG_CMD    , 0xe1},   //Gamma setting                     ( positive)
	{SMART_CONFIG_PRM   , 0x00},
	{SMART_CONFIG_PRM   , 0x05},
	{SMART_CONFIG_PRM   , 0x09},
	{SMART_CONFIG_PRM   , 0x04},
	{SMART_CONFIG_PRM   , 0x02},
	{SMART_CONFIG_PRM   , 0x0b},
	{SMART_CONFIG_PRM   , 0x0a},
	{SMART_CONFIG_PRM   , 0x09},
	{SMART_CONFIG_PRM   , 0x05},
	{SMART_CONFIG_PRM   , 0x08},
	{SMART_CONFIG_PRM   , 0x10},
	{SMART_CONFIG_PRM   , 0x05},
	{SMART_CONFIG_PRM   , 0x06},
	{SMART_CONFIG_PRM   , 0x11},
	{SMART_CONFIG_PRM   , 0x09},
	{SMART_CONFIG_PRM   , 0x01},

	{SMART_CONFIG_CMD    , 0x00},
	{SMART_CONFIG_PRM   , 0x00},
	{SMART_CONFIG_CMD    , 0xe2},  //Gamma setting                      ( negative)
	{SMART_CONFIG_PRM   , 0x00},
	{SMART_CONFIG_PRM   , 0x05},
	{SMART_CONFIG_PRM   , 0x09},
	{SMART_CONFIG_PRM   , 0x04},
	{SMART_CONFIG_PRM   , 0x02},
	{SMART_CONFIG_PRM   , 0x0b},
	{SMART_CONFIG_PRM   , 0x0a},
	{SMART_CONFIG_PRM   , 0x09},
	{SMART_CONFIG_PRM   , 0x05},
	{SMART_CONFIG_PRM   , 0x08},
	{SMART_CONFIG_PRM   , 0x10},
	{SMART_CONFIG_PRM   , 0x05},
	{SMART_CONFIG_PRM   , 0x06},
	{SMART_CONFIG_PRM   , 0x11},
	{SMART_CONFIG_PRM   , 0x09},
	{SMART_CONFIG_PRM   , 0x01},

	{SMART_CONFIG_CMD    , 0x00},
	{SMART_CONFIG_PRM   , 0x00},
	{SMART_CONFIG_CMD    , 0x00},  //End Gamma setting
	{SMART_CONFIG_PRM   , 0x00},

	{SMART_CONFIG_CMD    , 0x00},
	{SMART_CONFIG_PRM   , 0x80},
	{SMART_CONFIG_CMD    , 0xff}, //Orise mode  command Disable
	{SMART_CONFIG_PRM   , 0x00},
	{SMART_CONFIG_PRM   , 0x00},

	{SMART_CONFIG_CMD    , 0x00},
	{SMART_CONFIG_PRM   , 0x00},

	{SMART_CONFIG_CMD    , 0xff}, //Command 2 Disable
	{SMART_CONFIG_PRM   , 0xff},
	{SMART_CONFIG_PRM   , 0xff},
	{SMART_CONFIG_PRM   , 0xff},

	//{SMART_CONFIG_CMD  , 0x35}, //TE ON
	//{SMART_CONFIG_DATA , 0x00},

	{SMART_CONFIG_CMD    , 0x36}, //set X Y refresh direction
	{SMART_CONFIG_PRM   , 0x00},

	{SMART_CONFIG_CMD    , 0x3A},    //16-bit/pixe 565
	{SMART_CONFIG_PRM   , 0x05},

	{SMART_CONFIG_CMD    , 0x2A}, //Frame rate control	320
	{SMART_CONFIG_PRM   , 0x00},
	{SMART_CONFIG_PRM   , 0x00},
	{SMART_CONFIG_PRM   , 0x01},
	{SMART_CONFIG_PRM   , 0x3F},

	{SMART_CONFIG_CMD    , 0x2B}, //Display function control	 480
	{SMART_CONFIG_PRM   , 0x00},
	{SMART_CONFIG_PRM   , 0x00},
	{SMART_CONFIG_PRM   , 0x01},
	{SMART_CONFIG_PRM   , 0xDF},

	{SMART_CONFIG_CMD    , 0x11},
	{SMART_CONFIG_UDELAY , 12000},
	{SMART_CONFIG_CMD    , 0x29}, //display on

	{SMART_CONFIG_CMD    , 0x2c},

};

struct smart_config kd035hvfbd037_cfg = {
	.rdy_anti_jit = 0,
	.te_anti_jit = 1,
	.te_md = 0,
	.te_switch = 0,
	.rdy_switch = 0,
	.cs_en = 0,
	.cs_dp = 0,
	.rdy_dp = 1,
	.dc_md = 0,
	.wr_md = 1,
	.te_dp = 1,
	.smart_type = SMART_LCD_TYPE_8080,
	.pix_fmt = SMART_LCD_FORMAT_565,
	.dwidth = SMART_LCD_DWIDTH_8_BIT,
	.cwidth = SMART_LCD_CWIDTH_8_BIT,
	.bus_width = 8,

	.write_gram_cmd = 0x2c,
	.data_table = kd035hvfbd037_data_table,
	.length_data_table = ARRAY_SIZE(kd035hvfbd037_data_table),
};

/* SGM3146 supports 16 brightness step */
#define MAX_BRIGHTNESS_STEP     16
/* System support 256 brightness step */
#define CONVERT_FACTOR          (256/MAX_BRIGHTNESS_STEP)

static inline struct panel_dev *to_panel_dev(struct drm_panel *panel)
{
	return container_of(panel, struct panel_dev, base);
}
static int panel_update_status(struct backlight_device *bd)
{
	struct panel_dev *panel = dev_get_drvdata(&bd->dev);
	unsigned int i;
	int brightness = bd->props.brightness;
	int pulse_num = MAX_BRIGHTNESS_STEP - brightness / CONVERT_FACTOR - 1;

	if(bd->props.power > FB_BLANK_NORMAL)
		brightness = 0;

	if (brightness) {
		gpio_direction_output(panel->pwm.gpio,0);
		udelay(5000);
		gpio_direction_output(panel->pwm.gpio,1);
		udelay(100);

		for (i = pulse_num; i > 0; i--) {
			gpio_direction_output(panel->pwm.gpio,0);
			udelay(1);
			gpio_direction_output(panel->pwm.gpio,1);
			udelay(3);
		}
	} else {
		gpio_direction_output(panel->pwm.gpio, 0);
	}

	return 0;
}

static struct backlight_ops panel_backlight_ops = {
	.options = BL_CORE_SUSPENDRESUME,
	.update_status = panel_update_status,
};

static void panel_power_reset(struct board_gpio *rst)
{
	gpio_direction_output(rst->gpio, 0);
	mdelay(20);
	gpio_direction_output(rst->gpio, 1);
	mdelay(10);
}

#define POWER_IS_ON(pwr)        ((pwr) <= FB_BLANK_NORMAL)
static int panel_set_power(struct panel_dev *panel, int power)
{
	struct board_gpio *cs = &panel->cs;
	struct board_gpio *rst = &panel->rst;
	struct board_gpio *vdd_en = &panel->vdd_en;
	if(POWER_IS_ON(power) && !POWER_IS_ON(panel->power)) {
		gpio_direction_output(vdd_en->gpio, 1);
		gpio_direction_output(rst->gpio, 1);
		gpio_direction_output(cs->gpio, 1);
		mdelay(5);
		panel_power_reset(rst);
		gpio_direction_output(cs->gpio, 0);
		mdelay(10);
	}
	if(!POWER_IS_ON(power) && POWER_IS_ON(panel->power)) {
		gpio_direction_output(cs->gpio, 0);
		gpio_direction_output(rst->gpio, 0);
		gpio_direction_output(vdd_en->gpio, 0);
	}

	panel->power = power;
        return 0;
}

static int of_panel_parse(struct device *dev)
{
	struct panel_dev *panel = dev_get_drvdata(dev);
	struct device_node *np = dev->of_node;
	enum of_gpio_flags flags;
	int ret = 0;

	panel->cs.gpio = of_get_named_gpio_flags(np, "ingenic,cs-gpio", 0, &flags);
	if(gpio_is_valid(panel->cs.gpio)) {
		panel->cs.active_level = OF_GPIO_ACTIVE_LOW ? 0 : 1;
		ret = gpio_request_one(panel->cs.gpio, GPIOF_DIR_OUT, "cs");
		if(ret < 0) {
			dev_err(dev, "Failed to request cs pin!\n");
			return ret;
		}
	} else {
		dev_warn(dev, "invalid gpio cs.gpio: %d\n", panel->cs.gpio);
	}

	panel->rst.gpio = of_get_named_gpio_flags(np, "ingenic,rst-gpio", 0, &flags);
	if(gpio_is_valid(panel->rst.gpio)) {
		panel->rst.active_level = OF_GPIO_ACTIVE_LOW ? 0 : 1;
		ret = gpio_request_one(panel->rst.gpio, GPIOF_DIR_OUT, "rst");
		if(ret < 0) {
			dev_err(dev, "Failed to request rst pin!\n");
			goto err_request_rst;
		}
	} else {
		dev_warn(dev, "invalid gpio rst.gpio: %d\n", panel->rst.gpio);
	}

	panel->vdd_en.gpio = of_get_named_gpio_flags(np, "ingenic,vdd-en-gpio", 0, &flags);
	if(gpio_is_valid(panel->vdd_en.gpio)) {
		panel->vdd_en.active_level = OF_GPIO_ACTIVE_LOW ? 0 : 1;
		ret = gpio_request_one(panel->vdd_en.gpio, GPIOF_DIR_OUT, "vdd_en");
		if(ret < 0) {
			dev_err(dev, "Failed to request vdd_en pin!\n");
			goto err_request_rst;
		}
	} else {
		dev_warn(dev, "invalid gpio vdd_en.gpio: %d\n", panel->vdd_en.gpio);
	}
	panel->pwm.gpio = of_get_named_gpio_flags(np, "ingenic,pwm-gpio", 0, &flags);
	if(gpio_is_valid(panel->pwm.gpio)) {
		panel->pwm.active_level = OF_GPIO_ACTIVE_LOW ? 0 : 1;
		ret = gpio_request_one(panel->pwm.gpio, GPIOF_DIR_OUT, "pwm");
		if(ret < 0) {
			dev_err(dev, "Failed to request pwm pin!\n");
			goto err_request_rst;
		}
	} else {
		dev_warn(dev, "invalid gpio pwm.gpio: %d\n", panel->pwm.gpio);
	}

	return 0;
err_request_rst:
	gpio_free(panel->cs.gpio);
	return ret;
}
static int panel_dev_unprepare(struct drm_panel *panel)
{
	struct panel_dev *p = to_panel_dev(panel);

	panel_set_power(p, FB_BLANK_POWERDOWN);
	return 0;
}

static int panel_dev_prepare(struct drm_panel *panel)
{
	struct panel_dev *p = to_panel_dev(panel);
	panel_set_power(p, FB_BLANK_UNBLANK);
	return 0;
}

static int panel_dev_enable(struct drm_panel *panel)
{
	struct panel_dev *p = to_panel_dev(panel);

	if (p->backlight) {
		p->backlight->props.power = FB_BLANK_UNBLANK;
		backlight_update_status(p->backlight);
	}

	return 0;
}

static int panel_dev_disable(struct drm_panel *panel)
{
	struct panel_dev *p = to_panel_dev(panel);

	if (p->backlight) {
		p->backlight->props.power = FB_BLANK_POWERDOWN;
		backlight_update_status(p->backlight);
	}
	return 0;
}

static const struct drm_display_mode kd035hvfbd037_mode = {
	.clock = 10800,
	.hdisplay = 320,
	.hsync_start = 320,
	.hsync_end = 320,
	.htotal = 320,
	.vdisplay = 480,
	.vsync_start = 480,
	.vsync_end = 480,
	.vtotal = 480,
	.vrefresh = 60,
	.private = (int *)&kd035hvfbd037_cfg
};

static const struct panel_desc kd035hvfbd037_desc = {
	.modes = &kd035hvfbd037_mode,
	.num_modes = 1,
	.bpc = 6,
	.size = {
		.width = 61,
		.height = 96,
	},
};

static int panel_dev_get_fixed_modes(struct panel_dev *panel)
{
	struct drm_connector *connector = panel->base.connector;
	struct drm_device *drm = panel->base.drm;
	struct drm_display_mode *mode;
	unsigned int i, num = 0;

	if (!panel->desc)
		return 0;

	for (i = 0; i < panel->desc->num_timings; i++) {
		const struct display_timing *dt = &panel->desc->timings[i];
		struct videomode vm;

		videomode_from_timing(dt, &vm);
		mode = drm_mode_create(drm);
		if (!mode) {
			dev_err(drm->dev, "failed to add mode %ux%u\n",
				dt->hactive.typ, dt->vactive.typ);
			continue;
		}

		drm_display_mode_from_videomode(&vm, mode);
		drm_mode_set_name(mode);

		drm_mode_probed_add(connector, mode);
		num++;
	}

	for (i = 0; i < panel->desc->num_modes; i++) {
		const struct drm_display_mode *m = &panel->desc->modes[i];

		mode = drm_mode_duplicate(drm, m);
		if (!mode) {
			dev_err(drm->dev, "failed to add mode %ux%u@%u\n",
				m->hdisplay, m->vdisplay, m->vrefresh);
			continue;
		}

		drm_mode_set_name(mode);

		drm_mode_probed_add(connector, mode);
		num++;
	}

	connector->display_info.bpc = panel->desc->bpc;
	connector->display_info.width_mm = panel->desc->size.width;
	connector->display_info.height_mm = panel->desc->size.height;
	if (panel->desc->bus_format)
		drm_display_info_set_bus_formats(&connector->display_info,
						 &panel->desc->bus_format, 1);

	return num;
}

static int panel_dev_get_modes(struct drm_panel *panel)
{
	struct panel_dev *p = to_panel_dev(panel);
	int num = 0;

	num += panel_dev_get_fixed_modes(p);

	return num;
}

static int panel_dev_get_timings(struct drm_panel *panel,
				    unsigned int num_timings,
				    struct display_timing *timings)
{
	struct panel_dev *p = to_panel_dev(panel);
	unsigned int i;

	if (p->desc->num_timings < num_timings)
		num_timings = p->desc->num_timings;

	if (timings)
		for (i = 0; i < num_timings; i++)
			timings[i] = p->desc->timings[i];

	return p->desc->num_timings;
}

static const struct drm_panel_funcs drm_panel_funs = {
	.disable = panel_dev_disable,
	.unprepare = panel_dev_unprepare,
	.prepare = panel_dev_prepare,
	.enable = panel_dev_enable,
	.get_modes = panel_dev_get_modes,
	.get_timings = panel_dev_get_timings,
};

static int panel_probe(struct platform_device *pdev)
{
	int ret = 0;
	struct panel_dev *panel;
	struct backlight_properties props;

	memset(&props, 0, sizeof(props));
	panel = kzalloc(sizeof(struct panel_dev), GFP_KERNEL);
	if(panel == NULL) {
		dev_err(&pdev->dev, "Faile to alloc memory!");
		return -ENOMEM;
	}
	panel->dev = &pdev->dev;
	dev_set_drvdata(&pdev->dev, panel);

	ret = of_panel_parse(&pdev->dev);
	if(ret < 0) {
		goto err_of_parse;
	}

	props.type = BACKLIGHT_RAW;
	props.max_brightness = 255;
	panel->backlight = backlight_device_register("pwm-backlight.0",
						&pdev->dev, panel,
						&panel_backlight_ops,
						&props);
	if (IS_ERR_OR_NULL(panel->backlight)) {
		dev_err(panel->dev, "failed to register 'pwm-backlight.0'.\n");
		goto err_of_parse;
	}

	panel->desc = &kd035hvfbd037_desc;
	drm_panel_init(&panel->base);
	panel->base.dev = &pdev->dev;
	panel->base.funcs = &drm_panel_funs;
	drm_panel_add(&panel->base);

	panel->power = FB_BLANK_POWERDOWN;
	panel_dev_prepare(&panel->base);
	panel->backlight->props.brightness = props.max_brightness;
	panel_dev_enable(&panel->base);

	return 0;

err_of_parse:
	kfree(panel);
	return ret;
}

static int panel_remove(struct platform_device *pdev)
{
	struct panel_dev *panel = dev_get_drvdata(&pdev->dev);

	panel_set_power(panel, FB_BLANK_POWERDOWN);
	return 0;
}


#ifdef CONFIG_PM
static int panel_suspend(struct device *dev)
{
	struct panel_dev *panel = dev_get_drvdata(dev);

	panel_set_power(panel, FB_BLANK_POWERDOWN);
	return 0;
}

static int panel_resume(struct device *dev)
{
	struct panel_dev *panel = dev_get_drvdata(dev);

	panel_set_power(panel, FB_BLANK_UNBLANK);
	return 0;
}

static const struct dev_pm_ops panel_pm_ops = {
	.suspend = panel_suspend,
	.resume = panel_resume,
};

#endif
static const struct of_device_id panel_of_match[] = {
	{ .compatible = "ingenic,kd035hvfbd037", },
	{},
};

static struct platform_driver panel_driver = {
	.probe		= panel_probe,
	.remove		= panel_remove,
	.driver		= {
		.name	= "kd035hvfbd037",
		.of_match_table = panel_of_match,
#ifdef CONFIG_PM
		.pm = &panel_pm_ops,
#endif
	},
};

module_platform_driver(panel_driver);
