#include <ws2tcpip.h>
#include <winsock2.h>
#include <iostream>
using namespace std;

#pragma comment(lib, "ws2_32.lib")

DWORD WINAPI Handler(LPVOID Socket); // ���ڽ��տͻ�����Ϣ�ĺ���
SOCKET client[10]; // �û��б�
int cnt = 0; // �û�����

int main() {

    //����winsock2�Ļ���
    WSADATA wd;
    if (0 != WSAStartup(MAKEWORD(2, 2), &wd)) {
        cout << "WSAStartup error : " << GetLastError() << endl;
        return 0;
    }

    // ����Socket
    SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (INVALID_SOCKET == s) {
        cout << "socket error : " << GetLastError() << endl;
        return 0;
    }

    // ��IP�Ͷ˿ڣ�IPΪ������ַlocalhost���˿�Ϊ3000��
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(3000);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr); // ip��ַת�����ֽ���

    int len = sizeof(sockaddr_in);
    if (SOCKET_ERROR == bind(s, (SOCKADDR*)&addr, len)) {
        cout << "bind error : " << GetLastError() << endl;
        return 0;
    }

    // �������״̬
    listen(s, 5);

    //���߳�ѭ�����տͻ��˵�����
    while (true) {
        sockaddr_in addrClient;
        len = sizeof(sockaddr_in);

        // ������������accept()�������Կͻ��˵�����
        SOCKET c = accept(s, (SOCKADDR*)&addrClient, &len);
        client[cnt++] = c;
        if (INVALID_SOCKET != c) {
            // �����̣߳����Ҵ�����ͻ���ͨѶ��Socket
            HANDLE hThread = CreateThread(NULL, 0, Handler, (LPVOID)c, 0, NULL);
            CloseHandle(hThread); // �رն��̵߳�����
        }
    }

    // �رռ����׽���
    closesocket(s);

    // ����winsock2�Ļ���
    WSACleanup();

    return 0;
}

DWORD WINAPI Handler(LPVOID Socket) {
    SOCKET c = (SOCKET) Socket;
    cout << "��ӭ" << c << "���������ң�" << endl;

    // ��������
    char buf[100] = { 0 };
    sprintf_s(buf, "��ӭ%d ����������!", c);
    send(c, buf, 100, 0);

    // ѭ�����տͻ�������
    int ret = 0;
    do
    {
        char buf[100] = { 0 };
        ret = recv(c, buf, 100, 0);
        if (ret == SOCKET_ERROR) {
            sprintf_s(buf, "�뿪��������");
        }
        cout << c << "  ˵: " << buf << endl;

        // ���������������û�����ȥ���͸�����Ϣ���û���������Ϣ
        for (int i = 0; i < cnt; i++) {
            if (client[i] == c) {
                continue;
            }
            else {
                char msg[100] = {};
                sprintf_s(msg, "%d ˵��%s", c, buf);
                send(client[i], msg, 100, 0);
            }
        }

    } while (ret != SOCKET_ERROR && ret != 0);

    return 0;
}