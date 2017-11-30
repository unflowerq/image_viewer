#pragma once


// CMeanFilterDlg ��ȭ �����Դϴ�.

class CMeanFilterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMeanFilterDlg)

public:
	CMeanFilterDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMeanFilterDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

private:
	int nFilterSize;
	int nMeanFilterType;
	int Q;

	CEdit Edit_FilterSize;
	CEdit Edit_Q;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOKMfButtonClicked();
	afx_msg void OnRadioClicked(UINT msg);
	int m_radio_mf;

	int GetFilterSize();
	int GetMeanFilterType();
	int GetQ();
};
