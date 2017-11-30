#pragma once


// AddNoiseTab_1 대화 상자입니다.

class AddNoiseTab_1 : public CDialogEx
{
	DECLARE_DYNAMIC(AddNoiseTab_1)

public:
	AddNoiseTab_1(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~AddNoiseTab_1();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADDNOISE_TAB1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

public:
	CEdit Edit_GN_Mean;
	CEdit Edit_GN_Stdev;

	DECLARE_MESSAGE_MAP()
};
