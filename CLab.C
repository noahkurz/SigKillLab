#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

int child_pids[1000];
int parent_hp = 0;
int MAX_CHILDREN = 10;
int numChildren = 0;

void sig_handler_parent(int signum)
{
  printf("\nParent: Inside handler function\n");
  for(int i = 0; i < numChildren; i ++)
  {
    kill(child_pids[i], SIGUSR2);
  }
  exit(0);
}

void sig_handler_child(int signum)
{
  printf("\nChild:Inside handler function\n");
  printf("parent hp is %d!!!!\n", parent_hp);
  //printf("%d", getpid());
  printf("Now dying!!!%d\n", getpid());
  
  exit(0);
}
void kidKiller(int kidToKill)
{
  sleep(1);
  printf("\nenter KILL MODE\n");
  //added this 3 due to a weird race condition
  sleep(3);
  printf("Killed PID: %d\n", kidToKill);
  kill(getpid(), SIGKILL);

}


int main()
{
  //preset length of array TODO it would be cool not to but I also benefit in capping
  //out the number of running procs. just as a saftey net. 
  time_t t; 
  srand((unsigned) time(&t));
  parent_hp = rand() % 10 + 5; 
  signal(SIGINT, sig_handler_parent);
  while(1)
  {
    int pid = fork(); 
    if(pid == 0)
    {
      printf("Another kid born\n");
      
      signal(SIGUSR2, sig_handler_child);
      //pause keeps the child alive 
      pause();

    }

    else
    {
      //I am daddy 

      child_pids[numChildren] = pid; 
      numChildren ++; 
      // prevent race condition 
      sleep(1); 


      //kill(pid, SIGUSR2);
    }
    
  }
  return 0;
}
