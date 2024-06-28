#include "ss_bt.h"

#include <zephyr/kernel.h>

int main(void)
{

  init_ss_bt();

  while (1)
  {
    k_msleep(10000-106);

    ss_bt_update(1000,98);
  }
}
