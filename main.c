/*
************************************************************************
 ECE 362 - Mini-Project C Source File - Fall 2014                   
***********************************************************************
                                                                                                                          
 Team ID: < ? >

 Project Name: <Basketball Shotclock>

 Team Members:

   - Team/Doc Leader: Adit Ghosh      Signature: ______________________
  
   - Software Leader: < ? >      Signature: ______________________

   - Interface Leader: < ? >     Signature: ______________________

   - Peripheral Leader: < ? >    Signature: ______________________


 Academic Honesty Statement:  In signing above, we hereby certify that we
 are the individuals who created this HC(S)12 source file and that we have
 not copied the work of any other student (past or present) while completing
 it. We understand that if we fail to honor this agreement, we will receive
 a grade of ZERO and be subject to possible disciplinary action.

***********************************************************************

 The objective of this Mini-Project is to .... < ? >


***********************************************************************

 List of project-specific success criteria (functionality that will be
 demonstrated):

 1.

 2.

 3.

 4.

 5.

***********************************************************************

  Date code started: < ? >

  Update history (add an entry every time a significant change is made):

  Date: < ? >  Name: < ? >   Update: < ? >

  Date: < ? >  Name: < ? >   Update: < ? >

  Date: < ? >  Name: < ? >   Update: < ? >


***********************************************************************
*/

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <mc9s12c32.h>

/* All functions after main should be initialized here */
char inchar(void);
void outchar(char x);
/*void send_i(char x);
void pmsglcd(char[]);
void send_byte(char x);
void print_c(char x);
void shiftout(char x);
void lcdwait(void);
void fdisp(void);
*/
void fdisp(void);
void shiftout(char x);
void lcdwait(void);
void send_byte(char x);
void send_i(char x);
void chgline(char x);
void print_c(char x);
void pmsglcd(char[]);

/* Variable declarations */

//  float time = 24.0;
  int time = 250;
  int seconds = 0;
  int flag = 0;
  int tenths = 0;
  int cten = 0;
  int runstp = 0;

  int leftpb = 0;                                                                                                      
  int rghtpb = 0;
  
  int prevlpb = 0;
  int prevrpb = 0;
  int convert = 0;
  int result = 0;
  int pot = 0;
  
  int lcv;
  int temp;
  int pbstate,random;
  //float prec = 0.1;
/* Special ASCII characters */
#define CR 0x0D                // ASCII return 
#define LF 0x0A                // ASCII new line 

/* LCD COMMUNICATION BIT MASKS (note - different than previous labs) */
#define RS 0x10                // RS pin mask (PTT[4])
#define RW 0x20                // R/W pin mask (PTT[5])
#define LCDCLK 0x40        // LCD EN/CLK pin mask (PTT[6])

/* LCD INSTRUCTION CHARACTERS */
#define LCDON 0x0F        // LCD initialization command
#define LCDCLR 0x01        // LCD clear display command
#define TWOLINE 0x38        // LCD 2-line enable command
#define CURMOV 0xFE        // LCD cursor move instruction
#define LINE1 = 0x80        // LCD line 1 cursor position
#define LINE2 = 0xC0        // LCD line 2 cursor position




                                    
/*                                    
***********************************************************************
 Initializations
***********************************************************************
*/

