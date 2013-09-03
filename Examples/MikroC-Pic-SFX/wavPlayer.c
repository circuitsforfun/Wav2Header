
#include <built_in.h>
#include "meow.h"
#include "dog.h"

unsigned int x1;
unsigned int wavSel = 0;
int xStep;
//unsigned long tmrOffset = 64095; // 90us Timer Delay - High Qaulity Sample
unsigned long tmrOffset = 62655; // 180us Timer Delay - Med Quality Sample
//unsigned long tmrOffset = 61215; // 270us Timer Delay - Low Quality Sample

void InitTimer1(){
  T1CON         = 0x00;
  TMR1IF_bit         = 0;
  TMR1H         = Hi(tmrOffset);
  TMR1L         = Lo(tmrOffset);
  TMR1IE_bit         = 1;
  INTCON         = 0xC0;
}

void Interrupt(){
  unsigned int vcalc;
  unsigned long wts;
  unsigned short encVal = 0;
  unsigned char cs = 50;
  
  if (TMR1IF_bit)  // Timer1 Code
  {
      TMR1IF_bit = 0;
      TMR1H         = Hi(tmrOffset);
      TMR1L         = Lo(tmrOffset);
      if (wavSel == 0)
      {
         wts = (unsigned long)sizeof( envelope_table );
         vcalc = (envelope_table[x1]);
      }
      else if (wavSel == 1)
      {
         wts = (unsigned long)sizeof( envelope_table2 );
         vcalc = (envelope_table2[x1]);
      }
      x1+=xStep;
      if(x1 == wts)
      {
        xStep=1;
        x1 = 0;
        T1CON         = 0x00;
      }
      PWM1_Set_Duty( vcalc );
  }
}

void main() {
    char p1;
    unsigned int Duty;
    char tmp;

    OSCCON = 0b01111100;
    OSCCON2 = 0b10000111;
    OSCTUNE = 0b01000000;
    ADCON0 = 0;
    CCP1CON = 0;
    CCP2CON = 0;
    CCP3CON = 0;
    PWM1_Init( 44000 );
    PWM1_Start();
    x1 = 0;
    xStep = 1;
    TRISB4_bit = 1;
    TRISB5_bit = 1;
    InitTimer1();
    while(1)
    {
            if( PORTB.B4 == 1) // Play Cat Meow
            {
                x1 = 0;
                wavSel = 0;
                T1CON         = 0x01;
                Delay_ms(250);
            }
            if( PORTB.B5 == 1) // Play Dog Bark
            {
                x1 = 0;
                wavSel = 1;
                T1CON         = 0x01;
                Delay_ms(250);
            }
    }
}