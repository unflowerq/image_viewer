#pragma once


// ImgScalingDlg 대화 상자입니다.

class ImgScalingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ImgScalingDlg)

public:
	ImgScalingDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~ImgScalingDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMGSCALEDIALOG };
#endif
private:
	CStatic m_EditScaleRate;

	double dScaleRate;
	int nInterpolationType;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int m_radio_Interp;

	afx_msg void OnOkbutton2Clicked();
	afx_msg void OnRadioClicked(UINT msg);

	double GetScaleRate();
	int GetInterpType();
	
};
