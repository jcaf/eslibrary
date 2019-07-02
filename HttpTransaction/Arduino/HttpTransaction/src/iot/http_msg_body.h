#ifndef HTTP_MSG_BODY_H_
#define HTTP_MSG_BODY_H_

    #include <stdint.h>

    ////////////////////////////////////////////////////////////////////////////////////////////////
    //#define LONGLONG_TD_AVAILABLE
    //#define INT64T_TD_AVAILABLE
    enum http_msgbody_typedata{
        TD_BEGIN,
            //signeds:
            CHAR_TD,
            INT_TD,
            SHORT_TD,
            LONG_TD,
            LONGLONG_TD,
            //unsigneds:
            UCHAR_TD,
            UINT_TD,
            USHORT_TD,
            ULONG_TD,
            ULONGLONG_TD,
            //
            FLOAT_TD,
            DOUBLE_TD,
            //ANSI C99 <stdint.h>
            INT8_T_TD,
            INT16_T_TD,
            INT32_T_TD,
            INT64_T_TD,
            //
            UINT8_T_TD,
            UINT16_T_TD,
            UINT32_T_TD,
            UINT64_T_TD,
        TD_END,

        //special array:
        CSTRING_TD,//\0 NULL terminated

        //pure Array []
        ARRAY_TD_BEGIN,//begin of arrays []
            //signed arrays:
            CHAR_ARRAY_TD,
            INT_ARRAY_TD,
            SHORT_ARRAY_TD,
            LONG_ARRAY_TD,
            LONGLONG_ARRAY_TD,
            //unsigned arrays:
            UCHAR_ARRAY_TD,
            UINT_ARRAY_TD,
            USHORT_ARRAY_TD,
            ULONG_ARRAY_TD,
            ULONGLONG_ARRAY_TD,
            //
            FLOAT_ARRAY_TD,
            DOUBLE_ARRAY_TD,
            //
            INT8_T_ARRAY_TD,
            INT16_T_ARRAY_TD,
            INT32_T_ARRAY_TD,
            INT64_T_ARRAY_TD,
            //
            UINT8_T_ARRAY_TD,
            UINT16_T_ARRAY_TD,
            UINT32_T_ARRAY_TD,
            UINT64_T_ARRAY_TD,
        //
        ARRAY_TD_END,//end of arrays []
    };

    //////////////////////////////////////////////////////////////////////////////////////////
    #define _add_char(var)          #var, &var, CHAR_TD, 0
    #define _add_int(var)           #var, &var, INT_TD, 0
    #define _add_short(var)         #var, &var, SHORT_TD, 0
    #define _add_long(var)          #var, &var, LONG_TD, 0
    #define _add_longlong(var)      #var, &var, LONGLONG_TD, 0

    #define _add_uchar(var)         #var, &var, UCHAR_TD, 0
    #define _add_uint(var)          #var, &var, UINT_TD, 0
    #define _add_ushort(var)        #var, &var, USHORT_TD, 0
    #define _add_ulong(var)         #var, &var, ULONG_TD, 0
    #define _add_ulonglong(var)     #var, &var, ULONGLONG_TD, 0

    #define _add_float(var)         #var, &var, FLOAT_TD, 0
    #define _add_double(var)        #var, &var, DOUBLE_TD, 0
    //arrays:
    #define _add_char_array(var)        #var, &var, CHAR_ARRAY_TD, sizeof(var)/sizeof(char)
    #define _add_int_array(var)         #var, &var, INT_ARRAY_TD, sizeof(var)/sizeof(int)
    #define _add_short_array(var)       #var, &var, SHORT_ARRAY_TD, sizeof(var)/sizeof(short)
    #define _add_long_array(var)        #var, &var, LONG_ARRAY_TD, sizeof(var)/sizeof(long)
    #define _add_longlong_array(var)    #var, &var, LONGLONG_ARRAY_TD, sizeof(var)/sizeof(long long)

    #define _add_uchar_array(var)       #var, &var, UCHAR_ARRAY_TD, sizeof(var)/sizeof(unsigned char)
    #define _add_uint_array(var)        #var, &var, UINT_ARRAY_TD, sizeof(var)/sizeof(unsigned int)
    #define _add_ushort_array(var)      #var, &var, USHORT_ARRAY_TD, sizeof(var)/sizeof(unsigned short)
    #define _add_ulong_array(var)       #var, &var, ULONG_ARRAY_TD, sizeof(var)/sizeof(unsigned long)
    #define _add_ulonglong_array(var)   #var, &var, ULONGLONG_ARRAY_TD, sizeof(var)/sizeof(unsigned long long)

    #define _add_float_array(var)   #var, &var, FLOAT_ARRAY_TD, sizeof(var)/sizeof(float)
    #define _add_double_array(var)  #var, &var, DOUBLE_ARRAY_TD, sizeof(var)/sizeof(double)
    //special C-string array:
    #define _add_cstring(var)       #var, &var, CSTRING_TD, sizeof(var)/sizeof(char)
    //
    //
    #define _add_int8_t(var)        #var, &var, INT8_T_TD, 0
    #define _add_int16_t(var)       #var, &var, INT16_T_TD, 0
    #define _add_int32_t(var)       #var, &var, INT32_T_TD, 0
    #define _add_int64_t(var)       #var, &var, INT64_T_TD, 0
    //
    #define _add_uint8_t(var)        #var, &var, UINT8_T_TD, 0
    #define _add_uint16_t(var)       #var, &var, UINT16_T_TD, 0
    #define _add_uint32_t(var)       #var, &var, UINT32_T_TD, 0
    #define _add_uint64_t(var)       #var, &var, UINT64_T_TD, 0
    //
    #define _add_int8_t_array(var)      #var, &var, INT8_T_ARRAY_TD, sizeof(var)/sizeof(int8_t)
    #define _add_int16_t_array(var)     #var, &var, INT16_T_ARRAY_TD, sizeof(var)/sizeof(int16_t)
    #define _add_int32_t_array(var)     #var, &var, INT32_T_ARRAY_TD, sizeof(var)/sizeof(int32_t)
    #define _add_int64_t_array(var)     #var, &var, INT64_T_ARRAY_TD, sizeof(var)/sizeof(int64_t)
    //
    #define _add_uint8_t_array(var)     #var, &var, UINT8_T_ARRAY_TD, sizeof(var)/sizeof(uint8_t)
    #define _add_uint16_t_array(var)    #var, &var, UINT16_T_ARRAY_TD, sizeof(var)/sizeof(uint16_t)
    #define _add_uint32_t_array(var)    #var, &var, UINT32_T_ARRAY_TD, sizeof(var)/sizeof(uint32_t)
    #define _add_uint64_t_array(var)    #var, &var, UINT64_T_ARRAY_TD, sizeof(var)/sizeof(uint64_t)


    //////////////////////////////////////////////////////////////////////////////////////////
    #define http_msgbody_add_char(var)     http_rqstmsg_add_register(_add_char(var))
    #define http_msgbody_add_int(var)      http_rqstmsg_add_register(_add_int(var))
    #define http_msgbody_add_short(var)    http_rqstmsg_add_register(_add_short(var))
    #define http_msgbody_add_long(var)     http_rqstmsg_add_register(_add_long(var))
    #ifdef LONGLONG_TD_AVAILABLE
    #define http_msgbody_add_longlong(var) http_rqstmsg_add_register(_add_longlong(var))
    #endif // LONGLONG_TD_AVAILABLE
    //
    #define http_msgbody_add_uchar(var)        http_rqstmsg_add_register(_add_uchar(var))
    #define http_msgbody_add_uint(var)         http_rqstmsg_add_register(_add_uint(var))
    #define http_msgbody_add_ushort(var)       http_rqstmsg_add_register(_add_ushort(var))
    #define http_msgbody_add_ulong(var)        http_rqstmsg_add_register(_add_ulong(var))
    #ifdef LONGLONG_TD_AVAILABLE
    #define http_msgbody_add_ulonglong(var)    http_rqstmsg_add_register(_add_ulonglong(var))
    #endif // LONGLONG_TD_AVAILABLE
    //
    #define http_msgbody_add_float(var)        http_rqstmsg_add_register(_add_float(var))
    #define http_msgbody_add_double(var)       http_rqstmsg_add_register(_add_double(var))
    //
    #define http_msgbody_add_char_array(var)       http_rqstmsg_add_register(_add_char_array(var))
    #define http_msgbody_add_int_array(var)        http_rqstmsg_add_register(_add_int_array(var))
    #define http_msgbody_add_short_array(var)      http_rqstmsg_add_register(_add_short_array(var))
    #define http_msgbody_add_long_array(var)       http_rqstmsg_add_register(_add_long_array(var))
    #ifdef LONGLONG_TD_AVAILABLE
    #define http_msgbody_add_longlong_array(var)   http_rqstmsg_add_register(_add_longlong_array(var))
    #endif // LONGLONG_TD_AVAILABLE
    //
    #define http_msgbody_add_uchar_array(var)      http_rqstmsg_add_register(_add_uchar_array(var))
    #define http_msgbody_add_uint_array(var)       http_rqstmsg_add_register(_add_uint_array(var))
    #define http_msgbody_add_ushort_array(var)     http_rqstmsg_add_register(_add_ushort_array(var))
    #define http_msgbody_add_ulong_array(var)      http_rqstmsg_add_register(_add_ulong_array(var))
    #ifdef LONGLONG_TD_AVAILABLE
    #define http_msgbody_add_ulonglong_array(var)  http_rqstmsg_add_register(_add_ulonglong_array(var))
    #endif // LONGLONG_TD_AVAILABLE
    //
    #define http_msgbody_add_float_array(var)  http_rqstmsg_add_register(_add_float_array(var))
    #define http_msgbody_add_double_array(var) http_rqstmsg_add_register(_add_double_array(var))
    //
    #define http_msgbody_add_cstring(var)      http_rqstmsg_add_register(_add_cstring(var))
    //
    //
    #define http_msgbody_add_int8_t(var)   http_rqstmsg_add_register(_add_int8_t(var))
    #define http_msgbody_add_int16_t(var)  http_rqstmsg_add_register(_add_int16_t(var))
    #define http_msgbody_add_int32_t(var)  http_rqstmsg_add_register(_add_int32_t(var))
    #ifdef INT64T_TD_AVAILABLE
    #define http_msgbody_add_int64_t(var)  http_rqstmsg_add_register(_add_int64_t(var))
    #endif // INT64T_TD_AVAILABLE
    //
    #define http_msgbody_add_uint8_t(var)  http_rqstmsg_add_register(_add_uint8_t(var))
    #define http_msgbody_add_uint16_t(var) http_rqstmsg_add_register(_add_uint16_t(var))
    #define http_msgbody_add_uint32_t(var) http_rqstmsg_add_register(_add_uint32_t(var))
    #ifdef INT64T_TD_AVAILABLE
    #define http_msgbody_add_uint64_t(var) http_rqstmsg_add_register(_add_uint64_t(var))
    #endif // INT64T_TD_AVAILABLE
    //
    #define http_msgbody_add_int8_t_array(var)     http_rqstmsg_add_register(_add_int8_t_array(var))
    #define http_msgbody_add_int16_t_array(var)    http_rqstmsg_add_register(_add_int16_t_array(var))
    #define http_msgbody_add_int32_t_array(var)    http_rqstmsg_add_register(_add_int32_t_array(var))
    #ifdef INT64T_TD_AVAILABLE
    #define http_msgbody_add_int64_t_array(var)    http_rqstmsg_add_register(_add_int64_t_array(var))
    #endif // INT64T_TD_AVAILABLE
    //
    #define http_msgbody_add_uint8_t_array(var)    http_rqstmsg_add_register(_add_uint8_t_array(var))
    #define http_msgbody_add_uint16_t_array(var)   http_rqstmsg_add_register(_add_uint16_t_array(var))
    #define http_msgbody_add_uint32_t_array(var)   http_rqstmsg_add_register(_add_uint32_t_array(var))
    #ifdef INT64T_TD_AVAILABLE
    #define http_msgbody_add_uint64_t_array(var)   http_rqstmsg_add_register(_add_uint64_t_array(var))
    #endif // INT64T_TD_AVAILABLE

    ////////////////////////////////////////////////////////////////////////////////////////////////


    ////////////////////////////////////////////////////////////////////////////////////////////////
    #define FLOAT_N_DECIMALS 2
    //#define FLOAT_FORMAT_SPEC "%.2f" //unir ambos formatos
    //
    #define CONCATKF(STR) #STR    //Stringing final string
    #define _FORMAT_B(K_TO_CONCATENATE) CONCATKF(K_TO_CONCATENATE ## f)//Concatenate K to f
    #define FORMAT_B(K_TO_CONCATENATE) _FORMAT_B(K_TO_CONCATENATE)
    #define FORMAT_A "%."
    #define FLOAT_FORMAT_SPEC FORMAT_A FORMAT_B(FLOAT_N_DECIMALS)


    typedef uint16_t sizedata_t;    //max size 2^16 = 65535
    struct http_msgbody_register_t
    {
        #define REG_NAME_SIZE (10+1)//6+ \0 Max.num of characters allocated by the field "name" into  struct regsiter
        char name[REG_NAME_SIZE];
        //char name[1];
        void *addr;
        uint8_t type;
        sizedata_t size;//util for vector only.
    };
    #define REG_NUMMAX 21 //Num.of variables=registers [Max 127 defined by idx]
    extern struct http_msgbody_register_t body_register[REG_NUMMAX];

    ////////////////////////////////////////////////////////////////////////////////////////////////
    void http_rqstmsg_add_register(const char *name, void * addr, uint8_t type, sizedata_t size);
    int8_t body_register_get_num_regs(void);

    ///////////////////////////
    char* body_register_get_name(int8_t reg);
    void* body_register_get_addr(int8_t reg);
    int8_t body_register_get_type(int8_t reg);
    sizedata_t body_register_get_size(int8_t reg);


#endif

