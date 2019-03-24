#include "server.h"

#include <thread>

uint16_t servPort = 2000;
uint16_t clientPort = 3000;

void writeSocket()
{
    int sockFd = socket(AF_LOCAL, SOCK_STREAM, 0);
//    sockaddr_in addr;
//    addr.sin_family =AF_INET;
//    addr.sin_port = htons(clientPort);
//    addr.sin_addr.s_addr = htonl(INADDR_ANY);
//    bind(sockFd, (struct sockaddr*)&addr, sizeof(addr));

    sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_LOCAL;
    servAddr.sin_port = htons(servPort);
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    auto len = sizeof(servAddr);
    int ret = connect(sockFd, (struct sockaddr*)&servAddr, len);
    if (ret == -1) {
        perror("connect failed");
        return;
    }

    while (true) {
        send(sockFd, "Hello", 6, 0);
    }
}

int main(void)
{
//    std::thread t(writeSocket);

    Serv serv{servPort};
    serv.init();
    serv.start();

    return 0;
}