void  initializations(void) {

/* Set the PLL speed (bus clock = 24 MHz) */
  CLKSEL = CLKSEL & 0x80; //; disengage PLL from system
  PLLCTL = PLLCTL | 0x40; //; turn on PLL
  SYNR = 0x02;            //; set PLL multiplier
  REFDV = 0;              //; set PLL divider
  while (!(CRGFLG & 0x08)){  }
  CLKSEL = CLKSEL | 0x80; //; engage PLL

/* Disable watchdog timer (COPCTL register) */
  COPCTL = 0x40   ; //COP off; RTI and COP stopped in BDM-mode

/* Initialize asynchronous serial port (SCI) for 9600 baud, interrupts off initially */
  SCIBDH =  0x00; //set baud rate to 9600
  SCIBDL =  0x9C; //24,000,000 / 16 / 156 = 9600 (approx)  
  SCICR1 =  0x00; //$9C = 156
  SCICR2 =  0x0C; //initialize SCI for program-driven operation
  DDRB   =  0x10; //set PB4 for output mode
  PORTB  =  0x10; //assert DTR pin on COM port

/* Initialize peripherals */
DDRT = 0xBF;

            
/* Initialize interrupts */
TSCR1 = 0x80;
TSCR2 = 0x04;  //Imp line
TIE = 0x80;
TC7 = 15000;
TIOS = 0x80;    

RTICTL = 0x1F; //Used to be 50
CRGINT = 0x80;
ATDDIEN = 0xC0;

SPICR1_SPE = 1;
  SPICR1_MSTR = 1;
  SPICR1_CPOL = 1;
  SPIBR = 0x00;
     
  PTT_PTT1 = 0;
//LCD Initialized  
//PTT_PTT3 = 0;
//send_i(LCDON);
//send_i(TWOLINE);
//send_i(LCDCLR);
//lcdwait();

  DDRAD   = 0;
  ATDDIEN = 0xC0;

  ATDCTL2 = 0x80;
  ATDCTL3 = 0x10; //just for now later change to 10
  ATDCTL4 = 0x85;

PTT_PTT4 = 1;
  PTT_PTT3 = 0;
  send_i(LCDON);
  send_i(TWOLINE);
  send_i(LCDCLR);
  send_i(0x80);
  pmsglcd(" ECE 362 PROJECT");
  lcdwait();
  lcdwait();
  pmsglcd("Shot Clock") ;
  
 //PWM Initializations
 
 MODRR = 0x01;
 PWME =0x01;
 PWMPOL = 0x01;
 PWMCTL = 0x00;
 PWMPRCLK = 0x02;
 PWMCAE  =0x00;
 PWMPER0 = 0xFF;
 PWMDTY0 = 0x00;
 PWMCLK = 0x00;
 

}

                                                                                      
/*                                                                                      
***********************************************************************
Main
***********************************************************************
*/
void main(void) {
          DisableInterrupts
        initializations();                                                                              
        EnableInterrupts;

 for(;;){
  
/* < start of your main loop > */
ATDCTL5 = 0x10;
   
 	  while(ATDSTAT0_SCF !=1){
 	}
 	
 	  convert = ATDDR0H;
 	  pot = ATDDR1H;
 	  result = convert*pot/255;
 	  if(time > 5) {
 	   
 	    PWMDTY0 = result;
 	  }
 	  if(time <= 5) {
 	   PWMDTY0 = 0;
 	  } 
 	  
 
   if( leftpb == 1 ){
      time = 250;
      fdisp();
      leftpb = 0;
      runstp = 1;
      
    }
    if( rghtpb == 1 ){
      time = 250;
      runstp = 0;
      rghtpb = 0;
    } 

    if(flag == 1 && runstp == 1) {
      flag = 0;
      if(time > 100) {
        time = time - 10;
        fdisp();
      }
    }
//Code changed after here    
    if( tenths == 1 && runstp == 1){
      tenths=0;
      if( time <= 100){
        time = time - 1;
        fdisp();
      }
      if(time <= 0) {
        PTT_PTT1 = 1;
        time = 0;
        fdisp();
        //PWMDTY0 = 0;
        
        runstp = 0;
       }
    }
  
        if (PTT_PTT6 != 0){
          runstp = 1;
          time = 250;
          PTT_PTT1 = 0; //new line for buzzer stop
        }
  
   } /* loop forever */
  
}   /* do not leave main */




/*
***********************************************************************                      
 RTI interrupt service routine: RTI_ISR
************************************************************************
*/

interrupt 7 void RTI_ISR(void)
{
          // clear RTI interrupt flagt
          CRGFLG = CRGFLG | 0x80;
          if( PORTAD0_PTAD7 == 0){
            if(prevlpb == 1){
            leftpb = 1;
            }
          }
          
          if( PORTAD0_PTAD6 == 0){
            if(prevrpb == 1){
            rghtpb = 1;
            }
          }
          
          prevlpb = PORTAD0_PTAD7;
          prevrpb = PORTAD0_PTAD6; 

    
}

/*
***********************************************************************                      
  TIM interrupt service routine                          
***********************************************************************
*/

