#include "ss_bt.h"
#include "bme280.h"
#include "batt.h"
#include "pwr.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);



struct bme280_sensor_vals temp, press, humidity;
int16_t batt_mv;

int main(void)
{
  batt_init();
  bme280_init();

  while (1)
  {
    pwr_wake_bus();
    bme280_read(&temp,&press,&humidity);
    batt_mv = batt_read();
    init_ss_bt();
    ss_bt_update(temp,press,humidity,batt_mv);

    k_msleep(CONFIG_BT_TX_TIME);

    pwr_sleep_bus();

    k_sleep(K_SECONDS(CONFIG_TIME_BETWEEN_MEASURMENTS));

  }
}
