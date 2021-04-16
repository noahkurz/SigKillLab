#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int MAX_CHILDREN = 10;
int numChildren = 0;

void sig_handler_parent(int signum)
{
  printf("\nParent: Inside handler function\n");
}

void sig_handler_child(int signum)
{
  printf("\nChild:Inside handler function\n");
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

void sigIntHandler(int signum)
{
  sleep(1);
  printf("\nInterupted\n");
  int varToPass = getpid();
  kidKiller(varToPass);
  sleep(1);
}

int main()
{
  //preset length of array TODO it would be cool not to but I also benefit in capping
  //out the number of running procs. just as a saftey net. 
  int arrPID[MAX_CHILDREN];

  for (int a = 0; a < MAX_CHILDREN; a++)
  {
    sleep(1);
    int pid = fork();
    //probably should have a error check here but welp 

    if (pid == 0)
    {
      //child
      numChildren++;
      signal(SIGUSR1, sig_handler_child);
    }

    else

    {
      //parent
      arrPID[a] = pid;
      sleep(1);
      signal(SIGUSR1, sig_handler_parent);
      signal(SIGINT, sigIntHandler);
      printf("PID: %d\n", getpid());

      while (1)

      {
        kill(pid, SIGUSR1);
        sleep(1);
      }
    }
  }
//print out array 
  for (int a = 0; a < MAX_CHILDREN; a++)
  {
    printf("PID: %d\n", arrPID[a]); 
  }
  //let the user know they can SIGINT in order to start killing orphans
  printf("max number of kids have been born...\n time to kill some kids\nHit 'cntrl C' to see some killing\n");
}
