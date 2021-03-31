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
void getIntensity(vector<vector<Complex>> &dataset); // �����ź�ǿ��
void correlationAnalyze(vector<vector<Complex>> &dataset, vector<vector<Complex>> &pssset); // ������ؼ��
double getCorrelationValue(int k, int idx, int pos, vector<vector<Complex>> &dataset, vector<vector<Complex>> &pssset); // ���㵥�����ֵ

int main() {
    vector<vector<Complex>> dataSet; // ���ݼ�
    vector<vector<Complex>> pssSet; // PSS

    /* Step-1: ��ȡdata���ݺ�PSS���� */
    readDataSet(dataSet, "data");
    readDataSet(pssSet, "PSS");

    /* Step-2: �����ź�ǿ�Ȳ����� */
    getIntensity(dataSet);

    /* Step-3: ������ؼ�� */
    correlationAnalyze(dataSet, pssSet);

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
        inFile.open(filename, ios_base::in);

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
void getIntensity(vector<vector<Complex>> &dataset) {
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
        cout << "����Ϊ" << i + 1 << "��" << "С��" << intensity[i][0] << "��ǿ�ȣ�" << intensity[i][1] << endl;
}

void correlationAnalyze(vector<vector<Complex>> &dataset, vector<vector<Complex>> &pssset) {
    cout << endl << "--------------------������ؼ���--------------------" << endl;
    int dataSetSize = dataset.size();
    int pssSetSize = pssset.size();
    double result[dataSetSize][pssSetSize];
    for(int cnt = 0; cnt < dataSetSize; cnt++) {
        for(int pos = 0; pos < pssSetSize; pos++) {
            vector<double> tempCorrelation;
            int len = dataSetSize - pssSetSize; // ���г���
            for(int k = 0; k < len; k++)
                tempCorrelation.push_back(getCorrelationValue(k, cnt, pos, dataset, pssset));
            // �ҵ���ǰ���е����ֵ
            auto maxValue = max_element(tempCorrelation.begin(), tempCorrelation.end());
            result[cnt][pos] = *maxValue;
            cout << dataset[cnt][0].id << "��PSS" << pos << ".txt������ԣ�" << *maxValue << endl;
        }
        cout << endl;
    }
    // �ҳ��������ǿ��ֵ
    int maxResultRow = 0;
    int maxResultColumn = 0;
    double tempMaxResult = result[maxResultRow][maxResultColumn];
    for(int i = 0; i < dataSetSize; i++) {
        for(int j = 0; j < pssSetSize; j++) {
            if (result[i][j] > tempMaxResult) {
                maxResultRow = i;
                maxResultColumn = j;
                tempMaxResult = result[maxResultRow][maxResultColumn];
            }
        }
    }
    cout << "�������ǿ��Ϊ" << dataset[maxResultRow][0].id << "��PSS" << maxResultColumn + 1 << ".txt" << "�������Ϊ"
         << result[maxResultRow][maxResultColumn] << endl;
}

// ���㵥��������ؼ��ֵ
double getCorrelationValue(int k, int idx, int pos, vector<vector<Complex>> &dataset, vector<vector<Complex>> &pssset) {
    double sum = 0;
    for(int i = 0; i < pssset[pos].size(); i++)
        sum += inner_product(pssset[pos][i], dataset[idx][i+k]);
    return sum;
}