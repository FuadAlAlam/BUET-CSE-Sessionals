// Append these functions to kernel/sysproc.c; do not compile this file separately.
// Requires priority in struct proc and the macros specified in the question in kernel/param.h.
uint64
sys_setpriority(void)
{
  int priority;
  argint(0, &priority);
  struct proc *p = myproc();
  if(priority < MIN_PRIORITY || priority > MAX_PRIORITY){
    p->priority = DEFAULT_PRIORITY;
    return -1;
  }
  p->priority = priority;
  return 0;
}

uint64
sys_getpriority(void)
{
  return myproc()->priority;
}
