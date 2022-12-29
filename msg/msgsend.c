#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/msg.h>
#include <error.h>

#define MSG_TYPE 10

struct msg_st
{
    long int msg_type;
    char text[BUFSIZ];
};

int main()
{
    int running = 1;
    struct msg_st data;
    char buf[BUFSIZ];
    int msgid = -1;
    key_t key = ftok(".", 100);
    
    msgid = msgget(key, 0666 | IPC_CREAT);
    if(msgid == -1)
    {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    data.msg_type = MSG_TYPE;
    while (running)
    {
        printf("Enter some text: ");
        fgets(buf, BUFSIZ, stdin);
        strcpy(data.text, buf);

        if(msgsnd(msgid,(void*)&data, BUFSIZ, 0) == -1)
        {
            perror("msgsnd");
            exit(EXIT_FAILURE);
        }

        if(strncmp(buf, "end", 3) == 0)
        {
            running = 0;
        }
    }
    return EXIT_SUCCESS;
}

