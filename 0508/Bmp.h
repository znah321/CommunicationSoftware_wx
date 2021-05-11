#ifndef INC_0508_BMP_H
#define INC_0508_BMP_H

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef long LONG;

// λͼ�ļ�ͷ
typedef struct tagBITMAPFILEHEADER{
    //WORD bfType;//�ļ����ͣ�������0x424D�����ַ���BM��
    DWORD bfSize;//�ļ���С
    WORD bfReserved1;//������
    WORD bfReserved2;//������
    DWORD bfOffBits;//���ļ�ͷ��ʵ��λͼ���ݵ�ƫ���ֽ���
}BITMAPFILEHEADER;

// λͼ��Ϣͷ
typedef struct tagBITMAPINFOHEADER{
    DWORD biSize;//��Ϣͷ��С
    LONG biWidth;//ͼ����
    LONG biHeight;//ͼ��߶�
    WORD biPlanes;//λƽ����������Ϊ1
    WORD biBitCount;//ÿ����λ��
    DWORD  biCompression; //ѹ������
    DWORD  biSizeImage; //ѹ��ͼ���С�ֽ���
    LONG  biXPelsPerMeter; //ˮƽ�ֱ���
    LONG  biYPelsPerMeter; //��ֱ�ֱ���
    DWORD  biClrUsed; //λͼʵ���õ���ɫ����
    DWORD  biClrImportant; //��λͼ����Ҫ��ɫ����
}BITMAPINFOHEADER;

//������Ϣ
typedef struct tagIMAGEDATA
{
    BYTE blue;
    BYTE green;
    BYTE red;
}IMAGEDATA;

#endif //INC_0508_BMP_H
