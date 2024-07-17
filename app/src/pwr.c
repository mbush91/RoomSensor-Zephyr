#include "pwr.h"
#include "ss_bt.h"

#include <errno.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/pm/device.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(pwr, LOG_LEVEL_INF);

const struct device *const i2c_bus = DEVICE_DT_GET(DT_NODELABEL(i2c0));

void pwr_sleep_bus(void) {

  int err;

  if (!device_is_ready(i2c_bus)) {
		LOG_ERR("%s: device not ready.\n", i2c_bus->name);
	}

  err = pm_device_action_run(i2c_bus, PM_DEVICE_ACTION_SUSPEND);
  if(err != 0 && err != -EALREADY) {
    LOG_ERR("%d:Suspend failed, err %d",__LINE__,err);
  }

  // TODO : NRFX API to disable ADC

  disable_bt();
}


void pwr_wake_bus(void) {

    int err;

    err = pm_device_action_run(i2c_bus, PM_DEVICE_ACTION_RESUME);
    if(err != 0 && err != -EALREADY) {
      LOG_ERR("%d:Suspend failed, err %d",__LINE__,err);
    }

    // TODO : NRFX API to enable ADC
  }
