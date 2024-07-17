#ifndef RS_BATT
#define RS_BATT

#include <stdint.h>

void batt_init(void);
int16_t batt_read(void);

#endif // RS_BATT
