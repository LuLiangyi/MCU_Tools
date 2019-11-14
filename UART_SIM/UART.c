#include "UART.h"

void UART_SIM_STAT(UART_Tran *T,UART_Recv *R)
{
	if(!(UART_ISR&TI_Bit) && UART_SCON&TIEN) UART_ISR |= TI_Bit; 
	if(UART_ISR&TI_Byte) 
	{
		UART_ISR &= ~TI_Byte;
		if(SpTran<12)
		{
			T->SBUF = DatTran[++SpTran];
			T->BitSndBuf = ((uint16_t)(T->SBUF << 1)) | 0x0200;  //0x0200  1 stop bit
		}
		else UART_SCON &= ~TIEN;
	}
	
	if(!(UART_ISR&RI_Bit))  //RXD
	{
		if(!(UART_RXD_Port&UART_RXD_Pin) && R->lastLev) UART_ISR |= RI_Bit; 
	}
	R->lastLev = UART_RXD_Port&UART_RXD_Pin;
	
	if(UART_ISR&RI_Byte)
	{
		UART_ISR &= ~RI_Byte;
		if(SpRecv<12) DatRecv[SpRecv++] = R->SBUF;
	}
	
}


void UART_SIM_SendByte(UART_Tran *T)
{
	if(UART_ISR&TI_Bit)  
	{
		T->BitCnt++;		
		
		if(T->BitSndBuf&0x01) UART_TXD_Port |= UART_TXD_Pin;  //set 1
		else UART_TXD_Port &= ~UART_TXD_Pin; //set 0
		
		if(T->BitCnt >= UART_BitCnt) 
		{
			T->BitCnt = 0;  
			T->BitNum++;	
			T->BitSndBuf >>= 1; 
		}
		
		if(T->BitNum >= UART_BitNum) 
		{
			T->BitNum = 0; 
			UART_ISR &= ~TI_Bit; 
			UART_ISR |= TI_Byte;
		}
	}
	else UART_TXD_Port |= UART_TXD_Pin;
}


void UART_SIM_RecvByte(UART_Recv *R)
{
	if(UART_ISR&RI_Bit)  
	{
		R->BitCnt++; 
		R->sampCnt++;
		
		if(R->sampCnt >UART_BitCnt/4) 
		{
			R->sampCnt = 0;
			if(UART_RXD_Port&UART_RXD_Pin) 
				R->sample ++;
		}
		
		if(R->BitCnt >= UART_BitCnt)  
		{
				R->BitCnt = 0;  
				R->sampCnt = 0;
				if(R->sample > 1) R->BitRcvBuf |= (1<<R->BitNum);
				R->sample = 0;
				R->BitNum++; 
		}
		
		if(R->BitNum >= UART_BitNum)
		{
			R->BitNum = 0; 
			UART_ISR &= ~RI_Bit; 
			R->SBUF = R->BitRcvBuf >> 1;
			R->BitRcvBuf = 0;
			UART_ISR |= RI_Byte;
		}
	}
}
