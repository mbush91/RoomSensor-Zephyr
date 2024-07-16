#include <zephyr/kernel.h>
#include <zephyr/types.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gap.h>
#include <zephyr/logging/log.h>

#include "ss_bt.h"

LOG_MODULE_REGISTER(bt_coded, LOG_LEVEL_INF);

// static void start_advertising_coded(struct k_work *work);
// static K_WORK_DEFINE(start_advertising_worker, start_advertising_coded);

static struct bt_le_ext_adv *adv;

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

static const struct bt_data ad[] = {
    BT_DATA(BT_DATA_MANUFACTURER_DATA, data_buffer, ARRAY_SIZE(data_buffer))
  };

static int create_advertising_coded(void)
{
	int err;
	struct bt_le_adv_param param = BT_LE_ADV_PARAM_INIT(
      BT_LE_ADV_OPT_EXT_ADV | BT_LE_ADV_OPT_CODED | BT_LE_ADV_OPT_USE_IDENTITY ,
      BT_GAP_ADV_FAST_INT_MIN_2,
      BT_GAP_ADV_FAST_INT_MAX_2,
      NULL);

	err = bt_le_ext_adv_create(&param, NULL, &adv);
	if (err) {
		LOG_ERR("Failed to create advertiser set (err %d)\n", err);
		return err;
	}

	LOG_INF("Created adv: \n");

	err = bt_le_ext_adv_set_data(adv, ad, ARRAY_SIZE(ad), NULL, 0);
	if (err) {
		LOG_ERR("Failed to set advertising data (err %d)\n", err);
		return err;
	}

	return 0;
}

static void start_advertising_coded(void)
{
	int err;

  LOG_INF("Starting Adv....");

	err = bt_le_ext_adv_start(adv, NULL);
	if (err) {
		LOG_ERR("Failed to start advertising set (err %d)\n", err);
		return;
	}

	LOG_INF("Advertiser set started\n");
}

static void start_coded_adv(int err)
{
  if (err) {
    LOG_ERR("Enable BT failed, err %d", err);
    return;
  } else {
    LOG_INF("BT Enabled");
  }

  err = create_advertising_coded();
  if (err) {
		LOG_ERR("Advertising failed to create (err %d)\n", err);
		return;
	}

  start_advertising_coded();
}

void en_coded_adv(void)
{
  (void)bt_enable(start_coded_adv);
}
