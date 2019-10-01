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
#include <libpic30.h>
_FICD(ICS_PGD2 & JTAGEN_OFF); // Para hacer debuging por el puerto 2
_FOSCSEL(FNOSC_FRC);
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT);
_FWDT(FWDTEN_OFF);
#pragma config ALTI2C = ON              // Alternate I2C  pins (I2C mapped to ASDA1/ASCL1 pins)
//defino funciones
void paso (void);
void ConfigIni(void);
void ConfigI2C(void);
void ConfigUart(void);
void EnvioUart (void);
void EnviarI2C (unsigned char auxiliar );
//defino variables 
unsigned char aux;
//funcion main
void main(void){
    ConfigIni();
    ConfigI2C();
    ConfigUart();
    U1TXREG = 'I'; // inicio de comunicacion
    while(IFS0bits.U1TXIF==0){}
    IFS0bits.U1TXIF=0;
    U1TXREG = 0x0D;//manda un enter
    I2C1CONbits.SEN=1;
    while (I2C1CONbits.SEN==1){}
    EnviarI2C(0x38);
    paso();
    while (I2C1STATbits.ACKSTAT==1){}
    paso();
    EnviarI2C(0x0D);
    while (I2C1STATbits.ACKSTAT==1){}
    paso();
    I2C1CONbits.RSEN=1;
    while (I2C1CONbits.RSEN==1){}
    paso();
    EnviarI2C(0x39);
    while (I2C1STATbits.ACKSTAT==1){}
     I2C1CONbits.RCEN=1;
    while (I2C1CONbits.RCEN==1){}
    paso();
    while(I2C1STATbits.RBF==0){}
    paso();
    aux=I2C1RCV;
    I2C1CONbits.ACKDT=1;
    I2C1CONbits.ACKEN=1;
    while(I2C1CONbits.ACKEN==1){}
    paso();
    I2C1CONbits.PEN=1;
    if (aux==0x1A){
                U1TXREG = '1'; // 1 si es el id
                while(IFS0bits.U1TXIF==0){}
                IFS0bits.U1TXIF=0;
                U1TXREG = 0x0D;//manda un enter
                while(IFS0bits.U1TXIF==0){}
                IFS0bits.U1TXIF=0;
                }
    else{
        U1TXREG= '0'; //0 si no es el id
        while(IFS0bits.U1TXIF==0){}
        IFS0bits.U1TXIF=0;
        U1TXREG = 0x0D;//manda un enter 
        while(IFS0bits.U1TXIF==0){}
                IFS0bits.U1TXIF=0;
        }
    I2C1CONbits.ACKDT=0;
    while(1){}
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
    
    
    
    //REMAPEO DE PINES 
    OSCCON = 0x46;			// Command Sequence- Registro de control del oscilador
    OSCCON = 0x57;
    OSCCONbits.IOLOCK = 0;		// Peripherial pin select is not locked
    //RECEPTOR
    RPINR18bits.U1RXR=0b01000; //remapea a rb8 rx del micro
    //TRANSMISOR
    RPOR3bits.RP7R=0b00011; //remapeo a rb7 tx del micro
    OSCCONbits.IOLOCK = 1;
}
void ConfigI2C(void){
//configuro I2C
    I2C1BRG=0x5D;
    I2C1CONbits.I2CEN=1;
    I2C1CONbits.DISSLW=1;
    IFS1bits.SI2C1IF = 0;
}
void ConfigUart(void){
//config uart
    #define FP 40000000
    #define BAUDRATE 19200
    #define BRGVAL ((FP/BAUDRATE)/16)-1
    U1MODEbits.STSEL = 0; // 1-Stop bit
    U1MODEbits.PDSEL = 0; // No Parity, 8-Data bits
    U1MODEbits.ABAUD = 0; // Auto-Baud disabled
    U1MODEbits.BRGH = 0; // Standard-Speed mode
    U1BRG = 129; // Baud Rate setting for 19200
    U1STAbits.UTXISEL0 = 0; // Interrupt after one TX character is transmitted
    U1STAbits.UTXISEL1 = 0;
    IEC0bits.U1TXIE = 0; // Enable UART TX interrupt
    U1MODEbits.UARTEN = 1; // Enable UART
    U1STAbits.UTXEN = 1; // Enable UART TX
}
void EnvioUart(void){
}
void EnviarI2C (unsigned char auxiliar){
    if (I2C1STATbits.TRSTAT==0){
        I2C1TRN=auxiliar;
        while(I2C1STATbits.TBF==1){}
        while (I2C1STATbits.TRSTAT==1){}   
    }
}
void paso (void){
    U1TXREG = 'a';
    while(IFS0bits.U1TXIF==0){}
                IFS0bits.U1TXIF=0;
                U1TXREG = 0x0D;//manda un enter
                while(IFS0bits.U1TXIF==0){}
                IFS0bits.U1TXIF=0;
}