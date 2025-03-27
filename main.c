#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "hardware.h"

const uint8_t bin_to_7seg [] = {~LED_0, ~LED_1, ~LED_2, ~LED_3, ~LED_4, ~LED_5, ~LED_6, ~LED_7, ~LED_8, ~LED_9};
const uint8_t bin_to_seg [] = {~__SEG_A, ~__SEG_B, ~__SEG_C, ~__SEG_D, ~__SEG_E, ~__SEG_F, ~__SEG_G};
const uint8_t bin_to_tran [] = {~_T1, ~_T2, ~_T3, ~_T4, ~_T5, ~_T6, ~_T7, ~_T8};

volatile uint8_t y1_t=0, y2_t=0, t1=0, t2=0, t3=0, t4=0;
volatile uint8_t y1_del=0, y2_del=0, t1_del=0, t2_del=0, t3_del=0, t4_del=0, clk_1hz=0;

ISR (TIMER0_OVF_vect)
{
	static uint8_t tick;
	if (!(tick++ & 0x1f)) {
		clk_1hz ^= 1;
		
		if (t1_del > 1) {
			t1_del--;
		} else if (t1_del == 1) {
			t1_del = 0;
			t1 = 1;
		}

		if (t2_del > 1) {
			t2_del--;
		} else if (t2_del == 1) {
			t2_del = 0;
			t2 = 1;
		}

		if (t3_del > 1) {
			t3_del--;
		} else if (t3_del == 1) {
			t3_del = 0;
			t3 = 1;
		}

		if (t4_del > 1) {
			t4_del--;
		} else if (t4_del == 1) {
			t4_del = 0;
			t4 = 1;
		}

		if (y1_del > 1) {
			y1_del--;
		} else if (y1_del == 1) {
			y1_del = 0;
			y1_t = 1;
		}

		if (y2_del > 1) {
			y2_del--;
		} else if (y2_del == 1) {
			y2_del = 0;
			y2_t = 1;
		}
	}
}

void print_tran (uint8_t b)
{
	i2c_start (0x70);
	i2c_write (b);
	i2c_stop();
}

void print_c (uint8_t b)
{
	i2c_start (0x42);
	i2c_write (b);
	i2c_stop();
}

void demo ()
{
	uint8_t i;
	/*for (i = 0; i < 8; i++) {
		print_tran (bin_to_tran[i]);
		_delay_ms (500);
	}*/
	print_tran (0);
	print_c (bin_to_7seg[8]);
	PORT_X |= (_X0|_X1|_X2|_X3|_X4|_X5);
	PORT_OUT |= (_H1|_H2);
	/*
	for (i = 0; i < 10; i++) {
		print_c (bin_to_7seg[i]);
		_delay_ms (500);
	}
*/
	_delay_ms (1000);
}

int main (){
	uint8_t x0=1,x1=0,x2=0,x3=0,x4=0,x5=0;
	uint8_t s1=0,s2=1, b1=0,c=0;

	uint8_t h1=0,h2=0,y1=0,y2=0;
	uint8_t h1_s=0,h2_s=0,y1_s=0,y2_s=0;
	
	char cmd, val;
	uint16_t time = 0;
	uint8_t input, tranzycja, zmiana;	

	DDR_OUT |= _H1|_H2|_Y1|_Y2;
	DDR_X |= _X0|_X1|_X2|_X3|_X4|_X5;
	
	DDR_IN &= ~(_S1|_S2|_B1);
	PORT_IN |= _S1|_S2|_B1;
	
	usart_init();
	i2c_init();

	TCCR0B |= (1<<CS02)|(1<<CS00);
	TIMSK0 |= (1<<TOIE0);
	
	sei ();

	demo();

	for (;;) {
		input = PIN_IN;
		s1 = (input & _S1)? 0:1;
		s2 = (input & _S2)? 1:0;
		b1 = (input & _B1)? 0:1;

		tranzycja = 0;
		zmiana = 0;
		if(s1) {
			tranzycja |= _T1;
			if(x0 && !zmiana) {
				zmiana = 1;
				x0=0;
				x1=1;
				h1_s = 1;
				if (!y1_del) y1_del = 4; //D|Y1
			}
		}
		if(b1) {
			tranzycja |= _T2;
			if(x1 && !zmiana) {
				zmiana = 1;
				x1=0;
				x2=1;
				c++;
				if (!t1_del) t1_del = 4; //D|t1
				y1_t = 0;
			}
		}
		if(!s2) {
			tranzycja |= _T7;
			if (x1 && !zmiana) {
				zmiana = 1;
				x1=0;
				x3=1;
				if (!t2_del) t2_del = 24; //D|t2
				if (!y2_del) y2_del = 20; //L|Y2
				y1_t = 0;
			}
		}
		if(t1 || !s2) {
			t1 = 0;
			tranzycja |= _T3;
			if (x2 && !zmiana) {
				zmiana = 1;
				x2=0;
				x3=1;
				if (!t2_del) t2_del = 24; //D|t2
				if (!y2_del) y2_del = 20; //L|Y2
			}
		} 
		if(t2) {
			t2 = 0;
			tranzycja |= _T4;
			if (x3 && !zmiana) {
				zmiana = 1;
				x3=0;
				x4=1;
				//c++;
				y2_t = 0;
				if (!t3_del) t3_del = 10; //D|t2
			}
		} 
		if ((c == 4) && t3) {
			t3 = 0;
			tranzycja |= _T5;
			if (x4 && !zmiana) {
				zmiana = 1;
				x4=0;
				x5=1;
				if (!t4_del) t4_del = 24; //D|t3
			}
		} 
		if (c < 4 && t3) {
			t3 = 0;
			tranzycja |= _T8;
			if (x4 && !zmiana) {
				zmiana = 1;
				x4=0;
				x1=1;
				h1_s = 1;
				if (!y1_del) y1_del = 4; //D|Y1
			}
		} 
		if (t4) {
			t4 = 0;
			tranzycja |= _T6;
			if (x5 && !zmiana) {
				zmiana = 1;
				x5=0;
				x0=1;
				c = 0;
				h1_s = 0;
			}
		}
		print_tran(~tranzycja);
		
		h2 = x5 && clk_1hz || x2;

		
		if (h1 || h1_s) {
			PORT_OUT |= _H1;
		} else {
			PORT_OUT &= ~_H1;
		}
		
		if (h2 || h2_s) {
			PORT_OUT |= _H2;
		} else {
			PORT_OUT &= ~_H2;
		}
		
		if (y1 || (y1_t && x1)) {
			PORT_OUT |= _Y1;
		} else {
			PORT_OUT &= ~_Y1;
		}
		
		if (y2 || (!y2_t && x3)) {
			PORT_OUT |= _Y2;
		} else {
			PORT_OUT &= ~_Y2;
		}

		if (x0) {
			PORT_X |= _X0;
		} else {
			PORT_X &= ~_X0;
		}
		if (x1) {
			PORT_X |= _X1;
		} else {
			PORT_X &= ~_X1;
		}
		if (x2) {
			PORT_X |= _X2;
		} else {
			PORT_X &= ~_X2;
		}
		if (x3) {
			PORT_X |= _X3;
		} else {
			PORT_X &= ~_X3;
		}
		if (x4) {
			PORT_X |= _X4;
		} else {
			PORT_X &= ~_X4;
		}
		if (x5) {
			PORT_X |= _X5;
		} else {
			PORT_X &= ~_X5;
		}
		
		print_c (bin_to_7seg[c]);
		_delay_ms (200);
	}
	
    return 0;
}
