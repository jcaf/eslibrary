
;CodeVisionAVR C Compiler V2.03.9 Evaluation
;(C) Copyright 1998-2008 Pavel Haiduc, HP InfoTech s.r.l.
;http://www.hpinfotech.com

;Chip type              : ATmega8
;Program type           : Application
;Clock frequency        : 1.000000 MHz
;Memory model           : Small
;Optimize for           : Size
;(s)printf features     : int, width
;(s)scanf features      : int, width
;External RAM size      : 0
;Data Stack size        : 256 byte(s)
;Heap size              : 0 byte(s)
;Promote char to int    : No
;char is unsigned       : Yes
;global const stored in FLASH  : Yes
;8 bit enums            : No
;Enhanced core instructions    : On
;Smart register allocation : On
;Automatic register allocation : On

	#pragma AVRPART ADMIN PART_NAME ATmega8
	#pragma AVRPART MEMORY PROG_FLASH 8192
	#pragma AVRPART MEMORY EEPROM 512
	#pragma AVRPART MEMORY INT_SRAM SIZE 1024
	#pragma AVRPART MEMORY INT_SRAM START_ADDR 0x60

	.LISTMAC
	.EQU UDRE=0x5
	.EQU RXC=0x7
	.EQU USR=0xB
	.EQU UDR=0xC
	.EQU SPSR=0xE
	.EQU SPDR=0xF
	.EQU EERE=0x0
	.EQU EEWE=0x1
	.EQU EEMWE=0x2
	.EQU EECR=0x1C
	.EQU EEDR=0x1D
	.EQU EEARL=0x1E
	.EQU EEARH=0x1F
	.EQU WDTCR=0x21
	.EQU MCUCR=0x35
	.EQU GICR=0x3B
	.EQU SPL=0x3D
	.EQU SPH=0x3E
	.EQU SREG=0x3F

	.DEF R0X0=R0
	.DEF R0X1=R1
	.DEF R0X2=R2
	.DEF R0X3=R3
	.DEF R0X4=R4
	.DEF R0X5=R5
	.DEF R0X6=R6
	.DEF R0X7=R7
	.DEF R0X8=R8
	.DEF R0X9=R9
	.DEF R0XA=R10
	.DEF R0XB=R11
	.DEF R0XC=R12
	.DEF R0XD=R13
	.DEF R0XE=R14
	.DEF R0XF=R15
	.DEF R0X10=R16
	.DEF R0X11=R17
	.DEF R0X12=R18
	.DEF R0X13=R19
	.DEF R0X14=R20
	.DEF R0X15=R21
	.DEF R0X16=R22
	.DEF R0X17=R23
	.DEF R0X18=R24
	.DEF R0X19=R25
	.DEF R0X1A=R26
	.DEF R0X1B=R27
	.DEF R0X1C=R28
	.DEF R0X1D=R29
	.DEF R0X1E=R30
	.DEF R0X1F=R31

	.MACRO __CPD1N
	CPI  R30,LOW(@0)
	LDI  R26,HIGH(@0)
	CPC  R31,R26
	LDI  R26,BYTE3(@0)
	CPC  R22,R26
	LDI  R26,BYTE4(@0)
	CPC  R23,R26
	.ENDM

	.MACRO __CPD2N
	CPI  R26,LOW(@0)
	LDI  R30,HIGH(@0)
	CPC  R27,R30
	LDI  R30,BYTE3(@0)
	CPC  R24,R30
	LDI  R30,BYTE4(@0)
	CPC  R25,R30
	.ENDM

	.MACRO __CPWRR
	CP   R@0,R@2
	CPC  R@1,R@3
	.ENDM

	.MACRO __CPWRN
	CPI  R@0,LOW(@2)
	LDI  R30,HIGH(@2)
	CPC  R@1,R30
	.ENDM

	.MACRO __ADDB1MN
	SUBI R30,LOW(-@0-(@1))
	.ENDM

	.MACRO __ADDB2MN
	SUBI R26,LOW(-@0-(@1))
	.ENDM

	.MACRO __ADDW1MN
	SUBI R30,LOW(-@0-(@1))
	SBCI R31,HIGH(-@0-(@1))
	.ENDM

	.MACRO __ADDW2MN
	SUBI R26,LOW(-@0-(@1))
	SBCI R27,HIGH(-@0-(@1))
	.ENDM

	.MACRO __ADDW1FN
	SUBI R30,LOW(-2*@0-(@1))
	SBCI R31,HIGH(-2*@0-(@1))
	.ENDM

	.MACRO __ADDD1FN
	SUBI R30,LOW(-2*@0-(@1))
	SBCI R31,HIGH(-2*@0-(@1))
	SBCI R22,BYTE3(-2*@0-(@1))
	.ENDM

	.MACRO __ADDD1N
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	SBCI R22,BYTE3(-@0)
	SBCI R23,BYTE4(-@0)
	.ENDM

	.MACRO __ADDD2N
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	SBCI R24,BYTE3(-@0)
	SBCI R25,BYTE4(-@0)
	.ENDM

	.MACRO __SUBD1N
	SUBI R30,LOW(@0)
	SBCI R31,HIGH(@0)
	SBCI R22,BYTE3(@0)
	SBCI R23,BYTE4(@0)
	.ENDM

	.MACRO __SUBD2N
	SUBI R26,LOW(@0)
	SBCI R27,HIGH(@0)
	SBCI R24,BYTE3(@0)
	SBCI R25,BYTE4(@0)
	.ENDM

	.MACRO __ANDBMNN
	LDS  R30,@0+@1
	ANDI R30,LOW(@2)
	STS  @0+@1,R30
	.ENDM

	.MACRO __ANDWMNN
	LDS  R30,@0+@1
	ANDI R30,LOW(@2)
	STS  @0+@1,R30
	LDS  R30,@0+@1+1
	ANDI R30,HIGH(@2)
	STS  @0+@1+1,R30
	.ENDM

	.MACRO __ANDD1N
	ANDI R30,LOW(@0)
	ANDI R31,HIGH(@0)
	ANDI R22,BYTE3(@0)
	ANDI R23,BYTE4(@0)
	.ENDM

	.MACRO __ANDD2N
	ANDI R26,LOW(@0)
	ANDI R27,HIGH(@0)
	ANDI R24,BYTE3(@0)
	ANDI R25,BYTE4(@0)
	.ENDM

	.MACRO __ORBMNN
	LDS  R30,@0+@1
	ORI  R30,LOW(@2)
	STS  @0+@1,R30
	.ENDM

	.MACRO __ORWMNN
	LDS  R30,@0+@1
	ORI  R30,LOW(@2)
	STS  @0+@1,R30
	LDS  R30,@0+@1+1
	ORI  R30,HIGH(@2)
	STS  @0+@1+1,R30
	.ENDM

	.MACRO __ORD1N
	ORI  R30,LOW(@0)
	ORI  R31,HIGH(@0)
	ORI  R22,BYTE3(@0)
	ORI  R23,BYTE4(@0)
	.ENDM

	.MACRO __ORD2N
	ORI  R26,LOW(@0)
	ORI  R27,HIGH(@0)
	ORI  R24,BYTE3(@0)
	ORI  R25,BYTE4(@0)
	.ENDM

	.MACRO __DELAY_USB
	LDI  R24,LOW(@0)
