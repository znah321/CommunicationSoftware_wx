#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <ctime>

using namespace std;
class Point {
public:
    string name;
    float* score;
    int cluster;
    Point() {};
    Point(string name, float* score, int cluster) {
        this->name = name;
        this->score = score;
        this->cluster = cluster;
    };
    Point* clone() {
        return new Point(*this);
    }
    ~Point() {};
    float distance(Point p);

    friend ostream& operator<<(ostream& os, Point p);
};

vector<Point> loadDataset(string filepath);
void normalize(vector<Point>& dataset);
void kmeans(vector<Point> dataset, int centroids, int iterations);

int main() {
    vector<Point> dataset = loadDataset("data.txt");
    normalize(dataset);
    kmeans(dataset, 3, 2);
    return 0;
}

vector<Point> loadDataset(string filepath) {
    /* �������ݼ��ļ� */
    ifstream inFile;
    inFile.open(filepath, ios_base::in);

    vector<Point> dataset;
    string name;
    float* score;
    while (!inFile.eof()) {
        score = new float[3];
        inFile >> name
               >> score[0] >> score[1] >> score[2];
        Point point(name, score, -1);
        dataset.push_back(point);
    }

    return dataset;
}

void normalize(vector<Point>& dataset) {
    /* ���ݹ�һ������ */
    for (int i = 0; i < 3; i++) {
        float minScore = 0;
        float maxScore = 0;
        // Step-1 �����ֵ����Сֵ
        vector<float> tempScores;
        for (int j = 0; j < dataset.size(); j++)
            tempScores.push_back(dataset[j].score[i]);
        auto minScorePos = min_element(tempScores.begin(), tempScores.end());
        minScore = *minScorePos;
        auto maxScorePos = max_element(tempScores.begin(), tempScores.end());
        maxScore = *maxScorePos;
        // Step-2 ��һ��
        for (int j = 0; j < dataset.size(); j++) {
            dataset[j].score[i] = (dataset[j].score[i] - minScore) / (maxScore - minScore);
            dataset[j].score[i] = round(dataset[j].score[i] * 100) / 100.0;
        }
    }
}

float Point::distance(Point p) {
    /* ����ŷ�Ͼ��� */
    float res = 0;
    for (int i = 0; i < 3; i++) {
        res += pow(this->score[i] - p.score[i], 2);
    }
    return sqrt(res);
}

ostream& operator<<(ostream& os, Point p) {
    cout << p.name << " ";
    cout << p.score[0] << "\t" << p.score[1] << "\t" << p.score[2] << endl;
    return os;
}

void kmeans(vector<Point> dataset, int centroids, int iterations) {
    /* kmeans�����㷨 */
    srand(time(NULL));;

    // Step-1 �������centroids�����ĵ�
    vector<Point> centers;
    vector<int> tempIdx;
    for (int i = 0; i < centroids; i++) {
        int tempValue = 0;
        while (true) {
            tempValue = rand() % 15;
            bool hasValue = (find(tempIdx.begin(), tempIdx.end(), tempValue) != tempIdx.end());
            if (!hasValue) {
                tempIdx.push_back(tempValue);
                float* centerPoint = new float[3];
                memcpy(centerPoint, dataset[tempValue].score, 3 * sizeof(float));
                centers.push_back(Point("NULL", centerPoint, i+1));
                break;
            }
        }
    }
    sort(tempIdx.begin(), tempIdx.end());
    // Step-2 ��ʼ����
    int it = 1;
    while (it <= iterations) {
        // �����ݼ��еĵ���з���
        for (int i = 0; i < dataset.size(); i++) {
            // ���������㵽ÿ�����ĵ�ľ���
            vector<float> distance;
            for (int j = 0; j < centers.size(); j++) {
                float dis = dataset[i].distance(centers[j]);
                distance.push_back(dis);
            }
            dataset[i].cluster = (int)(min_element(distance.begin(), distance.end()) - distance.begin()) + 1;
        }

        // �������ĵ�����
        float sumScores[3] = { 0, 0, 0 };
        for (int i = 0; i < centroids; i++) {
            int count = 0;
            for (int j = 0; j < dataset.size(); j++) {
                if (dataset[j].cluster == i + 1) {
                    sumScores[0] += dataset[j].score[0];
                    sumScores[1] += dataset[j].score[1];
                    sumScores[2] += dataset[j].score[2];
                    count++;
                }
            }
            centers[i].score[0] = sumScores[0] / count;
            centers[i].score[1] = sumScores[1] / count;
            centers[i].score[2] = sumScores[2] / count;
        }
        it++;
    }

    cout << "�������" << endl;
    for (int i = 0; i < centroids; i++) {
        cout << "��" << i + 1 << "��" << endl;
        for (int j = 0; j < dataset.size(); j++) {
            if (dataset[j].cluster == i + 1)
                cout << dataset[j].name << " ";
        }
        cout << endl;
    }
}