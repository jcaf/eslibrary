
#include <xc.h>
#include <stdint.h>
#include "kk.h"

//#define KKB_NUM_KEYS 16
int mm[KKB_NUM_KEYS];

void ikey_scanx(void)
{
    int i;
    int8_t k = KKB_NUM_KEYS-1;
    for (i=0; i<KKB_NUM_KEYS; i++ ) 
        key[i].keyRead();
}

