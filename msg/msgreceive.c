#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MSG_TYPE 10

struct msg_st
{
    long int msg_type;
    char text[BUFSIZ];
};

int main(int argc, char** argv)
{
    int running = 1;
    int msgid = -1;
    struct msg_st data;
    long int msgtype = -1;
    key_t key = ftok(".", 100);

    msgid = msgget(key, 0666 | IPC_CREAT);
    if(msgid == -1)
    {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    while(running)
    {
        if(msgrcv(msgid, (void*)&data, BUFSIZ, MSG_TYPE, 0) == -1)
        {
            perror("msgrcv");
            exit(EXIT_FAILURE);
        }
        printf("You wrote %s", data.text);

        if(strncmp(data.text, "end", 3) == 0)
        {
            running = 0;
        }
    }

    if(msgctl(msgid, IPC_CREAT, 0) == -1)
    {
        perror("msgctl");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}