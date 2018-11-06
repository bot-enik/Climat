/*===========================================================================*\
|* Project:     SEGMENT                                                      *|
|* Module:      uart_deb.h                                                   *|
|* Description: ������� UART ��� ������� � �����                             *|
|* Copyright:   Informtekhnika, 2016                                         *|
|* Author:      Ramil Kurbanov                                               *|
|* Date:        29.03.16                                                     *|
\*===========================================================================*/

#ifndef UART_DEB_H_
#define UART_DEB_H_

  #include "stm32l1xx_usart.h"

  // ������������� ���������������� UART
  void uart_deb_init(void);

  // ���������� ���������������� UART
  void uart_deb_disable(void);    

  // ������ ������ �� ��������� ������ FIFO ��� ����  
  uint16_t read_deb(uint8_t *pbuf, uint16_t sizebuf);
  
  // ������ ������ � ����� FIFO ��� �������� � ��� ����  
  uint16_t write_deb(uint8_t *pbuf, uint16_t sizebuf);
  
  // ������� �������� ������ 
  void clean_read_deb( void);

#endif