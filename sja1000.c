#include<stdio.h>

/* definition for direct access to 8051 memory areas */
#define XBYTE ((unsigned char volatile xdata *) 0)

/* address and bit definitions for the Mode & Control Register */
#define ModeControlReg XBYTE[0]
#define RM_RR_Bit 0x01 /* reset mode (request) bit */
#define RM_NR_Bit 0xFE /* Normal mode (request) bit */

#define InterruptEnReg XBYTE[0] /* Control Register */
#define RIE_Bit 0x02 /* Receive Interrupt enable bit */
#define TIE_Bit 0x04 /* Transmit Interrupt enable bit */
#define EIE_Bit 0x08 /* Error Interrupt enable bit */
#define DOIE_Bit 0x10 /* Overrun Interrupt enable bit */

/* address and bit definitions for the Command Register */
#define CommandReg XBYTE[1]
#define TR_Bit 0x01 /* transmission request bit */
#define AT_Bit 0x02 /* abort transmission bit */
#define RRB_Bit 0x04 /* release receive buffer bit */
#define CDO_Bit 0x08 /* clear data overrun bit */

/* address and bit definitions for the Status Register */
#define StatusReg XBYTE[2]
#define RBS_Bit 0x01 /* receive buffer status bit */
#define TBS_Bit 0x04 /* transmit buffer status bit */
#define TCS_Bit 0x08 /* transmission complete status bit */


/* address and bit definitions for the Interrupt Register */
#define InterruptReg XBYTE[3]
#define RI_Bit 0x01 /* receive interrupt bit */
#define TI_Bit 0x02 /* transmit interrupt bit */

/* address and bit definitions for the Bus Timing Registers */
#define BusTiming0Reg XBYTE[6]
#define BusTiming1Reg XBYTE[7]

/* address definitions of Acceptance Code & Mask Registers */
#define AcceptCodeReg XBYTE[4]
#define AcceptMaskReg XBYTE[5]

* address definitions for Rx-Buffer */
#define RxBuffer1 XBYTE[20]
#define RxBuffer2 XBYTE[21]
#define RxBuffer3 XBYTE[22]
#define RxBuffer4 XBYTE[23]
#define RxBuffer5 XBYTE[24]
#define RxBuffer6 XBYTE[25]
#define RxBuffer7 XBYTE[26]
#define RxBuffer8 XBYTE[27]
#define RxBuffer9 XBYTE[28]
#define RxBuffer10 XBYTE[29]

/* address definitions for Tx-Buffer */
#define TxBuffer1 XBYTE[10]
#define TxBuffer2 XBYTE[11]
#define TxBuffer3 XBYTE[12]
#define TxBuffer4 XBYTE[13]
#define TxBuffer5 XBYTE[14]
#define TxBuffer6 XBYTE[15]
#define TxBuffer7 XBYTE[16]
#define TxBuffer8 XBYTE[17]
#define TxBuffer9 XBYTE[18]
#define TxBuffer10 XBYTE[19]







bool Config() { /*SJA1000 configuration function to be completed…*/
flag=FALSE;
/* Select the RESET mode*/
/*
ControlReg : | * | * | * |OIE_Bit|EIE_Bit|TIE_Bit|RIE_Bit|RR_Bit| ; RM_RR_Bit=0x01
*/

ModeControlReg = RM_RR_Bit | RM_NR_Bit ;
InterruptEnReg = (DOIE_Bit|EIE_Bit|TIE_Bit|RIE_Bit);
CommandReg = (CDO_Bit|RRB_Bit|AT_Bit|TR_Bit);
StatusReg = (TCS_Bit|TBS_Bit|RBS_Bit);
InterruptReg = (TI_Bit|RI_Bit);

while ( ( ModeControlReg=RM_RR_Bit )!= RM_RR_Bit );
/* all interupts are then disabled */
OIE_Bit=0;
EIE_Bit=0;
TIE_Bit=0;
RIE_Bit=0;	
/* configuration for acceptance code and mask registers */
AcceptCodeReg=0x00;
AcceptMaskReg=0xF0;
/* configuration of bus timing */
/* bus timing values for
- bit-rate : 125 kBit/s
- oscillator frequency : 16 MHz */
/* baud rate prescaler : 3 */
/* SJW : 1 */
/* TSEG1 : 11 */
/* TSEG2 : 4 */
/* SPL:1  the bus is sampled 3 times*/

#define btr0 0x03
#define btr1 0xBB 

/* Normal mode request */
while( ( ModeControlReg &= RM_NR_Bit) & 0x01 );
	return flag;
}


/* reception function*/

void can_getd(int id, int data)
{

	if(RIE_Bit== 1)/* test can receive interrupt */
		{
		if(RBS_Bit== 1)/* receive statut = full */
			{
			/* stock received data frame*/

			/* RxBuffer1 = id(bit3 to bit10)*/
			/* RxBuffer2 = id(bit0 to bit2, RTR, and DLC*/
			id= (RxBuffer1<<3 ) | ((RxBuffer2 & 0xE0)>>5);/* data frame id 11 bits */
			data[0]= RxBuffer3; /* the first byte */
			data[1]= RxBuffer4;
			data[2]= RxBuffer5;
			data[3]= RxBuffer6;
			data[4]= RxBuffer7;
			data[5]= RxBuffer8;
			data[6]= RxBuffer9;
 			data[7]= RxBuffer10;

			RRB_Bit== 1;/*realise receive buffer; clear bytes*/
	
			}
		}
	
}
 /* transmission function */
void can_putd(int id1, int id2, int RTR, int data)

{
	
	if(StatusReg == TBS_Bit) /* or test transsmit status bit */
		{ 
			if (RTR==0) /* transsmit data frame */
			{
				
				TxBuffer1 = id1;
				TxBuffer2 = id2;
				TxBuffer3 = data[0]; /* the first byte */
				TxBuffer4 = data[1];
				TxBuffer5 = data[2];
				TxBuffer6 = data[3];
				TxBuffer7 = data[4];
				TxBuffer8 = data[5];
				TxBuffer9 = data[6];
 				TxBuffer10 = data[7];
			}

			else 
				TxBuffer1 = id1;
				TxBuffer2 = id2;
				
		}
/* Start the transmission */
TR_Bit== 1 ; /* Set Transmission Request bit */
if(StatusReg == TCS_Bit) /* transsmission complete */
StatusReg = 0x00; /* disable flags transmission */					
}