#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cstring>
#include "Bmp.h"

using namespace std;

WORD bfType; // �ļ�����
BITMAPFILEHEADER bmpFileHeader; // ԭʼ�ļ�ͷ
BITMAPINFOHEADER bmpInfoHeader; // ԭʼ��Ϣͷ
IMAGEDATA *imgData; // ԭʼ������Ϣ
IMAGEDATA *newImgData; // ��ת��������Ϣ
int width; // ��
int height; // ��

void showBmpFileHeader(BITMAPFILEHEADER header);
void showBmpInfoHeader(BITMAPINFOHEADER header);
void readBMPImage(char const* filename);
int getSupWordCount(BITMAPINFOHEADER & info);
void rotateBmpFile();

int main() {
    const char *filename = "1.bmp";
    readBMPImage(filename);
    rotateBmpFile();
    return 0;
}

void showBmpFileHeader(BITMAPFILEHEADER header) {
    cout << "===============λͼ�ļ�ͷ===============" << endl;
    cout << "�ļ���С��" << header.bfSize << endl;
    cout << "������-1��" << header.bfReserved1 << endl;
    cout << "������-2��" << header.bfReserved2 << endl;
    cout << "ʵ��λͼ���ݵ�ƫ���ֽ�����" << header.bfOffBits << endl << endl;
}

void showBmpInfoHeader(BITMAPINFOHEADER header) {
    cout << "===============λͼ��Ϣͷ===============" << endl;
    cout << "λͼ��Ϣͷ��" << endl;
    cout << "�ṹ��ĳ��ȣ�"<< header.biSize << endl;
    cout << "λͼ��"<< header.biWidth << endl;
    cout << "λͼ�ߣ�"<< header.biHeight << endl;
    cout << "biPlanesƽ������"<< header.biPlanes << endl;
    cout << "biBitCount������ɫλ����" << header.biBitCount << endl;
    cout << "ѹ����ʽ��" << header.biCompression << endl;
    cout << "biSizeImageʵ��λͼ����ռ�õ��ֽ�����" << header.biSizeImage << endl;
    cout << "X����ֱ��ʣ�" << header.biXPelsPerMeter << endl;
    cout << "Y����ֱ��ʣ�" << header.biYPelsPerMeter << endl;
    cout << "ʹ�õ���ɫ����" << header.biClrUsed << endl;
    cout << "��Ҫ��ɫ����" << header.biClrImportant << endl;
}

void readBMPImage(char const* filename) {
    FILE *fp;
    fp = fopen(filename, "rb"); // ���ļ�
    if (fp == NULL) {
        cout << "Can't open the file " << filename << "!" << endl;
        return;
    } else {
        // ���ļ�����
        fread(&bfType, 1, sizeof(WORD), fp);
        if (bfType != 0x4d42) {
            cout << "This file is not .BMP file!" << endl;
            return;
        } else {
            // ���ļ�ͷ����Ϣͷ
            fread(&bmpFileHeader, 1, sizeof(tagBITMAPFILEHEADER), fp);
            fread(&bmpInfoHeader, 1, sizeof(tagBITMAPINFOHEADER), fp);
            width = bmpInfoHeader.biWidth;
            height = bmpInfoHeader.biHeight;
            showBmpFileHeader(bmpFileHeader);
            showBmpInfoHeader(bmpInfoHeader);
        }
    }
    // ��ԭʼ������Ϣ
    int size = width * height;
    imgData = new IMAGEDATA[size];
    int supCnt = getSupWordCount(bmpInfoHeader);
    for(int i = 0; i < height; i++) {
        fread((char*) imgData + i * width * 3, 3, width, fp);
        fseek(fp, supCnt, SEEK_CUR);
    }

    fclose(fp);
}

int getSupWordCount(BITMAPINFOHEADER & info) {
    int wordCountPerLine = (info.biWidth * info.biBitCount + 31) / 8;
    wordCountPerLine -= wordCountPerLine % 4;
    return wordCountPerLine - info.biWidth * info.biBitCount / 8;
}

void rotateBmpFile() {
    int newWidth = height;
    int newHeight = width;
    int newSize = newWidth * newHeight;

    FILE *fp;
    fp = fopen("1_new.bmp", "wb");
    BITMAPFILEHEADER newFileHeader = bmpFileHeader;
    BITMAPINFOHEADER newInfoHeader = bmpInfoHeader;

    // �޸���ת��ĳߴ硢��ȡ��߶�
    newInfoHeader.biWidth = (DWORD) newWidth;
    newInfoHeader.biHeight = (DWORD) newHeight;

    // ��ȡ��Ҫ������ֽ���
    int newSupCnt = getSupWordCount(newInfoHeader);
    newInfoHeader.biSizeImage = (DWORD) ((newWidth * 3 + newSupCnt) * newHeight);

    // д�ļ�ͷ����Ϣͷ
    fwrite(&bfType, 1, sizeof(WORD), fp);
    fwrite(&newFileHeader, 1, sizeof(tagBITMAPFILEHEADER), fp);
    fwrite(&newInfoHeader, 1, sizeof(tagBITMAPINFOHEADER), fp);

    newImgData = new IMAGEDATA[newSize];
    for(int i = 0; i < newHeight; i++) {
        for(int j = 0; j < newWidth; j++) {
            *(newImgData + i * newWidth + j) = *(imgData + j * width + newHeight - i - 1);
        }
    }

    // д��������
    char sup = '0';
    for(int i = 0; i < newHeight; i++) {
        fwrite((char *) newImgData + i * newWidth * 3, 3, newWidth, fp);
        fwrite(&sup, 1, newSupCnt, fp);
    }

    fclose(fp);
    delete[] imgData;
    delete[] newImgData;
    showBmpFileHeader(newFileHeader);
    showBmpInfoHeader(newInfoHeader);
}