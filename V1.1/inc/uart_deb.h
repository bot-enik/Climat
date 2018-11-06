/*===========================================================================*\
|* Project:     SEGMENT                                                      *|
|* Module:      uart_deb.h                                                   *|
|* Description: драйвер UART для отладки и логов                             *|
|* Copyright:   Informtekhnika, 2016                                         *|
|* Author:      Ramil Kurbanov                                               *|
|* Date:        29.03.16                                                     *|
\*===========================================================================*/

#ifndef UART_DEB_H_
#define UART_DEB_H_

  #include "stm32l1xx_usart.h"

  // Инициализация технологического UART
  void uart_deb_init(void);

  // выключение технологического UART
  void uart_deb_disable(void);    

  // чтение данных из приемного буфера FIFO тех порт  
  uint16_t read_deb(uint8_t *pbuf, uint16_t sizebuf);
  
  // запись данных в буфер FIFO для передачи в тех порт  
  uint16_t write_deb(uint8_t *pbuf, uint16_t sizebuf);
  
  // очистка входного буфера 
  void clean_read_deb( void);

#endif