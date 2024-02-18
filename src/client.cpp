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

    connect(socketfd, (struct sockaddr *)&addr, sizeof(addr));

    while(true)
    {
        char buffer[256] = { 0 };
        printf("%s\n", "Rec: ");
        recv(socketfd, buffer, 255, 0);
        printf("%s\n", buffer);
    }
    close(socketfd);
}
