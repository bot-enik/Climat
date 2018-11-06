/*===========================================================================*\
|* Project:     SEGMENT                                                      *|
|* Module:      uart_deb.с                                                   *|
|* Description: драйвер UART для отладки и логов                             *|
|* Copyright:   Informtekhnika, 2016                                         *|
|* Author:      Ramil Kurbanov                                               *|
|* Date:        29.03.16                                                     *|
\*===========================================================================*/
#include "misc.h"
#include "gpio.h"
#include <stdint.h>
#include "stm32l1xx_rcc.h"
#include "uart_deb.h"

#define USART_DEB     USART1

#define         N_BUF_DEB_RX        1024     // размер буфера для приема  
#define         N_BUF_DEB_TX        1024     // размер буфера для передачи  

// структура для организации обмена с тех портом
typedef  struct  tagT_EXCH_DEB_STRC
  {  
     int16_t 	i_rx_wr;        //индексы приемного буфера
     int16_t      i_rx_rd;	//индексы приемного буфера
     int16_t 	i_tx_wr;        //индексы буфера передачи
     int16_t      i_tx_rd;	//индексы буфера передачи
     int8_t	rx_buf[N_BUF_DEB_RX];	//буфер FIFO для приема
     int8_t	tx_buf[N_BUF_DEB_TX];	//буфер FIFO для передачи     
  }  T_EXCH_DEB_STRC;

T_EXCH_DEB_STRC  exch_deb;

// Инициализация технологического UART
void uart_deb_init(void)
{
    GPIO_InitTypeDef             gpio_init_strc;      
    USART_InitTypeDef            deb_usart_strc;  
    NVIC_InitTypeDef             nvic_init_strc;

    // инициализация указателей передающего и приемного буферов
    exch_deb.i_tx_wr = exch_deb.i_tx_rd = 0;	    
    exch_deb.i_rx_wr = exch_deb.i_rx_rd = 0;
        
    // Tx и Rx pins as alternate USART functions
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);      
 
    
    // Configure USART Tx as alternate function push-pull 
    // Configure USART Rx as alternate function push-pull     
    gpio_init_strc.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10 ;
    gpio_init_strc.GPIO_Mode = GPIO_Mode_AF;
    gpio_init_strc.GPIO_Speed = GPIO_Speed_40MHz;
    gpio_init_strc.GPIO_OType = GPIO_OType_PP;
//    gpio_init_strc.GPIO_PuPd = GPIO_PuPd_NOPULL;
    gpio_init_strc.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &gpio_init_strc);      
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); // выдача CLOCLK на модуль USART
    USART_DeInit(USART_DEB); // сброс USART
    
    // USART configuration 
    deb_usart_strc.USART_BaudRate = 115200;   //Кбит/сек
    deb_usart_strc.USART_WordLength = USART_WordLength_8b;// 8 бит данных 
    deb_usart_strc.USART_StopBits = USART_StopBits_1; // 1 стоповй бит
    deb_usart_strc.USART_Parity = USART_Parity_No;   // контроль нечетности выключен
    deb_usart_strc.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // управление потоком выключено
    deb_usart_strc.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;       // прием и передача включены 
    USART_Init(USART_DEB, &deb_usart_strc);

    // Enable the usartx Interrupt 
    nvic_init_strc.NVIC_IRQChannel = USART1_IRQn;  //USART1 global Interrupt
    nvic_init_strc.NVIC_IRQChannelPreemptionPriority = 0;
    nvic_init_strc.NVIC_IRQChannelSubPriority = 0;
    nvic_init_strc.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic_init_strc);
    
    // Enable USART 
    USART_Cmd(USART_DEB, ENABLE);      
    //Разрешение прерывний RX  
    USART_ITConfig(USART_DEB, USART_IT_RXNE, ENABLE);
      // Global IT enable
  //  NVIC_EnableIRQ(USART1_IRQn);
    
    return;
}

