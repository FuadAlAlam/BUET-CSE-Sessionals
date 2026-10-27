#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define BYEL "\e[1;33m"
#define BRED "\e[1;31m"
#define CRESET "\e[0m"

int
main(int argc, char *argv[])
{
  if(argc < 3){
    printf("Usage: testloop <iterations> <priority>\n");
    exit(1);
  }

  int pid = getpid();
  uint32 iters = atoi(argv[1]);
  int priority = atoi(argv[2]);

  setpriority(priority);
  sleep(5); // let the scheduler pick up the new priority before we start timing

  int entry_time = uptime();
  printf(BYEL "PID %d: starting %u iterations at time %d, priority %d\n" CRESET,
         pid, iters, entry_time, getpriority());

  for(uint32 i = 0; i < iters; i++){
    for(int j = 0; j < 50000000; j++){
      int x = j * j;
      x = x + 1;
    }
  }

  int exit_time = uptime();
  printf(BRED "PID %d: finished at time %d\n" CRESET, pid, exit_time);
  exit(0);
}
