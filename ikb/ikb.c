#include "main.h"
#include "system.h"
#include "ikb.h"

    struct _key
    {
        PTRFX_retUINT8_T keyRead;
        PTRFX_retVOID keyDo;
        uint8_t sm0;        //maquina de estados
        uint8_t counter0;//contador
        uint8_t sm1;
        uint16_t counter1;
        uint8_t  num_group_x;
        struct _bf
        {
            unsigned state:1;

            unsigned OnKeyPressed:1;//when pressed
            unsigned Reptt:1;
            unsigned OnKeyReleased:1;//when released
            unsigned whilePressing:1;//new 2019
            unsigned AtTimeExpired:1;
            unsigned AtTimeExpired2:1;//new mode 2017
            //
            
            unsigned ReadyToRead:1;		//soft-populate
            unsigned AtTimeExpired_BeforeOrAfter:1;//para usar a nivel de app
            unsigned OwnerOfGroup:1;
            unsigned InProcessing:1;
        } bf;
        struct _repp
        {
            uint16_t breakTime;     //break this time to enter to repetition
            uint16_t period;        //each time access to repp after the "breakTime"
        } repp;
    };
    volatile struct _key key[KB_NUM_KEYS];
    
    void ikey_scan(void);
    void ikey_parsing(void);
    int8_t ikey_is_all_hw_released(void);
    void ikey_clear_all_flag(void);
    int8_t ikey_is_all_process_finished(void);
    
