#include <iostream>
#include <fstream>
#include <list>
#include <algorithm>
#define INFINITY 65535

using namespace std;
int** initGraph(string filename, int* vectorCnt); // ��ʼ��һ��ͼ
void Dijkstra(int** G, int n); // Dijkstra�㷨�����·��

int main() {
    int vectorCnt; // �������
    int *(*G) = initGraph("graph.txt", &vectorCnt);
    Dijkstra(G, vectorCnt);
    return 0;
}

int** initGraph(string filename, int* vectorCnt) {
    ifstream inFile;
    inFile.open(filename, ios_base::in);
    int x_pos = 0, y_pos = 0, weight = 0, n = 0;
    inFile >> n;
    int *(*G) = new int*[n];
    for(int i = 0; i < n; i++) {
        G[i] = new int[n];
        for(int j = 0; j < n; j++)
            G[i][j] = INFINITY;
    }
    while (!inFile.eof()) {
        inFile >> x_pos;
        x_pos--;
        inFile >> y_pos;
        y_pos--;
        inFile >> weight;
        G[x_pos][y_pos] = weight;
    }
    cout << "�ڽӾ���Ϊ��" << endl;
    cout << "\t";
    for(int i = 0; i < n; i++)
        cout << i + 1 << "\t";
    cout << endl;
    for(int i = 0; i < n; i++) {
        cout << i + 1 << "\t";
        for(int j = 0; j < n; j++) {
            if (G[i][j] == INFINITY) {
                cout << "��" << "\t";
            } else {
                cout << G[i][j] << "\t";
            }
        }
        cout << endl;
    }
    inFile.close();
    *vectorCnt = n;
    return G;
}

void Dijkstra(int** G, int n) {
    cout << "===================ʹ��Dijkstra�㷨�������·��===================" << endl;
    list<int> T;
    int* dis = new int[n];
    string paths[n];
    for(int i = 0; i < n; i++) {
        dis[i] = INFINITY;
        paths[i] = "1 --> " + to_string(i + 1);
    }
    // Step 1-��ʼ��
    dis[0] = 0;
    for(int i = 0; i < n; i++) {
        if (G[0][i] != 0) {
            dis[i] = G[0][i];
        }
    }
    T.push_back(1);
    while (T.size() < n) {
        // ��dis�����е���Сֵ
        int min = INFINITY, index = 0;
        for(int i = 0; i < n; i++) {
            bool hasVector = (find(T.begin(), T.end(), i + 1) != T.end());
            if (dis[i] < min && !hasVector) {
                index = i;
                min = dis[i];
            }
        }
        string newPath = paths[index];
        //min == INFINITY ˵���㷨�Ѿ���������Щ����Դ���޷������
        if (min == INFINITY) {
            break;
        }
        // �ŵ�����T����
        T.push_back(index + 1);
        // ����dis
        for(int i = 0; i < n; i++) {
            if (G[index][i] != INFINITY) {
                if (min + G[index][i] < dis[i]) {
                    dis[i] = min + G[index][i];
                    paths[i] = newPath + " --> " + to_string(i + 1);
                }
            }
        }
    }
    // ������·��
    for(int i = 0; i < paths->size() - 1; i++) {
        replace(paths[i].begin(), paths[i].end(), '1', 's');
        replace(paths[i].begin(), paths[i].end(), '2', 'a');
        replace(paths[i].begin(), paths[i].end(), '3', 'b');
        replace(paths[i].begin(), paths[i].end(), '4', 'e');
        replace(paths[i].begin(), paths[i].end(), '5', 'd');
        replace(paths[i].begin(), paths[i].end(), '6', 'c');
        if (i == 0)
            continue;
        else {
            if (dis[i] == INFINITY) {
                cout << "Դ��s������" << paths[i][paths[i].size() - 1] << "���������·��" << endl;
            } else {
                cout << "Դ��s������" << paths[i][paths[i].size() - 1] << "�����·��Ϊ��" << dis[i] << "��";
                cout << "·��Ϊ��" << paths[i] << endl;
            }
        }
    }
}
