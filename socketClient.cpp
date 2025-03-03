#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>

using namespace std;

int main(void){

    int clientSocket = socket(AF_INET, SOCK_STREAM,0);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(9032);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    connect(clientSocket,(struct sockaddr*)&serverAddress, sizeof(serverAddress));

    char buf[100]={""};
    fgets(buf,sizeof(buf), stdin);
    
    send(clientSocket,buf,strlen(buf),0);
    
    close(clientSocket);

    return 0;
}