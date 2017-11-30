#pragma once


// CFIlterDlg 대화 상자입니다.

class CFIlterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFIlterDlg)

public:
	CFIlterDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CFIlterDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FilterDlg };
#endif

private:
	CStatic m_EditFilterSize;

	int nFilterSize;
	int nFilterType;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);  // DDX/DDV 지원입니다.
	
	DECLARE_MESSAGE_MAP()

public:
	int m_radio_FilterType;
	afx_msg void OnOkbuttonClicked();
	afx_msg void OnCancelbuttonClicked();
	afx_msg void OnClickedRadio(UINT msg);

	int getFilterSize();
	int getFilterType();
	afx_msg void OnBnClickedGaussianfilterradio();
	afx_msg void OnBnClickedRadio2();
};