__DELAY_USB_LOOP:
	DEC  R24
	BRNE __DELAY_USB_LOOP
	.ENDM

	.MACRO __DELAY_USW
	LDI  R24,LOW(@0)
	LDI  R25,HIGH(@0)
__DELAY_USW_LOOP:
	SBIW R24,1
	BRNE __DELAY_USW_LOOP
	.ENDM

	.MACRO __GETD1S
	LDD  R30,Y+@0
	LDD  R31,Y+@0+1
	LDD  R22,Y+@0+2
	LDD  R23,Y+@0+3
	.ENDM

	.MACRO __PUTD1S
	STD  Y+@0,R30
	STD  Y+@0+1,R31
	STD  Y+@0+2,R22
	STD  Y+@0+3,R23
	.ENDM

	.MACRO __PUTD2S
	STD  Y+@0,R26
	STD  Y+@0+1,R27
	STD  Y+@0+2,R24
	STD  Y+@0+3,R25
	.ENDM

	.MACRO __POINTB1MN
	LDI  R30,LOW(@0+@1)
	.ENDM

	.MACRO __POINTW1MN
	LDI  R30,LOW(@0+@1)
	LDI  R31,HIGH(@0+@1)
	.ENDM

	.MACRO __POINTD1M
	LDI  R30,LOW(@0)
	LDI  R31,HIGH(@0)
	LDI  R22,BYTE3(@0)
	LDI  R23,BYTE4(@0)
	.ENDM

	.MACRO __POINTW1FN
	LDI  R30,LOW(2*@0+@1)
	LDI  R31,HIGH(2*@0+@1)
	.ENDM

	.MACRO __POINTD1FN
	LDI  R30,LOW(2*@0+@1)
	LDI  R31,HIGH(2*@0+@1)
	LDI  R22,BYTE3(2*@0+@1)
	LDI  R23,BYTE4(2*@0+@1)
	.ENDM

	.MACRO __POINTB2MN
	LDI  R26,LOW(@0+@1)
	.ENDM

	.MACRO __POINTW2MN
	LDI  R26,LOW(@0+@1)
	LDI  R27,HIGH(@0+@1)
	.ENDM

	.MACRO __POINTBRM
	LDI  R@0,LOW(@1)
	.ENDM

	.MACRO __POINTWRM
	LDI  R@0,LOW(@2)
	LDI  R@1,HIGH(@2)
	.ENDM

	.MACRO __POINTBRMN
	LDI  R@0,LOW(@1+@2)
	.ENDM

	.MACRO __POINTWRMN
	LDI  R@0,LOW(@2+@3)
	LDI  R@1,HIGH(@2+@3)
	.ENDM

	.MACRO __POINTWRFN
	LDI  R@0,LOW(@2*2+@3)
	LDI  R@1,HIGH(@2*2+@3)
	.ENDM

	.MACRO __GETD1N
	LDI  R30,LOW(@0)
	LDI  R31,HIGH(@0)
	LDI  R22,BYTE3(@0)
	LDI  R23,BYTE4(@0)
	.ENDM

	.MACRO __GETD2N
	LDI  R26,LOW(@0)
	LDI  R27,HIGH(@0)
	LDI  R24,BYTE3(@0)
	LDI  R25,BYTE4(@0)
	.ENDM

	.MACRO __GETD2S
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	LDD  R24,Y+@0+2
	LDD  R25,Y+@0+3
	.ENDM

	.MACRO __GETB1MN
	LDS  R30,@0+@1
	.ENDM

	.MACRO __GETB1HMN
	LDS  R31,@0+@1
	.ENDM

	.MACRO __GETW1MN
	LDS  R30,@0+@1
	LDS  R31,@0+@1+1
	.ENDM

	.MACRO __GETD1MN
	LDS  R30,@0+@1
	LDS  R31,@0+@1+1
	LDS  R22,@0+@1+2
	LDS  R23,@0+@1+3
	.ENDM

	.MACRO __GETBRMN
	LDS  R@0,@1+@2
	.ENDM

	.MACRO __GETWRMN
	LDS  R@0,@2+@3
	LDS  R@1,@2+@3+1
	.ENDM

	.MACRO __GETWRZ
	LDD  R@0,Z+@2
	LDD  R@1,Z+@2+1
	.ENDM

	.MACRO __GETD2Z
	LDD  R26,Z+@0
	LDD  R27,Z+@0+1
	LDD  R24,Z+@0+2
	LDD  R25,Z+@0+3
	.ENDM

	.MACRO __GETB2MN
	LDS  R26,@0+@1
	.ENDM

	.MACRO __GETW2MN
	LDS  R26,@0+@1
	LDS  R27,@0+@1+1
	.ENDM

	.MACRO __GETD2MN
	LDS  R26,@0+@1
	LDS  R27,@0+@1+1
	LDS  R24,@0+@1+2
	LDS  R25,@0+@1+3
	.ENDM

	.MACRO __PUTB1MN
	STS  @0+@1,R30
	.ENDM

	.MACRO __PUTW1MN
	STS  @0+@1,R30
	STS  @0+@1+1,R31
	.ENDM

	.MACRO __PUTD1MN
	STS  @0+@1,R30
	STS  @0+@1+1,R31
	STS  @0+@1+2,R22
	STS  @0+@1+3,R23
	.ENDM

	.MACRO __PUTB1EN
	LDI  R26,LOW(@0+@1)
	LDI  R27,HIGH(@0+@1)
	RCALL __EEPROMWRB
	.ENDM

	.MACRO __PUTW1EN
	LDI  R26,LOW(@0+@1)
	LDI  R27,HIGH(@0+@1)
	RCALL __EEPROMWRW
	.ENDM

	.MACRO __PUTD1EN
	LDI  R26,LOW(@0+@1)
	LDI  R27,HIGH(@0+@1)
	RCALL __EEPROMWRD
	.ENDM

	.MACRO __PUTBR0MN
	STS  @0+@1,R0
	.ENDM

	.MACRO __PUTDZ2
	STD  Z+@0,R26
	STD  Z+@0+1,R27
	STD  Z+@0+2,R24
	STD  Z+@0+3,R25
	.ENDM

	.MACRO __PUTBMRN
	STS  @0+@1,R@2
	.ENDM

	.MACRO __PUTWMRN
	STS  @0+@1,R@2
	STS  @0+@1+1,R@3
	.ENDM

	.MACRO __PUTBZR
	STD  Z+@1,R@0
	.ENDM

	.MACRO __PUTWZR
	STD  Z+@2,R@0
	STD  Z+@2+1,R@1
	.ENDM

	.MACRO __GETW1R
	MOV  R30,R@0
	MOV  R31,R@1
	.ENDM

	.MACRO __GETW2R
	MOV  R26,R@0
	MOV  R27,R@1
	.ENDM

	.MACRO __GETWRN
	LDI  R@0,LOW(@2)
	LDI  R@1,HIGH(@2)
	.ENDM

	.MACRO __PUTW1R
	MOV  R@0,R30
	MOV  R@1,R31
	.ENDM

	.MACRO __PUTW2R
	MOV  R@0,R26
	MOV  R@1,R27
	.ENDM

	.MACRO __ADDWRN
	SUBI R@0,LOW(-@2)
	SBCI R@1,HIGH(-@2)
	.ENDM

	.MACRO __ADDWRR
	ADD  R@0,R@2
	ADC  R@1,R@3
	.ENDM

	.MACRO __SUBWRN
	SUBI R@0,LOW(@2)
	SBCI R@1,HIGH(@2)
	.ENDM

	.MACRO __SUBWRR
	SUB  R@0,R@2
	SBC  R@1,R@3
	.ENDM

	.MACRO __ANDWRN
	ANDI R@0,LOW(@2)
	ANDI R@1,HIGH(@2)
	.ENDM

	.MACRO __ANDWRR
	AND  R@0,R@2
	AND  R@1,R@3
	.ENDM

	.MACRO __ORWRN
	ORI  R@0,LOW(@2)
	ORI  R@1,HIGH(@2)
	.ENDM

	.MACRO __ORWRR
	OR   R@0,R@2
	OR   R@1,R@3
	.ENDM

	.MACRO __EORWRR
	EOR  R@0,R@2
	EOR  R@1,R@3
	.ENDM

	.MACRO __GETWRS
	LDD  R@0,Y+@2
	LDD  R@1,Y+@2+1
	.ENDM

	.MACRO __PUTWSR
	STD  Y+@2,R@0
	STD  Y+@2+1,R@1
	.ENDM

	.MACRO __MOVEWRR
	MOV  R@0,R@2
	MOV  R@1,R@3
	.ENDM

	.MACRO __INWR
	IN   R@0,@2
	IN   R@1,@2+1
	.ENDM

	.MACRO __OUTWR
	OUT  @2+1,R@1
	OUT  @2,R@0
	.ENDM

	.MACRO __CALL1MN
	LDS  R30,@0+@1
	LDS  R31,@0+@1+1
	ICALL
	.ENDM

	.MACRO __CALL1FN
	LDI  R30,LOW(2*@0+@1)
	LDI  R31,HIGH(2*@0+@1)
	RCALL __GETW1PF
	ICALL
	.ENDM

	.MACRO __CALL2EN
	LDI  R26,LOW(@0+@1)
	LDI  R27,HIGH(@0+@1)
	RCALL __EEPROMRDW
	ICALL
	.ENDM

	.MACRO __GETW1STACK
	IN   R26,SPL
	IN   R27,SPH
	ADIW R26,@0+1
	LD   R30,X+
	LD   R31,X
	.ENDM

	.MACRO __NBST
	BST  R@0,@1
	IN   R30,SREG
	LDI  R31,0x40
	EOR  R30,R31
	OUT  SREG,R30
	.ENDM


	.MACRO __PUTB1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	RCALL __PUTDP1
	.ENDM

	.MACRO __PUTB1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	RCALL __PUTDP1
	.ENDM

	.MACRO __PUTB1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	RCALL __PUTDP1
	.ENDM

	.MACRO __PUTB1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	RCALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	RCALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RNS
	MOVW R26,R@0
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RNS
	MOVW R26,R@0
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RNS
	MOVW R26,R@0
	ADIW R26,@1
	RCALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	RCALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	RCALL __PUTDP1
	.ENDM


	.MACRO __GETB1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R30,Z
	.ENDM

	.MACRO __GETB1HSX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R31,Z
	.ENDM

	.MACRO __GETW1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R0,Z+
	LD   R31,Z
	MOV  R30,R0
	.ENDM

	.MACRO __GETD1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R0,Z+
	LD   R1,Z+
	LD   R22,Z+
	LD   R23,Z
	MOVW R30,R0
	.ENDM

	.MACRO __GETB2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R26,X
	.ENDM

	.MACRO __GETW2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	.ENDM

	.MACRO __GETD2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R1,X+
	LD   R24,X+
	LD   R25,X
	MOVW R26,R0
	.ENDM

	.MACRO __GETBRSX
	MOVW R30,R28
	SUBI R30,LOW(-@1)
	SBCI R31,HIGH(-@1)
	LD   R@0,Z
	.ENDM

	.MACRO __GETWRSX
	MOVW R30,R28
	SUBI R30,LOW(-@2)
	SBCI R31,HIGH(-@2)
	LD   R@0,Z+
	LD   R@1,Z
	.ENDM

	.MACRO __LSLW8SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R31,Z
	CLR  R30
	.ENDM

	.MACRO __PUTB1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X+,R31
	ST   X+,R22
	ST   X,R23
	.ENDM

	.MACRO __CLRW1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X,R30
	.ENDM

	.MACRO __CLRD1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X+,R30
	ST   X+,R30
	ST   X,R30
	.ENDM

	.MACRO __PUTB2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z,R26
	.ENDM

	.MACRO __PUTW2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z+,R26
	ST   Z,R27
	.ENDM

	.MACRO __PUTD2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z+,R26
	ST   Z+,R27
	ST   Z+,R24
	ST   Z,R25
	.ENDM

	.MACRO __PUTBSRX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z,R@1
	.ENDM

	.MACRO __PUTWSRX
	MOVW R30,R28
	SUBI R30,LOW(-@2)
	SBCI R31,HIGH(-@2)
	ST   Z+,R@0
	ST   Z,R@1
	.ENDM

	.MACRO __PUTB1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X+,R31
	ST   X+,R22
	ST   X,R23
	.ENDM

	.MACRO __MULBRR
	MULS R@0,R@1
	MOVW R30,R0
	.ENDM

	.MACRO __MULBRRU
	MUL  R@0,R@1
	MOVW R30,R0
	.ENDM

	.MACRO __MULBRR0
	MULS R@0,R@1
	.ENDM

	.MACRO __MULBRRU0
	MUL  R@0,R@1
	.ENDM

	.MACRO __MULBNWRU
	LDI  R26,@2
	MUL  R26,R@0
	MOVW R30,R0
	MUL  R26,R@1
	ADD  R31,R0
	.ENDM

	.CSEG
	.ORG 0x00

