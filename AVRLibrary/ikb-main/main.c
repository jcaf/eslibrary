/*
 * main.c
 *
 *  Created on: Sep 28, 2021
 *      Author: jcaf
 */
#include "system.h"
#include "main.h"
#include "types.h"
#include "ikb/ikb.h"

volatile struct _isr_flag
{
	unsigned sysTickMs :1;
	unsigned __a :7;
} isr_flag = { 0 };
struct _mainflag mainflag;


//#define iKPAD
#define iKEY

#ifdef iKPAD

	#define KB_NUM_KEYS 16  //Total Number of keys

	/* physical pinout KEYPAD4x4
	PORTWxKBCOL_1 PORTWxKBCOL_2 PORTWxKBCOL_3 PORTWxKBCOL_4
	PORTWxKBFIL_1 	1  2  3  4
	PORTWxKBFIL_2 	5  6  7  8
	PORTWxKBFIL_3 	9  10 11 12
	PORTWxKBFIL_4 	13 14 15 16
	 */
	#define KeyPad4x4_readkey_setupTime() _delay_us(2)

	#define PORTWxKBFIL_0 		PORTD
	#define PORTRxKBFIL_0 		PIND
	#define CONFIGIOxKBFIL_0 	DDRD
	#define PINxKBFIL_0 		2

	#define PORTWxKBFIL_1 		PORTD
	#define PORTRxKBFIL_1 		PIND
	#define CONFIGIOxKBFIL_1 	DDRD
	#define PINxKBFIL_1 		3

	#define PORTWxKBFIL_2 		PORTD
	#define PORTRxKBFIL_2 		PIND
	#define CONFIGIOxKBFIL_2 	DDRD
	#define PINxKBFIL_2 		4

	#define PORTWxKBFIL_3 		PORTD
	#define PORTRxKBFIL_3 		PIND
	#define CONFIGIOxKBFIL_3 	DDRD
	#define PINxKBFIL_3 		5

	#define PORTWxKBCOL_0 		PORTD
	#define PORTRxKBCOL_0 		PIND
	#define CONFIGIOxKBCOL_0 	DDRD
	#define PINxKBCOL_0 		6

	#define PORTWxKBCOL_1 		PORTD
	#define PORTRxKBCOL_1 		PIND
	#define CONFIGIOxKBCOL_1 	DDRD
	#define PINxKBCOL_1		    7

	#define PORTWxKBCOL_2 		PORTB
	#define PORTRxKBCOL_2 		PINB
	#define CONFIGIOxKBCOL_2 	DDRB
	#define PINxKBCOL_2 		0

	#define PORTWxKBCOL_3 		PORTB
	#define PORTRxKBCOL_3 		PINB
	#define CONFIGIOxKBCOL_3 	DDRB
	#define PINxKBCOL_3 		1

	//////////////////////////////////////
	#define KB_LYOUT_KEY_1      0
	#define KB_LYOUT_KEY_2      1
	#define KB_LYOUT_KEY_3      2
	#define KB_LYOUT_KEY_UP     3

	#define KB_LYOUT_KEY_4      4
	#define KB_LYOUT_KEY_5      5
	#define KB_LYOUT_KEY_6      6
	#define KB_LYOUT_KEY_DOWN   7

	#define KB_LYOUT_KEY_7      8
	#define KB_LYOUT_KEY_8      9
	#define KB_LYOUT_KEY_9      10
	#define KB_LYOUT_KEY_2ND    11

	#define KB_LYOUT_KEY_CLEAR  12
	#define KB_LYOUT_KEY_0      13
	#define KB_LYOUT_KEY_MENU   14
	#define KB_LYOUT_KEY_ENTER  15

	static inline uint8_t keyRead_pin_key0(void)
    {
        PinTo0(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo1(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo1(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo1(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return PinRead(PORTRxKBCOL_KEY0, PINxKBCOL_KEY0);
    }
    static inline uint8_t keyRead_pin_key1(void)
    {
        PinTo0(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo1(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo1(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo1(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return PinRead(PORTRxKBCOL_KEY1, PINxKBCOL_KEY1);
    }
    static inline uint8_t keyRead_pin_key2(void)
    {
        PinTo0(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo1(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo1(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo1(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return PinRead(PORTRxKBCOL_KEY2, PINxKBCOL_KEY2);
    }
    static inline uint8_t keyRead_pin_key3(void)
    {
        PinTo0(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo1(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo1(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo1(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return PinRead(PORTRxKBCOL_KEY3, PINxKBCOL_KEY3);
    }
    //
    static inline uint8_t keyRead_pin_key4(void)
    {
        PinTo1(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo0(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo1(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo1(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return PinRead(PORTRxKBCOL_KEY4, PINxKBCOL_KEY4);
    }
    static inline uint8_t keyRead_pin_key6(void)
    {
        PinTo1(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo0(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo1(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo1(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return PinRead(PORTRxKBCOL_KEY5, PINxKBCOL_KEY5);
    }
    static inline uint8_t keyRead_pin_key7(void)
    {
        PinTo1(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo0(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo1(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo1(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return PinRead(PORTRxKBCOL_KEY6, PINxKBCOL_KEY6);
    }
    static inline uint8_t keyRead_pin_key8(void)
    {
        PinTo1(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo0(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo1(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo1(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return PinRead(PORTRxKBCOL_KEY7, PINxKBCOL_KEY7);
    }
    //
    static inline uint8_t keyRead_pin_key9(void)
    {
        PinTo1(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo1(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo0(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo1(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return PinRead(PORTRxKBCOL_KEY8, PINxKBCOL_KEY8);
    }
    static inline uint8_t keyRead_pin_key10(void)
    {
        PinTo1(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo1(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo0(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo1(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return PinRead(PORTRxKBCOL_KEY9, PINxKBCOL_KEY9);
    }
    static inline uint8_t keyRead_pin_key11(void)
    {
        PinTo1(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo1(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo0(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo1(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return PinRead(PORTRxKBCOL_KEY10, PINxKBCOL_KEY10);
    }
    static inline uint8_t keyRead_pin_key12(void)
    {
        PinTo1(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo1(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo0(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo1(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return PinRead(PORTRxKBCOL_KEY11, PINxKBCOL_KEY11);
    }
    static inline uint8_t keyRead_pin_key13(void)
    {
        PinTo1(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo1(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo1(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo0(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return PinRead(PORTRxKBCOL_KEY12, PINxKBCOL_KEY12);
    }
    static inline uint8_t keyRead_pin_key14(void)
    {
        PinTo1(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo1(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo1(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo0(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return PinRead(PORTRxKBCOL_KEY13, PINxKBCOL_KEY13);
    }
    static inline uint8_t keyRead_pin_key15(void)
    {
        PinTo1(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo1(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo1(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo0(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return PinRead(PORTRxKBCOL_KEY14, PINxKBCOL_KEY14);
    }
    static inline uint8_t keyRead_pin_key16(void)
    {
        PinTo1(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo1(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo1(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo0(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return PinRead(PORTRxKBCOL_KEY15, PINxKBCOL_KEY15);
    }
#endif // iKB

#ifdef iKEY
	#define KB_NUM_KEYS 6  		//Total Number of keys

	#define PORTWxKB_KEY0		PORTC
	#define PORTRxKB_KEY0		PINC
	#define CONFIGIOxKB_KEY0	DDRC
	#define PINxKB_KEY0			6

	#define PORTWxKB_KEY1		PORTA
	#define PORTRxKB_KEY1		PINA
	#define CONFIGIOxKB_KEY1	DDRA
	#define PINxKB_KEY1			6

	#define PORTWxKB_KEY2		PORTD
	#define PORTRxKB_KEY2		PIND
	#define CONFIGIOxKB_KEY2	DDRD
	#define PINxKB_KEY2			5

	#define PORTWxKB_KEY3		PORTB
	#define PORTRxKB_KEY3		PINB
	#define CONFIGIOxKB_KEY3	DDRB
	#define PINxKB_KEY3			5

	#define PORTWxKB_KEY4		PORTB
	#define PORTRxKB_KEY4		PINB
	#define CONFIGIOxKB_KEY4	DDRB
	#define PINxKB_KEY4			2

	#define PORTWxKB_KEY5		PORTA
	#define PORTRxKB_KEY5		PINA
	#define CONFIGIOxKB_KEY5	DDRA
	#define PINxKB_KEY5			0

    static inline
	uint8_t keyRead_pin_key0(void)
    {
        return PinRead(PORTRxKB_KEY0, PINxKB_KEY0);
    }
    static inline
	uint8_t keyRead_pin_key1(void)
    {
        return PinRead(PORTRxKB_KEY1, PINxKB_KEY1);
    }
    static inline
	uint8_t keyRead_pin_key2(void)
    {
        return PinRead(PORTRxKB_KEY2, PINxKB_KEY2);
    }
    static inline
	uint8_t keyRead_pin_key3(void)
    {
        return PinRead(PORTRxKB_KEY3, PINxKB_KEY3);
    }
    static inline
	uint8_t keyRead_pin_key4(void)
    {
        return PinRead(PORTRxKB_KEY4, PINxKB_KEY4);
    }
    static inline
	uint8_t keyRead_pin_key5(void)
	{
		return PinRead(PORTRxKB_KEY5, PINxKB_KEY5);
	}

#endif // iKEY

struct _key key[KB_NUM_KEYS];
const struct _key_prop propEmpty;

#define KB_PERIODIC_ACCESS_MS 20		//msE-3
#define KB_KEY_SCAN_COUNT_DEBOUNCE_MS 20

void ikb_init(void)
{
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

	#endif // iKPAD
	#ifdef iKEY
		/* Activate pull-ups */
		PinTo1(PORTWxKB_KEY0, PINxKB_KEY0);
		PinTo1(PORTWxKB_KEY1, PINxKB_KEY1);
		PinTo1(PORTWxKB_KEY2, PINxKB_KEY2);
		PinTo1(PORTWxKB_KEY3, PINxKB_KEY3);
		PinTo1(PORTWxKB_KEY4, PINxKB_KEY4);
		PinTo1(PORTWxKB_KEY5, PINxKB_KEY5);
		__delay_ms(1);
		//
		ConfigInputPin(CONFIGIOxKB_KEY0, PINxKB_KEY0);
		ConfigInputPin(CONFIGIOxKB_KEY1, PINxKB_KEY1);
		ConfigInputPin(CONFIGIOxKB_KEY2, PINxKB_KEY2);
		ConfigInputPin(CONFIGIOxKB_KEY3, PINxKB_KEY3);
		ConfigInputPin(CONFIGIOxKB_KEY4, PINxKB_KEY4);
		ConfigInputPin(CONFIGIOxKB_KEY5, PINxKB_KEY5);
		//
		key[0].keyRead = keyRead_pin_key0;
		key[1].keyRead = keyRead_pin_key1;
		key[2].keyRead = keyRead_pin_key2;
		key[3].keyRead = keyRead_pin_key3;
		key[4].keyRead = keyRead_pin_key4;
		key[5].keyRead = keyRead_pin_key5;
	#endif // iKEY

	/*by default init with onKeyPressed = 1*/
	struct _key_prop prop = {0};
	prop = propEmpty;
	prop.uFlag.f.onKeyPressed = 1;
	for (int8_t k=0; k< KB_NUM_KEYS; k++)
	{
		ikb_setKeyProp(key, k, prop);
	}
	key[0].prop.numGroup=1;
	key[1].prop.numGroup=1;
	key[2].prop.numGroup=1;

	key[3].prop.numGroup=2;
	key[4].prop.numGroup=2;
	key[5].prop.numGroup=2;
}

int main(void)
{
	int8_t c = 0;

	PORTA=PORTB=PORTC=PORTD = 0;
	ikb_init();

	ConfigOutputPin(CONFIGIOxD1GREEN, PINx_D1GREEN);
	ConfigOutputPin(CONFIGIOxD1RED, PINx_D1RED);

	ConfigOutputPin(CONFIGIOxD2GREEN, PINx_D2GREEN);
	ConfigOutputPin(CONFIGIOxD2RED, PINx_D2RED);

	ConfigOutputPin(CONFIGIOxD3GREEN, PINx_D3GREEN);
	ConfigOutputPin(CONFIGIOxD3RED, PINx_D3RED);

	ConfigOutputPin(CONFIGIOxD4GREEN, PINx_D4GREEN);
	ConfigOutputPin(CONFIGIOxD4RED, PINx_D4RED);

	ConfigOutputPin(CONFIGIOxD5GREEN, PINx_D5GREEN);
	ConfigOutputPin(CONFIGIOxD5RED, PINx_D5RED);

	ConfigOutputPin(CONFIGIOxD6GREEN, PINx_D6GREEN);
	ConfigOutputPin(CONFIGIOxD6RED, PINx_D6RED);

	ConfigOutputPin(CONFIGIOxBUZZER, PINx_BUZZER);
	ConfigOutputPin(CONFIGIOxADM2587_RE, PINx_ADM2587_RE);
	ConfigOutputPin(CONFIGIOxADM2587_DE, PINx_ADM2587_DE);

	//Config to 10ms, antes de generar la onda senoidal
	TCNT0 = 0x00;
	TCCR0 = (1 << WGM01) | (1 << CS02) | (0 << CS01) | (1 << CS00); //CTC, PRES=1024
	OCR0 = CTC_SET_OCR_BYTIME(10e-3, 1024); //TMR8-BIT @16MHz @PRES=1024-> BYTIME maximum = 16ms
	TIMSK |= (1 << OCIE0);
	//
	sei();
	while (1)
	{
		if (isr_flag.sysTickMs)
		{
			isr_flag.sysTickMs = 0;
			mainflag.sysTickMs = 1;
		}
		//----------------------------------
		if (mainflag.sysTickMs)
		{
			if (++c >= (20/SYSTICK_MS) )
			{
				c = 0;
				//
				int8_t k_debounce_ms = 1;//KB_KEY_SCAN_COUNT_DEBOUNCE_MS/KB_PERIODIC_ACCESS_MS;

				ikey_scan(key, KB_NUM_KEYS, k_debounce_ms);
				ikey_parsing(key, KB_NUM_KEYS);

				if (ikb_key_is_ready2read(key,0))
				{
					ikb_key_was_read(key,0);
					//
					PinToggle(PORTWxD1RED, PINx_D1RED);
				}
				if (ikb_key_is_ready2read(key,1))
				{
					ikb_key_was_read(key,1);
					//
					PinToggle(PORTWxD2RED, PINx_D2RED);
				}

				if (ikb_key_is_ready2read(key,2))
				{
					ikb_key_was_read(key,2);
					//
					PinToggle(PORTWxD3RED, PINx_D3RED);
				}

				if (ikb_key_is_ready2read(key,3))
				{
					ikb_key_was_read(key,3);
					//
					PinToggle(PORTWxD4RED, PINx_D4RED);
				}

				if (ikb_key_is_ready2read(key,4))
				{
					ikb_key_was_read(key,4);
					//
					PinToggle(PORTWxD5RED, PINx_D5RED);
				}

				if (ikb_key_is_ready2read(key,5))
				{
					ikb_key_was_read(key,5);
					//
					PinToggle(PORTWxD6RED, PINx_D6RED);
				}
			}
		}

		//----------------------------------
		mainflag.sysTickMs = 0;
	}
}
ISR(TIMER0_COMP_vect)
{
	isr_flag.sysTickMs = 1;
}

