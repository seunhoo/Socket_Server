#pragma once

// ������ ����ϱ� ���ؼ� ���̺귯�� �����ؾ� �Ѵ�.
#pragma comment(lib, "ws2_32")
// inet_ntoa�� deprecated�� �Ǿ��µ�.. ����Ϸ��� �Ʒ� ������ �ؾ� �Ѵ�.
#pragma warning(disable:4996)
#include <stdio.h>
#include <iostream>
#include <vector>
#include <thread>
#include <queue>
// ������ ����ϱ� ���� ���̺귯��
#include <WinSock2.h>
using namespace std;


// ���� ���� ������
#define BUFFERSIZE 8196

// ��Ʈ
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


	// Ŭ���̾�Ʈ ���� ���� client list
	vector<thread*> clientlist;
	// ���� ���� ������ ����
	WSADATA wsaData;
	// Internet�� Stream ������� ���� ����
	SOCKET serverSock;
	// ���� �ּ� ����
	SOCKADDR_IN addr;

	// thr �ѹ� ( client ��ȣ )
	int iThrNum;
};

