#include "pwr.h"
#include "ss_bt.h"

#include <nrfx_twim.h>

#include <errno.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/pm/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(pwr, LOG_LEVEL_INF);

const struct device *const i2c_bus = DEVICE_DT_GET_OR_NULL(DT_NODELABEL(i2c0));
const struct device *const bme280 = DEVICE_DT_GET_OR_NULL(DT_NODELABEL(bme280));



void pwr_sleep_bus(void) {

  int err;

  if (!device_is_ready(bme280)) {
		LOG_ERR("%s: device not ready.\n", bme280->name);
	} else {

    err = pm_device_action_run(bme280, PM_DEVICE_ACTION_SUSPEND);
    if(err != 0 && err != -EALREADY) {
      LOG_ERR("%d:Suspend failed, err %d",__LINE__,err);
    }
  }

  if (!device_is_ready(i2c_bus)) {
		LOG_ERR("%s: device not ready.\n", i2c_bus->name);
	} else {

    // *(volatile uint32_t *)0x40003FFC = 0;
    // *(volatile uint32_t *)0x40003FFC;
    // *(volatile uint32_t *)0x40003FFC = 1;

    err = pm_device_action_run(i2c_bus, PM_DEVICE_ACTION_SUSPEND);
    if(err != 0 && err != -EALREADY) {
      LOG_ERR("%d:Suspend failed, err %d",__LINE__,err);
    }
    //NRF_TWIM0->ENABLE = 0;
    // nrfx_twim_disable(i2c_instance);
    // nrfx_twim_uninit(i2c_instance);
    // erratta?
    //*(volatile uint32_t *)0x40003FFC = 0;
    // *(volatile uint32_t *)0x40003FFC;
    // *(volatile uint32_t *)0x40003FFC = 1;
  }
  // TODO : NRFX API to disable ADC


  disable_bt();
}


void pwr_wake_bus(void) {

    int err;
    //NRF_TWIM0->ENABLE = 6;
    err = pm_device_action_run(i2c_bus, PM_DEVICE_ACTION_RESUME);
    if(err != 0 && err != -EALREADY) {
      LOG_ERR("%d:Wake failed, err %d",__LINE__,err);
    }

    err = pm_device_action_run(bme280, PM_DEVICE_ACTION_RESUME);
    if(err != 0 && err != -EALREADY) {
      LOG_ERR("%d:Wake failed, err %d",__LINE__,err);
    }

    // TODO : NRFX API to enable ADC
  }
