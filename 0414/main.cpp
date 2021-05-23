#include <iostream>
#include "drawGraph_MATLAB.h"
using namespace std;

void initGraph(int** graph, int n); // 读入一个无向图，以邻接矩阵形式储存
void drawGraphByMatlab(int** graph, int n, int cnt); // 绘图

int main() {
    int n;
    cout << "请输入顶点的个数：";
    cin >> n;
    // 无向图初始化
    int *(*graph) = new int*[n];
    for(int i = 0; i < n; i++) {
        graph[i] = new int[n];
        for(int j = 0; j < n; j++)
            graph[i][j] = 0;
    }
    initGraph(graph, n);

    // 统计边的个数
    int cnt = 0;
    for(int i = 0; i < n; i++) {
        for(int j = i; j < n; j++) {
            if (graph[i][j] != 0)
                cnt++;
        }
    }

    // 绘图
    drawGraphByMatlab(graph, n, cnt);
    return 0;
}

void initGraph(int** graph, int n) {
    graph[0][1] = 2;
    graph[0][2] = 4;
    graph[0][3] = 22;
    graph[1][0] = 2;
    graph[1][2] = 1;
    graph[1][3] = 6;
    graph[2][0] = 4;
    graph[2][1] = 1;
    graph[2][3] = 1;
    graph[2][4] = 4;
    graph[3][0] = 22;
    graph[3][1] = 6;
    graph[3][2] = 1;
    graph[3][4] = 10;
    graph[3][5] = 5;
    graph[4][2] = 4;
    graph[4][3] = 10;
    graph[4][5] = 3;
    graph[5][3] = 5;
    graph[5][4] = 3;
    /*
    int rowCnt = 0;
    int colCnt = 0;
    while (rowCnt < n) {
        rowCnt++;
        colCnt = 0;
        while (colCnt < n) {
            cout << "请输入与第" << rowCnt << "个顶点相连的顶点编号（输入-1结束）：" << endl;
            int pos;
            cin >> pos;
            if (pos == -1) {
                break;
            } else if (pos > n || pos <= 0){
                cout << "顶点编号不合法！" << endl;
                continue;
            }

            cout << "请输入两个顶点间的权值：" << endl;
            int weight;
            cin >> weight;
            if (weight <= 0) {
                cout << "权值必须为正整数！" << endl;
            } else {
                graph[rowCnt - 1][pos - 1] = weight;
                graph[pos - 1][rowCnt - 1] = weight;
                colCnt++;
            }
        }
    }
     */
    system("cls");
    cout << "邻接矩阵为：" << endl;
    cout << "\t";
    for(int i = 0; i < n; i++)
        cout << i + 1 << "\t";
    cout << endl;
    for(int i = 0; i < n; i++) {
        cout << i + 1 << "\t";
        for(int j = 0; j < n; j++) {
            cout << graph[i][j] << "\t";
        }
        cout << endl;
    }
}

void drawGraphByMatlab(int** graph, int n, int cnt) {
    int* s = new int[cnt];
    int* t = new int[cnt];
    int* w = new int[cnt];
    int pos = 0;
    for(int i = 0; i < n && pos < cnt; i++) {
        for(int j = i; j < n && pos < cnt; j++) {
            if (graph[i][j] != 0) {
                s[pos] = i + 1;
                t[pos] = j + 1;
                w[pos] = graph[i][j];
                pos++;
            }
        }
    }

    // 初始化drawGraph函数
    if (!drawGraph_MATLABInitialize()) {
        exit(-1);
    }

    mwArray s_mat(1, cnt, mxDOUBLE_CLASS);
    s_mat.SetData(s, cnt);
    mwArray t_mat(1, cnt, mxDOUBLE_CLASS);
    t_mat.SetData(t, cnt);
    mwArray w_mat(1, cnt, mxDOUBLE_CLASS);
    w_mat.SetData(w, cnt);

    drawGraph(s_mat, t_mat, w_mat);

    system("pause");

    drawGraph_MATLABTerminate();
}