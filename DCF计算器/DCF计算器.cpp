// DCF������.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "DCF������.h"
#include"DCF_Data.h"
#include"List.h"

// ȫ�ֱ���: 
HINSTANCE hInst;								// ��ǰʵ��
CList<DCF_Data> m_DCFList;


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO:  �ڴ˷��ô��롣
	hInst = hInstance;
	DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), NULL, WinProc);
	m_DCFList.EmptyList();
	return GetLastError();
}


VOID OnSelect(HWND hDlg)
{
	HWND hCombo = GetDlgItem(hDlg, IDC_COMBO1);
	TCHAR sz[30] = _T("\0");
	ComboBox_GetLBText(hCombo, ComboBox_GetCurSel(hCombo), sz);
	int count = m_DCFList.GetCount();
	for (DCF_Data*p = m_DCFList.GetFirst(); count; p = m_DCFList.GetNext()) {
		count--;
		if (lstrcmp(p->m_szName, sz) == 0){
			p->SelectShare(hDlg);
		}
	}
}

VOID Init(HWND hDlg)
{
	HICON hICon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_DCF));
	SendMessage(hDlg, WM_SETICON, NULL, (LPARAM)hICon);

	TCHAR szPath[MAX_PATH] = _T("DCF��Ŀ.DCF");
	HANDLE hFile = CreateFile(szPath, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
	if (hFile == INVALID_HANDLE_VALUE) {//�ļ�������
		return;
	}
	DWORD dwFileSizeHigh = 0;
	DWORD dwFileSize = GetFileSize(hFile, &dwFileSizeHigh);
	int count = dwFileSize / sizeof(DCF_Data);//��Ŀ����
	DCF_Data* p = NULL;
	DWORD dwRead = 0;
	for (; count;) {
		count--;
		p = new DCF_Data;
		ReadFile(hFile, p, sizeof(DCF_Data), &dwRead, NULL);
		m_DCFList.AddToTail(p);
	}
	CloseHandle(hFile);

	HWND hCombo = GetDlgItem(hDlg, IDC_COMBO1);
	count = m_DCFList.GetCount();
	for (DCF_Data* p = m_DCFList.GetFirst(); count; p = m_DCFList.GetNext()) {
		count--;
		ComboBox_AddString(hCombo, p->m_szName);
	}

	ComboBox_SetCurSel(hCombo, 0);
	OnSelect(hDlg);
}


VOID Calculate(HWND hDlg)
{
	HWND hCombo = GetDlgItem(hDlg, IDC_COMBO1);
	TCHAR szName[30] = _T("\0");
	GetDlgItemText(hDlg, IDC_COMBO1, szName, _countof(szName));
	int count = m_DCFList.GetCount();
	for (DCF_Data*p = m_DCFList.GetFirst(); count; p = m_DCFList.GetNext()) {
		count--;
		if (lstrcmp(szName, p->m_szName) == 0) {//�Ѵ��ڸù�Ʊ
			p->Calculate(hDlg);
			return;
		}
	}
	ComboBox_AddString(hCombo, szName);
	DCF_Data* pDCF = new DCF_Data;
	pDCF->Calculate(hDlg);
	m_DCFList.AddToTail(pDCF);
}

VOID OnDeleteShare(HWND hDlg)
{
	HWND hCombo = GetDlgItem(hDlg, IDC_COMBO1);
	int cur = ComboBox_GetCurSel(hCombo);
	TCHAR szName[30];
	ComboBox_GetLBText(hCombo, cur, szName);
	int count = m_DCFList.GetCount();
	for (DCF_Data*p = m_DCFList.GetFirst(); count; p = m_DCFList.GetNext()) {
		count--;
		if (lstrcmp(p->m_szName, szName) == 0) {
			m_DCFList.EmptyNode(p);
		}
	}
	ComboBox_DeleteString(hCombo, cur);


	TCHAR sz[] = _T("");
	SetDlgItemText(hDlg, IDC_EDIT1, sz);//������(*0.01)
	SetDlgItemText(hDlg, IDC_EDIT2, sz);//������������
	SetDlgItemText(hDlg, IDC_EDIT3, sz);//���������� ��*0.01��
	SetDlgItemText(hDlg, IDC_EDIT4, sz);//���������� ��*0.01��
	SetDlgItemText(hDlg, IDC_EDIT6, sz);//�����ֽ���
	SetDlgItemText(hDlg, IDC_EDIT5, sz);
}

VOID OnSave(HWND hDlg)
{
	//SendMessage(hDlg, WM_COMMAND, IDOK, NULL);//����ǰ�ȼ��㵱ǰ���ڼ�ֵ
	TCHAR szPath[MAX_PATH]=_T("DCF��Ŀ.DCF");
	HANDLE hFile = CreateFile(szPath, GENERIC_READ | GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
	int count = m_DCFList.GetCount();
	for (DCF_Data* p = m_DCFList.GetFirst(); count; p = m_DCFList.GetNext()) {
		count--;
		p->Save(hFile);
	}
	CloseHandle(hFile);
}

INT_PTR CALLBACK WinProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		Init(hDlg);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (HIWORD(wParam) == CBN_SELCHANGE) {
			OnSelect(hDlg);
			break;
		}
		switch (LOWORD(wParam))
		{
		case IDOK://�������ڼ�ֵ
			Calculate(hDlg);
			break;
		case IDC_BUTTON1://ɾ����Ʊ
			OnDeleteShare(hDlg);
			break;
		case IDCANCEL:
			OnSave(hDlg);//����
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
			break;

		}
	default:
		break;
	}
	return (INT_PTR)FALSE;
}

