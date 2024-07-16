#include "ss_bt.h"
#include "bme280.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);



struct bme280_sensor_vals temp, press, humidity;

int main(void)
{
  int res = 1000;
  //init_ss_bt();
  bme280_init();

  while (1)
  {
    k_msleep(1000);
    bme280_read(&temp,&press,&humidity);

    res++;
    LOG_INF("main");
  }
}
