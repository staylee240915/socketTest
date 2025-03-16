#include <cstring>
#include <iostream>
#include <netinet/in.h> //인터넷 프로토콜 (ipv4관련 구조체와 상수 정의 헤더)
#include <sys/socket.h> //소켓프로그래밍을 위한 기본적인 함수와 구조체 상수 정의
#include <sys/select.h> //select를 통한 멀티플렉싱 사용
#include <unistd.h> //sleep위함
#include <pthread.h>
#include <arpa/inet.h>
#include <vector>

using namespace std;

const int port = 9032;
#define BUF_SIZE 128;

void *acceptThread(void *arg){
    //client -> server로 보내는 내용에 대한 receive buffer
    char sRecvBuffer[128] = { 0 };
}
void *thread_function(void *ptr);

int main(void){

    //1. 소켓 생성
    int serverSocket = socket(AF_INET,SOCK_STREAM,0);

    //2. socket 바인딩.
    sockaddr_in serverAddress = { 0 };
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);



    //ip 주소가 무엇인지 신경쓰지 않는다 (*ip주소는 nic 카드 갯수에 따라서 달라지므로)
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if(::bind(
        serverSocket,(sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
    {
        perror("Binding Error : 연결종료");
        return 0;
    };

    //3. 접속대기로 전환
    if(::listen(serverSocket,SOMAXCONN)==-1){
        puts("ERROR : 접속대기 상태로 전환 불가");
        return 0;
    }

    //4. 클라이언트 접속처리 및 내용
    int nRecv = 0;
    int clientSocket =0;
    sockaddr_in clientAddr = { 0 };
    socklen_t clientAddrLen;
    vector<pthread_t> cpthread();
    pthread_attr_t cpthreadAttr;
    //4. 클라이언트의 연결을 받아 새로운 소켓 생성
    //accept의 경우 socket을 위한 파일지정번호를 반환.
    while((clientSocket = accept(serverSocket, 
        (sockaddr*)&clientAddr, &clientAddrLen))!=-1)
    {
        //새 client가 들어올 때 마다 새로운 스레드 생성
        //새 스레드 속성 오브젝트 작성.
        pthread_t threads;
        int *threadRet;
        pthread_attr_init(&cpthreadAttr);
        if(pthread_create( 
            &threads,
            NULL,
            thread_function,
            (void *) "thread 1"
        )!=-1){
            cout<<"thread 생성 실패"<<"\n";
        };

        cout << "클라이언트와 연결이 완료되었습니다" << endl;
        cout << "Client IP: " << inet_ntoa(clientAddr.sin_addr) << endl;
		cout << "Port: " << ntohs(clientAddr.sin_port) << endl;

        puts("accept : 새로운 client 연결"); fflush(stdout);
        //연결시 연결정보에 대한 내용을 추후 로그로 기록 남길 예정.

        char buffer[128] = {0};
    
        while((nRecv=recv(clientSocket,buffer,sizeof(buffer),0))!=-1){
            //수신한 문자열을 그대로 client에 반환하여 전송.
            send(clientSocket,buffer,sizeof(buffer),0);

            cout<<"message from client"<<clientSocket<<"\n";
            //메세지 표출
            puts(buffer);
            //메모리 초기화.
            memset(buffer,0,sizeof(buffer));
        }
        //client에서 shutdown 읽기버퍼 쓰기버퍼 모두 종료.
        shutdown(serverSocket,SHUT_RDWR);
        
        //서버와 연결된 클라이언트의 소켓 close;
        close(clientSocket);

        puts("connection closed."); fflush(stdout);
    }
    //리슨 소켓 닫기
    close(serverSocket);

    return 0;
}