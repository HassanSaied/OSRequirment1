#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <defs.h>
#define null 0

struct processData
{
    int arrival_time;
    int priority;
    int runningtime;
    int id;
};

int main(int argc, char * argv[])
{
    FILE * pFile;
    pFile = fopen(PROCESS_FILE_NAME, "w");
    int no;
    struct processData pData;
    printf("please enter the number of processes you want to generate:");
    scanf("%d", &no);
    srand(time(null));
    //fprintf(pFile,"%d\n",no);
    fprintf(pFile, "#id arrival runtime priority\n");
    pData.arrival_time = 1;
    for (int i = 1 ; i <= no ; i++)
    {
        //generate Data Randomly
        //[min-max] = rand() % (max_number + 1 - minimum_number) + minimum_number
        pData.id = i;
        pData.arrival_time += rand() % (11); //processes arrives in order
        pData.runningtime = rand() % (30);
        pData.priority = rand() % (10) + 1;
        fprintf(pFile, "%d\t%d\t%d\t%d\n", pData.id, pData.arrival_time, pData.runningtime, pData.priority);
    }
    fclose(pFile);
}