#ifdef iKPAD

    static inline uint8_t keyRead_pin_key0(void)
    {
        PinTo0(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo1(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo1(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo1(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return ReadPin(PORTRxKBCOL_KEY0, PINxKBCOL_KEY0);
    }
    static inline uint8_t keyRead_pin_key1(void)
    {
        PinTo0(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo1(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo1(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo1(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return ReadPin(PORTRxKBCOL_KEY1, PINxKBCOL_KEY1);
    }
    static inline uint8_t keyRead_pin_key2(void)
    {
        PinTo0(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo1(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo1(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo1(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return ReadPin(PORTRxKBCOL_KEY2, PINxKBCOL_KEY2);
    }
    static inline uint8_t keyRead_pin_key3(void)
    {
        PinTo0(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo1(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo1(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo1(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return ReadPin(PORTRxKBCOL_KEY3, PINxKBCOL_KEY3);
    }
    //
    static inline uint8_t keyRead_pin_key4(void)
    {
        PinTo1(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo0(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo1(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo1(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return ReadPin(PORTRxKBCOL_KEY4, PINxKBCOL_KEY4);
    }
    static inline uint8_t keyRead_pin_key6(void)
    {
        PinTo1(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo0(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo1(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo1(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return ReadPin(PORTRxKBCOL_KEY5, PINxKBCOL_KEY5);
    }
    static inline uint8_t keyRead_pin_key7(void)
    {
        PinTo1(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo0(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo1(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo1(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return ReadPin(PORTRxKBCOL_KEY6, PINxKBCOL_KEY6);
    }
    static inline uint8_t keyRead_pin_key8(void)
    {
        PinTo1(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo0(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo1(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo1(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return ReadPin(PORTRxKBCOL_KEY7, PINxKBCOL_KEY7);
    }
    //
    static inline uint8_t keyRead_pin_key9(void)
    {
        PinTo1(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo1(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo0(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo1(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return ReadPin(PORTRxKBCOL_KEY8, PINxKBCOL_KEY8);
    }
    static inline uint8_t keyRead_pin_key10(void)
    {
        PinTo1(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo1(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo0(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo1(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return ReadPin(PORTRxKBCOL_KEY9, PINxKBCOL_KEY9);
    }
    static inline uint8_t keyRead_pin_key11(void)
    {
        PinTo1(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo1(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo0(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo1(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return ReadPin(PORTRxKBCOL_KEY10, PINxKBCOL_KEY10);
    }
    static inline uint8_t keyRead_pin_key12(void)
    {
        PinTo1(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo1(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo0(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo1(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return ReadPin(PORTRxKBCOL_KEY11, PINxKBCOL_KEY11);
    }
    static inline uint8_t keyRead_pin_key13(void)
    {
        PinTo1(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo1(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo1(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo0(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return ReadPin(PORTRxKBCOL_KEY12, PINxKBCOL_KEY12);
    }
    static inline uint8_t keyRead_pin_key14(void)
    {
        PinTo1(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo1(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo1(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo0(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return ReadPin(PORTRxKBCOL_KEY13, PINxKBCOL_KEY13);
    }
    static inline uint8_t keyRead_pin_key15(void)
    {
        PinTo1(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo1(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo1(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo0(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return ReadPin(PORTRxKBCOL_KEY14, PINxKBCOL_KEY14);
    }
    static inline uint8_t keyRead_pin_key16(void)
    {
        PinTo1(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo1(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo1(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo0(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return ReadPin(PORTRxKBCOL_KEY15, PINxKBCOL_KEY15);
    }
#endif // iKB

#ifdef iKEY
    static inline uint8_t keyRead_pin_key0(void)
    {
        return ReadPin(PORTRxKB_KEY0, PINxKB_KEY0);
    }
    static inline uint8_t keyRead_pin_key1(void)
    {
        return ReadPin(PORTRxKB_KEY1, PINxKB_KEY1);
    }
    static inline uint8_t keyRead_pin_key2(void)
    {
        return ReadPin(PORTRxKB_KEY2, PINxKB_KEY2);
    }
    static inline uint8_t keyRead_pin_key3(void)
    {
        return ReadPin(PORTRxKB_KEY3, PINxKB_KEY3);
    }
    static inline uint8_t keyRead_pin_key4(void)
    {
        return ReadPin(PORTRxKB_KEY4, PINxKB_KEY4);
    }
   
#endif // iKEY

    void ikb_init(void)
    {
        int8_t i;
        #ifdef iKPAD
            ConfigOutputPin(CONFIGIOxKBFIL_0, PINxKBFIL_0);
            ConfigOutputPin(CONFIGIOxKBFIL_1, PINxKBFIL_1);
            ConfigOutputPin(CONFIGIOxKBFIL_2, PINxKBFIL_2);
            ConfigOutputPin(CONFIGIOxKBFIL_3, PINxKBFIL_3);

            ConfigInputPin(CONFIGIOxKBCOL_0, PINxKBCOL_0);
            ConfigInputPin(CONFIGIOxKBCOL_1, PINxKBCOL_1);
            ConfigInputPin(CONFIGIOxKBCOL_2, PINxKBCOL_2);
            ConfigInputPin(CONFIGIOxKBCOL_3, PINxKBCOL_3);

            key[0].keyRead = keyRead_pin_key0;
            key[1].keyRead = keyRead_pin_key1;
            key[2].keyRead = keyRead_pin_key2;
            key[3].keyRead = keyRead_pin_key3;
            key[4].keyRead = keyRead_pin_key4;
            key[5].keyRead = keyRead_pin_key6;
            key[6].keyRead = keyRead_pin_key7;
            key[7].keyRead = keyRead_pin_key8;
            key[8].keyRead = keyRead_pin_key9;
            key[9].keyRead = keyRead_pin_key10;
            key[10].keyRead = keyRead_pin_key11;
            key[11].keyRead = keyRead_pin_key12;
            key[12].keyRead = keyRead_pin_key13;
            key[13].keyRead = keyRead_pin_key14;
            key[14].keyRead = keyRead_pin_key15;
            key[15].keyRead = keyRead_pin_key16;

            for (i=0; i<KB_NUM_KEYS; i++)
            {
                key[i].bf.OnKeyPressed = 1;
                key[i].bf.OnKeyReleased = 0;
                key[i].bf.AtTimeExpired = 0;
                key[i].bf.Reptt = 0;
                key[i].num_group_x = 0;
                key[i].repp.breakTime= 500.0/5;
                key[i].repp.period = 200.0/5;
            }
            /*
            key[0].bf.OnKeyPressed = 1;
            key[0].bf.OnKeyPressedAndKeyRelease = 0;
            key[0].bf.AtTimeExpired = 0;
            key[0].bf.Reptt = 0;
            key[0].num_group_x = 0;
            key[0].repp.to_start= 500.0/5;
            key[0].repp.after_start = 200.0/5;

            key[1].bf.OnKeyPressed = 1;
            key[1].bf.OnKeyPressedAndKeyRelease = 0;
            key[1].bf.AtTimeExpired = 0;
            key[1].bf.Reptt = 0;
            key[1].num_group_x = 0;
            key[1].repp.to_start= 500.0/5;
            key[1].repp.after_start = 200.0/5;

            key[2].bf.OnKeyPressed = 1;
            key[2].bf.OnKeyPressedAndKeyRelease = 0;
            key[2].bf.AtTimeExpired = 0;
            key[2].bf.Reptt = 0;
            key[2].num_group_x = 0;
            key[2].repp.to_start= 500.0/5;
            key[2].repp.after_start = 200.0/5;

            key[3].bf.OnKeyPressed = 1;
            key[3].bf.OnKeyPressedAndKeyRelease = 0;
            key[3].bf.AtTimeExpired = 0;
            key[3].bf.Reptt = 0;
            key[3].num_group_x = 0;
            key[3].repp.to_start= 500.0/5;
            key[3].repp.after_start = 200.0/5;

            key[4].bf.OnKeyPressed = 1;
            key[4].bf.OnKeyPressedAndKeyRelease = 0;
            key[4].bf.AtTimeExpired = 0;
            key[4].bf.Reptt = 0;
            key[4].num_group_x = 0;
            key[4].repp.to_start= 500.0/5;
            key[4].repp.after_start = 200.0/5;

            key[5].bf.OnKeyPressed = 1;
            key[5].bf.OnKeyPressedAndKeyRelease = 0;
            key[5].bf.AtTimeExpired = 0;
            key[5].bf.Reptt = 0;
            key[5].num_group_x = 0;
            key[5].repp.to_start= 500.0/5;
            key[5].repp.after_start = 200.0/5;

            key[6].bf.OnKeyPressed = 1;
            key[6].bf.OnKeyPressedAndKeyRelease = 0;
            key[6].bf.AtTimeExpired = 0;
            key[6].bf.Reptt = 0;
            key[6].num_group_x = 0;
            key[6].repp.to_start= 500.0/5;
            key[6].repp.after_start = 200.0/5;

            key[7].bf.OnKeyPressed = 1;
            key[7].bf.OnKeyPressedAndKeyRelease = 0;
            key[7].bf.AtTimeExpired = 0;
            key[7].bf.Reptt = 0;
            key[7].num_group_x = 0;
            key[7].repp.to_start= 500.0/5;
            key[7].repp.after_start = 200.0/5;

            key[8].bf.OnKeyPressed = 1;
            key[8].bf.OnKeyPressedAndKeyRelease = 0;
            key[8].bf.AtTimeExpired = 0;
            key[8].bf.Reptt = 0;
            key[8].num_group_x = 0;
            key[8].repp.to_start= 500.0/5;
            key[8].repp.after_start = 200.0/5;

            key[9].bf.OnKeyPressed = 1;
            key[9].bf.OnKeyPressedAndKeyRelease = 0;
            key[9].bf.AtTimeExpired = 0;
            key[9].bf.Reptt = 0;
            key[9].num_group_x = 0;
            key[9].repp.to_start= 500.0/5;
            key[9].repp.after_start = 200.0/5;

            key[10].bf.OnKeyPressed = 1;
            key[10].bf.OnKeyPressedAndKeyRelease = 0;
            key[10].bf.AtTimeExpired = 0;
            key[10].bf.Reptt = 0;
            key[10].num_group_x = 0;
            key[10].repp.to_start= 500.0/5;
            key[10].repp.after_start = 200.0/5;

            key[11].bf.OnKeyPressed = 1;
            key[11].bf.OnKeyPressedAndKeyRelease = 0;
            key[11].bf.AtTimeExpired = 0;
            key[11].bf.Reptt = 0;
            key[11].num_group_x = 0;
            key[11].repp.to_start= 500.0/5;
            key[11].repp.after_start = 200.0/5;

            key[12].bf.OnKeyPressed = 1;
            key[12].bf.OnKeyPressedAndKeyRelease = 0;
            key[12].bf.AtTimeExpired = 0;
            key[12].bf.Reptt = 0;
            key[12].num_group_x = 0;
            key[12].repp.to_start= 500.0/5;
            key[12].repp.after_start = 200.0/5;

            key[13].bf.OnKeyPressed = 1;
            key[13].bf.OnKeyPressedAndKeyRelease = 0;
            key[13].bf.AtTimeExpired = 0;
            key[13].bf.Reptt = 0;
            key[13].num_group_x = 0;
            key[13].repp.to_start= 500.0/5;
            key[13].repp.after_start = 200.0/5;

            key[14].bf.OnKeyPressed = 1;
            key[14].bf.OnKeyPressedAndKeyRelease = 0;
            key[14].bf.AtTimeExpired = 0;
            key[14].bf.Reptt = 0;
            key[14].num_group_x = 0;
            key[14].repp.to_start= 500.0/5;
            key[14].repp.after_start = 200.0/5;

            key[15].bf.OnKeyPressed = 1;
            key[15].bf.OnKeyPressedAndKeyRelease = 0;
            key[15].bf.AtTimeExpired = 0;
            key[15].bf.Reptt = 0;
            key[15].num_group_x = 0;
            key[15].repp.to_start= 500.0/5;
            key[15].repp.after_start = 200.0/5;
            */
        #endif // iKPAD
        #ifdef iKEY
            ConfigInputPin(CONFIGIOxKB_KEY0, PINxKB_KEY0);
            ConfigInputPin(CONFIGIOxKB_KEY1, PINxKB_KEY1);
            ConfigInputPin(CONFIGIOxKB_KEY2, PINxKB_KEY2);
            ConfigInputPin(CONFIGIOxKB_KEY3, PINxKB_KEY3);
            ConfigInputPin(CONFIGIOxKB_KEY4, PINxKB_KEY4);
            key[0].keyRead = keyRead_pin_key0;
            key[1].keyRead = keyRead_pin_key1;
            key[2].keyRead = keyRead_pin_key2;
            key[3].keyRead = keyRead_pin_key3;
            key[4].keyRead = keyRead_pin_key4;
            
            //+-
            key[0].bf.OnKeyPressed = 1;
            key[0].bf.OnKeyReleased = 0;
            key[0].bf.AtTimeExpired = 0;
            key[0].bf.AtTimeExpired2 = 0;
            key[0].bf.whilePressing = 0;
            key[0].num_group_x = 1;
            //
            key[0].bf.Reptt = 1;
            key[0].repp.breakTime= (uint16_t)500.0/KB_PERIODIC_ACCESS;
            key[0].repp.period = (uint16_t)300.0/KB_PERIODIC_ACCESS;
            //-+
            //+-
            key[1].bf.OnKeyPressed = 1;
            key[1].bf.OnKeyReleased = 0;
            key[1].bf.AtTimeExpired = 0;
            key[1].bf.AtTimeExpired2 = 0;
            key[1].bf.whilePressing = 0;
            key[1].num_group_x = 1;
            //
            key[1].bf.Reptt = 1;
            key[1].repp.breakTime= (uint16_t)500.0/KB_PERIODIC_ACCESS;
            key[1].repp.period = (uint16_t)300.0/KB_PERIODIC_ACCESS;
            //-+
            //+-
            key[2].bf.OnKeyPressed = 0;
            key[2].bf.OnKeyReleased = 0;
            key[2].bf.AtTimeExpired = 0;
            key[2].bf.AtTimeExpired2 = 1;
            key[2].bf.whilePressing = 0;
            key[2].num_group_x = 0;
            //
            key[2].bf.Reptt = 0;
            key[2].repp.breakTime= (uint16_t)300.0/KB_PERIODIC_ACCESS;
            key[2].repp.period = (uint16_t)800.0/KB_PERIODIC_ACCESS;
            //-+
            //+-
            key[3].bf.OnKeyPressed = 0;
            key[3].bf.OnKeyReleased = 0;
            key[3].bf.AtTimeExpired = 0;
            key[3].bf.AtTimeExpired2 = 1;
            key[3].bf.whilePressing = 0;
            key[3].num_group_x = 0;
            //
            key[3].bf.Reptt = 0;
            key[3].repp.breakTime= (uint16_t)300.0/KB_PERIODIC_ACCESS;
            key[3].repp.period = (uint16_t)800.0/KB_PERIODIC_ACCESS;
            //-+
            
             //+-
            key[4].bf.OnKeyPressed = 0;
            key[4].bf.OnKeyReleased = 0;
            key[4].bf.AtTimeExpired = 0;
            key[4].bf.AtTimeExpired2 = 0;
            key[4].bf.whilePressing = 1;
            key[4].num_group_x = 0;
            //
            key[4].bf.Reptt = 0;
            key[4].repp.breakTime= (uint16_t)300.0/KB_PERIODIC_ACCESS;
            key[4].repp.period = (uint16_t)800.0/KB_PERIODIC_ACCESS;
            //-+
            
           
        #endif // iKEY
    }

void ikb_job(void)
{
    ikey_scan();
    ikey_parsing();
}
void ikb_flush(void)
{
    ikey_clear_all_flag();
}
uint8_t ikb_get_AtTimeExpired_BeforeOrAfter(uint8_t k)
{
    return key[k].bf.AtTimeExpired_BeforeOrAfter;
}
uint8_t ikb_key_is_ready2read(uint8_t k)
{
    return key[k].bf.ReadyToRead;
}
void ikb_set_ready2read(uint8_t k)
{
    key[k].bf.ReadyToRead = 1;
}
void ikb_key_was_read(uint8_t k)
{
    key[k].bf.ReadyToRead = 0;
}
void ikb_execfunct(uint8_t k)
{
    key[k].keyDo();
}

/************* ikey ****************/
//set by software
#define KB_KEY_STATE_PRESSED 1
#define KB_KEY_STATE_RELEASED 0

#define IKEY_SCAN_SETUP_TIMEuS 5//us

void ikey_scan(void)
{
    int8_t k = KB_NUM_KEYS-1;
    do
    {
        __delay_us(IKEY_SCAN_SETUP_TIMEuS);
        
        if (key[k].sm0 == 0)
        {
            if (key[k].keyRead() == KB_KEY_PINLEVEL_PRESSED)
            {
                key[k].sm0++;
            }
        }
        else if (key[k].sm0 == 1)
        {
            if (++key[k].counter0 == KB_KEY_SCAN_COUNT_DEBOUNCE)
            {
                key[k].counter0 = 0x00;

                if (key[k].keyRead() != KB_KEY_PINLEVEL_PRESSED)
                {
                    key[k].sm0--;
                }
                else
                {
                    key[k].bf.state = KB_KEY_STATE_PRESSED;
                    key[k].sm0++;
                }
            }
        }
        else if (key[k].sm0 == 2)
        {
            if (key[k].keyRead() == KB_KEY_PINLEVEL_RELEASED)
                key[k].sm0++;
        }
        else if (key[k].sm0 == 3)
        {
            if (++key[k].counter0 == KB_KEY_SCAN_COUNT_DEBOUNCE)
            {
                key[k].counter0 = 0x00;

                if (key[k].keyRead() != KB_KEY_PINLEVEL_RELEASED)
                {
                    key[k].sm0--;
                }
                else
                {
                    key[k].bf.state = KB_KEY_STATE_RELEASED;
                    key[k].sm0 = 0x00;
                }
            }
        }
    
    }while (--k >= 0);
}

///////////////////////////////////////////////////////////////////////////
#define NO_GROUP_X 0
//#define AGRUPED 1
#define GROUP_LOCKED 1
#define GROUP_UNLOCKED	0

uint8_t group_x[KB_NUM_KEYS];

static uint8_t ikey_is_accessible(uint8_t k)
{
    if (key[k].num_group_x > NO_GROUP_X)	//esta agrupado ?
    {
        //aqui fue puesto al presionar la tecla
        if (group_x[key[k].num_group_x] == GROUP_LOCKED)
        {
            //a todos sus companeros se les prohibe el ingreso, solo quien fue el primero en ganar el acceso al grupo entra
            if ( !key[k].bf.OwnerOfGroup)	//Si no es quien gano la propiedad...
            {
                return 0;
            }
        }
    }
    return 1;
}

///////////////////////////////////////////////////////////////////////////
#define KBAPP_READY2READ	1
#define KBAPP_ALREADYREAD	0

//Stages definition, incremental and contiguous order
enum _STAGE_
{
    _STAGE_START_PARSING_=0,
    _STAGE_ISKEY_RELEASED_,
    _STAGE_ISKEY_RELEASED2_,
    _STAGE_ISKEY_READFROMAPP_,
    _STAGE_FIREATIMEEXPIRED_,
    _STAGE_FIREATIMEEXPIRED_2_,
    _STAGE_ONKEYRELEASED_,
    _STAGE_WHILEPRESSING_,
    //Reptt, _1, _2 deben ser contiguos!
    _STAGE_REPTT_,
    _STAGE_REPTT_1_,
    _STAGE_REPTT_2_,
    //
    _STAGE_END_PARSING_,
};

void ikey_parsing(void)
{
    uint8_t k;

    for (k=0; k < KB_NUM_KEYS; k++)
    {
        if ( key[k].sm1 == _STAGE_START_PARSING_ )
        {
            if ( ikey_is_accessible(k) )
            {
                if (key[k].bf.state == KB_KEY_STATE_PRESSED)
                {
                    //........................................................
                    if (key[k].bf.OnKeyPressed)
                    {
                        key[k].bf.ReadyToRead = KBAPP_READY2READ;	//Populate Key to app.

                        if (key[k].bf.Reptt)
                        {
                            key[k].sm1 = _STAGE_REPTT_;
                        }
                        else
                        {
                            key[k].sm1 = _STAGE_ISKEY_RELEASED_;
                        }
                    }
                    else if ( key[k].bf.OnKeyReleased)
                    {
                        key[k].sm1 = _STAGE_ONKEYRELEASED_;
                    }
                    else if ( key[k].bf.whilePressing)
                    {
                        key[k].sm1 = _STAGE_WHILEPRESSING_;
                    }
                    else if (key[k].bf.AtTimeExpired)
                    {
                        key[k].sm1 = _STAGE_FIREATIMEEXPIRED_;
                    }
                    else if (key[k].bf.AtTimeExpired2)
                    {
                        key[k].sm1 = _STAGE_FIREATIMEEXPIRED_2_;
                    }

                    //Add++
                    key[k].bf.InProcessing = 1;
                    //++

                    //........................................................

                    if (key[k].num_group_x > NO_GROUP_X)
                    {
                        group_x[ key[k].num_group_x ] = GROUP_LOCKED;
                        key[k].bf.OwnerOfGroup = 1;
                    }
                    //........................................................
                }
            }
        }

        //_________________________________________________________________________________________
        //comprueba entre las pasadas de repeticion/no repeticion si se ha soltado la tecla
        if ( (key[k].sm1 >= _STAGE_REPTT_ ) && (key[k].sm1 <= _STAGE_REPTT_2_))
        {
            if (key[k].bf.state == KB_KEY_STATE_RELEASED)			//ya solto?
            {
                key[k].counter1 = 0;				//stop timmer
                key[k].sm1 = _STAGE_ISKEY_READFROMAPP_;
            }
        }
        //.............................................................................
        if (key[k].sm1 == _STAGE_REPTT_ )
        {
            if ( ++key[k].counter1 >= key[k].repp.breakTime)
            {
                key[k].counter1 =0;
                key[k].sm1 = _STAGE_REPTT_1_;
            }
        }
        if (key[k].sm1 == _STAGE_REPTT_1_)	//ping-pong between 1<->2
        {
            if (key[k].bf.ReadyToRead == KBAPP_ALREADYREAD)			//ya leyo?
            {
                key[k].bf.ReadyToRead = KBAPP_READY2READ;				//Repopulate again
                key[k].sm1 = _STAGE_REPTT_2_;
            }
        }
        if (key[k].sm1 == _STAGE_REPTT_2_)	//ping-pong 2<->1
        {
            if ( ++key[k].counter1 >= key[k].repp.period)
            {
                key[k].counter1 = 0;
                key[k].sm1 = _STAGE_REPTT_1_;
            }
        }
        //_________________________________________________________________________________________
        if (key[k].sm1 == _STAGE_ONKEYRELEASED_ )
        {
            if (key[k].bf.state == KB_KEY_STATE_RELEASED)	//ya solto?
            {
                key[k].bf.ReadyToRead = KBAPP_READY2READ;							//Populate Key to app.
                key[k].sm1 = _STAGE_ISKEY_READFROMAPP_;		//esperar x leer
            }
        }
        if (key[k].sm1 == _STAGE_WHILEPRESSING_ )   //added 2019
        {
            key[k].bf.ReadyToRead = KBAPP_READY2READ;	//Populate Key to app.
            key[k].sm1 = _STAGE_ISKEY_RELEASED2_;
        }
        //_________________________________________________________________________________________
        if (key[k].sm1 == _STAGE_FIREATIMEEXPIRED_ )
        {
            key[k].counter1++;

            if (key[k].bf.state == KB_KEY_STATE_RELEASED)	//ya solto?
            {
                if (key[k].counter1 <= (_FIRE_AT_TIME_THRESHOLD_) )	//USANDO UN SOLO CONTADOR,
                {
                    key[k].counter1	= 0;							//PUES SE USA EN TIEMPOS DIFERENTES

                    key[k].bf.ReadyToRead = KBAPP_READY2READ;
                    key[k].bf.AtTimeExpired_BeforeOrAfter = KB_BEFORE_THR;

                    key[k].sm1 = _STAGE_ISKEY_READFROMAPP_;		//Go directly to wait for read key from high-level application
                }
            }

            if (key[k].counter1 > (_FIRE_AT_TIME_THRESHOLD_) )
            {
                key[k].counter1 = 0x0000;

                key[k].bf.ReadyToRead = KBAPP_READY2READ;
                key[k].bf.AtTimeExpired_BeforeOrAfter = KB_AFTER_THR;	//queda senializado

                if (!key[k].bf.Reptt)							//x No Reptt
                {
                    key[k].sm1 = _STAGE_ISKEY_RELEASED_;   //wait for key is release
                }
                else
                {
                    key[k].sm1 = _STAGE_REPTT_2_;
                }
            }
        }
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        //NO SE ADMITE REPETICION
        //.........|thr|....................................|thr2|
        //before       | no se reconoce como pulsacion      |aftersi cumplio este tiempo, se reconoce como la segunda pulsacion
        if (key[k].sm1 == _STAGE_FIREATIMEEXPIRED_2_)//nueva opcion 2017
        {
            key[k].counter1++;

            if (key[k].bf.state == KB_KEY_STATE_RELEASED) //ya solto?
            {
                if (key[k].counter1 <= (_FIRE_AT_TIME_THRESHOLD_)) //primer threshold
                {
                    key[k].counter1 = 0; //PUES SE USA EN TIEMPOS DIFERENTES

                    key[k].bf.ReadyToRead = KBAPP_READY2READ;
                    key[k].bf.AtTimeExpired_BeforeOrAfter = KB_BEFORE_THR;

                    key[k].sm1 = _STAGE_ISKEY_READFROMAPP_; //Go directly to wait for read key from high-level application
                }
            }

            //#define _FIRE_AT_TIME_THRESHOLD2_ (3000E-3/KB_PERIODIC_ACCESS)
            if (key[k].counter1 >= (_FIRE_AT_TIME_THRESHOLD2_))
            {
                key[k].counter1 = 0x0000;

                key[k].bf.ReadyToRead = KBAPP_READY2READ;
                key[k].bf.AtTimeExpired_BeforeOrAfter = KB_AFTER_THR; //queda senializado

                //NO SE ADMITE REPETICION
                key[k].sm1 = _STAGE_ISKEY_RELEASED2_; //wait for key is release
            }
        }
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        //_________________________________________________________________________________________
        if (key[k].sm1 == _STAGE_ISKEY_RELEASED_)		//MODO->FireOnKeyPressed + no Repetitibilidad 	//MODO->solo esperar que suelte
        {
            if (key[k].bf.state == KB_KEY_STATE_RELEASED)						//ya solto?
            {
                key[k].sm1 = _STAGE_ISKEY_READFROMAPP_;   //esperar x leer
            }
        }
        //_________________________________________________________________________________________
        if (key[k].sm1 == _STAGE_ISKEY_RELEASED2_)		//added 2019
        {
            if (key[k].bf.state == KB_KEY_STATE_RELEASED)					
            {
                key[k].bf.ReadyToRead = KBAPP_ALREADYREAD;
                key[k].sm1 = _STAGE_END_PARSING_;
            }
        }
        //_________________________________________________________________________________________
        if (key[k].sm1 == _STAGE_ISKEY_READFROMAPP_)
        {
            if (key[k].bf.ReadyToRead == KBAPP_ALREADYREAD)	//ya leyo? //added 2019: || (terminate_key) la app. mata el proceso...
            {
                key[k].sm1 = _STAGE_END_PARSING_;
            }
        }
        //_________________________________________________________________________________________
        if (key[k].sm1 == _STAGE_END_PARSING_)
        {
            if (key[k].num_group_x > NO_GROUP_X)	//forma grupo?
            {
                group_x[key[k].num_group_x] = GROUP_UNLOCKED;
                key[k].bf.OwnerOfGroup = 0;
            }

            key[k].sm1 = _STAGE_START_PARSING_;

            //Add++
            key[k].bf.InProcessing = 0;
            //++
        }
    }
}
int8_t ikey_is_all_hw_released(void)//hardware release
{
    uint8_t i;
    for (i=0; i<KB_NUM_KEYS; i++)
    {
        if (key[i].bf.state == KB_KEY_STATE_PRESSED)
        {
            return 0;
        }
    }
    return 1;
}

void ikey_clear_all_flag(void)//clear ReadyToRead
{
    uint8_t i;
    for (i=0; i<KB_NUM_KEYS; i++)
    {
        //if (key[i].bf.ReadyToRead)
            key[i].bf.ReadyToRead = 0;
    }
}

int8_t ikey_is_all_process_finished(void)
{
    uint8_t i;
    for (i=0; i<KB_NUM_KEYS; i++)
    {
        if (key[i].bf.InProcessing == 1)
        {
            return 0;
        }
    }
    return 1;
}
