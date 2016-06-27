/*
 *      Copyright (C) 2012 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef HAL_FAMILY_OSCPUIMPL_INLINES_H_
#define HAL_FAMILY_OSCPUIMPL_INLINES_H_

inline OSInterruptsMask_t
OSCPUImpl::getInterruptsMask(void)
{
  register unsigned int tmp;

  asm volatile
  (
      " mrs %[R], basepri       \n"

      : [R] "=r" (tmp)
      :
      :
  );
  return tmp;
}

inline void
OSCPUImpl::setInterruptsMask(OSInterruptsMask_t mask)
{
  register unsigned int tmp;

  tmp = mask;

  asm volatile
  (
      " msr    primask, %[R] \n"

      :
      : [R] "r" (tmp)
      :
  );
}

inline OSResetBits_t
OSCPUImpl::fetchResetBits(void)
{
  return -1; // TODO: fetch bits telling reset reason.
}


#endif /* HAL_FAMILY_OSCPUIMPL_INLINES_H_ */
