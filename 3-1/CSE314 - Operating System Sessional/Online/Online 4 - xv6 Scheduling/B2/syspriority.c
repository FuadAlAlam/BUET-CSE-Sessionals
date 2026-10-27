// Append to kernel/sysproc.c; do not compile this file separately.
// Requires int priority and int budget in struct proc, and the question's
// MIN_PRIORITY, MAX_PRIORITY, DEFAULT_PRIORITY and BUDGET_UNIT macros.

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
  // Update the priority and budget together, without timer preemption.
  acquire(&p->lock);
  p->priority = priority;
  p->budget = priority * BUDGET_UNIT;
  release(&p->lock);
  return result;
}

uint64
sys_getpriority(void)
{
  return myproc()->priority;
}
