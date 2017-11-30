// AddNoiseTab_2.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Viewer.h"
#include "AddNoiseTab_2.h"
#include "afxdialogex.h"


// AddNoiseTab_2 대화 상자입니다.

IMPLEMENT_DYNAMIC(AddNoiseTab_2, CDialogEx)

AddNoiseTab_2::AddNoiseTab_2(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ADDNOISE_TAB2, pParent)
{

}

AddNoiseTab_2::~AddNoiseTab_2()
{
}

void AddNoiseTab_2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SPROB, Edit_SN_Prob);
	DDX_Control(pDX, IDC_EDIT_PPROB, Edit_PN_Prob);
}


BEGIN_MESSAGE_MAP(AddNoiseTab_2, CDialogEx)
END_MESSAGE_MAP()


// AddNoiseTab_2 메시지 처리기입니다.
