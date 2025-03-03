#include <cstring>
#include <iostream>
#include <netinet/in.h> //인터넷 프로토콜 (ipv4관련 구조체와 상수 정의 헤더)
#include <sys/socket.h> //소켓프로그래밍을 위한 기본적인 함수와 구조체 상수 정의
#include <unistd.h>
using namespace std;

int main(void){

    //1. 소켓 생성
    int serverSocket = socket(AF_INET,SOCK_STREAM,0);

    //2. 소켓 주소 지정
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(9032);

    //연결을 받을 주소
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    //socket 바인딩.
    if(bind(serverSocket, (struct sockaddr*) &serverAddress, sizeof(serverAddress))<0){
        perror("bind error");
    };

    listen(serverSocket,5);

    int clientSocket
        = accept(serverSocket,nullptr,nullptr);
    
    char buffer[1024] = {0};
    recv(clientSocket,buffer,sizeof(buffer),0);
    cout<<"message from client "<<clientSocket<<" : "<< buffer<<"\n";

    close(serverSocket);

    return 0;
}