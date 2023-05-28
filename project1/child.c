#include "local.h"

int x = 0;
void check_unhappy();

void printPeople(int num){
  fflush(stdout);
  x = 0;
}

struct msg
{
    long int type;
    char content[36];
    pid_t pid;
}p,p1;


int main(int argc, char** argv) {
  if(argc < 2){
    perror("No arguments passed");
    exit(0);
  }

  sigset(SIGUSR1, check_unhappy);

  while (1)
  {
    if(strcmp(argv[1],"1") == 0  ){
      if( x == 0){
        FILE *Range_File = fopen("range.txt", "r");
        char range[36];
        char *token;
        int MAX,MIN;

        if(Range_File == NULL){
          perror("Error opening range file");
        }
        else{
          while(fgets(range,36,Range_File) != NULL)
          {
            token = strtok(range,",");  
            MIN = atoi(token);
            token = strtok(NULL, ",");
            MAX = atoi(token);
          }
        }
        // printf("\nMin:%d\n",MIN);
        // printf("Max:%d\n",MAX);

        // Get the child's id
        int child_id = getpid();

        // Generate a random value in the range [MIN, MAX]
        srand((unsigned int) getpid());
        float value = ((float)rand()/(float)(RAND_MAX))*(MAX - MIN) + MIN;
        // printf("vlaue = %f\n",value);

        // Create the file name
        char filename[100];
        snprintf(filename, sizeof(filename), "%d.txt", child_id);

        // Write the value to a file
        FILE *value_file = fopen(filename, "w");
        fprintf(value_file, "%f\n", value);
        fclose(value_file);

        //Exit the child process
        wait(NULL);
        x = 1;
      }
    }
    else if(strcmp(argv[1],"0") == 0){
      //printf("child5 pid:%d\n",getpid());
      int m;
      m = msgget(KEY,0666|IPC_CREAT);
      while (msgrcv(m,&p,sizeof(p),1,0) != -1 ){
        //printf("%s",p.content);

        char *token,con[20], str1[10], str2[10];
        float value1,value2,value3,value4,sum1,sum2;

        token = strtok(p.content,",");
        value1 = atof(token);

        token = strtok(NULL,",");
        value2 = atof(token);

        sum1 = value1 + value2;
        // sprintf(str1,"%f",sum1);

        token = strtok(NULL,",");
        value3 = atof(token);

        token = strtok(NULL,"\n");
        value4 = atof(token);
        
        sum2 = value3 + value4;
        // sprintf(str2,"%f",sum2);

        // printf("sum1:%s\n",str1);
        // printf("sum2:%s\n",str2);

        sprintf(con,"%f,%f",sum1,sum2);
        //printf("%s",con);

        p.type = p.pid;
        p.pid = getpid();
        strcpy(p.content,con);
        msgsnd(m,&p,sizeof(p),0);
      }
    }
  }
}
void check_unhappy()
{
  x = 0;
}
