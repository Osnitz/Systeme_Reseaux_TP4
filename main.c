//
// Created by matthieu on 24/10/24.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "segdef.h"

void start_clients(int num_clients) {
    for (int i = 0; i < num_clients; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("Fork failed");
            exit(1);
        }

        if (pid == 0) {
            // Code du client ici
            char *args[] = {"./client", NULL};
            execv(args[0], args);
            perror("Exec failed");
            exit(1);
        }
    }

    while (wait(NULL) > 0);
}

int main() {

    int num_clients = 4096;
    start_clients(num_clients);

    return 0;
}