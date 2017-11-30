// AddNoiseDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Viewer.h"
#include "AddNoiseDlg.h"
#include "afxdialogex.h"


// CAddNoiseDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAddNoiseDlg, CDialogEx)

CAddNoiseDlg::CAddNoiseDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ADDNOISEDIALOG, pParent)
{

}

CAddNoiseDlg::~CAddNoiseDlg()
{
}

void CAddNoiseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ADDNOISETAB, m_mainTab);
}


BEGIN_MESSAGE_MAP(CAddNoiseDlg, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_ADDNOISETAB, &CAddNoiseDlg::OnTcnSelchangeAddnoisetab)
	ON_BN_CLICKED(IDOK, &CAddNoiseDlg::OnOkButtonClicked)
END_MESSAGE_MAP()


// CAddNoiseDlg 메시지 처리기입니다.

BOOL CAddNoiseDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	CString strTab = _T("");
	strTab.Format(_T("Gaussian"));
	this->m_mainTab.InsertItem(0, strTab, 0);
	strTab.Format(_T("Salt&Pepper"));
	this->m_mainTab.InsertItem(1, strTab, 0);

	CRect rect;
	this->m_mainTab.GetClientRect(&rect);
	this->Tab1.Create(IDD_ADDNOISE_TAB1, &this->m_mainTab);
	this->Tab1.SetWindowPos(NULL, 5, 25, rect.Width() - 10, rect.Height() - 30, SWP_SHOWWINDOW | SWP_NOZORDER);

	this->Tab2.Create(IDD_ADDNOISE_TAB2, &this->m_mainTab);
	this->Tab2.SetWindowPos(NULL, 5, 25, rect.Width() - 10, rect.Height() - 30, SWP_NOZORDER);

	this->m_pwndShow = &this->Tab1;

	return TRUE;
}

void CAddNoiseDlg::OnTcnSelchangeAddnoisetab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (this->m_pwndShow != NULL)
	{
		this->m_pwndShow->ShowWindow(SW_HIDE);
		this->m_pwndShow = NULL;
	}

	int tabIndex = this->m_mainTab.GetCurSel();

	switch (tabIndex)
	{
	case 0:
		this->Tab1.ShowWindow(SW_SHOW);
		this->m_pwndShow = &this->Tab1;
		break;
	case 1:
		this->Tab2.ShowWindow(SW_SHOW);
		this->m_pwndShow = &this->Tab2;
		break;
	}

	*pResult = 0;
}


void CAddNoiseDlg::OnOkButtonClicked()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString str1, str2;
	AddNoiseTab_1* Tab_1 = NULL;
	AddNoiseTab_2* Tab_2 = NULL;
	int tabIndex = this->m_mainTab.GetCurSel();

	switch (tabIndex)
	{
	case 0:
		Tab_1 = (AddNoiseTab_1*)this->m_pwndShow;
		Tab_1->Edit_GN_Mean.GetWindowTextA(str1);
		Tab_1->Edit_GN_Stdev.GetWindowTextA(str2);
		NoiseType = 0;
		break;
	case 1:
		Tab_2 = (AddNoiseTab_2*)this->m_pwndShow;
		Tab_2->Edit_SN_Prob.GetWindowTextA(str1);
		Tab_2->Edit_PN_Prob.GetWindowTextA(str2);
		NoiseType = 1;
		break;
	}

	value_1 = atof(str1);
	value_2 = atof(str2);

	CDialog::OnOK();
}

float CAddNoiseDlg::GetVal_1()
{
	return this->value_1;
}

float CAddNoiseDlg::GetVal_2()
{
	return this->value_2;
}

int CAddNoiseDlg::GetNoiseType()
{
	return this->NoiseType;
}