#pragma once


// CRotationDlg 대화 상자입니다.

class CRotationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRotationDlg)

public:
	CRotationDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CRotationDlg();

private:
	CStatic m_EditRotateAngle;

	int nAngle;
	int nInterpolationType;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMGROTATIONDLG };
#endif



protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int m_radio_interp;
	afx_msg void OnClickedOkbutton3();
	afx_msg void OnRadioClicked(UINT msg);

	int GetAngle();
	int GetInterpolationType();
};