;INTERRUPT VECTORS
	RJMP __RESET
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00

_smallFont:
	.DB  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x2F
	.DB  0x0,0x0,0x0,0x7,0x0,0x7,0x0,0x14
	.DB  0x7F,0x14,0x7F,0x14,0x24,0x2A,0x7F,0x2A
	.DB  0x12,0xC4,0xC8,0x10,0x26,0x46,0x36,0x49
	.DB  0x55,0x22,0x50,0x0,0x5,0x3,0x0,0x0
	.DB  0x0,0x1C,0x22,0x41,0x0,0x0,0x41,0x22
	.DB  0x1C,0x0,0x14,0x8,0x3E,0x8,0x14,0x8
	.DB  0x8,0x3E,0x8,0x8,0x0,0x0,0x50,0x30
	.DB  0x0,0x10,0x10,0x10,0x10,0x10,0x0,0x60
	.DB  0x60,0x0,0x0,0x20,0x10,0x8,0x4,0x2
	.DB  0x3E,0x51,0x49,0x45,0x3E,0x0,0x42,0x7F
	.DB  0x40,0x0,0x42,0x61,0x51,0x49,0x46,0x21
	.DB  0x41,0x45,0x4B,0x31,0x18,0x14,0x12,0x7F
	.DB  0x10,0x27,0x45,0x45,0x45,0x39,0x3C,0x4A
	.DB  0x49,0x49,0x30,0x1,0x71,0x9,0x5,0x3
	.DB  0x36,0x49,0x49,0x49,0x36,0x6,0x49,0x49
	.DB  0x29,0x1E,0x0,0x36,0x36,0x0,0x0,0x0
	.DB  0x56,0x36,0x0,0x0,0x8,0x14,0x22,0x41
	.DB  0x0,0x14,0x14,0x14,0x14,0x14,0x0,0x41
	.DB  0x22,0x14,0x8,0x2,0x1,0x51,0x9,0x6
	.DB  0x32,0x49,0x59,0x51,0x3E,0x7E,0x11,0x11
	.DB  0x11,0x7E,0x7F,0x49,0x49,0x49,0x36,0x3E
	.DB  0x41,0x41,0x41,0x22,0x7F,0x41,0x41,0x22
	.DB  0x1C,0x7F,0x49,0x49,0x49,0x41,0x7F,0x9
	.DB  0x9,0x9,0x1,0x3E,0x41,0x49,0x49,0x7A
	.DB  0x7F,0x8,0x8,0x8,0x7F,0x0,0x41,0x7F
	.DB  0x41,0x0,0x20,0x40,0x41,0x3F,0x1,0x7F
	.DB  0x8,0x14,0x22,0x41,0x7F,0x40,0x40,0x40
	.DB  0x40,0x7F,0x2,0xC,0x2,0x7F,0x7F,0x4
	.DB  0x8,0x10,0x7F,0x3E,0x41,0x41,0x41,0x3E
	.DB  0x7F,0x9,0x9,0x9,0x6,0x3E,0x41,0x51
	.DB  0x21,0x5E,0x7F,0x9,0x19,0x29,0x46,0x46
	.DB  0x49,0x49,0x49,0x31,0x1,0x1,0x7F,0x1
	.DB  0x1,0x3F,0x40,0x40,0x40,0x3F,0x1F,0x20
	.DB  0x40,0x20,0x1F,0x3F,0x40,0x38,0x40,0x3F
	.DB  0x63,0x14,0x8,0x14,0x63,0x7,0x8,0x70
	.DB  0x8,0x7,0x61,0x51,0x49,0x45,0x43,0x0
	.DB  0x7F,0x41,0x41,0x0,0x55,0x2A,0x55,0x2A
	.DB  0x55,0x0,0x41,0x41,0x7F,0x0,0x4,0x2
	.DB  0x1,0x2,0x4,0x40,0x40,0x40,0x40,0x40
	.DB  0x0,0x1,0x2,0x4,0x0,0x20,0x54,0x54
	.DB  0x54,0x78,0x7F,0x48,0x44,0x44,0x38,0x38
	.DB  0x44,0x44,0x44,0x20,0x38,0x44,0x44,0x48
	.DB  0x7F,0x38,0x54,0x54,0x54,0x18,0x8,0x7E
	.DB  0x9,0x1,0x2,0xC,0x52,0x52,0x52,0x3E
	.DB  0x7F,0x8,0x4,0x4,0x78,0x0,0x44,0x7D
	.DB  0x40,0x0,0x20,0x40,0x44,0x3D,0x0,0x7F
	.DB  0x10,0x28,0x44,0x0,0x0,0x41,0x7F,0x40
	.DB  0x0,0x7C,0x4,0x18,0x4,0x78,0x7C,0x8
	.DB  0x4,0x4,0x78,0x38,0x44,0x44,0x44,0x38
	.DB  0x7C,0x14,0x14,0x14,0x8,0x8,0x14,0x14
	.DB  0x18,0x7C,0x7C,0x8,0x4,0x4,0x8,0x48
	.DB  0x54,0x54,0x54,0x20,0x4,0x3F,0x44,0x40
	.DB  0x20,0x3C,0x40,0x40,0x20,0x7C,0x1C,0x20
	.DB  0x40,0x20,0x1C,0x3C,0x40,0x30,0x40,0x3C
	.DB  0x44,0x28,0x10,0x28,0x44,0xC,0x50,0x50
	.DB  0x50,0x3C,0x44,0x64,0x54,0x4C,0x44,0x0
	.DB  0x6,0x9,0x9,0x6

