#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(bme280, LOG_LEVEL_INF);

static const struct device* dev = NULL;

static void get_bme280_device(void)
{
	dev = DEVICE_DT_GET_ANY(bosch_bme280);

	if (dev == NULL) {
		/* No such node, or the node does not have status "okay". */
		LOG_ERR("\nError: no device found.\n");
	}

	if (!device_is_ready(dev)) {
		LOG_ERR("\nError: Device \"%s\" is not ready; "
		       "check the driver initialization logs for errors.\n",
		       dev->name);
	}

	LOG_INF("Found device \"%s\", getting sensor data\n", dev->name);
}

void bme280_init(void) {
  get_bme280_device();
}

void bme280_read(struct sensor_value* temp, struct sensor_value* press, struct sensor_value* humidity) {
  int err;

  if(dev != NULL) {
    err = sensor_sample_fetch(dev);
    if(!err) {
      sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, temp);
      sensor_channel_get(dev, SENSOR_CHAN_PRESS, press);
      sensor_channel_get(dev, SENSOR_CHAN_HUMIDITY, humidity);

      LOG_INF("temp: %d.%06d; press: %d.%06d; humidity: %d.%06d\n",
          temp->val1, temp->val2, press->val1, press->val2,
          humidity->val1, humidity->val2);
    } else {
      LOG_ERR("Boo");
    }
  }
}
