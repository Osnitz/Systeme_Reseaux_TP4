//
// Created by matthieu on 24/10/24.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "segdef.h"

#define maxreq 50

typedef struct {
    int semId;
    int shmId;
    segment *seg;
} InitResult;

struct sembuf sop ;

InitResult init() {
    InitResult result;
    int semId = semget(cle,0,0);
    if(semId == -1)
    {
        perror("semget \n");
        exit(1);
    }
    result.semId = semId;

    int shmId = shmget(cle,0,0);
    if(shmId == -1)
    {
        perror("shmget \n");
        exit(1);
    }
    result.shmId = shmId;

    segment* seg = (segment*)shmat(shmId,NULL,0);
    if(seg == (void*)-1)
    {
        perror("shmat \n");
        exit(1);
    }
    result.seg = seg;

    init_rand();
    return result;
}



int main()
{
    int request_count = 0;
    //printf("Step 0 ... \n");
    InitResult result = init();
   // printf("Step  : ok \n");
    int semId = result.semId;
    segment* seg = result.seg;
    while (request_count < maxreq)
    {
        long client_result = 0;
        request_count++;

       // printf("Step 1 : acq seg_dispo  \n");
        acq_sem(semId,seg_dispo);
       // printf("Step 1 : ok \n");


        //printf("Step 2 :  init seg + acq seg_init \n");
        seg -> pid = getpid();
        seg -> req = request_count;
        for(int i = 0; i < maxval; i++)
        {
            seg -> tab[i] = getrand();
        }
        acq_sem(semId,seg_init);
        //printf("Step 2 ok \n");

        for(int i=0; i<maxval; i++){
            client_result += seg->tab[i];
        }
        client_result = client_result/maxval;

        //printf("Step 3 : wait res_ok = 0 \n");
        wait_sem(semId,res_ok);
       // printf("Step 3 ok \n");

        //printf("Step 4 : read result + free seg_init \n");
        long server_result = seg -> result;
        lib_sem(semId,seg_init);
        //printf("Step 4 ok \n");

        //printf("Step 5 : wait res_ok is free \n");
        acq_sem(semId,res_ok);
        lib_sem(semId,res_ok);
        //printf("Step 5 ok \n");


        //printf("Step 6 : free seg_dispo \n");
        lib_sem(semId,seg_dispo);
        //printf("Step 6 ok \n");

        //printf("Client result: %ld\n",client_result);
        //printf("Server result: %ld\n",server_result);
        if (client_result != server_result)
        {
            printf("Error: client result is different from server result\n");
        }
        else
        {
            printf("True \n");
        }
    }

    if(shmdt(seg)==-1)
    {
        perror("shmdt\n");
    }
    return 0;
}

