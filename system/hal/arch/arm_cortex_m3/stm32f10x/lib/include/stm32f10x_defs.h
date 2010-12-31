#ifndef __STM32F10x_CONF_H
#define __STM32F10x_CONF_H


typedef enum
  {
  GPIO_Speed_10MHz = 0x1, GPIO_Speed_2MHz = 0x2, GPIO_Speed_50MHz = 0x3
  } GPIOSpeed_TypeDef;

typedef enum
  {
  GPIO_Mode_AIN = 0x0,
  GPIO_Mode_IN_FLOATING = 0x1,
  GPIO_Mode_IPUD = 0x2,
  GPIO_Mode_Out_PP = 0x0,
  GPIO_Mode_Out_OD = 0x1,
  GPIO_Mode_AF_PP = 0x2,
  GPIO_Mode_AF_OD = 0x3
  } GPIOConf_TypeDef;

#endif /* __STM32F10x_CONF_H */
