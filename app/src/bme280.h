#include <zephyr/drivers/sensor.h>
#include <stdint.h>

struct bme280_sensor_vals {
	/** Integer part of the value. */
	int8_t val1;
	/** Fractional part of the value (in one-hundreth parts). */
	int8_t val2;
};



void bme280_init(void);
void bme280_read(
  struct bme280_sensor_vals* temp,
  struct bme280_sensor_vals* press,
  struct bme280_sensor_vals* humidity
);