// выключение технологического UART
void uart_deb_disable(void)      
{
    NVIC_InitTypeDef             nvic_init_strc;
   // Запрет прерываний 
    nvic_init_strc.NVIC_IRQChannel = USART1_IRQn;  //USART1 global Interrupt
    nvic_init_strc.NVIC_IRQChannelPreemptionPriority = 0;
    nvic_init_strc.NVIC_IRQChannelSubPriority = 0;
    nvic_init_strc.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&nvic_init_strc);  
    USART_Cmd(USART_DEB, DISABLE);        // Disable USART   
    USART_DeInit(USART_DEB);              // Reset USART        
    RCC_APB1PeriphClockCmd(RCC_APB2Periph_USART1, DISABLE); // снятие CLOCLK с модуля USART    
    return;	
}

// обработчик прерываний UART:  TX, RX
void USART1_IRQHandler(void)
{    
    if(USART_GetFlagStatus(USART_DEB, USART_FLAG_RXNE)!= RESET)
    {   // "Read data register not empty" -  принят байт 
        int32_t i_rx_wr = exch_deb.i_rx_wr;
        exch_deb.rx_buf[i_rx_wr] = (uint8_t) USART_ReceiveData(USART_DEB);	// сохраним принятый байт
        if(++i_rx_wr >= N_BUF_DEB_RX)
            i_rx_wr=0;
        exch_deb.i_rx_wr = i_rx_wr;
        return;
    }
    
    if(USART_GetFlagStatus(USART_DEB, USART_FLAG_TXE)!= RESET)
    {   // "Transmit data register empty"     
        int32_t  i_tx_rd = exch_deb.i_tx_rd; 
        if (i_tx_rd != exch_deb.i_tx_wr)
        {   // передаем очередной байт из буфера
            USART_SendData(USART_DEB, exch_deb.tx_buf[i_tx_rd]);  
            if(++i_tx_rd >= N_BUF_DEB_TX)
                i_tx_rd=0;            
            exch_deb.i_tx_rd = i_tx_rd;
            if (i_tx_rd != exch_deb.i_tx_wr)
                return;
        }  
        // нет данных для передачи - запрешаем прерывания TX   
        USART_ITConfig(USART_DEB, USART_IT_TXE, DISABLE);
    }       
    
    return;
}

/*=================================================================================*/
// чтение данных из приемного буфера FIFO тех порт  
uint16_t read_deb(uint8_t *pbuf, uint16_t sizebuf)
{
    int32_t n, i_rx_rd;
    i_rx_rd = exch_deb.i_rx_rd; 
    n =0;
    while ( (i_rx_rd != exch_deb.i_rx_wr) && (sizebuf > 0) )
    {
        *pbuf++ = exch_deb.rx_buf[i_rx_rd]; // очередной байт из буфера FIFO
        n++;	//кол-во прочитанных байт                
        sizebuf--;	
        if (++i_rx_rd >= N_BUF_DEB_RX )
            i_rx_rd = 0; // на начало;
        exch_deb.i_rx_rd = i_rx_rd;
    } 
	
    return (n);
}

// запись данных в буфер FIFO для передачи в тех порт  
uint16_t write_deb(uint8_t *pbuf, uint16_t sizebuf)
{
    int32_t n, wr_next_tx;	    
    wr_next_tx = exch_deb.i_tx_wr;	//для контроля переполнения буфера 
    if (++wr_next_tx >= N_BUF_DEB_TX) 
        wr_next_tx = 0;	//на начало буфера 
            
    n = 0;
    while ( (wr_next_tx != exch_deb.i_tx_rd)  &&  (sizebuf > 0) )
    {
        exch_deb.tx_buf[exch_deb.i_tx_wr] = *pbuf++;
        n++;	        // кол-во байт для отправки 
        sizebuf--;      // кол-во оставшихся байт  				
        //сдвигаем указатель на след. поз 	
        exch_deb.i_tx_wr = wr_next_tx;
        if (++wr_next_tx >= N_BUF_DEB_TX) 
            wr_next_tx = 0;	//на начало буфера  
    }    
    // запуск передачи данных по  UART  из буфера FIFO
        // Разрешение прерывний TX  
    if( n )
        USART_ITConfig(USART_DEB, USART_IT_TXE, ENABLE);
    return n;
}

// очистка входного буфера 
void clean_read_deb( void)
{
    exch_deb.i_rx_rd = exch_deb.i_rx_wr;
}
