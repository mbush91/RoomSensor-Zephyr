#include "ss_bt.h"
#include "bme280.h"
#include "batt.h"
#include "pwr.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);


int32_t init_done = 0;
struct bme280_sensor_vals temp, press, humidity;
int16_t batt_mv;

void main_loop(void *, void *, void*) {
    while (1)
    {
      if(init_done) {
        LOG_INF("Thread Running, BT Transmitting");

        k_msleep(CONFIG_BT_TX_TIME);

        pwr_sleep_bus(); // TODO, break this into a ACD/I2C sleep vs BT

        k_sleep(K_SECONDS(CONFIG_TIME_BETWEEN_MEASURMENTS));

        pwr_wake_bus();

        batt_mv = batt_read();
        bme280_read(&temp,&press,&humidity);

        ss_bt_update(temp,press,humidity,batt_mv);

      } else {
        k_msleep(50);
      }
    }
}

K_THREAD_DEFINE(main_loop_tid, 2048, main_loop, NULL, NULL, NULL, 2, 0, 0);

int main(void)
{
  batt_init();
  bme280_init();
  init_ss_bt();

  // Do inital reads
  batt_mv = batt_read();
  bme280_read(&temp,&press,&humidity);

  // Update and start ADV
  ss_bt_update(temp,press,humidity,batt_mv);

  init_done = 1;
}
