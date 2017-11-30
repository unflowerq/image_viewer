// RotationDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Viewer.h"
#include "RotationDlg.h"
#include "afxdialogex.h"


// CRotationDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CRotationDlg, CDialogEx)

CRotationDlg::CRotationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_IMGROTATIONDLG, pParent)
	, m_radio_interp(0)
{

}

CRotationDlg::~CRotationDlg()
{
}

void CRotationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_BILINEAR_R, m_radio_interp);
	DDX_Control(pDX, IDC_EDIT_ROTATEANGLE, m_EditRotateAngle);
}


BEGIN_MESSAGE_MAP(CRotationDlg, CDialogEx)
	ON_BN_CLICKED(IDC_OKBUTTON3, &CRotationDlg::OnClickedOkbutton3)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_BILINEAR_R, IDC_RADIO_BICUBIC_R, OnRadioClicked)
END_MESSAGE_MAP()


// CRotationDlg 메시지 처리기입니다.


void CRotationDlg::OnClickedOkbutton3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString cstrAngle;
	m_EditRotateAngle.GetWindowTextA(cstrAngle);
	nAngle = atoi(cstrAngle);
	CDialog::OnOK();
}
void CRotationDlg::OnRadioClicked(UINT msg)
{
	UpdateData(TRUE);
	switch (m_radio_interp)
	{
	case 0:
		nInterpolationType = 0;
		break;
	case 1:
		nInterpolationType = 1;
		break;
	}
}

int CRotationDlg::GetAngle() { return nAngle; }
int CRotationDlg::GetInterpolationType() { return nInterpolationType; }
