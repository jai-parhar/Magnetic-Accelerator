#include <msp430.h> 


/***
 * Magnetic Launcher
 *
 *  Pins
 *      6.0 is adc
 *      1.4 is IN1
 *      1.5 is IN2
 *
***/

enum LauncherState{
    READY,
    NORTH_PULL,
    SOUTH_PULL
};

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	enum LauncherState currState = READY;

	ADC12CTL0 = ADC12SHT02 + ADC12ON;   // Sampling time, ADC12 on
	ADC12CTL1 = ADC12SHP;               // sampling timer
	ADC12CTL0 |= ADC12ENC;                    // ADC enable

	P6SEL |= 0x01;                     // P6.0 allow ADC on pin 6.0

	ADC12MCTL0 = ADC12INCH_0;
	          //selects which input results are
	          //stored in memory ADC12MEM0. Input
	          //one is selected on reset so this line is not needed
	          //Must be written before enabling conversions
	ADC12CTL0 |= ADC12ENC;                    // ADC enable

	// Set up pins 1.4 and 1.5
	P1DIR |= BIT4;
	P1DIR |= BIT5;

	// Coil off
	P1OUT &= ~BIT4;
	P1OUT &= ~BIT5;

	// Set up and turn off P1.0 (onboard LED)
	P1DIR |= BIT0;
	P1OUT &= ~BIT0;



	while(1)
	{

	    ADC12CTL0 |= ADC12SC;                   // Start sampling
	    while (ADC12CTL1 & ADC12BUSY);//while bit ADC12BUSY in register ADC12CTL1 is high wait

	    float sensorPercent = ((float)ADC12MEM0)/4095.0f; // Returns a number between 0 and 1 that corresponds to the voltage between 0 and 3.3V

	    float thresholdPercent = 0.04f; // Threshold, prevents small fluctuations in magnetic field from activating

	    if(currState == READY)
	    {
	        // sees North
	        if((sensorPercent - 0.5f) < -thresholdPercent)
	        {
	            currState = NORTH_PULL;

	            // Turn on red LED
	            P1OUT |= BIT0;

	            // Set polarity such that magnet is attracted
	            P1OUT &= ~BIT4;
	            P1OUT |= BIT5;

	            // Wait for magnet to reach center
	            __delay_cycles(135000);

	            // Set polarity such that magnet is repelled
	            P1OUT |= BIT4;
	            P1OUT &= ~BIT5;
	        }

	        // sees South
	        if((sensorPercent - 0.5f) > thresholdPercent)
            {
                currState = SOUTH_PULL;

                // Turn on red LED
                P1OUT |= BIT0;

                // Set polarity such that magnet is attracted
                P1OUT |= BIT4;
                P1OUT &= ~BIT5;

                // Wait for magnet to reach center
                __delay_cycles(135000);

                // Set polarity such that magnet is repelled
                P1OUT &= ~BIT4;
                P1OUT |= BIT5;
            }

	    }

	    // Just fired
	    if (currState != READY)
	    {
	        // Wait to make sure magnet is fully out
	        __delay_cycles(2000000);

	        // Turn off magnet
	        P1OUT &= ~BIT4;
	        P1OUT &= ~BIT5;

	        //Turn off red LED
            P1OUT &= ~BIT0;

            // Set state to ready
	        currState = READY;
	    }
	}



	return 0;
}
