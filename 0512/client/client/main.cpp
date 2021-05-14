
#include <winsock2.h>
#include <WS2tcpip.h>
#include <iostream>
using  namespace std;

// 链接dll的lib
#pragma comment(lib, "ws2_32.lib")
DWORD WINAPI RecvMessage(LPVOID lpThread);

int  main() {

    // 加载winsock2的环境
    WSADATA wd;
    if (WSAStartup(MAKEWORD(2, 2), &wd) != 0) {
        cout << "WSAStartup  error：" << GetLastError() << endl;
        return 0;
    }

    // 创建Socket，基于TCP协议
    SOCKET  s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s == INVALID_SOCKET) {
        cout << "Socket  error：" << GetLastError() << endl;
        return 0;
    }

    // 连接服务器（服务器地址为localhost，端口为8891）
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(3000);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);// ip地址转网络字节序

    int len = sizeof(sockaddr_in);
    if (connect(s, (SOCKADDR*)&addr, len) == SOCKET_ERROR) {
        cout << "Connect  error：" << GetLastError() << endl;
        return 0;
    }

    // 接收服务端的消息
    char buf[100] = { 0 };
    recv(s, buf, 100, 0);
    cout << buf << endl;

    // 启动一个线程，用于接收服务端的消息
    HANDLE hThread = CreateThread(NULL, 0, RecvMessage, (LPVOID)s, 0, NULL);
    CloseHandle(hThread);

    // 处理用户输入的消息
    int  ret = 0;
    do {
        char buf[100] = { 0 };
        cout << "请输入聊天内容:";
        cin >> buf;
        ret = send(s, buf, 100, 0);
    } while (ret != SOCKET_ERROR && ret != 0);


    // 关闭Socket
    closesocket(s);

    // 清理winsock2的环境
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

