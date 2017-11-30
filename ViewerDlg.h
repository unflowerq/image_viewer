
// ViewerDlg.h : 헤더 파일
//

#pragma once

#include "opencv2\opencv.hpp"
#include "RawInfoDlg.h"
#include "ImgScalingDlg.h"
#include "RotationDlg.h"
#include "FIlterDlg.h"
#include "AddNoiseDlg.h"
#include <math.h>
#include "MeanFilterDlg.h"
#include "OrderStatisticsDlg.h"
#include <algorithm>
#include "PSNRDlg.h"

using namespace cv;

unsigned char** MemAlloc2D(int nHeight, int nWidth, unsigned char nInitVal);
void MemFree2D(unsigned char** Mem, int nHeight);

// CViewerDlg 대화 상자
class CViewerDlg : public CDialogEx
{
// 생성입니다.
public:
	CViewerDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIEWER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

	enum ImageFormat
	{
		FORMAT_RGB,
		FORMAT_YUV444,
		FORMAT_YUV420,
		FORMAT_GRAY
	};

	enum InterpolationType
	{
		BILINEAR,
		BICUBIC
	};

	enum FilterType
	{
		FILTER_AVERAGE,
		FILTER_SMOOTH,
		FILTER_SHARPEN,
		FILTER_GAUSSIAN
	};

	enum NoiseFilterType
	{
		NF_ARITHMETIC,
		NF_GEOMETRIC,
		NF_HARMONIC,
		NF_CONTRAHARMONIC,
		NF_MEDIAN,
		NF_ADAPTIVEMEDIAN
	};

// 구현입니다.

private:
	unsigned char **ch_in_1, **ch_in_2, **ch_in_3;
	
	unsigned char **ch_in_gray;
	
	int nHeight_in, nWidth_in;
	int nFormat;
	bool isImageOpened;
	int *Hist;

	CString InpFileName;

public:
	CStatic m_PicOri;
	CStatic m_PicProc;

	//CRawInfoDlg RawInfoDlg;
	//ImgScalingDlg ImgScalingDlg;
	//CRotationDlg RotationDlg;
	//CFIlterDlg FilterDlg;

	Mat Ori_Img, Proc_Img;
	CRect rect;
	CDC* m_pDC;

	//display
	void DisplayImage(Mat DispMat, bool bInOut);
	void DisplayBitmap(CDC *pDC, CRect rect, IplImage* DisplplImage);
	void ResizeImage(IplImage* src, IplImage** dst, CRect* rect);
	
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnConvertgrayBtnClick();
	afx_msg void OnFileOpenMenuClick();
	afx_msg void OnCloseMenuClick();
	afx_msg void OnEditScalingClick();
	afx_msg void OnEditRotationClick();
	afx_msg void OnHistogramGetClick();
	afx_msg void OnHistogramEqualizationClicked();
	afx_msg void OnHistogramMatchingClicked();
	afx_msg void OnEditFilterClicked();
	afx_msg void OnNoisereductionAddnoiseClicked();
	afx_msg void OnNoisefilteringMeanfilterClicked();
	afx_msg void OnNoisefilteringOrderStatisticClicked();
	afx_msg void OnGetpsnr();
};

//interpolation
bool isInsideBoundary(int nHeight, int nWidth, double h, double w);
unsigned char BilinearInterpolation(unsigned char** In, int nHeight_Ori, int nWidth_Ori, double h_Cvt, double w_Cvt);
unsigned char BicubicInterpolation(unsigned char** In, int nHeight_Ori, int nWidth_Ori, double h_Cvt, double w_Cvt);
unsigned char **Padding(unsigned char** In, int nHeight, int nWidth, int nFilterSize);
void AverageFilter(unsigned char** In_Pad, unsigned char** Out, int nHeight, int nWidth, int nFilterSize);
void SmoothFilter(unsigned char** In_Pad, unsigned char** Out, int nHeight, int nWidth);
void SharpenFilter(unsigned char** In_Pad, unsigned char** Out, int nHeight, int nWidth);
void GaussianFilter(unsigned char** In_Pad, unsigned char** Out, int nHeight, int nWidth);
void InputGaussianNoise(unsigned char** In, unsigned char** Out, int nHeight, int nWidth, float fMean, float fStdev);
void InputSaltPepperNoise(unsigned char** In, unsigned char** Out, int nHeight, int nWidth, float fSProb, float fPProb);
void GetGaussianPDF(float* EmptyPDF, int nLength, float fMean, float fStDev);
float GetNoise(float* PDF, int nLength);
void ArithmeticMeanFilter(unsigned char **Img_in, unsigned char** Out, int nHeight, int nWidth, int nFilterSize);
void GeometricMeanFilter(unsigned char **Img_in, unsigned char** Out, int nHeight, int nWidth, int nFilterSize);
void HarmonicMeanFilter(unsigned char **Img_in, unsigned char** Out, int nHeight, int nWidth, int nFilterSize);

void ContraharmonicMeanFilter(unsigned char **Img_in, unsigned char** Out, int nHeight, int nWidth, int nFilterSize, int Q);
void MedianFilter(unsigned char **Img_in, unsigned char** Out, int nHeight, int nWidth, int nFilterSize);
void AdaptiveMedianFilter(unsigned char **Img_in, unsigned char** Out, int nHeight, int nWidth, int nFilterSize_min, int nFilterSize_Max);