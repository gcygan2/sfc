#define	PORT_X	PORTD
#define	DDR_X	DDRD
#define	_X0	(1<<2)
#define	_X1	(1<<3)
#define	_X2	(1<<4)
#define	_X3	(1<<5)
#define	_X4	(1<<6)
#define	_X5	(1<<7)

#define	_T1	(1<<2)	/*I2C*/
#define	_T2	(1<<3)	/*I2C*/	
#define	_T3	(1<<4)	/*I2C*/	
#define	_T4	(1<<5)	/*I2C*/	
#define	_T5	(1<<6)	/*I2C*/	
#define	_T6	(1<<7)	/*I2C*/	
#define	_T7	(1<<1)	/*I2C*/	
#define	_T8	(1<<0)	/*I2C*/	

#define	PORT_OUT PORTB
#define	DDR_OUT	DDRB
#define	_H1	(1<<0)
#define	_H2	(1<<2)
#define	_Y1	(1<<3)
#define	_Y2	(1<<4)

#define	PORT_IN PORTC
#define	PIN_IN PINC
#define	DDR_IN	DDRC
#define	_S1	(1<<0)
#define	_S2	(1<<1)
#define	_B1	(1<<3)

#define __SEG_A (1<<6)
#define __SEG_B (1<<7)
#define __SEG_C (1<<1)
#define __SEG_D (1<<2)
#define __SEG_E (1<<3)
#define __SEG_F (1<<5)
#define __SEG_G (1<<4)

#define LED_0 (__SEG_A|__SEG_B|__SEG_C|__SEG_D|__SEG_E|__SEG_F)
#define LED_1 (__SEG_B|__SEG_C)
#define LED_2 (__SEG_A|__SEG_B|__SEG_D|__SEG_E|__SEG_G)
#define LED_3 (__SEG_A|__SEG_B|__SEG_C|__SEG_D|__SEG_G)
#define LED_4 (__SEG_B|__SEG_C|__SEG_F|__SEG_G)
#define LED_5 (__SEG_A|__SEG_C|__SEG_D|__SEG_F|__SEG_G)
#define LED_6 (__SEG_A|__SEG_C|__SEG_D|__SEG_E|__SEG_F|__SEG_G)
#define LED_7 (__SEG_A|__SEG_B|__SEG_C)
#define LED_8 (__SEG_A|__SEG_B|__SEG_C|__SEG_D|__SEG_E|__SEG_F|__SEG_G)
#define LED_9 (__SEG_A|__SEG_B|__SEG_C|__SEG_D|__SEG_F|__SEG_G)
