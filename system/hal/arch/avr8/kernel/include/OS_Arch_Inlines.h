/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef HAL_OS_ARCH_INLINES_H_
#define HAL_OS_ARCH_INLINES_H_


// release processor to next ready thread
inline void
OSSchedulerImpl::yield(void)
{
  OSSchedulerImpl::registersSave();
  OSSchedulerImpl::stackPointerSave();
    {
      OSScheduler::performContextSwitch(); // same as from ISRs, no return
    }
  OSSchedulerImpl::stackPointerRestore();
  OSSchedulerImpl::registersRestore();

  // For unknown reasons, interrupts must be re-enabled here
  OSCPU::interruptsEnable();
  OSCPU::returnFromSubroutine(); // necessary because it is 'naked'
}


#endif /* HAL_OS_ARCH_INLINES_H_ */
