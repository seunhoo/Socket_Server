#include "Server.h"

vector<otherClient> vcOC;

Server::Server()
{

}

Server::~Server()
{
}

// 서버 정보 초기화
int Server::Initialize()
{
    // 소켓 실행.
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        return 0;
    }

    serverSock = socket(PF_INET, SOCK_STREAM, 0);

    // 구조체 초기화
    memset(&addr, 0, sizeof(addr));
    // 소켓은 Internet 타입
    addr.sin_family = AF_INET;
    // 서버이기 때문에 local 설정한다.
    // Any인 경우는 호스트를 127.0.0.1로 잡아도 되고 localhost로 잡아도 되고 양쪽 다 허용하게 할 수 있따. 그것이 INADDR_ANY이다.
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // 서버 포트 설정...저는 9090으로 설정함.
    addr.sin_port = htons(PORT);

    iThrNum = 0;

    // 설정된 소켓 정보를 소켓에 바인딩한다.
    if (bind(serverSock, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
    {
        // 에러 콘솔 출력
        cout << "error" << endl;
        return 1;
    }

    // 소켓을 대기 상태로 기다린다.
    if (listen(serverSock, SOMAXCONN) == SOCKET_ERROR)
    {
        // 에러 콘솔 출력
        cout << "error" << endl;
        return 0;
    }
    return 1;
}


void ChatingRelayServer()
{
    while (1)
    {
        Sleep(100);
        int first = -1;
        int second = -1;
        for(int i = 0 ; i < vcOC.size(); i++)
        {
            if(vcOC[i].sState == WAITING && first == -1)
            {
                first = i;
            }
            else if (vcOC[i].sState == WAITING && second == -1)
            {
                second = i;
                vcOC[first].sState = CONNECTING;
                vcOC[first].iConnectedUser = second;
                vcOC[first].sendClientsock = vcOC[second].clientsock;

                vcOC[second].sState = CONNECTING;
                vcOC[second].sendClientsock = vcOC[first].clientsock;
                vcOC[second].iConnectedUser = first;
            }
        }
    }
}

// 접속되는 client별 쓰레드
void client(SOCKET clientSock, SOCKADDR_IN clientAddr, vector<thread*>* clientlist, int thrnum)
{
    // 접속 정보를 콘솔에 출력한다.
    cout << "Client connected Information = " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << " | Number : " << thrnum << endl;

    // client로 메시지를 보낸다.

    otherClient oc;
    oc.clientsock = clientSock;
    oc.clientaddr = clientAddr;
    oc.iThrnum = thrnum;
    oc.sState = WAITING;
    vcOC.push_back(oc);

    while(vcOC[thrnum].sState == WAITING)
    {
        Sleep(1000);
        const char* message = "/noclient";
        send(clientSock, message, strlen(message) +1, 0);
    }

    const char* message = "Welcome Chating!\r\n>";
    send(clientSock, message, strlen(message) +1, 0);

    // 수신 데이터
    string buffer;
    char x[BUFFERSIZE];
    while (1)
    {
        memset(x, 0x00, sizeof(x));
        // 데이터를 받는다. 에러가 발생하면 멈춘다.
        if (recv(clientSock, x, sizeof(x), 0) == SOCKET_ERROR)
        {
            // 에러 콘솔 출력
            cout << "error" << endl;
			break;
		}
        buffer = x;
		
        // 메시지가 exit일 경우는 수신대기를 멈춘다.
		if (buffer.compare("exit") == 0)
			break;

        cout << "Send : " << thrnum << " | " << buffer << " -> " << vcOC[thrnum].iConnectedUser << endl;

		send(vcOC[thrnum].sendClientsock, buffer.c_str(), buffer.size(), 0);
		continue;
    }
    // 수신 대기가 끝나면 client와 소켓 통신을 끊는다.
    closesocket(clientSock);
    // 접속 정보를 콘솔에 출력한다.
    cout << "Client disconnected IP address = " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << endl;
    // threadlist에서 현재 쓰레드를 제거한다.

    //vcOC.erase(vcOC.begin() + thrnum);

    for (auto ptr = clientlist->begin(); ptr < clientlist->end(); ptr++)
    {
        // thread 아이디가 같은 것을 찾아서
        if ((*ptr)->get_id() == this_thread::get_id())
        {
            // 리스트에서 뺀다.
            clientlist->erase(ptr);
            break;
        }
    }
    // thread 메모리 해지는 thread가 종료 됨으로 자동으로 처리된다.
}