interrupt 15 void TIM_ISR(void)
{
          // clear TIM CH 7 interrupt flag
         TFLG1 = TFLG1 | 0x80;
         seconds++;
         if(seconds == 23) {
           flag = 1;
           seconds = 0;
         }
         
         /*if(seconds > 100) {
           
           seconds = 0;
         }        */
         
         cten++;
         if( cten == 3){
          tenths = 1;
          cten=0;
         }

         /*if( cten > 10 ){
          tenths = 0; 
         } */
         
         
        
}

/*
***********************************************************************                      
  SCI interrupt service routine                                                   
***********************************************************************
*/

interrupt 20 void SCI_ISR(void)
{
 


}
/*
***********************************************************************
 Character I/O Library Routines for 9S12C32
***********************************************************************
 Name:         inchar
 Description:  inputs ASCII character from SCI serial port and returns it
 Example:      char ch1 = inchar();
***********************************************************************
*/

char inchar(void) {
  /* receives character from the terminal channel */
        while (!(SCISR1 & 0x20)); /* wait for input */
    return SCIDRL;
}

/*
***********************************************************************
 Name:         outchar    (use only for DEBUGGING purposes)
 Description:  outputs ASCII character x to SCI serial port
 Example:      outchar('x');
***********************************************************************
*/

void outchar(char x) {
  /* sends a character to the terminal channel */
    while (!(SCISR1 & 0x80));  /* wait for output buffer empty */
    SCIDRL = x;
}

/*
***********************************************************************
  shiftout: Transmits the character x to external shift 
            register using the SPI.  It should shift MSB first.  
             
            MISO = PM[4]
            SCK  = PM[5]
***********************************************************************
*/
 
void shiftout(char x)
{
     
  while(SPISR_SPTEF == 0){
  }
  SPIDR = x;
  lcdwait();
 

}

/*
***********************************************************************
  lcdwait: Delay for approx 2 ms
***********************************************************************
*/

void lcdwait()
{
  /*temp = 0;                     
  while(temp++ < 48000){
   asm{
    nop
   }
    
  } */
  int cycle = 0;
  
  while(cycle<4800) {
    cycle++;
  }
    
}

/*
*********************************************************************** 
  send_byte: writes character x to the LCD
***********************************************************************
*/

void send_byte(char x)
{
     // shift out character
     // pulse LCD clock line low->high->low
     // wait 2 ms for LCD to process data     
    shiftout(x);
    PTT_PTT4 = 0;
    PTT_PTT4 = 1;
    PTT_PTT4 = 0;
    lcdwait();
     
}

/*
***********************************************************************
  send_i: Sends instruction byte x to LCD  
***********************************************************************
*/

void send_i(char x)
{
        // set the register select line low (instruction data)
        // send byte
    PTT_PTT2 = 0;
    send_byte(x);
   
}

/*
***********************************************************************
  chgline: Move LCD cursor to position x
  NOTE: Cursor positions are encoded in the LINE1/LINE2 variables
***********************************************************************
*/

void chgline(char x)
{
  send_i(CURMOV);
  send_i(x);

}

/*
***********************************************************************
  print_c: Print (single) character x on LCD            
***********************************************************************
*/
 
void print_c(char x)
{
   PTT_PTT2 = 1;
   send_byte(x);
}

/*
***********************************************************************
  pmsglcd: print character string str[] on LCD
***********************************************************************
*/

void pmsglcd(char str[])
{
   int index = 0;
   while(index < 16 && str[index] != '\0'){
    print_c(str[index++]);
   }

}

/* print_c((react/100)%10 + 0x30);
   print_c((react/10)%10 + 0x30);
   print_c((react)%10 + 0x30);*/
          

void fdisp(){
send_i(LCDCLR);
chgline(0x80);
  pmsglcd("Shot Clock") ;
  chgline(0xC0);
  pmsglcd("Time= ");
  if(time>=100){
    
    //((time/100)+ 0x30);
    //send_i(((time%100)/10)+0x30);

   
   print_c((time/100)%10 + 0x30);
   
   print_c((time/10)%10 + 0x30);


  }
  if(time < 100){
    //send_i((time/10)+ 0x30);
    //pmsglcd(".");
    //send_i((time%10)+0x30);
   
  
   print_c((time/10)%10 + 0x30);
    pmsglcd(".");
   print_c((time)%10 + 0x30);  
    
    }
    
}
