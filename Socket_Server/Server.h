#pragma once

// 소켓을 사용하기 위해서 라이브러리 참조해야 한다.
#pragma comment(lib, "ws2_32")
// inet_ntoa가 deprecated가 되었는데.. 사용하려면 아래 설정을 해야 한다.
#pragma warning(disable:4996)
#include <stdio.h>
#include <iostream>
#include <vector>
#include <thread>
#include <queue>
// 소켓을 사용하기 위한 라이브러리
#include <WinSock2.h>
using namespace std;


// 수신 버퍼 사이즈
#define BUFFERSIZE 8196

// 포트
#define PORT 9090

// MAX CLIENT 
#define MAXCLIENT 100

// other client 

enum State
{
	WAITING,
	CONNECTING,
};

struct otherClient
{
	SOCKET clientsock;
	SOCKET sendClientsock;
	SOCKADDR_IN clientaddr;

	int iThrnum;
	int iConnectedUser;
	State sState;
};

void ChatingRelayServer();
void client(SOCKET clientSock, SOCKADDR_IN clientAddr, vector<thread*>* clientlist, int thrnum);

class Server
{
public:

	Server();
	~Server();

	int Initialize();


	// 클라이언트 접속 중인 client list
	vector<thread*> clientlist;
	// 소켓 정보 데이터 설정
	WSADATA wsaData;
	// Internet의 Stream 방식으로 소켓 생성
	SOCKET serverSock;
	// 소켓 주소 설정
	SOCKADDR_IN addr;

	// thr 넘버 ( client 번호 )
	int iThrNum;
};