_0x20000:
	.DB  0x33,0x33,0x31,0x30,0x5F,0x4C,0x43,0x44
	.DB  0x0,0x62,0x79,0x20,0x44,0x48,0x41,0x52
	.DB  0x4D,0x41,0x4E,0x49,0x0
__RESET:
	CLI
	CLR  R30
	OUT  EECR,R30

;INTERRUPT VECTORS ARE PLACED
;AT THE START OF FLASH
	LDI  R31,1
	OUT  GICR,R31
	OUT  GICR,R30
	OUT  MCUCR,R30

;DISABLE WATCHDOG
	LDI  R31,0x18
	OUT  WDTCR,R31
	OUT  WDTCR,R30

;CLEAR R2-R14
	LDI  R24,(14-2)+1
	LDI  R26,2
	CLR  R27
__CLEAR_REG:
	ST   X+,R30
	DEC  R24
	BRNE __CLEAR_REG

;CLEAR SRAM
	LDI  R24,LOW(0x400)
	LDI  R25,HIGH(0x400)
	LDI  R26,0x60
__CLEAR_SRAM:
	ST   X+,R30
	SBIW R24,1
	BRNE __CLEAR_SRAM

;STACK POINTER INITIALIZATION
	LDI  R30,LOW(0x45F)
	OUT  SPL,R30
	LDI  R30,HIGH(0x45F)
	OUT  SPH,R30

;DATA STACK POINTER INITIALIZATION
	LDI  R28,LOW(0x160)
	LDI  R29,HIGH(0x160)

	RJMP _main

	.ESEG
	.ORG 0

	.DSEG
	.ORG 0x160

	.CSEG
