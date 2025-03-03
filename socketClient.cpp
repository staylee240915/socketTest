#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>

using namespace std;

int main(void){

    //ipv4와 tcp를 이용한 소켓client 생성
    int clientSocket = socket(AF_INET, SOCK_STREAM,0);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET; //ipv4
    serverAddress.sin_port = htons(9032); //9032 포트
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); //연결받을 주소

    //소켓 연결 시작.
    connect(clientSocket,(struct sockaddr*)&serverAddress, sizeof(serverAddress));

    //데이터 입력
    char buf[100]={""};
    fgets(buf,sizeof(buf), stdin);
    
    //데이터 전달. -> buff 부분에서 protocol 정의하여, 전송할 내용 전달할 것.
    send(clientSocket,buf,strlen(buf),0);
    
    //전송 종료.
    close(clientSocket);

    return 0;
}