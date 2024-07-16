#include "ss_bt.h"

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gap.h>
#include <zephyr/logging/log.h>
#include <errno.h>

LOG_MODULE_REGISTER(ss_bt, LOG_LEVEL_INF);

// Normal ADV
static struct bt_le_adv_param *adv_param = BT_LE_ADV_PARAM(BT_LE_ADV_OPT_USE_IDENTITY ,
                                                           TX_RATE,
                                                           TX_RATE + 1,
                                                           NULL);

// Extended + CODED
static void adv_sent_cb(struct bt_le_ext_adv *adv,struct bt_le_ext_adv_sent_info *info);
static struct bt_le_adv_param *adv_param_coded = BT_LE_EXT_ADV_CODED_NCONN_IDENTITY;
static struct bt_le_ext_adv_start_param ext_adv_start_param = {
	.timeout = 0,
	.num_events = 0,
};
const static struct bt_le_ext_adv_cb adv_callbacks = {
	.sent = adv_sent_cb,
};
static struct bt_le_ext_adv *adv_set;


// Data
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

static const struct bt_data ad_mnft_data_buffer[] = {
    BT_DATA(BT_DATA_MANUFACTURER_DATA, data_buffer, ARRAY_SIZE(data_buffer))
  };

static const struct bt_data sd[] = {BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN)};

static void adv_sent_cb(struct bt_le_ext_adv *adv,
			struct bt_le_ext_adv_sent_info *info)
{
	LOG_INF("Adv sent");
}

void ss_bt_update(int32_t impediance, int8_t batt)
{
  data_buffer[3] = (uint8_t)batt;
  memcpy(&data_buffer[4], &impediance, sizeof(impediance));

  // bt_le_adv_update_data(ad_mnft_data_buffer,
  //                       ARRAY_SIZE(ad_mnft_data_buffer),
  //                       sd,
  //                       ARRAY_SIZE(sd));

  int err = bt_le_ext_adv_set_data(adv_set, ad_mnft_data_buffer,
                        ARRAY_SIZE(ad_mnft_data_buffer),
                        NULL,
                        0);
	if (err) {
		LOG_ERR("Failed to set advertising data (err %d)\n", err);
		return;
	}
}

static void bt_ext_adv_start()
{
  LOG_INF("Advertising set create...");
	int err = bt_le_ext_adv_create(adv_param_coded, &adv_callbacks, &adv_set);
	if (err) {
		LOG_ERR("failed (err %d)\n", err);
		return;
	}
  LOG_INF("Extended advertising enable...");

  err = bt_le_ext_adv_set_data(adv_set, ad_mnft_data_buffer, ARRAY_SIZE(ad_mnft_data_buffer), NULL, 0);
	if (err) {
		LOG_ERR("Failed to set advertising data (err %d)\n", err);
		return;
	}

	err = bt_le_ext_adv_start(adv_set, &ext_adv_start_param);
	if (err) {
		LOG_ERR("failed (err %d)\n", err);
		return;
	}
}

static void bt_adv_start()
{
  int err = bt_le_adv_start(adv_param,
                        ad_mnft_data_buffer,
                        ARRAY_SIZE(ad_mnft_data_buffer),
                        sd,
                        ARRAY_SIZE(sd));

  if (err)
  {
    LOG_ERR("BT Error, err: %d", err);
  }
}

void bt_ready_cb(int err)
{

  //bt_adv_start();
}

void init_ss_bt(void)
{
  int32_t err;

  err = bt_enable(bt_ready_cb);
}