;//********************************************************
;//**** Functions for Interfacing NOKIA 3310 Display *****
;//********************************************************
;//Controller:	ATmega32 (Clock: 1 Mhz-internal)
;//Compiler:		ImageCraft ICCAVR
;//Author:		CC Dharmani, Chennai (India)
;//Date:			Sep 2008
;//********************************************************
;
;#include <mega8.h>
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x80
	.EQU __sm_mask=0x70
	.EQU __sm_powerdown=0x20
	.EQU __sm_powersave=0x30
	.EQU __sm_standby=0x60
	.EQU __sm_ext_standby=0x70
	.EQU __sm_adc_noise_red=0x10
	.SET power_ctrl_reg=mcucr
	#endif
;#include <delay.h>
;
;#include "3310_routines.h"
;
;/*--------------------------------------------------------------------------------------------------
;                                     5x7 size character generator
;--------------------------------------------------------------------------------------------------*/
;flash unsigned char smallFont[] =
;{
;     0x00, 0x00, 0x00, 0x00, 0x00,   // sp
;     0x00, 0x00, 0x2f, 0x00, 0x00,    // !
;     0x00, 0x07, 0x00, 0x07, 0x00,   // "
;     0x14, 0x7f, 0x14, 0x7f, 0x14,   // #
;     0x24, 0x2a, 0x7f, 0x2a, 0x12,   // $
;     0xc4, 0xc8, 0x10, 0x26, 0x46,   // %
;     0x36, 0x49, 0x55, 0x22, 0x50,   // &
;     0x00, 0x05, 0x03, 0x00, 0x00,   // '
;     0x00, 0x1c, 0x22, 0x41, 0x00,   // (
;     0x00, 0x41, 0x22, 0x1c, 0x00,   // )
;     0x14, 0x08, 0x3E, 0x08, 0x14,   // *
;     0x08, 0x08, 0x3E, 0x08, 0x08,   // +
;     0x00, 0x00, 0x50, 0x30, 0x00,   // ,
;     0x10, 0x10, 0x10, 0x10, 0x10,   // -
;     0x00, 0x60, 0x60, 0x00, 0x00,   // .
;     0x20, 0x10, 0x08, 0x04, 0x02,   // /
;     0x3E, 0x51, 0x49, 0x45, 0x3E,   // 0
;     0x00, 0x42, 0x7F, 0x40, 0x00,   // 1
;     0x42, 0x61, 0x51, 0x49, 0x46,   // 2
;     0x21, 0x41, 0x45, 0x4B, 0x31,   // 3
;     0x18, 0x14, 0x12, 0x7F, 0x10,   // 4
;     0x27, 0x45, 0x45, 0x45, 0x39,   // 5
;     0x3C, 0x4A, 0x49, 0x49, 0x30,   // 6
;     0x01, 0x71, 0x09, 0x05, 0x03,   // 7
;     0x36, 0x49, 0x49, 0x49, 0x36,   // 8
;     0x06, 0x49, 0x49, 0x29, 0x1E,   // 9
;     0x00, 0x36, 0x36, 0x00, 0x00,   // :
;     0x00, 0x56, 0x36, 0x00, 0x00,   // ;
;     0x08, 0x14, 0x22, 0x41, 0x00,   // <
;     0x14, 0x14, 0x14, 0x14, 0x14,   // =
;     0x00, 0x41, 0x22, 0x14, 0x08,   // >
;     0x02, 0x01, 0x51, 0x09, 0x06,   // ?
;     0x32, 0x49, 0x59, 0x51, 0x3E,   // @
;     0x7E, 0x11, 0x11, 0x11, 0x7E,   // A
;     0x7F, 0x49, 0x49, 0x49, 0x36,   // B
;     0x3E, 0x41, 0x41, 0x41, 0x22,   // C
;     0x7F, 0x41, 0x41, 0x22, 0x1C,   // D
;     0x7F, 0x49, 0x49, 0x49, 0x41,   // E
;     0x7F, 0x09, 0x09, 0x09, 0x01,   // F
;     0x3E, 0x41, 0x49, 0x49, 0x7A,   // G
;     0x7F, 0x08, 0x08, 0x08, 0x7F,   // H
;     0x00, 0x41, 0x7F, 0x41, 0x00,   // I
;     0x20, 0x40, 0x41, 0x3F, 0x01,   // J
;     0x7F, 0x08, 0x14, 0x22, 0x41,   // K
;     0x7F, 0x40, 0x40, 0x40, 0x40,   // L
;     0x7F, 0x02, 0x0C, 0x02, 0x7F,   // M
;     0x7F, 0x04, 0x08, 0x10, 0x7F,   // N
;     0x3E, 0x41, 0x41, 0x41, 0x3E,   // O
;     0x7F, 0x09, 0x09, 0x09, 0x06,   // P
;     0x3E, 0x41, 0x51, 0x21, 0x5E,   // Q
;     0x7F, 0x09, 0x19, 0x29, 0x46,   // R
;     0x46, 0x49, 0x49, 0x49, 0x31,   // S
;     0x01, 0x01, 0x7F, 0x01, 0x01,   // T
;     0x3F, 0x40, 0x40, 0x40, 0x3F,   // U
;     0x1F, 0x20, 0x40, 0x20, 0x1F,   // V
;     0x3F, 0x40, 0x38, 0x40, 0x3F,   // W
;     0x63, 0x14, 0x08, 0x14, 0x63,   // X
;     0x07, 0x08, 0x70, 0x08, 0x07,   // Y
;     0x61, 0x51, 0x49, 0x45, 0x43,   // Z
;     0x00, 0x7F, 0x41, 0x41, 0x00,   // [
;     0x55, 0x2A, 0x55, 0x2A, 0x55,   // 55
;     0x00, 0x41, 0x41, 0x7F, 0x00,   // ]
;     0x04, 0x02, 0x01, 0x02, 0x04,   // ^
;     0x40, 0x40, 0x40, 0x40, 0x40,   // _
;     0x00, 0x01, 0x02, 0x04, 0x00,   // '
;     0x20, 0x54, 0x54, 0x54, 0x78,   // a
;     0x7F, 0x48, 0x44, 0x44, 0x38,    // b
;     0x38, 0x44, 0x44, 0x44, 0x20,   // c
;     0x38, 0x44, 0x44, 0x48, 0x7F,   // d
;     0x38, 0x54, 0x54, 0x54, 0x18,   // e
;     0x08, 0x7E, 0x09, 0x01, 0x02,   // f
;     0x0C, 0x52, 0x52, 0x52, 0x3E,   // g
;     0x7F, 0x08, 0x04, 0x04, 0x78,   // h
;     0x00, 0x44, 0x7D, 0x40, 0x00,   // i
;     0x20, 0x40, 0x44, 0x3D, 0x00,   // j
;     0x7F, 0x10, 0x28, 0x44, 0x00,   // k
;     0x00, 0x41, 0x7F, 0x40, 0x00,   // l
;     0x7C, 0x04, 0x18, 0x04, 0x78,   // m
;     0x7C, 0x08, 0x04, 0x04, 0x78,   // n
;     0x38, 0x44, 0x44, 0x44, 0x38,   // o
;     0x7C, 0x14, 0x14, 0x14, 0x08,   // p
;     0x08, 0x14, 0x14, 0x18, 0x7C,   // q
;     0x7C, 0x08, 0x04, 0x04, 0x08,   // r
;     0x48, 0x54, 0x54, 0x54, 0x20,   // s
;     0x04, 0x3F, 0x44, 0x40, 0x20,   // t
;     0x3C, 0x40, 0x40, 0x20, 0x7C,   // u
;     0x1C, 0x20, 0x40, 0x20, 0x1C,   // v
;     0x3C, 0x40, 0x30, 0x40, 0x3C,   // w
;     0x44, 0x28, 0x10, 0x28, 0x44,   // x
;     0x0C, 0x50, 0x50, 0x50, 0x3C,   // y
;     0x44, 0x64, 0x54, 0x4C, 0x44,   // z
;	 0x00, 0x06, 0x09, 0x09, 0x06    // º
;};
;
;/*--------------------------------------------------------------------------------------------------
;  Name         :  LCD_init
;  Description  :  LCD controller initialization.
;  Argument(s)  :  None.
;  Return value :  None.
;--------------------------------------------------------------------------------------------------*/
;void LCD_init ( void )
; 0000 0079 {

	.CSEG
_LCD_init:
; 0000 007A 
; 0000 007B 	delay_ms(100);
	RCALL SUBOPT_0x0
; 0000 007C 
; 0000 007D 	CLEAR_SCE_PIN;    //Enable LCD
	CBI  0x18,2
; 0000 007E 
; 0000 007F 	CLEAR_RST_PIN;	//reset LCD
	SBI  0x18,4
; 0000 0080     delay_ms(100);
	RCALL SUBOPT_0x0
; 0000 0081     SET_RST_PIN;
	SBI  0x18,4
; 0000 0082 
; 0000 0083 	SET_SCE_PIN;	//disable LCD
	SBI  0x18,2
; 0000 0084 
; 0000 0085     LCD_writeCommand( 0x21 );  // LCD Extended Commands.
	LDI  R30,LOW(33)
	RCALL SUBOPT_0x1
; 0000 0086     LCD_writeCommand( 0xE0 );  // Set LCD Vop (Contrast).
	LDI  R30,LOW(224)
	RCALL SUBOPT_0x1
; 0000 0087     LCD_writeCommand( 0x04 );  // Set Temp coefficent.
	LDI  R30,LOW(4)
	RCALL SUBOPT_0x1
; 0000 0088     LCD_writeCommand( 0x13 );  // LCD bias mode 1:48.
	LDI  R30,LOW(19)
	RCALL SUBOPT_0x1
; 0000 0089     LCD_writeCommand( 0x20 );  // LCD Standard Commands, Horizontal addressing mode.
	LDI  R30,LOW(32)
	RCALL SUBOPT_0x1
; 0000 008A     LCD_writeCommand( 0x0c );  // LCD in normal mode.
	LDI  R30,LOW(12)
	RCALL SUBOPT_0x1
; 0000 008B 
; 0000 008C     LCD_clear();
	RCALL _LCD_clear
; 0000 008D }
	RET
