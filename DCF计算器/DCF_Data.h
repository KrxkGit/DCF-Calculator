#pragma once
#include"resource.h"

class DCF_Data
{
public:
	DCF_Data();
	~DCF_Data();

private:
	double m_r;//折现率(*0.01)
	int m_nYear;//快速增长年限
	double m_G;//快速增长率 （*0.01）
	double m_g;//永续增长率 （*0.01）
	double m_FCF1;//首年现金流
	double m_EValue;//内在价值估值
public:
	TCHAR m_szName[30];//股票名称
private:
	double CalN(double a, int n);//计算a的n次方
public:
	VOID Calculate(HWND);//窗口句柄用于参数获取有关
	VOID Save(HANDLE);//保存信息
	VOID SelectShare(HWND);//选择股票
};

