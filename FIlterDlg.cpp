// FIlterDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Viewer.h"
#include "FIlterDlg.h"
#include "afxdialogex.h"


// CFIlterDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CFIlterDlg, CDialogEx)

CFIlterDlg::CFIlterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FilterDlg, pParent)
{

}

CFIlterDlg::~CFIlterDlg()
{
}

void CFIlterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_AVERAGEFILTERRADIO, m_radio_FilterType);
	DDX_Control(pDX, IDC_EDIT_FILTERSIZE, m_EditFilterSize);
}

BEGIN_MESSAGE_MAP(CFIlterDlg, CDialogEx)
	ON_BN_CLICKED(IDC_OKBUTTON4, &CFIlterDlg::OnOkbuttonClicked)
	ON_BN_CLICKED(IDC_CANCELBUTTON4, &CFIlterDlg::OnCancelbuttonClicked)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_AVERAGEFILTERRADIO, IDC_GAUSSIANFILTERRADIO, OnClickedRadio)
	ON_BN_CLICKED(IDC_GAUSSIANFILTERRADIO, &CFIlterDlg::OnBnClickedGaussianfilterradio)
	ON_BN_CLICKED(IDC_RADIO2, &CFIlterDlg::OnBnClickedRadio2)
END_MESSAGE_MAP()


// CFIlterDlg �޽��� ó�����Դϴ�.

void CFIlterDlg::OnOkbuttonClicked()
{
	CString cstrTemp;
	m_EditFilterSize.GetWindowTextA(cstrTemp);
	nFilterSize = atoi(cstrTemp);

	CDialog::OnOK();
}

void CFIlterDlg::OnCancelbuttonClicked()
{
	CDialog::OnCancel();
}

void CFIlterDlg::OnClickedRadio(UINT msg)
{
	UpdateData(TRUE);
	switch (m_radio_FilterType)
	{
	case 0:
		nFilterType = 0;
		break;
	case 1:
		nFilterType = 1;
		break;
	case 2:
		nFilterType = 2;
		break;
	case 3:
		nFilterType = 3;
		break;
	}
}

int CFIlterDlg::getFilterSize() { return nFilterSize; }
int CFIlterDlg::getFilterType() { return nFilterType; }


void CFIlterDlg::OnBnClickedGaussianfilterradio()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CFIlterDlg::OnBnClickedRadio2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}
