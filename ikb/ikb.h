/*
Reglas:
-primero se lee el teclado. el indice empiezo con ellos
*/
#ifndef KB_H_
#define KB_H_
    #include "types.h"

    #define KB_NUM_KEYS 16  //Total Number of keys

    //#define iKPAD
    #define iKEY

    //////////////////////////////////////////////////////////////////////////////////////
    #define KB_PERIODIC_ACCESS		5E-3
    #define KB_KEY_SCAN_COUNT_DEBOUNCE 4

    //scan hardware
    #define KB_KEY_PINLEVEL_PRESSED 0
    #define KB_KEY_PINLEVEL_RELEASED 1

    #define _FIRE_AT_TIME_THRESHOLD_ 1000E-3/KB_PERIODIC_ACCESS
    #define _FIRE_AT_TIME_THRESHOLD2_ 3000E-3/KB_PERIODIC_ACCESS
    #define KB_BEFORE_THR	0
    #define KB_AFTER_THR	1
    //////////////////////////////////////////////////////////////////////////////////////

    #ifdef iKPAD
        /* physical pinout KEYPAD4x4
        PORTWxKBCOL_1 PORTWxKBCOL_2 PORTWxKBCOL_3 PORTWxKBCOL_4
        PORTWxKBFIL_1 	1  2  3  4
        PORTWxKBFIL_2 	5  6  7  8
        PORTWxKBFIL_3 	9  10 11 12
        PORTWxKBFIL_4 	13 14 15 16
         */
        #define readkey_setup_time() _delay_us(2)

        #define PORTWxKBFIL_1 		PORTD
        #define PORTRxKBFIL_1 		PIND
        #define CONFIGIOxKBFIL_1 	DDRD
        #define PINxKBFIL_1 		2

        #define PORTWxKBFIL_2 		PORTD
        #define PORTRxKBFIL_2 		PIND
        #define CONFIGIOxKBFIL_2 	DDRD
        #define PINxKBFIL_2 		3

        #define PORTWxKBFIL_3 		PORTD
        #define PORTRxKBFIL_3 		PIND
        #define CONFIGIOxKBFIL_3 	DDRD
        #define PINxKBFIL_3 		4

        #define PORTWxKBFIL_4 		PORTD
        #define PORTRxKBFIL_4 		PIND
        #define CONFIGIOxKBFIL_4 	DDRD
        #define PINxKBFIL_4 		5

        #define PORTWxKBCOL_1 		PORTD
        #define PORTRxKBCOL_1 		PIND
        #define CONFIGIOxKBCOL_1 	DDRD
        #define PINxKBCOL_1 		6

        #define PORTWxKBCOL_2 		PORTD
        #define PORTRxKBCOL_2 		PIND
        #define CONFIGIOxKBCOL_2 	DDRD
        #define PINxKBCOL_2		    7

        #define PORTWxKBCOL_3 		PORTB
        #define PORTRxKBCOL_3 		PINB
        #define CONFIGIOxKBCOL_3 	DDRB
        #define PINxKBCOL_3 		0

        #define PORTWxKBCOL_4 		PORTB
        #define PORTRxKBCOL_4 		PINB
        #define CONFIGIOxKBCOL_4 	DDRB
        #define PINxKBCOL_4 		1

        //////// KEYPAD 4X4 //////////////////////
        //////////////////////////////////////KEY1
        #define PORTWxKBFIL_KEY1 	PORTWxKBFIL_1
        #define PORTRxKBFIL_KEY1 	PORTRxKBFIL_1
        #define CONFIGIOxKBFIL_KEY1 	CONFIGIOxKBFIL_1
        #define PINxKBFIL_KEY1 		PINxKBFIL_1
        //
        #define PORTWxKBCOL_KEY1 	PORTWxKBCOL_1
        #define PORTRxKBCOL_KEY1 	PORTRxKBCOL_1
        #define CONFIGIOxKBCOL_KEY1 	CONFIGIOxKBCOL_1
        #define PINxKBCOL_KEY1 		PINxKBCOL_1
        //////////////////////////////////////KEY2
        #define PORTWxKBFIL_KEY2 	PORTWxKBFIL_1
        #define PORTRxKBFIL_KEY2 	PORTRxKBFIL_1
        #define CONFIGIOxKBFIL_KEY2 	CONFIGIOxKBFIL_1
        #define PINxKBFIL_KEY2 		PINxKBFIL_1
        //
        #define PORTWxKBCOL_KEY2 	PORTWxKBCOL_2
        #define PORTRxKBCOL_KEY2 	PORTRxKBCOL_2
        #define CONFIGIOxKBCOL_KEY2 	CONFIGIOxKBCOL_2
        #define PINxKBCOL_KEY2 		PINxKBCOL_2
        //////////////////////////////////////
        #define PORTWxKBFIL_KEY3 	PORTWxKBFIL_1
        #define PORTRxKBFIL_KEY3 	PORTRxKBFIL_1
        #define CONFIGIOxKBFIL_KEY3 	CONFIGIOxKBFIL_1
        #define PINxKBFIL_KEY3 		PINxKBFIL_1
        //
        #define PORTWxKBCOL_KEY3 	PORTWxKBCOL_3
        #define PORTRxKBCOL_KEY3 	PORTRxKBCOL_3
        #define CONFIGIOxKBCOL_KEY3 	CONFIGIOxKBCOL_3
        #define PINxKBCOL_KEY3 		PINxKBCOL_3
        //////////////////////////////////////
        #define PORTWxKBFIL_KEY4 	PORTWxKBFIL_1
        #define PORTRxKBFIL_KEY4 	PORTRxKBFIL_1
        #define CONFIGIOxKBFIL_KEY4 	CONFIGIOxKBFIL_1
        #define PINxKBFIL_KEY4 		PINxKBFIL_1
        //
        #define PORTWxKBCOL_KEY4 	PORTWxKBCOL_4
        #define PORTRxKBCOL_KEY4 	PORTRxKBCOL_4
        #define CONFIGIOxKBCOL_KEY4 	CONFIGIOxKBCOL_4
        #define PINxKBCOL_KEY4 		PINxKBCOL_4
        //////////////////////////////////////
        #define PORTWxKBFIL_KEY5 	PORTWxKBFIL_2
        #define PORTRxKBFIL_KEY5 	PORTRxKBFIL_2
        #define CONFIGIOxKBFIL_KEY5 	CONFIGIOxKBFIL_2
        #define PINxKBFIL_KEY5 		PINxKBFIL_2
        //
        #define PORTWxKBCOL_KEY5 	PORTWxKBCOL_1
        #define PORTRxKBCOL_KEY5 	PORTRxKBCOL_1
        #define CONFIGIOxKBCOL_KEY5 	CONFIGIOxKBCOL_1
        #define PINxKBCOL_KEY5 		PINxKBCOL_1
        //////////////////////////////////////
        #define PORTWxKBFIL_KEY6 	PORTWxKBFIL_2
        #define PORTRxKBFIL_KEY6 	PORTRxKBFIL_2
        #define CONFIGIOxKBFIL_KEY6 	CONFIGIOxKBFIL_2
        #define PINxKBFIL_KEY6 		PINxKBFIL_2
        //
        #define PORTWxKBCOL_KEY6 	PORTWxKBCOL_2
        #define PORTRxKBCOL_KEY6 	PORTRxKBCOL_2
        #define CONFIGIOxKBCOL_KEY6 	CONFIGIOxKBCOL_2
        #define PINxKBCOL_KEY6 		PINxKBCOL_2
        //////////////////////////////////////
        #define PORTWxKBFIL_KEY7 	PORTWxKBFIL_2
        #define PORTRxKBFIL_KEY7 	PORTRxKBFIL_2
        #define CONFIGIOxKBFIL_KEY7 	CONFIGIOxKBFIL_2
        #define PINxKBFIL_KEY7 		PINxKBFIL_2
        //
        #define PORTWxKBCOL_KEY7 	PORTWxKBCOL_3
        #define PORTRxKBCOL_KEY7 	PORTRxKBCOL_3
        #define CONFIGIOxKBCOL_KEY7 	CONFIGIOxKBCOL_3
        #define PINxKBCOL_KEY7 		PINxKBCOL_3
        //////////////////////////////////////
        #define PORTWxKBFIL_KEY8 	PORTWxKBFIL_2
        #define PORTRxKBFIL_KEY8 	PORTRxKBFIL_2
        #define CONFIGIOxKBFIL_KEY8 	CONFIGIOxKBFIL_2
        #define PINxKBFIL_KEY8 		PINxKBFIL_2
        //
        #define PORTWxKBCOL_KEY8 	PORTWxKBCOL_4
        #define PORTRxKBCOL_KEY8 	PORTRxKBCOL_4
        #define CONFIGIOxKBCOL_KEY8 	CONFIGIOxKBCOL_4
        #define PINxKBCOL_KEY8 		PINxKBCOL_4
        //////////////////////////////////////
        #define PORTWxKBFIL_KEY9 	PORTWxKBFIL_3
        #define PORTRxKBFIL_KEY9 	PORTRxKBFIL_3
        #define CONFIGIOxKBFIL_KEY9 	CONFIGIOxKBFIL_3
        #define PINxKBFIL_KEY9 		PINxKBFIL_3
        //
        #define PORTWxKBCOL_KEY9 	PORTWxKBCOL_1
        #define PORTRxKBCOL_KEY9 	PORTRxKBCOL_1
        #define CONFIGIOxKBCOL_KEY9 	CONFIGIOxKBCOL_1
        #define PINxKBCOL_KEY9 		PINxKBCOL_1
        //////////////////////////////////////
        #define PORTWxKBFIL_KEY10	PORTWxKBFIL_3
        #define PORTRxKBFIL_KEY10	PORTRxKBFIL_3
        #define CONFIGIOxKBFIL_KEY10	CONFIGIOxKBFIL_3
        #define PINxKBFIL_KEY10		PINxKBFIL_3
        //
        #define PORTWxKBCOL_KEY10	PORTWxKBCOL_2
        #define PORTRxKBCOL_KEY10	PORTRxKBCOL_2
        #define CONFIGIOxKBCOL_KEY10	CONFIGIOxKBCOL_2
        #define PINxKBCOL_KEY10		PINxKBCOL_2
        //////////////////////////////////////
        #define PORTWxKBFIL_KEY11	PORTWxKBFIL_3
        #define PORTRxKBFIL_KEY11	PORTRxKBFIL_3
        #define CONFIGIOxKBFIL_KEY11	CONFIGIOxKBFIL_3
        #define PINxKBFIL_KEY11		PINxKBFIL_3
        //
        #define PORTWxKBCOL_KEY11	PORTWxKBCOL_3
        #define PORTRxKBCOL_KEY11	PORTRxKBCOL_3
        #define CONFIGIOxKBCOL_KEY11	CONFIGIOxKBCOL_3
        #define PINxKBCOL_KEY11		PINxKBCOL_3
        //////////////////////////////////////
        #define PORTWxKBFIL_KEY12	PORTWxKBFIL_3
        #define PORTRxKBFIL_KEY12	PORTRxKBFIL_3
        #define CONFIGIOxKBFIL_KEY12	CONFIGIOxKBFIL_3
        #define PINxKBFIL_KEY12		PINxKBFIL_3
        //
        #define PORTWxKBCOL_KEY12	PORTWxKBCOL_4
        #define PORTRxKBCOL_KEY12	PORTRxKBCOL_4
        #define CONFIGIOxKBCOL_KEY12	CONFIGIOxKBCOL_4
        #define PINxKBCOL_KEY12		PINxKBCOL_4
        //////////////////////////////////////
        #define PORTWxKBFIL_KEY13	PORTWxKBFIL_4
        #define PORTRxKBFIL_KEY13	PORTRxKBFIL_4
        #define CONFIGIOxKBFIL_KEY13	CONFIGIOxKBFIL_4
        #define PINxKBFIL_KEY13		PINxKBFIL_4
        //
        #define PORTWxKBCOL_KEY13	PORTWxKBCOL_1
        #define PORTRxKBCOL_KEY13	PORTRxKBCOL_1
        #define CONFIGIOxKBCOL_KEY13	CONFIGIOxKBCOL_1
        #define PINxKBCOL_KEY13		PINxKBCOL_1
        //////////////////////////////////////
        #define PORTWxKBFIL_KEY14 	PORTWxKBFIL_4
        #define PORTRxKBFIL_KEY14	PORTRxKBFIL_4
        #define CONFIGIOxKBFIL_KEY14	CONFIGIOxKBFIL_4
        #define PINxKBFIL_KEY14		PINxKBFIL_4
        //
        #define PORTWxKBCOL_KEY14	PORTWxKBCOL_2
        #define PORTRxKBCOL_KEY14	PORTRxKBCOL_2
        #define CONFIGIOxKBCOL_KEY14	CONFIGIOxKBCOL_2
        #define PINxKBCOL_KEY14		PINxKBCOL_2
        //////////////////////////////////////
        #define PORTWxKBFIL_KEY15	PORTWxKBFIL_4
        #define PORTRxKBFIL_KEY15	PORTRxKBFIL_4
        #define CONFIGIOxKBFIL_KEY15	CONFIGIOxKBFIL_4
        #define PINxKBFIL_KEY15		PINxKBFIL_4
        //
        #define PORTWxKBCOL_KEY15 	PORTWxKBCOL_3
        #define PORTRxKBCOL_KEY15	PORTRxKBCOL_3
        #define CONFIGIOxKBCOL_KEY15	CONFIGIOxKBCOL_3
        #define PINxKBCOL_KEY15		PINxKBCOL_3
        //////////////////////////////////////
        #define PORTWxKBFIL_KEY16	PORTWxKBFIL_4
        #define PORTRxKBFIL_KEY16	PORTRxKBFIL_4
        #define CONFIGIOxKBFIL_KEY16	CONFIGIOxKBFIL_4
        #define PINxKBFIL_KEY16		PINxKBFIL_4
        //
        #define PORTWxKBCOL_KEY16	PORTWxKBCOL_4
        #define PORTRxKBCOL_KEY16	PORTRxKBCOL_4
        #define CONFIGIOxKBCOL_KEY16	CONFIGIOxKBCOL_4
        #define PINxKBCOL_KEY16		PINxKBCOL_4

        //////////////////////////////////////
        //kypad index -> layout
        #define IKPAD_POS1 0
        #define IKPAD_POS2 1
        #define IKPAD_POS3 2
        #define IKPAD_POS4 3
        #define IKPAD_POS5 4
        #define IKPAD_POS6 5
        #define IKPAD_POS7 6
        #define IKPAD_POS8 7
        #define IKPAD_POS9 8
        #define IKPAD_POS10 9
        #define IKPAD_POS11 10
        #define IKPAD_POS12 11
        #define IKPAD_POS13 12
        #define IKPAD_POS14 13
        #define IKPAD_POS15 14
        #define IKPAD_POS16 15

        #define KB_LYOUT_KEY_1      IKPAD_POS1
        #define KB_LYOUT_KEY_2      IKPAD_POS2
        #define KB_LYOUT_KEY_3      IKPAD_POS3
        #define KB_LYOUT_KEY_UP     IKPAD_POS4

        #define KB_LYOUT_KEY_4      IKPAD_POS5
        #define KB_LYOUT_KEY_5      IKPAD_POS6
        #define KB_LYOUT_KEY_6      IKPAD_POS7
        #define KB_LYOUT_KEY_DOWN   IKPAD_POS8

        #define KB_LYOUT_KEY_7      IKPAD_POS9
        #define KB_LYOUT_KEY_8      IKPAD_POS10
        #define KB_LYOUT_KEY_9      IKPAD_POS11
        #define KB_LYOUT_KEY_2ND    IKPAD_POS12

        #define KB_LYOUT_KEY_CLEAR  IKPAD_POS13
        #define KB_LYOUT_KEY_0      IKPAD_POS14
        #define KB_LYOUT_KEY_MENU   IKPAD_POS15
        #define KB_LYOUT_KEY_ENTER  IKPAD_POS16
    #endif // iKB

    #ifdef iKEY
        #define PORTWxKB_KEY1 		LATC
        #define PORTRxKB_KEY1 		PORTC
        #define CONFIGIOxKB_KEY1 	TRISC
        #define PINxKB_KEY1 		4

        #define PORTWxKB_KEY2 		LATC
        #define PORTRxKB_KEY2 		PORTC
        #define CONFIGIOxKB_KEY2 	TRISC
        #define PINxKB_KEY2 		5

        #define PORTWxKB_KEY3 		LATC
        #define PORTRxKB_KEY3 		PORTC
        #define CONFIGIOxKB_KEY3 	TRISC
        #define PINxKB_KEY3 		6

        #define PORTWxKB_KEY4 		LATC
        #define PORTRxKB_KEY4 		PORTC
        #define CONFIGIOxKB_KEY4 	TRISC
        #define PINxKB_KEY4 		7

        #define PORTWxKB_KEY5 		LATD
        #define PORTRxKB_KEY5 		PORTD
        #define CONFIGIOxKB_KEY5 	TRISD
        #define PINxKB_KEY5 		4

    #endif // iKEY

    void ikb_init(void);
    void ikb_job(void);
    void ikb_flush(void);
    //
    uint8_t ikb_get_AtTimeExpired_BeforeOrAfter(uint8_t k);
    //
    uint8_t ikb_key_is_ready2read(uint8_t k);
    void ikb_set_ready2read(uint8_t k);
    void ikb_key_was_read(uint8_t k);
    void ikb_execfunct(uint8_t k);

    //void ikb_processKeyRead_D(void);
    //void kb_change_keyDo(const PTRFX_retVOID const *keyDo);
    //void kb_change_keyDo(PTRFX_retVOID *keyDo);
    //void kb_change_keyDo_pgm(PTRFX_retVOID const *  keyDo);
    //////////////////////////////////////////////////////////////////////////////////////



#endif
