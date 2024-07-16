#include <zephyr/drivers/sensor.h>

void bme280_init(void);
void bme280_read(struct sensor_value* temp, struct sensor_value* press, struct sensor_value* humidity);
