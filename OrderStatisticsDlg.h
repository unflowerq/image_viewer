#pragma once


// COrderStatisticsDlg ��ȭ �����Դϴ�.

class COrderStatisticsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COrderStatisticsDlg)

public:
	COrderStatisticsDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~COrderStatisticsDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

private:
	int nFilterSize1;
	int nFilterSize2;
	int nOSFType;

	CEdit Edit_FilterSize1;
	CEdit Edit_FilterSize2;

public:
	//afx_msg void OnokOsfButtonClicked();
	//afx_msg void OnRadioClicked(UINT msg);

	int GetFilterSize1();
	int GetFilterSize2();
	int GetOSFType();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOkOsfButtonClicked();
	int m_radio_osf;
	afx_msg void OnRadioClicked(UINT msg);
};
