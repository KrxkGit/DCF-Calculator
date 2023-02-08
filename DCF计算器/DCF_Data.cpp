#include "stdafx.h"
#include "DCF_Data.h"

DCF_Data::DCF_Data()
{
	m_szName[0] = _T('\0');
	m_r = 0;
	m_nYear = 0;
	m_G = 0;
	m_g = 0;
	m_FCF1 = 0;
	m_EValue = 0;
}

DCF_Data::~DCF_Data()
{
}

VOID DCF_Data::Calculate(HWND hDlg)
{
	HWND hCombo = GetDlgItem(hDlg, IDC_COMBO1);
	GetDlgItemText(hDlg, IDC_COMBO1, m_szName, _countof(m_szName));

	m_r = 0.01*(double)GetDlgItemInt(hDlg, IDC_EDIT1, NULL, FALSE);//折现率(*0.01)
	m_nYear = GetDlgItemInt(hDlg, IDC_EDIT2, NULL, FALSE);//快速增长年限
	m_G = 0.01*(double)GetDlgItemInt(hDlg, IDC_EDIT3, NULL, FALSE);//快速增长率 （*0.01）
	m_g = 0.01*(double)GetDlgItemInt(hDlg, IDC_EDIT4, NULL, FALSE);//永续增长率 （*0.01）
	m_FCF1 = GetDlgItemInt(hDlg, IDC_EDIT6, NULL, FALSE);//首年现金流
	/*检查参数*/
	if (m_r == 0 || m_nYear == 0 || m_G == 0 || m_g == 0 || m_FCF1<0) {
		MessageBox(NULL, _T("参数错误！"), _T("DCF估值模型"), MB_ICONERROR);
		return;
	}

	double p = (1 + m_G) / (1 + m_r);
	if (p != 1) {
		m_EValue = m_FCF1*(((((1 - CalN(p, m_nYear + 1)) / (1 - p))) / (1 + m_r)) + (CalN(1 + m_G, m_nYear)*(1 + m_g) / ((m_r - m_g)*CalN(1 + m_r, m_nYear))));
	}
	else if (p == 1) {//快速增长率与折现率相同，不能套用等比数列求和公式
		m_EValue = m_FCF1*(m_nYear + (CalN(1 + m_G, m_nYear)*(1 + m_g) / ((m_r - m_g)*CalN(1 + m_r, m_nYear))));
	}
	SetDlgItemInt(hDlg, IDC_EDIT5, (int)m_EValue, TRUE);	 //考虑到单位较大，舍去小数
}

double DCF_Data::CalN(double a, int n)//计算a的n次方
{
	double a0 = a;
	for (n -= 1; n; n--) {
		a = a0*a;
	}
	return a;
}

VOID DCF_Data::SelectShare(HWND hDlg)
{
	SetDlgItemInt(hDlg, IDC_EDIT1, (UINT)(m_r*100), TRUE);//折现率(*0.01)
	SetDlgItemInt(hDlg, IDC_EDIT2, (UINT)m_nYear, TRUE);//快速增长年限
	SetDlgItemInt(hDlg, IDC_EDIT3, (UINT)(m_G*100), TRUE);//快速增长率 （*0.01）
	SetDlgItemInt(hDlg, IDC_EDIT4, (UINT)(m_g*100), TRUE);//永续增长率 （*0.01）
	SetDlgItemInt(hDlg, IDC_EDIT6, (UINT)m_FCF1, TRUE);//首年现金流
	SetDlgItemInt(hDlg, IDC_EDIT5, (UINT)m_EValue, TRUE);
}

VOID DCF_Data::Save(HANDLE hFile)
{
	DWORD dwSize = 0;
	WriteFile(hFile, this, sizeof(DCF_Data), &dwSize, NULL);
}