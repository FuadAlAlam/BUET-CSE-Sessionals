// Append to kernel/sysproc.c; do not compile this file separately.
// Requires int priority in struct proc and the question's priority macros.
// The scheduler checks the current priority when visiting a process slot.
// Changing priority does not restart either process-table scan.

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
