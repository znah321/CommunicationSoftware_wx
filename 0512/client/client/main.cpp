
#include <winsock2.h>
#include <WS2tcpip.h>
#include <iostream>
using  namespace std;

// ����dll��lib
#pragma comment(lib, "ws2_32.lib")
DWORD WINAPI RecvMessage(LPVOID lpThread);

int  main() {

    // ����winsock2�Ļ���
    WSADATA wd;
    if (WSAStartup(MAKEWORD(2, 2), &wd) != 0) {
        cout << "WSAStartup  error��" << GetLastError() << endl;
        return 0;
    }

    // ����Socket������TCPЭ��
    SOCKET  s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s == INVALID_SOCKET) {
        cout << "Socket  error��" << GetLastError() << endl;
        return 0;
    }

    // ���ӷ���������������ַΪlocalhost���˿�Ϊ8891��
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(3000);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);// ip��ַת�����ֽ���

    int len = sizeof(sockaddr_in);
    if (connect(s, (SOCKADDR*)&addr, len) == SOCKET_ERROR) {
        cout << "Connect  error��" << GetLastError() << endl;
        return 0;
    }

    // ���շ���˵���Ϣ
    char buf[100] = { 0 };
    recv(s, buf, 100, 0);
    cout << buf << endl;

    // ����һ���̣߳����ڽ��շ���˵���Ϣ
    HANDLE hThread = CreateThread(NULL, 0, RecvMessage, (LPVOID)s, 0, NULL);
    CloseHandle(hThread);

    // �����û��������Ϣ
    int  ret = 0;
    do {
        char buf[100] = { 0 };
        cout << "��������������:";
        cin >> buf;
        ret = send(s, buf, 100, 0);
    } while (ret != SOCKET_ERROR && ret != 0);


    // �ر�Socket
    closesocket(s);

    // ����winsock2�Ļ���
    WSACleanup();

    return 0;
}

DWORD WINAPI RecvMessage(LPVOID lpThread) {
    SOCKET s = (SOCKET)lpThread;
    int ret = 0;
    do
    {
        char buf[100] = { 0 };
        ret = recv(s, buf, 100, 0);
        cout << '\n' << buf<< endl;

    } while (ret != SOCKET_ERROR && ret != 0);

    return 0;
}

