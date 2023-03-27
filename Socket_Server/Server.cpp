#include "Server.h"

vector<otherClient> vcOC;

Server::Server()
{

}

Server::~Server()
{
}

// ���� ���� �ʱ�ȭ
int Server::Initialize()
{
    // ���� ����.
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        return 0;
    }

    serverSock = socket(PF_INET, SOCK_STREAM, 0);

    // ����ü �ʱ�ȭ
    memset(&addr, 0, sizeof(addr));
    // ������ Internet Ÿ��
    addr.sin_family = AF_INET;
    // �����̱� ������ local �����Ѵ�.
    // Any�� ���� ȣ��Ʈ�� 127.0.0.1�� ��Ƶ� �ǰ� localhost�� ��Ƶ� �ǰ� ���� �� ����ϰ� �� �� �ֵ�. �װ��� INADDR_ANY�̴�.
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // ���� ��Ʈ ����...���� 9090���� ������.
    addr.sin_port = htons(PORT);

    iThrNum = 0;

    // ������ ���� ������ ���Ͽ� ���ε��Ѵ�.
    if (bind(serverSock, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
    {
        // ���� �ܼ� ���
        cout << "error" << endl;
        return 1;
    }

    // ������ ��� ���·� ��ٸ���.
    if (listen(serverSock, SOMAXCONN) == SOCKET_ERROR)
    {
        // ���� �ܼ� ���
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

// ���ӵǴ� client�� ������
void client(SOCKET clientSock, SOCKADDR_IN clientAddr, vector<thread*>* clientlist, int thrnum)
{
    // ���� ������ �ֿܼ� ����Ѵ�.
    cout << "Client connected Information = " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << " | Number : " << thrnum << endl;

    // client�� �޽����� ������.

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

    // ���� ������
    string buffer;
    char x[BUFFERSIZE];
    while (1)
    {
        memset(x, 0x00, sizeof(x));
        // �����͸� �޴´�. ������ �߻��ϸ� �����.
        if (recv(clientSock, x, sizeof(x), 0) == SOCKET_ERROR)
        {
            // ���� �ܼ� ���
            cout << "error" << endl;
			break;
		}
        buffer = x;
		
        // �޽����� exit�� ���� ���Ŵ�⸦ �����.
		if (buffer.compare("exit") == 0)
			break;

        cout << "Send : " << thrnum << " | " << buffer << " -> " << vcOC[thrnum].iConnectedUser << endl;

		send(vcOC[thrnum].sendClientsock, buffer.c_str(), buffer.size(), 0);
		continue;
    }
    // ���� ��Ⱑ ������ client�� ���� ����� ���´�.
    closesocket(clientSock);
    // ���� ������ �ֿܼ� ����Ѵ�.
    cout << "Client disconnected IP address = " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << endl;
    // threadlist���� ���� �����带 �����Ѵ�.

    //vcOC.erase(vcOC.begin() + thrnum);

    for (auto ptr = clientlist->begin(); ptr < clientlist->end(); ptr++)
    {
        // thread ���̵� ���� ���� ã�Ƽ�
        if ((*ptr)->get_id() == this_thread::get_id())
        {
            // ����Ʈ���� ����.
            clientlist->erase(ptr);
            break;
        }
    }
    // thread �޸� ������ thread�� ���� ������ �ڵ����� ó���ȴ�.
}
