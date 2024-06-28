#include "ss_bt.h"

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gap.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(ss_bt, LOG_LEVEL_INF);

// advertising params
#define MANUFACTURER_ID 0x0877
#define SUBTYPE 0x55

#define MANUFACTURER_ID_LSB ((MANUFACTURER_ID >> 0) & 0xFF)
#define MANUFACTURER_ID_MSB ((MANUFACTURER_ID >> 8) & 0xFF)

#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)
#define TX_RATE ((CONFIG_BT_TX_RATE * 16) / 10)

static struct bt_le_adv_param *adv_param = BT_LE_ADV_PARAM(BT_LE_ADV_OPT_USE_IDENTITY ,
                                                           TX_RATE,
                                                           TX_RATE + 1,
                                                           NULL);

static uint8_t data_buffer[] = {
    MANUFACTURER_ID_LSB, // MANUFACTURER ID (lsb)
    MANUFACTURER_ID_MSB, // MANUFACTURER ID (msb)
    SUBTYPE,             // SUBTYPE
    0x00,                // BATT %
    0x00,                // IMPEDIANCE BYTE
    0x00,                // IMPEDIANCE BYTE
    0x00,                // IMPEDIANCE BYTE
    0x00                 // IMPEDIANCE BYTE
};

static const struct bt_data ad_mnft_data_buffer[] = {BT_DATA(BT_DATA_MANUFACTURER_DATA, data_buffer, ARRAY_SIZE(data_buffer))};

static const struct bt_data sd[] = {BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN)};

void ss_bt_update(int32_t impediance, int8_t batt)
{
  data_buffer[3] = (uint8_t)batt;
  memcpy(&data_buffer[4], &impediance, sizeof(impediance));

  bt_le_adv_update_data(ad_mnft_data_buffer,
                        ARRAY_SIZE(ad_mnft_data_buffer),
                        sd,
                        ARRAY_SIZE(sd));
}

void bt_ready_cb(int err)
{
  err = bt_le_adv_start(adv_param,
                        ad_mnft_data_buffer,
                        ARRAY_SIZE(ad_mnft_data_buffer),
                        sd,
                        ARRAY_SIZE(sd));

  if (err)
  {
    LOG_ERR("BT Error, err: %d", err);
  }
}

void init_ss_bt(void)
{
  int32_t err;

  err = bt_enable(bt_ready_cb);
}
