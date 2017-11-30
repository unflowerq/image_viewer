#pragma once


// CRawInfoDlg 대화 상자입니다.

class CRawInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRawInfoDlg)

public:
	CRawInfoDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CRawInfoDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RAWINFODIALOG };
#endif
private:
	int nHeight;
	int nWidth;
	int nFormat;

	CEdit m_EditHeight;
	CEdit m_EditWidth;

public:
	bool isTrue;
	int GetRawHeight()
	{
		return nHeight;
	}
	int GetRawWidth()
	{
		return nWidth;
	}
	int GetRawFormat()
	{
		return nFormat;
	}
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int m_radio_rawFileInfo;
	afx_msg void OnClickedRadio(UINT msg);
	afx_msg void OnBnClickedOKButton1();
	afx_msg void OnBnClickedCancelButton1();
};