;
;/*--------------------------------------------------------------------------------------------------
;  Name         :  LCD_writeCommand
;  Description  :  Sends command to display controller.
;  Argument(s)  :  command -> command to be sent
;  Return value :  None.
;--------------------------------------------------------------------------------------------------*/
;void LCD_writeCommand ( unsigned char command )
; 0000 0096 {
_LCD_writeCommand:
; 0000 0097     CLEAR_SCE_PIN;	  //enable LCD
;	command -> Y+0
	CBI  0x18,2
; 0000 0098 
; 0000 0099 	CLEAR_DC_PIN;	  //set LCD in command mode
	CBI  0x18,0
; 0000 009A 
; 0000 009B     //  Send data to display controller.
; 0000 009C     SPDR = command;
	LD   R30,Y
	OUT  0xF,R30
; 0000 009D 
; 0000 009E     //  Wait until Tx register empty.
; 0000 009F     while ( !(SPSR & 0x80) );
_0x3:
	SBIS 0xE,7
	RJMP _0x3
; 0000 00A0 
; 0000 00A1     SET_SCE_PIN;   	 //disable LCD
	RJMP _0x2000002
; 0000 00A2 }
;
;/*--------------------------------------------------------------------------------------------------
;  Name         :  LCD_writeData
;  Description  :  Sends Data to display controller.
;  Argument(s)  :  Data -> Data to be sent
;  Return value :  None.
;--------------------------------------------------------------------------------------------------*/
;void LCD_writeData ( unsigned char Data )
; 0000 00AB {
_LCD_writeData:
; 0000 00AC     CLEAR_SCE_PIN;	  //enable LCD
;	Data -> Y+0
	CBI  0x18,2
; 0000 00AD 
; 0000 00AE 	SET_DC_PIN;	  //set LCD in Data mode
	SBI  0x18,0
; 0000 00AF 
; 0000 00B0     //  Send data to display controller.
; 0000 00B1     SPDR = Data;
	LD   R30,Y
	OUT  0xF,R30
; 0000 00B2 
; 0000 00B3     //  Wait until Tx register empty.
; 0000 00B4     while ( !(SPSR & 0x80) );
_0x6:
	SBIS 0xE,7
	RJMP _0x6
; 0000 00B5 
; 0000 00B6     SET_SCE_PIN;   	 //disable LCD
_0x2000002:
	SBI  0x18,2
; 0000 00B7 }
	ADIW R28,1
	RET
;
;/*--------------------------------------------------------------------------------------------------
;  Name         :  LCD_clear
;  Description  :  Clears the display
;  Argument(s)  :  None.
;  Return value :  None.
;--------------------------------------------------------------------------------------------------*/
;void LCD_clear ( void )
; 0000 00C0 {
_LCD_clear:
; 0000 00C1     int i,j;
; 0000 00C2 
; 0000 00C3 	LCD_gotoXY (0,0);  	//start with (0,0) position
	RCALL __SAVELOCR4
;	i -> R16,R17
;	j -> R18,R19
	RCALL SUBOPT_0x2
	RCALL SUBOPT_0x2
	RCALL _LCD_gotoXY
; 0000 00C4 
; 0000 00C5     for(i=0; i<8; i++)
	__GETWRN 16,17,0
_0xA:
	__CPWRN 16,17,8
	BRGE _0xB
; 0000 00C6 	  for(j=0; j<90; j++)
	__GETWRN 18,19,0
_0xD:
	__CPWRN 18,19,90
	BRGE _0xE
; 0000 00C7 	     LCD_writeData( 0x00 );
	RCALL SUBOPT_0x2
	RCALL _LCD_writeData
	__ADDWRN 18,19,1
	RJMP _0xD
_0xE:
; 0000 00C9 LCD_gotoXY (0,0);
	__ADDWRN 16,17,1
	RJMP _0xA
_0xB:
	RCALL SUBOPT_0x2
	RCALL SUBOPT_0x2
	RCALL _LCD_gotoXY
; 0000 00CA    // LcdSend( 0x40, LCD_CMD );
; 0000 00CB    // LcdSend( 0x80, LCD_CMD );
; 0000 00CC 
; 0000 00CD }
	RCALL __LOADLOCR4
	ADIW R28,4
	RET
