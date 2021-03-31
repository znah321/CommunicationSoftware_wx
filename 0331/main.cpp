#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

/* 复数类 */
class Complex {
public:
    string id; // 所属文件的文件名
    double re; // 实部
    double im; // 虚部
    double len; // 模长
    Complex(string id, double re, double im); // 构造函数
    ~Complex() {}; // 析构函数
};

double inner_product(Complex z1, Complex z2); // 计算向量内积
void readDataSet(vector<vector<Complex>> &dataset, string type); // 读取数据
void getIntensity(vector<vector<Complex>> &dataset); // 计算信号强度
void correlationAnalyze(vector<vector<Complex>> &dataset, vector<vector<Complex>> &pssset); // 滑动相关检测
double getCorrelationValue(int k, int idx, int pos, vector<vector<Complex>> &dataset, vector<vector<Complex>> &pssset); // 计算单个相关值

int main() {
    vector<vector<Complex>> dataSet; // 数据集
    vector<vector<Complex>> pssSet; // PSS

    /* Step-1: 读取data数据和PSS数据 */
    readDataSet(dataSet, "data");
    readDataSet(pssSet, "PSS");

    /* Step-2: 计算信号强度并排序 */
    getIntensity(dataSet);

    /* Step-3: 滑动相关检测 */
    correlationAnalyze(dataSet, pssSet);

    cout << endl << "Over!";
    system("pause");
    return 0;
}

Complex::Complex(string id, double re, double im) {
    this->id = id;
    this->re = re;
    this->im = im;
    this->len = sqrt(pow(this->re, 2) + pow(this->im, 2)); // 计算复数的模长
}

// 计算向量内积
double inner_product(Complex z1, Complex z2) {
    return z1.re*z2.re + z1.im * z2.im;
}

// 读取数据文件
void readDataSet(vector<vector<Complex>> &dataset, string type)
{
    string  raw_filename = "data\\" + type; // 拼接文件路径
    cout << "Reading " << type << " ..." << endl;
    for(int i = 0; i < 100; i++) {
        // 打开文件
        string filename = raw_filename + to_string(i) + ".txt";
        ifstream inFile;
        inFile.open(filename, ios_base::in);

        // 读取数据
        vector<Complex> temp_vector;
        string re, im;
        if (!inFile.fail()) {
            while (!inFile.eof()) {
                inFile >> re >> im;
                Complex z(filename.substr(5), atof(re.c_str()), atof(im.c_str()));
                temp_vector.push_back(z);
            }
        }
        if (!temp_vector.empty()) // temp_vector为空代表该文件不存在，就不放进dataset中
            dataset.push_back(temp_vector);
        inFile.close();
    }
    cout << "Success!" << endl << endl;
}

// 计算信号强度
void getIntensity(vector<vector<Complex>> &dataset) {
    int size = dataset.size();
    double intensity[size][2];
    cout << "--------------------计算强度--------------------" << endl;
    // 计算强度
    for(int cnt = 0; cnt < size; cnt++) {
        double sum = 0;
        for(int i = 0; i < dataset[cnt].size(); i++)
            sum += dataset[cnt][i].len;
        intensity[cnt][0] = cnt;
        intensity[cnt][1] = sum;
    }
    // 排序
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
    // 输出结果
    cout << setprecision(12); // 设置输出精度
    for(int i = 0; i < size; i++)
        cout << "排名为" << i + 1 << "，" << "小区" << intensity[i][0] << "的强度：" << intensity[i][1] << endl;
}

void correlationAnalyze(vector<vector<Complex>> &dataset, vector<vector<Complex>> &pssset) {
    cout << endl << "--------------------滑动相关计算--------------------" << endl;
    int dataSetSize = dataset.size();
    int pssSetSize = pssset.size();
    double result[dataSetSize][pssSetSize];
    for(int cnt = 0; cnt < dataSetSize; cnt++) {
        for(int pos = 0; pos < pssSetSize; pos++) {
            vector<double> tempCorrelation;
            int len = dataSetSize - pssSetSize; // 序列长度
            for(int k = 0; k < len; k++)
                tempCorrelation.push_back(getCorrelationValue(k, cnt, pos, dataset, pssset));
            // 找到当前序列的最大值
            auto maxValue = max_element(tempCorrelation.begin(), tempCorrelation.end());
            result[cnt][pos] = *maxValue;
            cout << dataset[cnt][0].id << "与PSS" << pos << ".txt的相关性：" << *maxValue << endl;
        }
        cout << endl;
    }
    // 找出相关性最强的值
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
    cout << "相关性最强的为" << dataset[maxResultRow][0].id << "与PSS" << maxResultColumn + 1 << ".txt" << "，相关性为"
         << result[maxResultRow][maxResultColumn] << endl;
}

// 计算单个滑动相关检测值
double getCorrelationValue(int k, int idx, int pos, vector<vector<Complex>> &dataset, vector<vector<Complex>> &pssset) {
    double sum = 0;
    for(int i = 0; i < pssset[pos].size(); i++)
        sum += inner_product(pssset[pos][i], dataset[idx][i+k]);
    return sum;
}