#include "local.h"

struct msg
{
    long int type;
    char content[36];
    pid_t pid;
}p,p1;

int main()
{
    int r;
    char answer[10];
    printf("Would you like to change the number of rounds?[y]");
    scanf("%c",answer);
    if( strcmp(answer,"y")==0){
        printf("Kindly enter the number of rounds:");
        scanf("%d",&r);
    }
    else{
        r=5;
    }
    
    printf("The Match which consists of %d Rounds will begin:\nProject made by: Momen Assaf, Yousef, Amjad\n",r);
    pid_t parent_id = getpid();
    pid_t children_id[5];
    
    for (int i = 0; i < 6; i++){
        children_id[i] = fork();

        if(children_id[i] == -1){
            perror("No Memory\n");
            exit(-1);
        }
        else if (children_id[i] == 0){
            sleep(1);
            if(i < 4){
                //the fist 4 children run the same code
                execlp("./child","child","1",NULL);
            }
            else if(i==5){
                //the 5th child runs a different code
                execlp("./child","child","0",NULL);
            }
            else{
                execlp("./ogl","ogl",NULL);
            }
            //children shouldnt run this code
            perror("Exec Error");
            exit(-2);
            break;
        }
        else{
            //printf("The Parent procces with id %d created the child with id: %d\n",parent_id,children_id[i]);
        }
    }


    // team1 team2 #ofWins
    // make for loop "loops how many needed ask the user how many if user didnt enter assume 5"

    srand((unsigned)getpid());

    if (parent_id == getpid())
    {
        // random min max

        int min = rand() % 100 + 1; // generate a random value between 1 and 100 for min
        int max = rand() % 100 + 1; // generate a random value between 1 and 100 for max

        // Swap min and max if min is greater than max
        if (min > max)
        {
            int temp = min;
            min = max;
            max = temp;
        }

        // Open the file for writing
        FILE *file = fopen("range.txt", "w");
        // Write the min and max values to the file
        fprintf(file, "%d,%d", min, max);

        // Close the file
        fclose(file);

        // end of random min max
        int status;
        pid_t child;
        int team1 = 0,team2 = 0 , rplayed = 0;

        while( rplayed != r){
            
            int min = rand() % 100 + 1; // generate a random value between 1 and 100 for min
            int max = rand() % 100 + 1; // generate a random value between 1 and 100 for max

            // Swap min and max if min is greater than max
            if (min > max)
            {
                int temp = min;
                min = max;
                max = temp;
            }

            // Open the file for writing
            FILE *file = fopen("range.txt", "w");
            // Write the min and max values to the file
            fprintf(file, "%d,%d", min, max);

            // Close the file
            fclose(file);
            sleep(2);

            char value[24];
            char concat[64];
            strcpy(concat,"");

            for (int i = 0; i < 4; i++)
            {  
                char filename[10];
                float val;
                char *token;

                sprintf(filename,"%d.txt",children_id[i]);
                FILE *pid_File = fopen(filename,"r");
                fgets(value,24,pid_File);
                
                token = strtok(value,"\n");
                strcat(concat,token);

                if(i<3){
                strcat(concat,","); 
                }
                fclose(pid_File);
            }

            int m;
            m = msgget(KEY,0666|IPC_CREAT);
            p.type = 1;
            p.pid = parent_id;
            strcpy(p.content,concat);

            msgsnd(m,&p,sizeof(p),0);
            msgrcv(m,&p1,sizeof(p),p.pid,0);

            //printf("%s\n",p1.content);

            char *token;
            float sum1, sum2 , winner;

            token = strtok(p1.content,",");
            sum1 = atof(token);

            token = strtok(NULL,"\n");
            sum2 = atof(token);

            FILE * match_file = fopen("match.txt","w");


            if(sum1 > sum2 ){
                winner = sum1;
                team1++;                   
                fprintf(match_file,"%d,Team1,%f",rplayed+1,winner);
                printf("CONGRATULATIONS!! The Winner of round %d is Team 1, with a score of %f\n",rplayed+1,sum1);
            }
            else if (sum2 > sum1){
                winner = sum2;
                team2++;
                fprintf(match_file,"%d,Team2,%f",rplayed+1,winner);
                printf("CONGRATULATIONS! The Winner of round %d is Team 2, with a score of %f\n",rplayed+1,sum2);
            }
            else{
                team1++;
                team2++;
                printf("WELL DONE both teams, Its a draw\n");
            }
            rplayed++;
            fclose(match_file);

            for (int t = 0; t < 4; t++)
            {
                kill(children_id[t],SIGUSR1);
            } 
        }
        if(team1 > team2){
            printf("CONGRATULATIONS!, The winner of this Match is Team1 with %d Points.\n",team1);
        }
        else if(team2 > team1){
            printf("CONGRATULATIONS!, The winner of this Match is Team2 with %d Points.\n",team2);
        }
        else{
            printf("WELL DONE!! both teams, Its a Draw\n");

        }
        sleep(1);
        for (int t =0 ; t < 6 ; t++){
            kill(children_id[t],SIGTERM);
            printf("Pid:%d killed\n",children_id[t]);
        }

        if (child = wait(&status))
        {
            printf("The child %d: finished with status: %d\n", child, status);
            exit(0);
        }
    }
    return 0;
}