#include "ss_bt.h"
#include "bme280.h"
#include "batt.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);



struct bme280_sensor_vals temp, press, humidity;

int main(void)
{
  int res = 1000;
  int16_t batt_mv;
  batt_init();
  bme280_init();
  init_ss_bt();

  while (1)
  {
    k_msleep(1000);

    bme280_read(&temp,&press,&humidity);
    batt_mv = batt_read();
    LOG_INF("Batt %d",batt_mv);

    ss_bt_update(temp,press,humidity,batt_mv);

    res++;
  }
}
