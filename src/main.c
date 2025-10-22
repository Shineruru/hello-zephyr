// A simple program to turn on the LED
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

/* LED */
#define LED0_NODE DT_ALIAS(led0)

#if !DT_NODE_HAS_STATUS(LED0_NODE, okay)
#error "Unsupported board: led0 devicetree alias is not defined"
#endif

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

/* Button */
#define SW0_NODE DT_ALIAS(sw0)

static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios, {0});
               
void main(void)
{
	int ret;
	int sleep_ms = 500;

	if (!device_is_ready(led.port) || !device_is_ready(button.port)) {
		return;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return;
	}
	
    ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
    if (ret < 0) {
        return;
    }

	while(1) {
		ret = gpio_pin_toggle_dt(&led);
		if (ret < 0) {
			return;
		}

        int val = gpio_pin_get_dt(&button);                                                           
        if (val >= 0) {                                                                               
            sleep_ms = (val > 0) ? 100 : 500;                                                         
        }
		k_msleep(sleep_ms);
	}
}