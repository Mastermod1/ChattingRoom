#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <thread>

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
    std::thread printer = std::thread(
        [&socketfd]()
        {
            while (true)
            {
                char buffer[256] = {0};
                if (recv(socketfd, buffer, 255, 0) == 0)
                {
                    return 0;
                }
                printf("%s\n", "Rec: ");
                printf("%s\n", buffer);
            }
        });
    printer.detach();

    while (true)
    {
        char buffer[256] = {0};
        printf("%s\n", "Sending: ");
        read(0, buffer, 255);
        send(socketfd, buffer, 255, 0);
    }
    close(socketfd);
}
