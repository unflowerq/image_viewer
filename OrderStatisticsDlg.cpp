// OrderStatisticsDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Viewer.h"
#include "OrderStatisticsDlg.h"
#include "afxdialogex.h"


// COrderStatisticsDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(COrderStatisticsDlg, CDialogEx)

COrderStatisticsDlg::COrderStatisticsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG2, pParent)
	, m_radio_osf(0)
{
	nOSFType = 4;
}

COrderStatisticsDlg::~COrderStatisticsDlg()
{
}

void COrderStatisticsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_MEDIANF, m_radio_osf);
	DDX_Control(pDX, IDC_EDIT_FILTERSIZE_OSF, Edit_FilterSize1);
	DDX_Control(pDX, IDC_EDIT_MAXFILTERSIZE_OSF, Edit_FilterSize2);
}


BEGIN_MESSAGE_MAP(COrderStatisticsDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &COrderStatisticsDlg::OnOkOsfButtonClicked)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_MEDIANF, IDC_RADIO_ADAPTIVEMEDIANF, OnRadioClicked)
	//ON_BN_CLICKED(IDC_RADIO1, &COrderStatisticsDlg::OnRadioClicked)
END_MESSAGE_MAP()


// COrderStatisticsDlg 메시지 처리기입니다.


void COrderStatisticsDlg::OnOkOsfButtonClicked()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strTemp1, strTemp2;

	Edit_FilterSize1.GetWindowTextA(strTemp1);
	Edit_FilterSize2.GetWindowTextA(strTemp2);

	nFilterSize1 = atoi(strTemp1);
	nFilterSize2 = atoi(strTemp2);

	CDialog::OnOK();
}


void COrderStatisticsDlg::OnRadioClicked(UINT msg)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	switch (m_radio_osf)
	{
	case 0:
		nOSFType = 4;
		break;

	case 1:
		nOSFType = 5;
		break;
	}
}

int COrderStatisticsDlg::GetFilterSize1() { return nFilterSize1; }
int COrderStatisticsDlg::GetFilterSize2() { return nFilterSize2; }
int COrderStatisticsDlg::GetOSFType() { return nOSFType; }
