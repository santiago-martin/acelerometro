/* 
 * File:   Acelerometro.c
 * Author: santi
 *
 * Created on 24 de septiembre de 2019, 15:42
 */

#include <stdio.h>
#include <stdlib.h>

#include <p33fj128gp202.h>
#define FCY 40000000UL//

_FICD(ICS_PGD2 & JTAGEN_OFF); // Para hacer debuging por el puerto 2
_FOSCSEL(FNOSC_FRC);
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT);
_FWDT(FWDTEN_OFF);
#pragma config ALTI2C = ON              // Alternate I2C  pins (I2C mapped to ASDA1/ASCL1 pins)


void ConfigIni(void);

void main(void)
{
ConfigIni();
I2C1CONbits.SEN=1;
while(I2C1CONbits.SEN == 1){}

I2C1TRN=0x1C;// mando codigo de familia del acelerometro
while (I2C1STATbits.ACKSTAT == 1){}
I2C1TRN=0x0D
while (I2C1STATbits.ACKSTAT == 1){}

unsigned char aux=I2C1RCV;
if (aux==0x1A){
    
    
}
}




void ConfigIni (void) {
    // Configure Oscillator to operate the device at 40 MHz
    // Fosc = Fin * M/(N1 * N2), Fcy = Fosc/2
    // Fosc = 8M * 40/(2 * 2) = 80 MHz for 8M input clock

    PLLFBD = 40; 			    // M = 40
    CLKDIVbits.PLLPOST = 0;		// N2 = 2
    CLKDIVbits.PLLPRE = 0; 		// N1 = 2
    __builtin_write_OSCCONH(0x03);	// Initiate Clock Switch to Primary Oscillator with PLL (NOSC = 0b011)
    __builtin_write_OSCCONL(0x01);
    while(OSCCONbits.COSC != 0b011);// Wait for Clock switch to occur
    while(OSCCONbits.LOCK != 1){};	// Wait for PLL to lock
    
    
    
    //configuro los pines
    
    //configuro I2C
    I2C1BRG=0x5D;
    I2C1CONbits.I2CEN=1;
    IFS1bits.SI2C1IF = 0;
    //config uart
    #define FP 40000000
    #define BAUDRATE 9600
    #define BRGVAL ((FP/BAUDRATE)/16)-1
    U1MODEbits.STSEL = 0; // 1-Stop bit
    U1MODEbits.PDSEL = 0; // No Parity, 8-Data bits
    U1MODEbits.ABAUD = 0; // Auto-Baud disabled
    U1MODEbits.BRGH = 0; // Standard-Speed mode
    U1BRG = BRGVAL; // Baud Rate setting for 9600
    U1STAbits.UTXISEL0 = 0; // Interrupt after one TX character is transmitted
    U1STAbits.UTXISEL1 = 0;
    IEC0bits.U1TXIE = 1; // Enable UART TX interrupt
    U1MODEbits.UARTEN = 1; // Enable UART
    U1STAbits.UTXEN = 1; // Enable UART TX
    //U1TXREG = 'a'; // Transmit one character
    //REMAPEO DE PINES 
    OSCCON = 0x46;			// Command Sequence- Registro de control del oscilador
    OSCCON = 0x57;
    OSCCONbits.IOLOCK = 0;		// Peripherial pin select is not locked
    //RECEPTOR
    RPINR18bits.U1RXR=0b01000; //remapea a rb8
    //TRANSMISOR
    RPOR3bits.RP7R=0b00011; //remapeo a rb7
    OSCCONbits.IOLOCK = 1;
}