;
;/*--------------------------------------------------------------------------------------------------
;  Name         :  LCD_gotoXY
;  Description  :  Sets cursor location to xy location corresponding to basic font size.
;  Argument(s)  :  x - range: 0 to 84
;  			   	  y -> range: 0 to 5
;  Return value :  None.
;--------------------------------------------------------------------------------------------------*/
;void LCD_gotoXY ( unsigned char x, unsigned char y )
; 0000 00D7 {
_LCD_gotoXY:
; 0000 00D8     LCD_writeCommand (0x80 | x);   //column
;	x -> Y+1
;	y -> Y+0
	LDD  R30,Y+1
	ORI  R30,0x80
	RCALL SUBOPT_0x1
; 0000 00D9 	LCD_writeCommand (0x40 | y);   //row
	LD   R30,Y
	ORI  R30,0x40
	RCALL SUBOPT_0x1
; 0000 00DA }
	RJMP _0x2000001
;
;/*--------------------------------------------------------------------------------------------------
;  Name         :  LCD_writeChar
;  Description  :  Displays a character at current cursor location and increment cursor location.
;  Argument(s)  :  ch   -> Character to write.
;  Return value :  None.
;--------------------------------------------------------------------------------------------------*/
;void LCD_writeChar ( unsigned char ch)
; 0000 00E3 {
_LCD_writeChar:
; 0000 00E4    unsigned char j;
; 0000 00E5 
; 0000 00E6    LCD_writeData(0x00);
	ST   -Y,R17
;	ch -> Y+1
;	j -> R17
	RCALL SUBOPT_0x2
	RCALL _LCD_writeData
; 0000 00E7 
; 0000 00E8    for(j=0; j<5; j++)
	LDI  R17,LOW(0)
_0x10:
	CPI  R17,5
	BRSH _0x11
; 0000 00E9      LCD_writeData( smallFont [(unsigned int)(ch-32)*5 + j] );
	LDD  R30,Y+1
	SUBI R30,LOW(32)
	LDI  R26,LOW(5)
	MUL  R30,R26
	MOVW R30,R0
	MOVW R26,R30
	MOV  R30,R17
	LDI  R31,0
	ADD  R30,R26
	ADC  R31,R27
	SUBI R30,LOW(-_smallFont*2)
	SBCI R31,HIGH(-_smallFont*2)
	LPM  R30,Z
	ST   -Y,R30
	RCALL _LCD_writeData
	SUBI R17,-1
	RJMP _0x10
_0x11:
; 0000 00EB LCD_writeData( 0x00 );
	RCALL SUBOPT_0x2
	RCALL _LCD_writeData
; 0000 00EC }
	LDD  R17,Y+0
	RJMP _0x2000001
;
;/*--------------------------------------------------------------------------------------------------
;  Name         :  LCD_writeString_F
;  Description  :  Displays a character at current cursor location
;  Argument(s)  :  string -> Pointer to ASCII string (stored in FLASH)
;  Return value :  None.
;--------------------------------------------------------------------------------------------------*/
;void LCD_writeString_F ( flash unsigned char *string )
; 0000 00F5 {
_LCD_writeString_F:
; 0000 00F6     while ( *string )
;	*string -> Y+0
_0x12:
	LD   R30,Y
	LDD  R31,Y+1
	LPM  R30,Z
	CPI  R30,0
	BREQ _0x14
; 0000 00F7         LCD_writeChar( *string++ );
	LD   R30,Y
	LDD  R31,Y+1
	ADIW R30,1
	ST   Y,R30
	STD  Y+1,R31
	SBIW R30,1
	LPM  R30,Z
	ST   -Y,R30
	RCALL _LCD_writeChar
	RJMP _0x12
_0x14:
; 0000 00F8 }
_0x2000001:
	ADIW R28,2
	RET
;
;/*--------------------------------------------------------------------------------------------------
;  Name         :  LCD_drawBorder
;  Description  :  Draws rectangle border on the display
;  Argument(s)  :  None
;  Return value :  None
;--------------------------------------------------------------------------------------------------*/
;void LCD_drawBorder (void )
; 0000 0101 {
_LCD_drawBorder:
; 0000 0102   unsigned char i, j;
; 0000 0103 
; 0000 0104   for(i=0; i<7; i++)
	RCALL __SAVELOCR2
;	i -> R17
;	j -> R16
	LDI  R17,LOW(0)
_0x16:
	CPI  R17,7
	BRSH _0x17
; 0000 0105   {
; 0000 0106     LCD_gotoXY (0,i);
	RCALL SUBOPT_0x2
	ST   -Y,R17
	RCALL _LCD_gotoXY
; 0000 0107 
; 0000 0108 	for(j=0; j<84; j++)
	LDI  R16,LOW(0)
_0x19:
	CPI  R16,84
	BRSH _0x1A
; 0000 0109 	{
; 0000 010A 	  if(j == 0 || j == 83)
	CPI  R16,0
	BREQ _0x1C
	CPI  R16,83
	BRNE _0x1B
_0x1C:
; 0000 010B 		LCD_writeData (0xff);		// first and last column solid fill to make line
	LDI  R30,LOW(255)
	RJMP _0x23
; 0000 010C 	  else if(i == 0)
_0x1B:
	CPI  R17,0
	BRNE _0x1F
; 0000 010D 	    LCD_writeData (0x08);		// row 0 is having only 5 bits (not 8)
	LDI  R30,LOW(8)
	RJMP _0x23
; 0000 010E 	  else if(i == 6)
_0x1F:
	CPI  R17,6
	BRNE _0x21
; 0000 010F 	    LCD_writeData (0x04);		// row 6 is having only 3 bits (not 8)
	LDI  R30,LOW(4)
	RJMP _0x23
; 0000 0110 	  else
_0x21:
; 0000 0111 	    LCD_writeData (0x00);
	LDI  R30,LOW(0)
_0x23:
	ST   -Y,R30
	RCALL _LCD_writeData
; 0000 0112 	}
	SUBI R16,-1
	RJMP _0x19
_0x1A:
; 0000 0113   }
	SUBI R17,-1
	RJMP _0x16
_0x17:
; 0000 0114 }
	RCALL __LOADLOCR2P
	RET
