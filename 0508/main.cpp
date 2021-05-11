#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cstring>
#include "Bmp.h"

using namespace std;

WORD bfType; // 文件类型
BITMAPFILEHEADER bmpFileHeader; // 原始文件头
BITMAPINFOHEADER bmpInfoHeader; // 原始信息头
IMAGEDATA *imgData; // 原始像素信息
IMAGEDATA *newImgData; // 旋转后像素信息
int width; // 宽
int height; // 高

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
    cout << "===============位图文件头===============" << endl;
    cout << "文件大小：" << header.bfSize << endl;
    cout << "保留字-1：" << header.bfReserved1 << endl;
    cout << "保留字-2：" << header.bfReserved2 << endl;
    cout << "实际位图数据的偏移字节数：" << header.bfOffBits << endl << endl;
}

void showBmpInfoHeader(BITMAPINFOHEADER header) {
    cout << "===============位图信息头===============" << endl;
    cout << "位图信息头：" << endl;
    cout << "结构体的长度："<< header.biSize << endl;
    cout << "位图宽："<< header.biWidth << endl;
    cout << "位图高："<< header.biHeight << endl;
    cout << "biPlanes平面数："<< header.biPlanes << endl;
    cout << "biBitCount采用颜色位数：" << header.biBitCount << endl;
    cout << "压缩方式：" << header.biCompression << endl;
    cout << "biSizeImage实际位图数据占用的字节数：" << header.biSizeImage << endl;
    cout << "X方向分辨率：" << header.biXPelsPerMeter << endl;
    cout << "Y方向分辨率：" << header.biYPelsPerMeter << endl;
    cout << "使用的颜色数：" << header.biClrUsed << endl;
    cout << "重要颜色数：" << header.biClrImportant << endl;
}

void readBMPImage(char const* filename) {
    FILE *fp;
    fp = fopen(filename, "rb"); // 打开文件
    if (fp == NULL) {
        cout << "Can't open the file " << filename << "!" << endl;
        return;
    } else {
        // 读文件类型
        fread(&bfType, 1, sizeof(WORD), fp);
        if (bfType != 0x4d42) {
            cout << "This file is not .BMP file!" << endl;
            return;
        } else {
            // 读文件头和信息头
            fread(&bmpFileHeader, 1, sizeof(tagBITMAPFILEHEADER), fp);
            fread(&bmpInfoHeader, 1, sizeof(tagBITMAPINFOHEADER), fp);
            width = bmpInfoHeader.biWidth;
            height = bmpInfoHeader.biHeight;
            showBmpFileHeader(bmpFileHeader);
            showBmpInfoHeader(bmpInfoHeader);
        }
    }
    // 读原始像素信息
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

    // 修改旋转后的尺寸、宽度、高度
    newInfoHeader.biWidth = (DWORD) newWidth;
    newInfoHeader.biHeight = (DWORD) newHeight;

    // 获取需要补足的字节数
    int newSupCnt = getSupWordCount(newInfoHeader);
    newInfoHeader.biSizeImage = (DWORD) ((newWidth * 3 + newSupCnt) * newHeight);

    // 写文件头、信息头
    fwrite(&bfType, 1, sizeof(WORD), fp);
    fwrite(&newFileHeader, 1, sizeof(tagBITMAPFILEHEADER), fp);
    fwrite(&newInfoHeader, 1, sizeof(tagBITMAPINFOHEADER), fp);

    newImgData = new IMAGEDATA[newSize];
    for(int i = 0; i < newHeight; i++) {
        for(int j = 0; j < newWidth; j++) {
            *(newImgData + i * newWidth + j) = *(imgData + j * width + newHeight - i - 1);
        }
    }

    // 写入新像素
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