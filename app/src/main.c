#include "ss_bt.h"
#include "bme280.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
  int res = 1000;
  //en_coded_adv();
  bme280_init();

  while (1)
  {
    k_msleep(1000);
    bme280_read();
    //ss_bt_update(res,98);
    res++;
    LOG_INF("main");
  }
}
