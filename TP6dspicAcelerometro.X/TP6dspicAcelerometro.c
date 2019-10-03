/* 
 * File:   TP6dspicAcelerometro.c
 * Author: santi
 *
 * Created on 1 de octubre de 2019, 16:14
 */

#include <stdio.h>
#include <stdlib.h>
#include <p33fj128MC202.h>
#define FCY 40000000UL//
#include <libpic30.h>
_FICD(ICS_PGD2 & JTAGEN_OFF); // Para hacer debuging por el puerto 2
_FOSCSEL(FNOSC_FRC);
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT);
_FWDT(FWDTEN_OFF);
#pragma config ALTI2C = ON              // Alternate I2C  pins (I2C mapped to ASDA1/ASCL1 pins)
//defino funciones
void configini (void);
void configUARTPC (void);
void configI2C (void);
void aEnviarI2C (unsigned char auxiliar);
//defino variables 
unsigned char vEstado=0;

//defino Estados
#define ESPERANDOINSTRUCCION 0
#define INST1 0x41
#define INST2 0x42
#define INST3 0x43
#define INST4 0x44
#define INST5 0x45
#define INST6 0x46
#define INST7 0x47
#define INST8 0x48 
#define INST9 0x49
#define INST10 0x4A
//funcion interrupcion 
void __attribute__((__interrupt)) _U1RXInterrupt(void){
    
}
//codigo principal (DIAGRAMA DE ESTADOS)
void main(void){
    while(1){
        switch (vEstado){
                case ESPERANDOINSTRUCCION:
                    
                    break;
                    
                case INST1:
                        
                    break;
                case INST2:
                        
                    break;
                case INST3:
                            
                    break;
                case INST4:
                                
                    break;
                                
                case INST5:
                                    
                    break;
                                    
                case INST6:
                                        
                    break;
                case INST7:
                                            
                    break;
                                            
                case INST8:
                                                
                    break;
                case INST9:
                                                    
                    break;
                                                    
                case INST10:
                                                        
                    break;
        }
    }
}
//funciones
void configini (void){
    IEC0bits.U1RXIE=1;
}
void aEnviarI2C (unsigned char auxiliar){
    if (I2C1STATbits.TRSTAT==0){
        I2C1TRN=auxiliar;
        while(I2C1STATbits.TBF==1){}
        while (I2C1STATbits.TRSTAT==1){}   
    }
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
    U1BRG = BRGVAL; // Baud Rate setting for 19200
    U1STAbits.UTXISEL0 = 0; // Interrupt after one TX character is transmitted
    U1STAbits.UTXISEL1 = 0;
    IEC0bits.U1TXIE = 0; // disnable UART TX interrupt
    U1MODEbits.UARTEN = 1; // Enable UART
    U1STAbits.UTXEN = 1; // Enable UART TX
    U1STAbits.URXISEL0=0; 
    U1STAbits.URXISEL1=1;//INTERRUMPE DESPUES DE 3 RECEPCIONES
}