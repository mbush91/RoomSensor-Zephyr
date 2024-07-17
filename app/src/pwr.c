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

// void zero_twim(void) {
//   volatile void *buf0 = (volatile void *)0x40003000;
//   memset(buf0,0,0x1000);
//   // NRF_TWIM0->TASKS_STARTRX = 0;
//   // NRF_TWIM0->TASKS_STARTTX = 0;
//   // NRF_TWIM0->TASKS_STOP = 0;
//   // NRF_TWIM0->TASKS_SUSPEND = 0;
//   // NRF_TWIM0->TASKS_RESUME = 0;
//   // NRF_TWIM0->TASKS_STARTRX = 0;                /*!< (@ 0x00000000) Start TWI receive sequence                                 */
//   // NRF_TWIM0->TASKS_STARTTX = 0;                /*!< (@ 0x00000008) Start TWI transmit sequence                                */
//   // NRF_TWIM0->TASKS_STOP = 0;                   /*!< (@ 0x00000014) Stop TWI transaction. Must be issued while the*/
//   // NRF_TWIM0->TASKS_SUSPEND = 0;                /*!< (@ 0x0000001C) Suspend TWI transaction                                    */
//   // NRF_TWIM0->TASKS_RESUME = 0;                 /*!< (@ 0x00000020) Resume TWI transaction                                     */
//   // NRF_TWIM0->EVENTS_STOPPED = 0;               /*!< (@ 0x00000104) TWI stopped                                                */
//   // NRF_TWIM0->EVENTS_ERROR = 0;                 /*!< (@ 0x00000124) TWI error                                                  */
//   // NRF_TWIM0->EVENTS_SUSPENDED = 0;             /*!< (@ 0x00000148) Last byte has been sent out after the SUSPEND */
//   // NRF_TWIM0->EVENTS_RXSTARTED = 0;             /*!< (@ 0x0000014C) Receive sequence started                                   */
//   // NRF_TWIM0->EVENTS_TXSTARTED = 0;             /*!< (@ 0x00000150) Transmit sequence started                                  */
//   // NRF_TWIM0->EVENTS_LASTRX = 0;                /*!< (@ 0x0000015C) Byte boundary, starting to receive the last byte           */
//   // NRF_TWIM0->EVENTS_LASTTX = 0;                /*!< (@ 0x00000160) Byte boundary, starting to transmit the last*/
//   // NRF_TWIM0->SHORTS = 0;                       /*!< (@ 0x00000200) Shortcuts between local events and tasks                   */
//   // NRF_TWIM0->INTEN = 0;                        /*!< (@ 0x00000300) Enable or disable interrupt                                */
//   // NRF_TWIM0->INTENSET = 0;                     /*!< (@ 0x00000304) Enable interrupt                                           */
//   // NRF_TWIM0->INTENCLR = 0;                     /*!< (@ 0x00000308) Disable interrupt                                          */
//   // NRF_TWIM0->ERRORSRC = 0;                     /*!< (@ 0x000004C4) Error source                                               */
//   // NRF_TWIM0->ENABLE = 0;                       /*!< (@ 0x00000500) Enable TWIM                                                */
//   // //NRF_TWIM0->FREQUENCY = 0;                    /*!< (@ 0x00000524) TWI frequency. Accuracy depends on the HFCLK */
//   // NRF_TWIM0->ADDRESS = 0;                      /*!< (@ 0x00000588) Address used in the TWI transfer                           */

//   // volatile uint32_t *scl = (volatile uint32_t *)0x40003508;
//   // *scl = 0xFFFFFFFF;
//   // volatile uint32_t *sda = (volatile uint32_t *)0x4000350C;
//   // *sda = 0xFFFFFFFF;
//   // volatile uint32_t *list = (volatile uint32_t *)0x4000350C;
//   // *sda = 0xFFFFFFFF;

// }


void pwr_sleep_bus(void) {

  int err;

  // if (!device_is_ready(bme280)) {
	// 	LOG_ERR("%s: device not ready.\n", bme280->name);
	// } else {

  //   err = pm_device_action_run(bme280, PM_DEVICE_ACTION_SUSPEND);
  //   if(err != 0 && err != -EALREADY) {
  //     LOG_ERR("%d:Suspend failed, err %d",__LINE__,err);
  //   }
  // }

  if (!device_is_ready(i2c_bus)) {
		LOG_ERR("%s: device not ready.\n", i2c_bus->name);
	} else {

    err = pm_device_action_run(i2c_bus, PM_DEVICE_ACTION_SUSPEND);
    if(err != 0 && err != -EALREADY) {
      LOG_ERR("%d:Suspend failed, err %d",__LINE__,err);
    }
    // NRF_TWIM0->ENABLE = 6;
    // NRF_TWIM0->ENABLE = 0;

  }
  // TODO : NRFX API to disable ADC


  disable_bt();
}


void pwr_wake_bus(void) {

    int err;
    // NRF_TWIM0->ENABLE = 0;
    // NRF_TWIM0->ENABLE = 6;
    err = pm_device_action_run(i2c_bus, PM_DEVICE_ACTION_RESUME);
    if(err != 0 && err != -EALREADY) {
      LOG_ERR("%d:Wake failed, err %d",__LINE__,err);
    }

    // err = pm_device_action_run(bme280, PM_DEVICE_ACTION_RESUME);
    // if(err != 0 && err != -EALREADY) {
    //   LOG_ERR("%d:Wake failed, err %d",__LINE__,err);
    // }

    // TODO : NRFX API to enable ADC
  }
