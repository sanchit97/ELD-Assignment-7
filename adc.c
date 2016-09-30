#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/power.h>
#include <avr/sleep.h>
#define THRMT 130
#define LDRT 400
void init()
{
	ADMUX=(1<<REFS0);	//done to take reference voltage as 5V
	//ADMUX=(1<<MUX);
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);	//aden is adc enable, adps2/1/0 for selecting pre-scaler as 8
}
uint16_t adcread(uint8_t ch)
{
    ch &= 0b00000111;  // AND operation with 7 so that the range of the register does not cross 7
    ADMUX = (ADMUX & 0xF8)|ch;     // clears the bottom 3 bits before ORing
    ADCSRA |= (1<<ADSC);	//start conversion by setting adsc bit 1
    while(ADCSRA & (1<<ADSC));	//run this loop  till the adsc bit becomes 0 or the conversion finishes
    return (ADC);	//return the value calculated back to the function
}
void prr()
{
	PRR=0xFF;
	PRR=PRR&= ~_BV(PRADC);
	SMCR=0x00;
	SMCR|=_BV(SE)|_BV(SM0);
}
int main(void)
{
	prr();
	DDRB|=_BV(DDB5);	//connecting LEDs to 
	DDRC|=_BV(DDC3);	//connecting LEDs to 
	uint16_t result1=0,result2=0;
	init();
	// add prr
	while(1)
	{
		//_delay_ms(10);
		result1=adcread(0);	//read adc value from channel 0
		//_delay_ms(250);
		if(result1<LDRT)	//comparing with thresholds
		{
			//led lights up
			PORTB|= _BV(PORTB5);	//LED on
		}
		else
		{
			PORTB&= ~_BV(PORTB5);	//LED off
		}
		_delay_ms(10);
		result2=adcread(1);	//read adc value from channel 1
		if(result2>THRMT)
		{
			PORTC|= _BV(PORTC3);	//LED on
			//led2 lights up
		}
		else
		{
			PORTC&= ~_BV(PORTC3);	//LED off
		}
	}
}