// Append to kernel/sysproc.c; do not compile this file separately.
// Requires int priority in struct proc and the question's priority macros.
// Changing priority does not reset the scheduling wait counter.

uint64
sys_setpriority(void)
{
  int priority;
  int result = 0;
  argint(0, &priority);
  if(priority < MIN_PRIORITY || priority > MAX_PRIORITY){
    priority = DEFAULT_PRIORITY;
    result = -1;
  }

  struct proc *p = myproc();
  acquire(&p->lock);
  p->priority = priority;
  release(&p->lock);
  return result;
}

uint64
sys_getpriority(void)
{
  return myproc()->priority;
}
