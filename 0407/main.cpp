#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

/* ������ */
class Complex {
public:
    string id; // �����ļ����ļ���
    double re; // ʵ��
    double im; // �鲿
    double len; // ģ��
    Complex(string id, double re, double im); // ���캯��
    ~Complex() {}; // ��������
};

double inner_product(Complex z1, Complex z2); // ���������ڻ�
void readDataSet(vector<vector<Complex>> &dataset, string type); // ��ȡ����
int getIntensity(vector<vector<Complex>> &dataset); // �����ź�ǿ��
void correlationAnalyze(vector<Complex> &dataset, vector<vector<Complex>> &pssset); // ������ؼ��
double getCorrelationValue(int k, int pos, vector<Complex> &dataset, vector<vector<Complex>> &pssset); // ���㵥�����ֵ

int main() {
    vector<vector<Complex>> dataSet; // ���ݼ�
    vector<vector<Complex>> pssSet; // PSS

    /* Step-1: ��ȡdata���ݺ�PSS���� */
    readDataSet(dataSet, "data");
    readDataSet(pssSet, "PSS");

    /* Step-2: �����ź�ǿ�Ȳ����� */
    int maxIdx = getIntensity(dataSet);

    /* Step-3: ������ؼ�� */
    correlationAnalyze(dataSet[maxIdx], pssSet);

    cout << endl << "Over!";
    system("pause");
    return 0;
}

Complex::Complex(string id, double re, double im) {
    this->id = id;
    this->re = re;
    this->im = im;
    this->len = sqrt(pow(this->re, 2) + pow(this->im, 2)); // ���㸴����ģ��
}

// ���������ڻ�
double inner_product(Complex z1, Complex z2) {
    return z1.re*z2.re + z1.im * z2.im;
}

// ��ȡ�����ļ�
void readDataSet(vector<vector<Complex>> &dataset, string type)
{
    string  raw_filename = "data\\" + type; // ƴ���ļ�·��
    cout << "Reading " << type << " ..." << endl;
    for(int i = 0; i < 100; i++) {
        // ���ļ�
        string filename = raw_filename + to_string(i) + ".txt";
        ifstream inFile;
        inFile.open(filename, ios_base::in); // filenameΪ�ļ���
        // ��ȡ����
        vector<Complex> temp_vector;
        string re, im;
        if (!inFile.fail()) {
            while (!inFile.eof()) {
                inFile >> re >> im;
                Complex z(filename.substr(5), atof(re.c_str()), atof(im.c_str()));
                temp_vector.push_back(z);
            }
        }
        if (!temp_vector.empty()) // temp_vectorΪ�մ�����ļ������ڣ��Ͳ��Ž�dataset��
            dataset.push_back(temp_vector);
        inFile.close();
    }
    cout << "Success!" << endl << endl;
}

// �����ź�ǿ��
int getIntensity(vector<vector<Complex>> &dataset) {
    int size = dataset.size();
    double intensity[size][2];
    cout << "--------------------����ǿ��--------------------" << endl;
    // ����ǿ��
    for(int cnt = 0; cnt < size; cnt++) {
        double sum = 0;
        for(int i = 0; i < dataset[cnt].size(); i++)
            sum += dataset[cnt][i].len;
        intensity[cnt][0] = cnt;
        intensity[cnt][1] = sum;
    }
    // ����
    double temp = 0;
    int tempIdx;
    for(int i = 0; i < size - 1; i++) {
        tempIdx = i;
        temp = intensity[tempIdx][1];
        for(int j = i; j < size; j++) {
            if (intensity[j][1] > temp) {
                tempIdx = j;
                temp = intensity[tempIdx][1];
            }
        }
        swap(intensity[i][1], intensity[tempIdx][1]);
        swap(intensity[i][0], intensity[tempIdx][0]);
    }
    // ������
    cout << setprecision(12); // �����������
    for(int i = 0; i < size; i++)
        cout << "����Ϊ" << i + 1 << "��" << "С��" << intensity[i][0] << "(" << dataset[intensity[i][0]][0].id << ")"
             << "��ǿ�ȣ�\t" << intensity[i][1] << endl;
    cout << endl << "ǿ������С�����Ϊ" << intensity[0][0] << "��ǿ��Ϊ��" << intensity[0][1] << endl;
    cout << "��Ӧ���ļ�Ϊ��" << dataset[intensity[0][0]][0].id << endl;
    return intensity[0][0];
}

void correlationAnalyze(vector<Complex> &dataset, vector<vector<Complex>> &pssset) {
    cout << endl << "--------------------������ؼ���--------------------" << endl;
    int dataSetSize = dataset.size();
    int pssSetSize = pssset.size();
    double result[pssSetSize][2];
    for (int pos = 0; pos < pssSetSize; pos++) {
        vector<double> tempCorrelation;
        int len = dataSetSize - pssset[0].size(); // ���г���
        for (int k = 0; k < len; k++)
            tempCorrelation.push_back(getCorrelationValue(k, pos, dataset, pssset));
        // �ҵ���ǰ���е����ֵ
        auto maxValue = max_element(tempCorrelation.begin(), tempCorrelation.end());
        vector<double>::iterator iter = find(tempCorrelation.begin(), tempCorrelation.end(), *maxValue);
        result[pos][0] = iter - tempCorrelation.begin();
        result[pos][1] = *maxValue;
    }
    double maxValue = max(result[0][1], max(result[1][1], result[2][1]));
    int maxValueIndex = 0;
    for(int i = 0; i < pssSetSize; i++) {
        if (result[i][1] == maxValue)
            maxValueIndex = i;
    }
    cout << "���������ֵΪ��" << maxValue << "��λ��Ϊ��" << result[0][maxValueIndex] << endl;
    cout << "��Ӧ��PSS�ļ�Ϊ��" << pssset[maxValueIndex][0].id << endl;
}

// ���㵥��������ؼ��ֵ
double getCorrelationValue(int k, int pos, vector<Complex> &dataset, vector<vector<Complex>> &pssset) {
    double sum = 0;
    for(int i = 0; i < pssset[pos].size(); i++)
        sum += inner_product(pssset[pos][i], dataset[i+k]);
    return sum;
}