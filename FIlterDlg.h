#pragma once


// CFIlterDlg ��ȭ �����Դϴ�.

class CFIlterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFIlterDlg)

public:
	CFIlterDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CFIlterDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FilterDlg };
#endif

private:
	CStatic m_EditFilterSize;

	int nFilterSize;
	int nFilterType;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);  // DDX/DDV �����Դϴ�.
	
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
