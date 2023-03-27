#include "Server.h"

int main()
{
    // Server Ŭ���� ����
    Server* cServer = new Server();

    // ���� �ʱ� ����
    if (!cServer->Initialize())
    {
        return 0;
    }

    // ������ �����Ѵ�.
    cout << "Server Start" << endl;

    // ê�� �߰� ���� ����
    auto thrChating = new thread(ChatingRelayServer);

    // ���� ������ ���� while�� ������ ����Ѵ�.
    while (1)
    {
        // ���� ���� ����ü ������
        int len = sizeof(SOCKADDR_IN);
        // ���� ���� ����ü
        SOCKADDR_IN clientAddr;
        // client�� ������ �ϸ� SOCKET�� �޴´�.
        SOCKET clientSock = accept(cServer->serverSock, (SOCKADDR*)&clientAddr, &len);
        // �����带 �����ϰ� ������ ����Ʈ�� �ִ´�.
        auto thr = new thread(client, clientSock, clientAddr, &cServer->clientlist, cServer->iThrNum++);
        cServer->clientlist.push_back(thr);
    }
    // ���ᰡ �Ǹ� ������ ����Ʈ�� ���� �ִ� �����带 ������ ������ ��ٸ���.
    for (auto ptr = cServer->clientlist.begin(); ptr < cServer->clientlist.end(); ptr++)
    {
        (*ptr)->join();
    }
    // ���� ���� ����
    closesocket(cServer->serverSock);
    // ���� ����
    WSACleanup();
    delete cServer;
    return 0;
}
