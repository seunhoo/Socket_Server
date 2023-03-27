#pragma once

// ������ ����ϱ� ���ؼ� ���̺귯�� �����ؾ� �Ѵ�.
#pragma comment(lib, "ws2_32")
// inet_ntoa�� deprecated�� �Ǿ��µ�.. ����Ϸ��� �Ʒ� ������ �ؾ� �Ѵ�.
#pragma warning(disable:4996)
#include <stdio.h>
#include <iostream>
#include <vector>
#include <thread>
// ������ ����ϱ� ���� ���̺귯��
#include <WinSock2.h>
// ���� ���� ������
#define BUFFERSIZE 1024

// ��Ʈ
#define PORT 9090


using namespace std;

char* print(vector<char>* str);
void client(SOCKET clientSock, SOCKADDR_IN clientAddr, vector<thread*>* clientlist);
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
};

