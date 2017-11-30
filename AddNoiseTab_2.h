#pragma once


// AddNoiseTab_2 대화 상자입니다.

class AddNoiseTab_2 : public CDialogEx
{
	DECLARE_DYNAMIC(AddNoiseTab_2)

public:
	AddNoiseTab_2(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~AddNoiseTab_2();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADDNOISE_TAB2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

public:
	CEdit Edit_SN_Prob;
	CEdit Edit_PN_Prob;

	DECLARE_MESSAGE_MAP()
};
