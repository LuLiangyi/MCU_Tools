#ifndef __UART_H__
#define __UART_H__

typedef unsigned          char uint8_t;
typedef unsigned short     int uint16_t;

typedef struct{
	uint8_t BitNum;
	uint8_t BitCnt;
	uint16_t BitSndBuf;
	uint8_t SBUF;
}UART_Tran;

typedef struct{
	uint8_t BitNum;
	uint8_t BitCnt;
	uint16_t BitRcvBuf;
	uint16_t SBUF;
	uint8_t lastLev;
	uint8_t sampCnt;
	uint8_t sample;  
}UART_Recv;

uint8_t UART_SCON;
#define RIEN (1<<0)
#define TIEN (1<<1)

uint8_t UART_ISR;
#define RI_Bit (1<<0)
#define TI_Bit (1<<1)
#define RI_Byte (1<<2)
#define TI_Byte (1<<3)

#define UART_BitNum   10
#define UART_BitCnt	  13 //7 9600//52 1200//13 4800

#define UART_TXD_Port M0P_GPIO->P3OUT
#define UART_RXD_Port M0P_GPIO->P3IN
#define UART_TXD_Pin  (1<<5)
#define UART_RXD_Pin  (1<<6)

void UART_SIM_STAT(UART_Tran *T,UART_Recv *R);
void UART_SIM_SendByte(UART_Tran *T);
void UART_SIM_RecvByte(UART_Recv *R);

#endif