/*
 *      Copyright (C) 2008-2012 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef DEVICELM74_DEFINES_H_
#define DEVICELM74_DEFINES_H_

#define OS_CONFIG_LM74_CLK_PORT_CONFIG          DDRA
#define OS_CONFIG_LM74_CLK_PORT_WRITE           PORTA
#define OS_CONFIG_LM74_CLK_BIT                  5

#define OS_CONFIG_LM74_DATA_PORT_CONFIG         DDRA
#define OS_CONFIG_LM74_DATA_PORT_WRITE          PORTA
#define OS_CONFIG_LM74_DATA_PORT_READ           PINA
#define OS_CONFIG_LM74_DATA_BIT                 4

#endif /* DEVICELM74_DEFINES_H_ */
