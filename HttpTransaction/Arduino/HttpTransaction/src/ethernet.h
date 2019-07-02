#ifndef ETHERNET_H_
#define ETHERNET_H_
    extern "C"
    {
        #include "iot/ntsetting.h"
    }
    int8_t ethernet_begin(void);

    void nt_debug_begin(long baud);

//compile-time
#define NT_DEBUG_PRINT

    #if defined(__AVR__) && defined(__GNUC__)
      #include <Arduino.h>
      #ifdef Arduino_h

            #define nt_debug_print(d) do{ if (ntsetting_serialdbg_is_enabled()){ Serial.print(d);}}while(0)

            //#define nt_debug_print_prgmem(d) do{ if (ntsetting_serialdbg_is_enabled()){ Serial.print( reinterpret_cast<const __FlashStringHelper *> (d) );}}while(0)

            #ifdef NT_DEBUG_PRINT
                #define nt_debug_print_prgmem(d) do{Serial.print( reinterpret_cast<const __FlashStringHelper *> (d) );}while(0)
            #else
                #define nt_debug_print_prgmem(d)
            #endif



            #define nt_debug_print_float(d) do{ if (ntsetting_serialdbg_is_enabled()){ Serial.print(d, FLOAT_N_DECIMALS);}}while(0)

      #endif // defined
   #else
        #define nt_debug_print(d) do{ if (ntsetting_serialdbg_is_enabled()){ printf(d);}}while(0)
   #endif // Arduino_h

#endif // ETHERNET_H_
