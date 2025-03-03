#include <cstring>
#include <iostream>
#include <netinet/in.h> //인터넷 프로토콜 (ipv4관련 구조체와 상수 정의 헤더)
#include <sys/socket.h> //소켓프로그래밍을 위한 기본적인 함수와 구조체 상수 정의
#include <sys/select.h> //select를 통한 멀티플렉싱 사용
#include <unistd.h> //close()위함
#include <vector>

using namespace std;

const int port = 9032;
const int maxConn = SOMAXCONN;
#define BUF_SIZE 128;

int main(void){

    //1. 소켓 생성
    int serverSocket = socket(AF_INET,SOCK_STREAM,0);

    //2. 소켓 주소 지정
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    //ip 주소가 무엇인지 신경쓰지 않는다 (*ip주소는 nic 카드 갯수에 따라서 달라지므로)
    serverAddress.sin_addr.s_addr = INADDR_ANY;


    //socket 바인딩.
    int isBinding = ::bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    if(isBinding <0){
        perror("bind error : 연결종료");
        close(serverSocket);
        return 0;
    };

    //접속대기
    int isListen = listen(serverSocket,maxConn);
    if(isListen == -1){
        puts("ERROR : 접속대기 불가");
        return 0;
    }

    sockaddr addrInfo;
    socklen_t addrLen;

    int clientSocket
        = accept(serverSocket, (struct sockaddr*)&addrInfo, &addrLen);
    while(clientSocket != -1){
        puts("accept : 새로운 client 연결");
        //연결시 연결정보에 대한 내용을 추후 로그로 기록 남길 예정.

        char buffer[128] = {0};
    
        int nRecv = recv(clientSocket,buffer,sizeof(buffer),0);
        while(nRecv != -1){
            //수신한 문자열을 그대로 client에 반환하여 전송.
            send(clientSocket,buffer,sizeof(buffer),0);
            cout<<"message from client"<<clientSocket<<"\n";
            puts(buffer);
            //메모리 초기화.
            memset(buffer,0,sizeof(buffer));
        }
        //client에서 shutdown 읽기버퍼 쓰기버퍼 모두 종료.
        shutdown(serverSocket,SHUT_RDWR);
        close(clientSocket);

        puts("connection closed.");
    }
    //리슨 소켓 닫기
    close(serverSocket);

    return 0;
}