#include "Server.h"

int main()
{
    // Server 클래스 생성
    Server* cServer = new Server();

    // 서버 초기 설정
    if (!cServer->Initialize())
    {
        return 0;
    }

    // 서버를 시작한다.
    cout << "Server Start" << endl;
    // 다중 접속을 위해 while로 소켓을 대기한다.
    while (1)
    {
        // 접속 설정 구조체 사이즈
        int len = sizeof(SOCKADDR_IN);
        // 접속 설정 구조체
        SOCKADDR_IN clientAddr;
        // client가 접속을 하면 SOCKET을 받는다.
        SOCKET clientSock = accept(cServer->serverSock, (SOCKADDR*)&clientAddr, &len);
        // 쓰레드를 실행하고 쓰레드 리스트에 넣는다.
        auto thr = new thread(client, clientSock, clientAddr, &cServer->clientlist);
        cServer->clientlist.push_back(thr);
    }
    // 종료가 되면 쓰레드 리스트에 남아 있는 쓰레드를 종료할 때까지 기다린다.
    for (auto ptr = cServer->clientlist.begin(); ptr < cServer->clientlist.end(); ptr++)
    {
        (*ptr)->join();
    }
    // 서버 소켓 종료
    closesocket(cServer->serverSock);
    // 소켓 종료
    WSACleanup();
    delete cServer;
    return 0;
}
