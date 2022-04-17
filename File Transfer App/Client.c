#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define h_addr h_addr_list[0]

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(1);
    }

    int sockfd, portno, n;
    char buffer[256];

    struct sockaddr_in serv_addr;
    // info about host
    struct hostent *server;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        error("Error in opening socket");
    }

    portno = atoi(argv[2]);
    server = gethostbyname(argv[1]);

    if (server == NULL)
    {
        fprintf(stderr, "Error no such host");
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        error("Connection failed");
    }

    bzero(buffer, 255);

    FILE *f;
    int words = 0;
    char c;
    f = fopen("sample.txt", "r");

    int count_space = 1;

    while ((c = fgetc(f)) != EOF)
    {
        if (isspace(c) || c == '\t')
            count_space = 1;
        else if (count_space == 1)
        {
            words++;
            count_space = 0;
        }
    }

    printf("words: %d\n", words);

    write(sockfd, &words, sizeof(int));
    rewind(f);

    char ch;

    while (ch != EOF)
    {
        fscanf(f, "%s", buffer);
        write(sockfd, buffer, 255);
        ch = fgetc(f);
    }

    printf("The file has been sent");

    close(sockfd);
    return 0;
}