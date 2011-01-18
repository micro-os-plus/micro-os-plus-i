/*
 *      Copyright (C) 2009 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef HAL_FAMILY_OS_DEFS_H_
#define HAL_FAMILY_OS_DEFS_H_

#define OS_GPIO_PORT_CONFIG(_PORT_CONFIG, _BIT, _VALUE)
#define OS_GPIO_PORT_HIGH(_PORT, _BIT)
#define OS_GPIO_PORT_LOW(_PORT, _BIT)
#define OS_GPIO_PORT_TOGGLE(_PORT, _BIT)

#define OS_GPIO_PORT_ISHIGH(_PORT, _BIT)	(false)
#define OS_GPIO_PORT_ISLOW(_PORT, _BIT)		(true)

#endif /* HAL_FAMILY_OS_DEFS_H_ */
