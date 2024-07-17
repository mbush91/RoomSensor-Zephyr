#ifndef SS_BT
#define SS_BT

#include <stdint.h>
#include "bme280.h"

#define MANUFACTURER_ID 0x0877
#define SUBTYPE 0x7E

#define MANUFACTURER_ID_LSB ((MANUFACTURER_ID >> 0) & 0xFF)
#define MANUFACTURER_ID_MSB ((MANUFACTURER_ID >> 8) & 0xFF)

#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)
#define TX_RATE ((CONFIG_BT_TX_RATE * 16) / 10)

void init_ss_bt(void);
void disable_bt(void);
void ss_bt_update(
  struct bme280_sensor_vals temp,
  struct bme280_sensor_vals press,
  struct bme280_sensor_vals humidity,
  int16_t batt);

#endif
