#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define BYEL "\033[1;33m"
#define BRED "\033[1;31m"
#define CRESET "\033[0m"

int
main(int argc, char *argv[])
{
  if(argc != 3){
    fprintf(2, "Usage: testloop <iterations> <priority>\n");
    exit(1);
  }

  int iters = atoi(argv[1]);
  int priority = atoi(argv[2]);
  if(iters <= 0){
    fprintf(2, "testloop: iterations must be positive\n");
    exit(1);
  }
  if(setpriority(priority) < 0)
    fprintf(2, "testloop: invalid priority; using default\n");

  // Delay before timing; setpriority already takes effect immediately.
  // This does not synchronize separately launched processes.
  sleep(5);

  int pid = getpid();
  printf(BYEL "PID %d: starting %d iterations at time %d, priority %d\n" CRESET,
         pid, iters, uptime(), getpriority());

  // Volatile keeps the workload present when compiling with optimization.
  for(int i = 0; i < iters; i++)
    for(volatile int j = 0; j < 20000000; j++)
      ;

  printf(BRED "PID %d: finished at time %d\n" CRESET, pid, uptime());
  exit(0);
}
