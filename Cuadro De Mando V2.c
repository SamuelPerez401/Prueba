// PIC16F628A Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = INTOSCIO  // Oscillator Selection bits (INTOSC oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is digital input, MCLR internally tied to VDD)
#pragma config BOREN = OFF      // Brown-out Detect Enable bit (BOD disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (RB4/PGM pin has digital I/O function, HV on MCLR must be used for programming)
#pragma config CPD = ON        // Data EE Memory Code Protection bit (Data memory code protection off)
#pragma config CP = ON         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#define _XTAL_FREQ 4000000
#define AP PORTAbits.RA7
#define CL PORTAbits.RA0
#define LP PORTAbits.RA1
#define LFT PORTAbits.RA6
#define SW PORTAbits.RA4
#define DIP1 PORTBbits.RB1
#define DIP2 PORTBbits.RB2
#define FCA PORTBbits.RB3
#define PB PORTBbits.RB4
#define FT PORTBbits.RB5
#define FCC PORTBbits.RB6
yguaytuagfiu

void main(void) {
    
////////////////////////////////////////////////////////////////////////////////    
    
    TRISA = 0b00111100;
    TRISB = 0b01111110;     // puertos
    PORTA = 0b00000000;
    
////////////////////////////////////////////////////////////////////////////////
    
   int EAP = 0;    // variable de estado apertura
   int EP = 0;    //variable de estado pausa
   int ECL = 0;   // variable estado cierre
   int ED1 = 0;   // variable estado dip switch 1
   int ED2 = 0;   // variable estado dip switch 2
   int ETCA = 0;  // variable estado cierre automatico
   int TIMER = 0;  // variable para activar el conteo por timer0
   int EB = 0;     // variable estado boton
   int B = 0;      // variable boton
    
////////////////////////////////////////////////////////////////////////////////
   
    int TCA = 0;
    
    TMR0 = 6;           // config timer 0
    T0IE = 0;
    T0IF = 0;
    OPTION_REG = 0b00000001;  

////////////////////////////////////////////////////////////////////////////////   

    while(1){
        
////////////////////////////////////////////////////////////////////////////////

            
              
            if (PB==0 && EB == 0)         // Primera vez que se pulso el PB
            {                            //  
		 	  B = 1;
              EB = 1;
         }
            
       
     else          // PB esta presionado ahora mismo
            
         if(PB == 1)
          	{
			      EB = 0;
         	
				
			}	
			
			
		
////////////////////////////////////////////////////////////////////////////////
        
        if(ETCA){
            
            if(T0IF){
                
            T0IF = 0;       // cierre automatico
            TMR0 = 6;
            TCA ++;
        }
    }
            
            if(FT == 1){
                         // reinicio el cierre cuando ay alguien en la fotocelda
                TCA = 0;
            }
                
            
////////////////////////////////////////////////////////////////////////////////            
            
            if(EAP == 0 && ECL == 0 && FCA == 0 && TIMER == 0 && EP == 0 && (SW == 0 || B == 1)){
                
                AP = 1;
                B = 0;
                EAP = 1;            // etapa apertura
                __delay_ms(200);
            }
            
            if(EAP == 1 && (SW == 0 || B == 1)){
                
                AP = 0;
                B = 0;
                EAP = 0;            // etapa pausa cuando esta abriendo
                EP = 1;
                __delay_ms(200);
            }
            
            if(EP == 1 && FT == 1 && (SW == 0 || B == 1)){
                
                AP = 1;
                B = 0;        // si esta parada y alguien en el medio no cierra solo abre
                EAP = 1;
                __delay_ms(200);
            }
            
            if(FT == 0 && EP == 1 && (SW == 0 || B == 1)){
                
                CL = 1;
                B = 0;
                EP = 0;       // si esta parada y nadie en el medio cierra
                ECL = 1;
                __delay_ms(200);
            }
           
            if(ECL == 1 && (SW == 0 || B == 1)){
                
                CL = 0;
                B = 0;
                __delay_ms(1250);   // si esta cerrando y pulsan el switch devuelve
                AP = 1;
                EAP = 1;
                ECL = 0;
                __delay_ms(200);
            }
            
            if(TIMER && (SW == 0 || B == 1)){
                
                CL = 1;
                B = 0;
                __delay_ms(200);
                ECL = 1;         // aun este esperando el cierre automatico la puedo cerrar con el boton
                TCA = 0;
                ETCA = 0;
                TIMER = 0;
            }
            
////////////////////////////////////////////////////////////////////////////////

            if(EAP == 1 && FCA == 1){
                
                AP = 0;
                EAP = 0;   // si esta abriendo y se abre el final de carrera se apaga el apertura
                TIMER = 1;
            }
            
////////////////////////////////////////////////////////////////////////////////

            if(ECL == 1 && FCC == 1){
                
                CL = 0;        // si esta cerrando y se abre final de carrera se apaga el cierre
                ECL = 0;
            }
            
////////////////////////////////////////////////////////////////////////////////

            if(ECL == 1 && FT == 1){
                
                CL = 0;
                ECL = 0;    // si esta cerrando y se entran en la fotocelda se devuelve
                __delay_ms(1250);
                AP = 1;
                EAP = 1;
            }
            
////////////////////////////////////////////////////////////////////////////////

        if(DIP1 == 0){
            
            ED1 = 1;
        }
        
        else 
            
            ED1 = 0;      // etapa para ver los dip switch
        
        if(DIP2 == 0){
            
            ED2 = 1;
        }
        
        else
            
            ED2 = 0;
        
////////////////////////////////////////////////////////////////////////////////

        if(ED1 == 1 && ED2 == 0 && TIMER == 1){
            
            ETCA = 1;
            
            if(TCA == 8000){
                                // primer cierre automatico
                CL = 1;
                ECL = 1;
                TCA = 0;
                ETCA = 0;
                TIMER = 0;
            }
        }
        
        
         

         
////////////////////////////////////////////////////////////////////////////////

        if(ED1 == 1 && ED2 == 1 && TIMER == 1){
            
            ETCA = 1;
            
            if(TCA == 18000){
                
                CL = 1;         // segundo cierre automatico
                EP = 0;
                ECL = 1;
                TCA = 0;
                ETCA = 0;
                TIMER = 0;
            }
        }

////////////////////////////////////////////////////////////////////////////////

        if(ED1 == 0 && ED2 == 1 && TIMER == 1){
            
            ETCA = 1;
            
            if(TCA == 30000){
                
                CL = 1;       // tercer cierre automatico
                EP = 0;
                ECL = 1;
                TCA = 0;
                ETCA = 0;
                TIMER = 0;
            }
        }

////////////////////////////////////////////////////////////////////////////////            
           
    }
    
    return;
}