;
;
;/*--------------------------------------------------------------------------------------------------
;                                         End of file.
;--------------------------------------------------------------------------------------------------*/
;
;//********************************************************
;//***** PROGRAM FOR Interfacing NOKIA 3310 Display ******
;//********************************************************
;//Controller:	ATmega32 (Clock: 1 Mhz-internal)
;//Compiler:		ImageCraft ICCAVR
;//Author:		CC Dharmani, Chennai (India)
;//Date:			Sep 2008
;//********************************************************
;
;#include <mega8.h>
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x80
	.EQU __sm_mask=0x70
	.EQU __sm_powerdown=0x20
	.EQU __sm_powersave=0x30
	.EQU __sm_standby=0x60
	.EQU __sm_ext_standby=0x70
	.EQU __sm_adc_noise_red=0x10
	.SET power_ctrl_reg=mcucr
	#endif
;#include <delay.h>
;#include "3310_routines.h"
;
;void port_init(void)
; 0001 000F {

	.CSEG
_port_init:
; 0001 0010  PORTB = 0xFF;
	LDI  R30,LOW(255)
	OUT  0x18,R30
; 0001 0011  DDRB  = 0xFF;
	OUT  0x17,R30
; 0001 0012  PORTC = 0x00;
	LDI  R30,LOW(0)
	OUT  0x15,R30
; 0001 0013  DDRC  = 0x00;
	OUT  0x14,R30
; 0001 0014  PORTD = 0x00;
	OUT  0x12,R30
; 0001 0015  DDRD  = 0xFF;
	LDI  R30,LOW(255)
	OUT  0x11,R30
; 0001 0016 }
	RET
;
;//SPI initialize
;//clock rate: 250000hz
;void spi_init(void)
; 0001 001B {
_spi_init:
; 0001 001C  SPCR = 0x58; //setup SPI
	LDI  R30,LOW(88)
	OUT  0xD,R30
; 0001 001D }
	RET
;
;//TWI initialize
;// bit rate:2 (freq: 50Khz)
;void twi_init(void)
; 0001 0022 {
_twi_init:
; 0001 0023  TWCR= 0X00; //disable twi
	LDI  R30,LOW(0)
	OUT  0x36,R30
; 0001 0024  TWBR= 0x02; //set bit rate
	LDI  R30,LOW(2)
	OUT  0x0,R30
; 0001 0025  TWSR= 0x00; //set prescale
	LDI  R30,LOW(0)
	OUT  0x1,R30
; 0001 0026  TWAR= 0x00; //set slave address
	OUT  0x2,R30
; 0001 0027  TWCR= 0x44; //enable twi
	LDI  R30,LOW(68)
	OUT  0x36,R30
; 0001 0028 }
	RET
;
;//call this routine to initialize all peripherals
;void init_devices(void)
; 0001 002C {
_init_devices:
; 0001 002D  //stop errant interrupts until set up
; 0001 002E  port_init();
	RCALL _port_init
; 0001 002F  spi_init();
	RCALL _spi_init
; 0001 0030  twi_init();
	RCALL _twi_init
; 0001 0031 
; 0001 0032  MCUCR = 0x00;
	LDI  R30,LOW(0)
	OUT  0x35,R30
; 0001 0033  GICR  = 0x00;
	OUT  0x3B,R30
; 0001 0034  TIMSK = 0x00; //timer interrupt sources
	OUT  0x39,R30
; 0001 0035  //SEI(); //re-enable interrupts
; 0001 0036  //all peripherals are now initialized
; 0001 0037 }
	RET
;
;//
;void main(void)
; 0001 003B {
_main:
; 0001 003C  unsigned char LED=0;
; 0001 003D 
; 0001 003E  init_devices();
;	LED -> R17
	LDI  R17,0
	RCALL _init_devices
; 0001 003F 
; 0001 0040  LCD_init();
	RCALL _LCD_init
; 0001 0041 
; 0001 0042  delay_ms(100);	   //delay for ds1621 write complete
	RCALL SUBOPT_0x0
; 0001 0043 
; 0001 0044  LCD_drawBorder();
	RCALL _LCD_drawBorder
; 0001 0045 
; 0001 0046  LCD_gotoXY (4,2);
	LDI  R30,LOW(4)
	ST   -Y,R30
	LDI  R30,LOW(2)
	ST   -Y,R30
	RCALL _LCD_gotoXY
; 0001 0047  LCD_writeString_F("3310_LCD");
	__POINTW1FN _0x20000,0
	ST   -Y,R31
	ST   -Y,R30
	RCALL _LCD_writeString_F
; 0001 0048 
; 0001 0049  LCD_gotoXY (4,3);
	LDI  R30,LOW(4)
	ST   -Y,R30
	LDI  R30,LOW(3)
	ST   -Y,R30
	RCALL _LCD_gotoXY
; 0001 004A  LCD_writeString_F("by DHARMANI");
	__POINTW1FN _0x20000,9
	ST   -Y,R31
	ST   -Y,R30
	RCALL _LCD_writeString_F
; 0001 004B 
; 0001 004C 
; 0001 004D  while(1)
_0x20003:
; 0001 004E  {
; 0001 004F    if(LED == 0)
	CPI  R17,0
	BRNE _0x20006
; 0001 0050    {
; 0001 0051  	 PORTD |= 0x01;
	SBI  0x12,0
; 0001 0052 	 LED = 1;
	LDI  R17,LOW(1)
; 0001 0053    }
; 0001 0054    else
	RJMP _0x20007
_0x20006:
; 0001 0055    {
; 0001 0056 	 PORTD &= ~0x01;
	CBI  0x12,0
; 0001 0057 	 LED = 0;
	LDI  R17,LOW(0)
; 0001 0058    }
_0x20007:
; 0001 0059 
; 0001 005A  }
	RJMP _0x20003
; 0001 005B 
; 0001 005C }
_0x20008:
	RJMP _0x20008

	.CSEG
;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0x0:
	LDI  R30,LOW(100)
	LDI  R31,HIGH(100)
	ST   -Y,R31
	ST   -Y,R30
	RJMP _delay_ms

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x1:
	ST   -Y,R30
	RJMP _LCD_writeCommand

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x2:
	LDI  R30,LOW(0)
	ST   -Y,R30
	RET


	.CSEG
_delay_ms:
	ld   r30,y+
	ld   r31,y+
	adiw r30,0
	breq __delay_ms1
__delay_ms0:
	__DELAY_USW 0xFA
	wdr
	sbiw r30,1
	brne __delay_ms0
__delay_ms1:
	ret

__SAVELOCR4:
	ST   -Y,R19
__SAVELOCR3:
	ST   -Y,R18
__SAVELOCR2:
	ST   -Y,R17
	ST   -Y,R16
	RET

__LOADLOCR4:
	LDD  R19,Y+3
__LOADLOCR3:
	LDD  R18,Y+2
__LOADLOCR2:
	LDD  R17,Y+1
	LD   R16,Y
	RET

__LOADLOCR2P:
	LD   R16,Y+
	LD   R17,Y+
	RET

;END OF CODE MARKER
__END_OF_CODE:
