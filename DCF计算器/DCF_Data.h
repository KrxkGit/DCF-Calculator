#pragma once
#include"resource.h"

class DCF_Data
{
public:
	DCF_Data();
	~DCF_Data();

private:
	double m_r;//������(*0.01)
	int m_nYear;//������������
	double m_G;//���������� ��*0.01��
	double m_g;//���������� ��*0.01��
	double m_FCF1;//�����ֽ���
	double m_EValue;//���ڼ�ֵ��ֵ
public:
	TCHAR m_szName[30];//��Ʊ����
private:
	double CalN(double a, int n);//����a��n�η�
public:
	VOID Calculate(HWND);//���ھ�����ڲ�����ȡ�й�
	VOID Save(HANDLE);//������Ϣ
	VOID SelectShare(HWND);//ѡ���Ʊ
};

