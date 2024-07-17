#include "ss_bt.h"

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gap.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(ss_bt, LOG_LEVEL_INF);

static struct bt_le_adv_param *adv_param = BT_LE_ADV_PARAM(BT_LE_ADV_OPT_USE_IDENTITY,
                                                           TX_RATE,
                                                           TX_RATE + 1,
                                                           NULL);

static uint8_t data_buffer[] = {
    MANUFACTURER_ID_LSB, // MANUFACTURER ID (lsb)
    MANUFACTURER_ID_MSB, // MANUFACTURER ID (msb)
    SUBTYPE,             // SUBTYPE
    0x00,                // BATT_MV 1
    0x00,                // BATT_MV 2
    0x00,                // T1
    0x00,                // T2
    0x00,                // P1
    0x00,                // P2
    0x00,                // H1
    0x00                 // H2
};

static const struct bt_data ad_mnft_data_buffer[] = {BT_DATA(BT_DATA_MANUFACTURER_DATA, data_buffer, ARRAY_SIZE(data_buffer))};

static const struct bt_data sd[] = {BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN)};

void ss_bt_update(
  struct bme280_sensor_vals temp,
  struct bme280_sensor_vals press,
  struct bme280_sensor_vals humidity,
  int16_t batt)
{
  memcpy(&data_buffer[3],&batt,sizeof(batt));
  data_buffer[5] = (uint8_t)temp.val1;
  data_buffer[6] = (uint8_t)temp.val2;
  data_buffer[7] = (uint8_t)press.val1;
  data_buffer[8] = (uint8_t)press.val2;
  data_buffer[9] = (uint8_t)humidity.val1;
  data_buffer[10] = (uint8_t)humidity.val2;


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

void disable_bt(void) {
  int err = bt_disable();

  if(err) {
    LOG_ERR("Couldn't disable BT, err %d", err);
  }
}
