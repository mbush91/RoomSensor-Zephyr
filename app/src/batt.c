#include "batt.h"

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(batt, LOG_LEVEL_INF);

#if !DT_NODE_EXISTS(DT_PATH(zephyr_user)) || \
    !DT_NODE_HAS_PROP(DT_PATH(zephyr_user), io_channels)
#error "No suitable devicetree overlay specified"
#endif

#define DT_SPEC_AND_COMMA(node_id, prop, idx) \
  ADC_DT_SPEC_GET_BY_IDX(node_id, idx),

/* Data of ADC io-channels specified in devicetree. */
static const struct adc_dt_spec adc_channels[] = {
    DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), io_channels,
                         DT_SPEC_AND_COMMA)};

void batt_init(void)
{
  int err;

  for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++)
  {
    if (!adc_is_ready_dt(&adc_channels[i]))
    {
      LOG_ERR("ADC controller device %s not ready\n", adc_channels[i].dev->name);
      return;
    }
    else
    {
      LOG_INF("Device %s is ready", adc_channels[i].dev->name);
    }

    err = adc_channel_setup_dt(&adc_channels[i]);
    if (err < 0)
    {
      LOG_ERR("Could not setup channel #%d (%d)\n", i, err);
      return;
    }
    else
    {
      LOG_INF("Setup %s", adc_channels[i].dev->name);
    }
  }
}

int16_t batt_read(void)
{
  int32_t val_mv = 0;
  int err;
  uint16_t buf;
  struct adc_sequence sequence = {
      .buffer = &buf,
      /* buffer size in bytes, not number of samples */
      .buffer_size = sizeof(buf),
  };

  for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++)
  {

    LOG_INF("- %s, channel %d: ",
           adc_channels[i].dev->name,
           adc_channels[i].channel_id);

    (void)adc_sequence_init_dt(&adc_channels[i], &sequence);

    err = adc_read(adc_channels[i].dev, &sequence);
    if (err < 0)
    {
      LOG_ERR("Could not read (%d)\n", err);
      continue;
    }

    val_mv = (int32_t)buf;

    LOG_INF("%" PRId32, val_mv);
    err = adc_raw_to_millivolts_dt(&adc_channels[i],
                                   &val_mv);
    /* conversion to mV may not be supported, skip if not */
    if (err < 0)
    {
      LOG_ERR(" (value in mV not available)\n");
    }
    else
    {
      LOG_INF(" = %" PRId32 " mV\n", val_mv);
    }
  }

  return (int16_t)val_mv;
}
