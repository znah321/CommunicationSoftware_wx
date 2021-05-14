#include <ws2tcpip.h>
#include <winsock2.h>
#include <iostream>
using namespace std;

#pragma comment(lib, "ws2_32.lib")

DWORD WINAPI Handler(LPVOID Socket); // 用于接收客户端消息的函数
SOCKET client[10]; // 用户列表
int cnt = 0; // 用户个数

int main() {

    //加载winsock2的环境
    WSADATA wd;
    if (0 != WSAStartup(MAKEWORD(2, 2), &wd)) {
        cout << "WSAStartup error : " << GetLastError() << endl;
        return 0;
    }

    // 创建Socket
    SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (INVALID_SOCKET == s) {
        cout << "socket error : " << GetLastError() << endl;
        return 0;
    }

    // 绑定IP和端口（IP为本机地址localhost，端口为3000）
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(3000);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr); // ip地址转网络字节序

    int len = sizeof(sockaddr_in);
    if (SOCKET_ERROR == bind(s, (SOCKADDR*)&addr, len)) {
        cout << "bind error : " << GetLastError() << endl;
        return 0;
    }

    // 进入监听状态
    listen(s, 5);

    //主线程循环接收客户端的链接
    while (true) {
        sockaddr_in addrClient;
        len = sizeof(sockaddr_in);

        // 调用阻塞函数accept()监听来自客户端的连接
        SOCKET c = accept(s, (SOCKADDR*)&addrClient, &len);
        client[cnt++] = c;
        if (INVALID_SOCKET != c) {
            // 创建线程，并且传入与客户端通讯的Socket
            HANDLE hThread = CreateThread(NULL, 0, Handler, (LPVOID)c, 0, NULL);
            CloseHandle(hThread); // 关闭对线程的引用
        }
    }

    // 关闭监听套接字
    closesocket(s);

    // 清理winsock2的环境
    WSACleanup();

    return 0;
}

DWORD WINAPI Handler(LPVOID Socket) {
    SOCKET c = (SOCKET) Socket;
    cout << "欢迎" << c << "进入聊天室！" << endl;

    // 发送数据
    char buf[100] = { 0 };
    sprintf_s(buf, "欢迎%d 进入聊天室!", c);
    send(c, buf, 100, 0);

    // 循环接收客户端数据
    int ret = 0;
    do
    {
        char buf[100] = { 0 };
        ret = recv(c, buf, 100, 0);
        if (ret == SOCKET_ERROR) {
            sprintf_s(buf, "离开了聊天室");
        }
        cout << c << "  说: " << buf << endl;

        // 向聊天室中所有用户（除去发送该条消息的用户）发送信息
        for (int i = 0; i < cnt; i++) {
            if (client[i] == c) {
                continue;
            }
            else {
                char msg[100] = {};
                sprintf_s(msg, "%d 说：%s", c, buf);
                send(client[i], msg, 100, 0);
            }
        }

    } while (ret != SOCKET_ERROR && ret != 0);

    return 0;
}