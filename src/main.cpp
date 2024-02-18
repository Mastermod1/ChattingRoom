#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd == -1)
    {
        std::cerr << "Socket not created\n";
        close(socketfd);
        exit(-1);
    }

    struct sockaddr_in addr = {AF_INET, htons(9999), 0};

    int bnd = bind(socketfd, (struct sockaddr *)&addr, sizeof(addr));
    if (bnd == -1)
    {
        std::cerr << "Bind failed\n";
        close(socketfd);
        exit(-1);
    }

    int lsit = listen(socketfd, 10);

    int clientfd = accept(socketfd, 0, 0);

    while(true)
    {
        char buffer[256] = { 0 };
        printf("%s\n", "Sending: ");
        read(0, buffer, 255);
        send(clientfd, buffer, 255, 0);
    }
    close(socketfd);
}