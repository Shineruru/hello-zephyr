#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

/* LED */
#define LED0_NODE DT_ALIAS(led0)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

/* Button */
#define SW0_NODE	DT_ALIAS(sw0)
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios, {0});

/* 1. 宣告一個回呼結構體變數 */
static struct gpio_callback button_cb_data;

/* 2. 撰寫我們的回呼函式 (Callback Function) */
// 當按鈕被按下時，這個函式會被 GPIO 驅動程式呼叫
__attribute__((noinline))
void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	gpio_pin_toggle_dt(&led);
}

void main(void)
{
	int ret;

	if (!device_is_ready(led.port) || !device_is_ready(button.port)) {
		return;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return;
	}

    /* 3. 設定按鈕為輸入模式 */
	ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
	if (ret < 0) {
		return;
	}

    /* 4. 設定按鈕的中斷觸發模式 */
    // GPIO_INT_EDGE_TO_ACTIVE: 當腳位電位從 "inactive" 變為 "active" 時觸發
    // 因為我們的按鈕是 ACTIVE_LOW，所以這代表的是 "下降緣 (Falling Edge)"
	ret = gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE);
	if (ret < 0) {
		return;
	}

    /* 5. 初始化回呼結構體，並註冊我們的函式 */
	gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin));
	gpio_add_callback(button.port, &button_cb_data);

    /* 6. 主迴圈現在可以做其他事，或直接休息，不再需要關心按鈕 */
	while (1) {
		// 主迴圈現在是空的，CPU 可以進入低功耗模式
		k_sleep(K_FOREVER);
	}
}
