
// ViewerDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "Viewer.h"
#include "ViewerDlg.h"
#include "afxdialogex.h"
#include <math.h>
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

unsigned char** Padding(unsigned char** In, int nHeight, int nWidth, int nFilterSize)
{
	int nPadSize = (int)(nFilterSize / 2);
	unsigned char** Pad = MemAlloc2D(nHeight + 2 * nPadSize, nWidth + 2 * nPadSize, 0);

	for (int h = 0; h < nHeight; h++)
	{
		for (int w = 0; w < nWidth; w++)
		{
			Pad[h + nPadSize][w + nPadSize] = In[h][w];
		}
	}
	
	for (int h = 0; h < nPadSize; h++)
	{
		for (int w = 0; w < nWidth; w++)
		{
			Pad[h][w + nPadSize] = In[0][w];
			Pad[h + (nHeight - 1)][w + nPadSize] = In[nHeight - 1][w];
		}
	}

	for (int h = 0; h < nHeight; h++)
	{
		for (int w = 0; w < nPadSize; w++)
		{
			Pad[h + nPadSize][w] = In[h][0];
			Pad[h + nPadSize][w + (nWidth - 1)] = In[h][nWidth - 1];
		}
	}

	for (int h = 0; h < nPadSize; h++)
	{
		for (int w = 0; w < nPadSize; w++)
		{
			Pad[h][w] = In[0][0];
			Pad[h + (nHeight - 1)][w] = In[nHeight - 1][0];
			Pad[h][w + (nWidth - 1)] = In[0][nWidth - 1];
			Pad[h + (nHeight - 1)][w + (nWidth - 1)] = In[nHeight - 1][nWidth - 1];
		}
	}

	return Pad;
}

void MemFree2D(unsigned char **Mem, int nHeight)
{
	for (int n = 0; n < nHeight; n++)
	{
		delete[] Mem[n];
	}
	delete[] Mem;
}

unsigned char** MemAlloc2D(int nHeight, int nWidth, unsigned char nInitVal)
{
	unsigned char** rtn = new unsigned char*[nHeight];
	for (int n = 0; n < nHeight; n++)
	{
		rtn[n] = new unsigned char[nWidth];
		memset(rtn[n], nInitVal, sizeof(unsigned char) * nWidth);
	}
	return rtn;
}

void DisplayHistogram(int* Hist)
{
	int nMax = 0;

	for (int n = 0; n < 256; n++)
	{
		if (nMax < Hist[n])
		{
			nMax = Hist[n];
		}
	}

	double dNormalizeFactor = 255.0 / nMax;

	Mat HistDisp = Mat(256, 256, CV_8UC1);
	HistDisp = Scalar::all(0);

	for (int w = 0; w < 256; w++)
	{
		int nNormalizedValue = (int)Hist[w] * dNormalizeFactor;
		for (int h = 255; h > 255 - nNormalizedValue; h--)
		{
			HistDisp.at<unsigned char>(h, w) = 255;
		}
	}

	imshow("Histogram", HistDisp);
	waitKey(0);
}

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CViewerDlg 대화 상자



CViewerDlg::CViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VIEWER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURE_IN, m_PicOri);
	DDX_Control(pDX, IDC_PICTURE_OUT, m_PicProc);
}

BEGIN_MESSAGE_MAP(CViewerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_CONVERTCOLOR_CONVERTGRAY, &CViewerDlg::OnConvertgrayBtnClick)
	ON_COMMAND(ID_FILE_FILEOPEN, &CViewerDlg::OnFileOpenMenuClick)
	ON_COMMAND(ID_FILE_FILECLOSE, &CViewerDlg::OnCloseMenuClick)
	ON_COMMAND(ID_EDIT_SCALING, &CViewerDlg::OnEditScalingClick)
	ON_COMMAND(ID_EDIT_ROTATION, &CViewerDlg::OnEditRotationClick)
	ON_COMMAND(ID_HISTOGRAM_GETHISTOGRAM, &CViewerDlg::OnHistogramGetClick)
	ON_COMMAND(ID_HISTOGRAM_HISTOGRAMEQUALIZATION, &CViewerDlg::OnHistogramEqualizationClicked)
	ON_COMMAND(ID_HISTOGRAM_HISTOGRAMMATCHING, &CViewerDlg::OnHistogramMatchingClicked)
	ON_COMMAND(ID_HISTOGRAM_FILTER, &CViewerDlg::OnEditFilterClicked)
	ON_COMMAND(ID_NOISEREDUCTION_ADDNOISE, &CViewerDlg::OnNoisereductionAddnoiseClicked)
	ON_COMMAND(ID_NOISEFILTERING_MEANFILTER, &CViewerDlg::OnNoisefilteringMeanfilterClicked)
	ON_COMMAND(ID_NOISEFILTERING_ORDER, &CViewerDlg::OnNoisefilteringOrderStatisticClicked)
	ON_COMMAND(ID_NOISEREDUCTION_GETPSNR, &CViewerDlg::OnGetpsnr)
END_MESSAGE_MAP()


// CViewerDlg 메시지 처리기

BOOL CViewerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CViewerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CViewerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CViewerDlg::DisplayImage(Mat DispMat, bool bInOut)
{
	if (bInOut)
	{
		GetDlgItem(IDC_PICTURE_IN)->GetWindowRect(rect);
		ScreenToClient(rect);
		m_pDC = m_PicOri.GetDC();
		m_PicOri.GetClientRect(&rect);
	}
	else
	{
		GetDlgItem(IDC_PICTURE_OUT)->GetWindowRect(rect);
		ScreenToClient(rect);
		m_pDC = m_PicProc.GetDC();
		m_PicProc.GetClientRect(&rect);
	}

	IplImage* Ori = &IplImage(DispMat);
	IplImage* Resize = NULL;

	ResizeImage(Ori, &Resize, &rect);
	DisplayBitmap(m_pDC, rect, Resize);
}
void CViewerDlg::DisplayBitmap(CDC * pDC, CRect rect, IplImage * DispIplImage)
{
	BITMAPINFO bitmapInfo;
	bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfo.bmiHeader.biPlanes = 1;
	bitmapInfo.bmiHeader.biCompression = BI_RGB;
	bitmapInfo.bmiHeader.biXPelsPerMeter = 100;
	bitmapInfo.bmiHeader.biYPelsPerMeter = 100;
	bitmapInfo.bmiHeader.biClrUsed = 0;
	bitmapInfo.bmiHeader.biClrImportant = 0;
	bitmapInfo.bmiHeader.biSizeImage = 0;
	bitmapInfo.bmiHeader.biWidth = DispIplImage->width;
	bitmapInfo.bmiHeader.biHeight = DispIplImage->height;

	if (DispIplImage->nChannels == 3)
	{
		IplImage* ReverseImg = cvCreateImage(CvSize(DispIplImage->width, DispIplImage->height), 8, 3);

		for (int h = 0; h < DispIplImage->height; h++)
		{
			for (int w = 0; w < DispIplImage->width; w++)
			{
				ReverseImg->imageData[h*DispIplImage->widthStep + w * 3] = DispIplImage->imageData[(DispIplImage->height - h-1)*DispIplImage->widthStep + w * 3];
				ReverseImg->imageData[h*DispIplImage->widthStep + w * 3 + 1] = DispIplImage->imageData[(DispIplImage->height - h-1)*DispIplImage->widthStep + w * 3 + 1];
				ReverseImg->imageData[h*DispIplImage->widthStep + w * 3 + 2] = DispIplImage->imageData[(DispIplImage->height - h-1)*DispIplImage->widthStep + w * 3 + 2];
			}
		}

		bitmapInfo.bmiHeader.biBitCount = DispIplImage->depth * DispIplImage->nChannels;
		pDC->SetStretchBltMode(COLORONCOLOR);
		::StretchDIBits(pDC->GetSafeHdc(), rect.left, rect.top, rect.right, rect.bottom, 0, 0, DispIplImage->width, DispIplImage->height, ReverseImg->imageData, &bitmapInfo, DIB_RGB_COLORS, SRCCOPY);

		cvReleaseImage(&ReverseImg);
	}

	else
	{
		IplImage* ReverseImg = cvCreateImage(cvGetSize(DispIplImage), DispIplImage->depth, 1);

		for (int h = 0; h < DispIplImage->height; h++)
		{
			for (int w = 0; w < DispIplImage->width; w++)
			{
				ReverseImg->imageData[h*DispIplImage->widthStep + w] = DispIplImage->imageData[(DispIplImage->height - h)*DispIplImage->widthStep + w];
			}
		}

		IplImage* tmp = cvCreateImage(cvGetSize(DispIplImage), DispIplImage->depth, 3);
		cvCvtColor(ReverseImg, tmp, CV_GRAY2BGR);

		bitmapInfo.bmiHeader.biBitCount = tmp->depth*tmp->nChannels;
		pDC->SetStretchBltMode(COLORONCOLOR);
		::StretchDIBits(pDC->GetSafeHdc(), rect.left, rect.top, rect.right, rect.bottom, 0, 0, tmp->width, tmp->height, tmp->imageData, &bitmapInfo, DIB_RGB_COLORS, SRCCOPY);

		cvReleaseImage(&ReverseImg);
		cvReleaseImage(&tmp);
	}
}

void CViewerDlg::ResizeImage(IplImage* src, IplImage** dst, CRect * rect)
{
	int ndst_width = 0, ndst_height = 0;

	if (src->width > src->height)
	{
		ndst_width = rect->Width();
		ndst_height = (src->height*rect->Width()) / src->width;
	}
	else
	{
		ndst_width = (src->width * rect->Height()) / src->height;
		ndst_height = rect->Height();
	}
	(*dst) = cvCreateImage(cvSize(ndst_width, ndst_height), IPL_DEPTH_8U, src->nChannels);

	cvResize(src, (*dst));
	rect->right = rect->left + ndst_width;
	rect->bottom = rect->top + ndst_height;
}


void CViewerDlg::OnConvertgrayBtnClick()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (isImageOpened)
	{
		if (nFormat == FORMAT_GRAY)
		{
			MessageBox("입력 영상이 회색조 영상입니다.");
			return;
		}

		if (nFormat == FORMAT_YUV444 || nFormat == FORMAT_YUV420)
		{
			Proc_Img = Mat(nHeight_in, nWidth_in, CV_8UC1);
			for (int h = 0; h < nHeight_in; h++)
			{
				for (int w = 0; w < nWidth_in; w++)
				{
					Proc_Img.at<unsigned char>(h, w) = ch_in_1[h][w];
				}
			}
			DisplayImage(Proc_Img, false);
			return;
		}

		if (nFormat == FORMAT_RGB)
		{
			if (ch_in_gray == NULL)
			{
				ch_in_gray = MemAlloc2D(nHeight_in, nWidth_in, 0);
			}

			for (int h = 0; h < nHeight_in; h++)
			{
				for (int w = 0; w < nWidth_in; w++)
				{
					ch_in_gray[h][w] = saturate_cast<unsigned char>(0.299*ch_in_1[h][w] + 0.587*ch_in_2[h][w] + 0.114*ch_in_3[h][w]);
				}
			}

			Proc_Img = Mat(nHeight_in, nWidth_in, CV_8UC1);
			for (int h = 0; h < nHeight_in; h++)
			{
				for (int w = 0; w < nWidth_in; w++)
				{
					Proc_Img.at<unsigned char>(h, w) = ch_in_gray[h][w];
				}
			}

			DisplayImage(Proc_Img, false);
			return;
		}
	}
	else
	{
		MessageBox("불러온 영상이 없습니다.");
	}

}

void CViewerDlg::OnFileOpenMenuClick()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CFileDialog dlg(TRUE);

	if (isImageOpened)
	{
		if (ch_in_1 != NULL)
		{
			MemFree2D(ch_in_1, nHeight_in);
		}
		if (ch_in_2 != NULL)
		{
			if (nFormat == FORMAT_YUV444)
			{
				MemFree2D(ch_in_2, nHeight_in / 2);
			}
			else MemFree2D(ch_in_2, nHeight_in);
		}
		if (ch_in_3 != NULL)
		{
			if (nFormat == FORMAT_YUV420)
			{
				MemFree2D(ch_in_3, nHeight_in / 2);
			}
			else MemFree2D(ch_in_3, nHeight_in);
		}
		if (ch_in_gray != NULL)
		{
			MemFree2D(ch_in_gray, nHeight_in);
		}
	}

	if (dlg.DoModal() == IDOK)
	{
		if (dlg.GetFileExt() != "jpg" && dlg.GetFileExt() != "JPG" && dlg.GetFileExt() != "raw" && dlg.GetFileExt() != "yuv")
		{
			MessageBox("JPG 또는 YUV, Gray 파일이 아닙니다.");
			return;
		}

		InpFileName = dlg.GetFileTitle();

		if (dlg.GetFileExt() == "jpg" || dlg.GetFileExt() == "JPG")
		{
			Ori_Img = imread((const char*)dlg.GetPathName());
			nHeight_in = Ori_Img.rows;
			nWidth_in = Ori_Img.cols;
			if (Ori_Img.channels() == 3)
			{
				ch_in_1 = MemAlloc2D(nHeight_in, nWidth_in, 0);
				ch_in_2 = MemAlloc2D(nHeight_in, nWidth_in, 0);
				ch_in_3 = MemAlloc2D(nHeight_in, nWidth_in, 0);

				for (int h = 0; h < nHeight_in; h++)
				{
					for (int w = 0; w < nWidth_in; w++)
					{
						Vec3b RGB = Ori_Img.at<Vec3b>(h, w);
						ch_in_1[h][w] = RGB[2];
						ch_in_2[h][w] = RGB[1];
						ch_in_3[h][w] = RGB[0];
					}
				}
				nFormat = FORMAT_RGB;
			}
			else
			{
				ch_in_gray = MemAlloc2D(nHeight_in, nWidth_in, 0);
				for (int h = 0; h < nHeight_in; h++)
				{
					for (int w = 0; w < nWidth_in; w++)
					{
						ch_in_gray[h][w] = Ori_Img.at<unsigned char>(h, w);
					}
				}
				nFormat = FORMAT_GRAY;
			}
			isImageOpened = true;
			DisplayImage(Ori_Img, true);
		}
		else
		{
			CRawInfoDlg RawInfoDlg;
			if (RawInfoDlg.DoModal() == IDOK)
			{
				nHeight_in = RawInfoDlg.GetRawHeight();
				nWidth_in = RawInfoDlg.GetRawWidth();
				nFormat = RawInfoDlg.GetRawFormat();

				if (nHeight_in == 0 || nWidth_in == 0)
				{
					MessageBox("값을 잘 못 입력하였습니다.");
					return;
				}

				FILE *fp_in;

				fopen_s(&fp_in, (const char*)dlg.GetPathName(), "rb");

				if (nFormat == FORMAT_GRAY)
				{
					ch_in_gray = MemAlloc2D(nHeight_in, nWidth_in, 0);
					for (int h = 0; h < nHeight_in; h++)
					{
						fread(ch_in_gray[h], sizeof(unsigned char), nWidth_in, fp_in);
					}
					Ori_Img = Mat(nHeight_in, nWidth_in, CV_8UC1);
					for (int h = 0; h < nHeight_in; h++)
					{
						for (int w = 0; w < nWidth_in; w++)
						{
							Ori_Img.at<unsigned char>(h, w) = ch_in_gray[h][w];
						}
					}
				}
				else if (nFormat == FORMAT_YUV444)
				{
					ch_in_1 = MemAlloc2D(nHeight_in, nWidth_in, 0);
					ch_in_2 = MemAlloc2D(nHeight_in, nWidth_in, 0);
					ch_in_3 = MemAlloc2D(nHeight_in, nWidth_in, 0);
					for (int h = 0; h < nHeight_in; h++)
					{
						fread(ch_in_1, sizeof(unsigned char), nWidth_in, fp_in);
					}
					for (int h = 0; h < nHeight_in; h++)
					{
						fread(ch_in_2, sizeof(unsigned char), nWidth_in, fp_in);
					}
					for (int h = 0; h < nHeight_in; h++)
					{
						fread(ch_in_3, sizeof(unsigned char), nWidth_in, fp_in);
					}
				}
				else
				{
					ch_in_1 = MemAlloc2D(nHeight_in, nWidth_in, 0);
					ch_in_2 = MemAlloc2D(nHeight_in / 2, nWidth_in / 2, 0);
					ch_in_3 = MemAlloc2D(nHeight_in / 2, nWidth_in / 2, 0);
					for (int h = 0; h < nHeight_in; h++)
					{
						fread(ch_in_1, sizeof(unsigned char), nWidth_in, fp_in);
					}
					for (int h = 0; h < nHeight_in / 2; h++)
					{
						fread(ch_in_2, sizeof(unsigned char), nWidth_in / 2, fp_in);
					}
					for (int h = 0; h < nHeight_in / 2; h++)
					{
						fread(ch_in_3, sizeof(unsigned char), nWidth_in / 2, fp_in);
					}
				}
				fclose(fp_in);

				isImageOpened = true;
				DisplayImage(Ori_Img, true);
			}
		}
	}
}


void CViewerDlg::OnCloseMenuClick()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (isImageOpened)
	{
		if (ch_in_1 != NULL)
		{
			MemFree2D(ch_in_1, nHeight_in);
		}
		if (ch_in_2 != NULL)
		{
			if (nFormat == FORMAT_YUV444)
			{
				MemFree2D(ch_in_2, nHeight_in / 2);
			}
			else
				MemFree2D(ch_in_2, nHeight_in);
		}
		if (ch_in_3 != NULL)
		{
			if (nFormat == FORMAT_YUV420)
			{
				MemFree2D(ch_in_3, nHeight_in / 2);
			}
			else
				MemFree2D(ch_in_3, nHeight_in);
		}
		if (ch_in_gray != NULL)
		{
			MemFree2D(ch_in_gray, nHeight_in);
		}
	}

	::SendMessage(this->m_hWnd, WM_CLOSE, NULL, NULL);
}

void CViewerDlg::OnEditScalingClick()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	ImgScalingDlg ImgScalingDlg;

	if (isImageOpened && nFormat == FORMAT_GRAY)
	{
		if (ImgScalingDlg.DoModal() == IDOK) 
		{
			int nInterpolationType = ImgScalingDlg.GetInterpType();
			double dScaleRate = ImgScalingDlg.GetScaleRate();
			if (dScaleRate < 1 || dScaleRate >2)
			{
				MessageBox("스케일링 범위가 잘못되었습니다.");
				return;
			}
			int nHeight_Scale = nHeight_in * dScaleRate;
			int nWidth_Scale = nWidth_in * dScaleRate;

			
			unsigned char** Out = MemAlloc2D(nHeight_Scale, nWidth_Scale, 0);

			for (int h = 0; h < nHeight_Scale; h++)
			{
				for (int w = 0; w < nWidth_Scale; w++)
				{
					double h_Ori = h / dScaleRate;
					double w_Ori = w / dScaleRate;
					if (isInsideBoundary(nHeight_Scale, nWidth_Scale, h_Ori, w_Ori))
					{
						if (nInterpolationType == BILINEAR) 
						{
							Out[h][w] = BilinearInterpolation(ch_in_gray, nHeight_in, nWidth_in, h_Ori, w_Ori);
						}
						else 
						{
							Out[h][w] = BicubicInterpolation(ch_in_gray, nHeight_in, nWidth_in, h_Ori, w_Ori);
						}

					}
				}
			}

			Ori_Img = Mat(nHeight_in, nWidth_in, CV_8UC1);
			Proc_Img = Mat(nHeight_Scale, nWidth_Scale, CV_8UC1);

			for (int h = 0; h < nHeight_in; h++)
			{
				for (int w = 0; w < nWidth_in; w++)
				{
					Ori_Img.at<unsigned char>(h, w) = ch_in_gray[h][w];
				}
			}
			for (int h = 0; h < nHeight_Scale; h++)
			{
				for (int w = 0; w < nWidth_Scale; w++)
				{
					Proc_Img.at<unsigned char>(h, w) = Out[h][w];
				}
			}

			imshow("Original Image", Ori_Img);
			imshow("Scaled Image", Proc_Img);
			waitKey(0);
		}
	}
	else
	{
		MessageBox("불러온 영상이 없거나 영상 포맷이 회색조 영상이 아닙니다.");
	}
}

bool isInsideBoundary(int nHeight, int nWidth, double h, double w)
{
	if (h >= 0 && w >= 0 && h < nHeight && w < nWidth)
	{
		return true;
	}
	else return false;
}

unsigned char BilinearInterpolation(unsigned char** In, int nHeight_Ori, int nWidth_Ori, double h_Cvt, double w_Cvt)
{
	unsigned char r1, r2, r3, r4;
	unsigned char return_value;
	int h1, w1, h2, w2, h3, w3, h4, w4;

	h1 = floor(h_Cvt); w1 = floor(w_Cvt);
	h2 = h1; w2 = w1 + 1;
	h3 = h1 + 1; w3 = w1;
	h4 = h1 + 1; w4 = w1 + 1;

	if (h1 == nHeight_Ori - 1 && w1 == nWidth_Ori - 1)
		return 0;
	else if (h1 == nHeight_Ori - 1 && w1 != nWidth_Ori - 1)
		return 0;
	else if (h1 != nHeight_Ori - 1 && w1 == nWidth_Ori - 1)
		return 0;
	else
	{
		r1 = In[h1][w1];
		r2 = In[h2][w2];
		r3 = In[h3][w3];
		r4 = In[h4][w4];

		return_value = (h3 - h_Cvt)*((w2 - w_Cvt)*r1 + (w_Cvt - w1)*r2) + (h_Cvt - h1)*((w2 - w_Cvt)*r3 + (w_Cvt - w1)*r4);
		return return_value;
	}
}

unsigned char BicubicInterpolation(unsigned char** In, int nHeight_Ori, int nWidth_Ori, double h_Cvt, double w_Cvt)
{
	unsigned char return_value;
	int h1, w1;

	h1 = floor(h_Cvt);
	w1 = floor(w_Cvt);

	double real_w = w_Cvt - w1;
	double real_h = h_Cvt - h1;

	if (h1 == 0)
		return 0;
	else if (w1 == 0)
		return 0;
	else if (w1 > nWidth_Ori - 3)
		return 0;
	else if (h1 > nHeight_Ori - 3)
		return 0;
	else
	{
		double reverse_matrix[4][4] = { { -0.166667, 0.5, -0.5, 0.166667 },{ 0.5, -1, 0.5, 0 },{ -0.333333, -0.5, 1, -0.166667 },{ 0, 1, 0, 0 } };

		double onefourfourfour[4];
		double findvalue[4];
		double onefourfourone[4];

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				onefourfourfour[j] = reverse_matrix[j][0] * In[(h1 - 1) + i][w1 - 1] + reverse_matrix[j][1] * In[(h1 - 1) + i][w1] + reverse_matrix[j][2] * In[(h1 - 1) + i][w1 + 1] + reverse_matrix[j][3] * In[(h1 - 1) + i][w1 + 2];
			}
			findvalue[i] = real_w*real_w*real_w*onefourfourfour[0] + real_w*real_w*onefourfourfour[1] + real_w*onefourfourfour[2] + onefourfourfour[3];
		}

		for (int i = 0; i < 4; i++)
		{
			onefourfourone[i] = reverse_matrix[i][0] * findvalue[0] + reverse_matrix[i][1] * findvalue[1] + reverse_matrix[i][2] * findvalue[2] + reverse_matrix[i][3] * findvalue[3];
		}

		return_value = real_h*real_h*real_h*onefourfourone[0] + real_h*real_h*onefourfourone[1] + real_h*onefourfourone[2] + onefourfourone[3];

		return return_value;
	}
}

void CViewerDlg::OnEditRotationClick()
{
	CRotationDlg RotationDlg;

	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (isImageOpened && nFormat == FORMAT_GRAY)
	{
		if (RotationDlg.DoModal() == IDOK)
		{
			int nInterpolationType = RotationDlg.GetInterpolationType();
			int nAngle = RotationDlg.GetAngle();

			if (nAngle < 0 || nAngle > 360)
			{
				MessageBox("회전 범위가 잘못되었습니다.");
				return;
			}

			unsigned char** Out = MemAlloc2D(nHeight_in, nWidth_in, 0);

			int nHeight_2 = nHeight_in / 2;
			int nWidth_2 = nWidth_in / 2;
			
			double PI = 3.14;

			double h_Rotate = 0;
			double w_Rotate = 0;

			double sin_value = (double)sin(nAngle * (PI / 180.0));
			double cos_value = (double)cos(nAngle * (PI / 180.0));

			for (int h = 0; h < nHeight_in; h++)
			{
				for (int w = 0; w < nWidth_in; w++) 
				{
					int rot_h = h - nHeight_2;
					int rot_w = w - nWidth_2;
					
					h_Rotate = sin_value*rot_w + cos_value*rot_h + nHeight_2;
					w_Rotate = cos_value*rot_w - sin_value*rot_h +nWidth_2;
					
					if (isInsideBoundary(nHeight_in, nWidth_in, h_Rotate, w_Rotate))
					{
						if (nInterpolationType == BILINEAR) 
						{
							Out[h][w] = BilinearInterpolation(ch_in_gray, nHeight_in, nWidth_in, h_Rotate, w_Rotate);
						}
						else 
						{

							Out[h][w] = BicubicInterpolation(ch_in_gray, nHeight_in, nWidth_in, h_Rotate, w_Rotate);
						}
					}
				}
			}

			Proc_Img = Mat(nHeight_in , nWidth_in , CV_8UC1);
			
			for (int h = 0; h < nHeight_in; h++)
			{
				for (int w = 0; w < nWidth_in; w++)
				{
					Proc_Img.at<unsigned char>(h, w) = Out[h][w];
				}
			}
			DisplayImage(Proc_Img, false);
			MemFree2D(Out, nHeight_in);
		}
	}
	else {
		MessageBox("불러온 영상이 없거나 영상 포맷이 회색조 영상이 아닙니다.");
	}
}


void CViewerDlg::OnHistogramGetClick()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	if (isImageOpened && nFormat == FORMAT_GRAY)
	{
		if (Hist == NULL)
		{
			Hist = new int[256];
		}
		memset(Hist, 0, sizeof(int) * 256);

		for (int h = 0; h < nHeight_in; h++)
		{
			for (int w = 0; w < nWidth_in; w++)
			{
				Hist[ch_in_gray[h][w]]++;
			}
		}

		int nMax = 0;

		for (int n = 0; n < 256; n++)
		{
			if (nMax < Hist[n])
			{
				nMax = Hist[n];
			}
		}

		double dNormalizeFactor = 255.0 / nMax;

		Mat HistDisp = Mat(256, 256, CV_8UC1);
		HistDisp = Scalar::all(0);

		for (int w = 0; w < 256; w++)
		{
			int dNormalizeValue = (int)Hist[w] * dNormalizeFactor;
			for (int h = 255; h > 256 - dNormalizeValue; h--)
			{
				HistDisp.at<unsigned char>(h, w) = 255;
			}
		}

		imshow("Histofram of input image", HistDisp);
		waitKey(0);
	}
}


void CViewerDlg::OnHistogramEqualizationClicked()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	if (isImageOpened && nFormat == FORMAT_GRAY)
	{
		if (Hist == NULL)
		{
			Hist = new int[256];
		}
		memset(Hist, 0, sizeof(int) * 256);

		for (int h = 0; h < nHeight_in; h++)
		{
			for (int w = 0; w < nWidth_in; w++)
			{
				Hist[ch_in_gray[h][w]]++;
			}
		}

		double Hist_CDF[256] = { 0,0 };
		Mat Eq_img = Mat(256, 256, CV_8UC1);

		/***************************************************************/
		

		for (int i = 0; i < 256; i++)
		{
			Hist_CDF[i] = Hist[i];
		}

		//////////////////누적합 구하기
		for (int i = 1; i < 256; i++)
		{
			Hist_CDF[i] = Hist_CDF[i - 1] + Hist_CDF[i];
		}

		for (int i = 0; i < 256; i++)
		{
			Hist_CDF[i] = Hist_CDF[i] / (nHeight_in*nWidth_in) * 255;
		}

		for (int h = 0; h < nHeight_in; h++)
		{
			for (int w = 0; w < nWidth_in; w++)
			{
				Eq_img.at<unsigned char>(h, w) = Hist_CDF[ch_in_gray[h][w]];
			}
		}

		/***************************************************************/

		int* Hist_Eq = new int[256];
		memset(Hist_Eq, 0, sizeof(int) * 256);

		for (int h = 0; h < nHeight_in; h++)
		{
			for (int w = 0; w < nWidth_in; w++)
			{
				Hist_Eq[Eq_img.at<unsigned char>(h, w)]++;
			}
		}

		DisplayImage(Eq_img, false);
		DisplayHistogram(Hist);
		DisplayHistogram(Hist_Eq);

		delete[] Hist_Eq;
	}
}


void CViewerDlg::OnHistogramMatchingClicked()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	if (isImageOpened && nFormat == FORMAT_GRAY)
	{
		if (Hist == NULL)
		{
			Hist = new int[256];
		}

		memset(Hist, 0, sizeof(int) * 256);

		for (int h = 0; h < nHeight_in; h++)
		{
			for (int w = 0; w < nWidth_in; w++)
			{
				Hist[ch_in_gray[h][w]]++;
			}
		}

		Mat MatchImg(256, 256, CV_8UC1); Mat RefImg(nHeight_in, nWidth_in, CV_8UC1);
		int Hist_Ref[256] = { 0 }; int Hist_Mat[256] = { 0 };
		float Hist_CDF[256]; float Hist_CDF_Ref[256];

		MatchImg = Scalar::all(0);
		RefImg = Scalar::all(0);

		srand(GetTickCount());

		for (int h = 0; h < RefImg.rows; h++)
		{
			for (int w = 0; w < RefImg.cols; w++)
			{
				RefImg.at<unsigned char>(h, w) = (unsigned char)rand() % 255;
				Hist_Ref[RefImg.at<unsigned char>(h, w)]++;
			}
		}

		Hist_CDF[0] = (double)Hist[0] / (nHeight_in * nWidth_in);
		Hist_CDF_Ref[0] = (double)Hist_Ref[0] / (nHeight_in * nWidth_in);

		for (int n = 1; n < 256; n++)
		{
			Hist_CDF[n] = (double)Hist[n] / (nHeight_in * nWidth_in) + Hist_CDF[n - 1];
			Hist_CDF_Ref[n] = (double)Hist_Ref[n] / (nHeight_in * nWidth_in) + Hist_CDF_Ref[n - 1];
		}

		for (int h = 0; h < MatchImg.rows; h++)
		{
			for (int w = 0; w < MatchImg.cols; w++)
			{
				//////MatchImg.at<unsigned char>(h, w) = Hist_CDF[w] * ch_in_gray[h][w];
				Hist_Mat[MatchImg.at<unsigned char>(h, w)]++;
			}
		}

		DisplayImage(MatchImg, false);
		DisplayHistogram(Hist_Ref);
		DisplayHistogram(Hist_Mat);
	}
}

Mat GrayToMat(unsigned char** Img, int nHeight, int nWidth)
{
	Mat rtn = Mat(nHeight, nWidth, CV_8UC1);

	for (int h = 0; h< nHeight; h++)
	{
		for (int w = 0; w<nWidth; w++)
		{
			rtn.at<unsigned char>(h, w) = Img[h][w];
		}
	}

	return rtn;
}

void CViewerDlg::OnEditFilterClicked()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	if (isImageOpened && nFormat == FORMAT_GRAY)
	{
		CFIlterDlg FilterDlg;
		unsigned char** Pad, **Out;
		
		if (FilterDlg.DoModal() == IDOK)
		{
			int nFilterType = FilterDlg.getFilterType();

			switch (nFilterType)
			{
			case FILTER_AVERAGE:
			{
				int nFilterSize = FilterDlg.getFilterSize();
				if (nFilterSize >= 3)
				{
					if (nFilterSize % 2 == 0)
					{
						nFilterSize++;
					}
					Pad = Padding(ch_in_gray, nHeight_in, nWidth_in, nFilterSize);
					Out = MemAlloc2D(nHeight_in, nWidth_in, 0);

					AverageFilter(Pad, Out, nHeight_in, nWidth_in, nFilterSize);

					DisplayImage(GrayToMat(Out, nHeight_in, nWidth_in), false);

					MemFree2D(Pad, nHeight_in + (int)(nFilterSize / 2) * 2);
					MemFree2D(Out, nHeight_in);
				}
				else MessageBox("Filter Size가 잘못되었습니다. Filter Size는 3 이상의 정수 값을 가져야 합니다");
				break;
			}
			case FILTER_SMOOTH:
			{
				Pad = Padding(ch_in_gray, nHeight_in, nWidth_in, 3);
				Out = MemAlloc2D(nHeight_in, nWidth_in, 0);

				SmoothFilter(Pad, Out, nHeight_in, nWidth_in);

				DisplayImage(GrayToMat(Out, nHeight_in, nWidth_in), false);

				MemFree2D(Pad, nHeight_in + 2);
				MemFree2D(Out, nHeight_in);
				break;
			}
			case FILTER_SHARPEN:
			{
				Pad = Padding(ch_in_gray, nHeight_in, nWidth_in, 3);
				Out = MemAlloc2D(nHeight_in, nWidth_in, 0);

				SharpenFilter(Pad, Out, nHeight_in, nWidth_in);

				DisplayImage(GrayToMat(Out, nHeight_in, nWidth_in), false);

				MemFree2D(Pad, nHeight_in + 2);
				MemFree2D(Out, nHeight_in);
				break;
			}
			case FILTER_GAUSSIAN:
			{
				Pad = Padding(ch_in_gray, nHeight_in, nWidth_in, 3);
				Out = MemAlloc2D(nHeight_in, nWidth_in, 0);

				GaussianFilter(Pad, Out, nHeight_in, nWidth_in);

				DisplayImage(GrayToMat(Out, nHeight_in, nWidth_in), false);

				MemFree2D(Pad, nHeight_in + 2);
				MemFree2D(Out, nHeight_in);
				break;
			}
			}
		}
	}
	else
	{
		MessageBox("불러온 영상이 없거나 영상 포맷이 회색조 영상이 아닙니다.");
	}
}

void AverageFilter(unsigned char** In_Pad, unsigned char** Out, int nHeight, int nWidth, int nFilterSize)
{
	int PadSize = (int)(nFilterSize / 2);
	int nSum = 0;

	for (int h = 0; h < nHeight; h++)
	{
		for (int w = 0; w < nWidth; w++)
		{
			nSum = 0;
			for (int n = 0; n < nFilterSize; n++)
			{
				for (int m = 0; m< nFilterSize; m++)
				{
					nSum += In_Pad[h + n][w + m] * (double)1 / (nFilterSize*nFilterSize);
				}
			}

			Out[h][w] = (int)nSum;
		}
	}
}

void SmoothFilter(unsigned char** In_Pad, unsigned char** Out, int nHeight, int nWidth)
{
	//int PadSize = (int)(nFilterSize / 2);
	int nSum = 0;
	double mask[3][3] = { { 1, 2, 1 },{ 2, 4, 2 },{ 1, 2, 1 } };

	for (int h = 0; h < nHeight; h++)
	{
		for (int w = 0; w < nWidth; w++)
		{
			nSum = 0;
			for (int n = 0; n < 3; n++)
			{
				for (int m = 0; m< 3; m++)
				{
					nSum += In_Pad[h + n][w + m] * (double)(mask[n][m] / 16);
				}
			}

			Out[h][w] = (int)nSum;
		}
	}
}

void SharpenFilter(unsigned char** In_Pad, unsigned char** Out, int nHeight, int nWidth)
{
	//int PadSize = (int)(nFilterSize / 2);
	int nSum = 0;
	double mask[3][3] = { { 0, -1, 0 },{ -1, 5, -1 },{ 0, -1, 0 } };

	for (int h = 0; h < nHeight; h++)
	{
		for (int w = 0; w < nWidth; w++)
		{
			nSum = 0;
			for (int n = 0; n < 3; n++)
			{
				for (int m = 0; m< 3; m++)
				{
					nSum += In_Pad[h + n][w + m] * (double)mask[n][m];
				}
			}

			if (nSum > 255)
				nSum = 255;
			if (nSum < 0)
				nSum = 0;

			Out[h][w] = (int)nSum;
		}
	}
}

double g(double h, double w)
{
	double gisu;
	double sigma = 1;
	gisu = -1 * (h*h + w*w) / (2 * sigma*sigma);
	double PI = 3.141592;
	PI = sqrt(PI*sigma*sigma);
	return exp(gisu) / PI;
}

void GaussianFilter(unsigned char** In_Pad, unsigned char** Out, int nHeight, int nWidth)
{
	//int PadSize = (int)(nFilterSize / 2);
	int nSum = 0;
	double gauSum = 0;
	double gausMask[3][3] = { 0 };

	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			gausMask[i + 1][j + 1] = g(i, j);
			gauSum += gausMask[i + 1][j + 1];
		}
	}

	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			gausMask[i + 1][j + 1] = gausMask[i + 1][j + 1] / gauSum;
		}
	}

	for (int h = 0; h < nHeight; h++)
	{
		for (int w = 0; w < nWidth; w++)
		{
			nSum = 0;
			for (int n = 0; n < 3; n++)
			{
				for (int m = 0; m< 3; m++)
				{
					nSum += In_Pad[h + n][w + m] * gausMask[n][m];
				}
			}

			Out[h][w] = (int)nSum;
		}
	}
}



void CViewerDlg::OnNoisereductionAddnoiseClicked()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CAddNoiseDlg AddNoiseDlg;
	BYTE** NoiseImg = MemAlloc2D(nHeight_in, nWidth_in, 0);
	if (isImageOpened && nFormat == FORMAT_GRAY)
	{
		if (AddNoiseDlg.DoModal() == IDOK)
		{
			CString OutFileName = InpFileName;
			float Value_1 = AddNoiseDlg.GetVal_1();
			float Value_2 = AddNoiseDlg.GetVal_2();
			int nNoiseType = AddNoiseDlg.GetNoiseType();

			if (nNoiseType == 0)
			{
				if (Value_2 < 1)
				{
					MessageBox("표준편차 값이 잘못되었습니다. 표준편차 값은 1 이상이여야 합니다.");
					MemFree2D(NoiseImg, nHeight_in);
					return;
				}
				InputGaussianNoise(ch_in_gray, NoiseImg, nHeight_in, nWidth_in, Value_1, Value_2);
				OutFileName += "_GN.raw";
			}
			else
			{
				if (Value_1 <= 0 && Value_2 <= 0)
				{
					MessageBox("두 개의 확률 값 중 적어도 하나는 0보다 커야 합니다.");
					MemFree2D(NoiseImg, nHeight_in);
					return;
				}
				InputSaltPepperNoise(ch_in_gray, NoiseImg, nHeight_in, nWidth_in, Value_1, Value_2);
				OutFileName += "_SPN.raw";
			}

			FILE* fp;

			fopen_s(&fp, OutFileName, "wb");
			for (int h = 0; h < nHeight_in; h++)
			{
				fwrite(NoiseImg[h], sizeof(unsigned char), nWidth_in, fp);
			}

			fclose(fp);
			MemFree2D(NoiseImg, nHeight_in);
		}
	}
}

void InputGaussianNoise(unsigned char** In, unsigned char** Out, int nHeight, int nWidth, float fMean, float fStdev)
{
	float fTemp = 0, fPDF[256] = { 0.0f };

	GetGaussianPDF(fPDF, 256, fMean, fStdev);

	srand(GetTickCount());

	for (int h = 0; h < nHeight; h++)
	{
		for (int w = 0; w < nWidth; w++)
		{
			fTemp = (float)In[h][w] + GetNoise(fPDF, 256);
			Out[h][w] = static_cast<unsigned char>(fTemp);
		}
	}
}

void GetGaussianPDF(float* EmptyPDF, int nLength, float fMean, float fStDev)
{
	int n;
	int Center = nLength / 2;
	float x;
	double PI = 3.141592;

	for (n = 0; n<nLength; n++)
	{
		x = (float)(n - Center);
		EmptyPDF[n] = (1 / ((float)sqrt(2 * PI) * fStDev)) * exp(-pow(x - fMean, 2) / (2 * fStDev * fStDev));
	}
}

float GetNoise(float* PDF, int nLength)
{
	int n;
	int Center = nLength / 2;
	float fRand, fComp, fTemp = 0;
	float x = 0, fDiff;
	float* CDF = new float[nLength];

	CDF[0] = 0;

	fRand = (float)rand() / (RAND_MAX + 1); // 0~1 Uniform Distribution

	for (n = 1; n<nLength; n++)
	{
		CDF[n] = (PDF[n] + PDF[n - 1]) / 2 + CDF[n - 1];
		fDiff = fRand - CDF[n];
		if (fDiff < 0)
		{
			x = ((float)n - Center);
			break;
		}
	}
	fComp = (fRand - CDF[n - 1]) / (CDF[n] - CDF[n - 1]);

	delete[] CDF;

	return x + fComp;
}

void InputSaltPepperNoise(unsigned char** In, unsigned char** Out, int nHeight, int nWidth, float fSProb, float fPProb)
{
	float Low = fSProb;
	float High = 1.0f - fPProb;
	float fRand;

	srand(GetTickCount());

	for (int h = 0; h < nHeight; h++)
	{
		for (int w = 0; w < nWidth; w++)
		{
			fRand = ((float)rand() / RAND_MAX);

			if (fRand < Low)
			{
				Out[h][w] = 255;
			}
			else if (fRand > High)
			{
				Out[h][w] = 0;
			}
			else Out[h][w] = In[h][w];
		}
	}
}


void CViewerDlg::OnNoisefilteringMeanfilterClicked()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CMeanFilterDlg MFDlg;

	if (isImageOpened == true && nFormat == FORMAT_GRAY)
	{
		if (MFDlg.DoModal() == IDOK)
		{
			int nFilterSize = MFDlg.GetFilterSize();
			int nFilterType = MFDlg.GetMeanFilterType();
			
			if (nFilterSize < 3)
			{
				MessageBox("필터 크기가 잘못되었습니다.");
				return;
			}

			if (nFilterSize % 2 == 0)
			{
				nFilterSize++;
			}

			CString OutFileName = InpFileName;
			unsigned char** Out = MemAlloc2D(nHeight_in, nWidth_in, 0);

			switch (nFilterType)
			{
			case NF_ARITHMETIC:
				ArithmeticMeanFilter(ch_in_gray, Out, nHeight_in, nWidth_in, nFilterSize);
				OutFileName += "_AMF.raw";
				break;

			case NF_GEOMETRIC:
				GeometricMeanFilter(ch_in_gray, Out, nHeight_in, nWidth_in, nFilterSize);
				OutFileName += "_GMF.raw";
				break;

			case NF_HARMONIC:
				HarmonicMeanFilter(ch_in_gray, Out, nHeight_in, nWidth_in, nFilterSize);
				OutFileName += "HMF.raw";
				break;

			case NF_CONTRAHARMONIC:
				int Q = MFDlg.GetQ();
				ContraharmonicMeanFilter(ch_in_gray, Out, nHeight_in, nWidth_in, nFilterSize, Q);
				OutFileName += "_CHMF.raw";
				break;
			}

			DisplayImage(GrayToMat(Out, nHeight_in, nWidth_in), false);

			FILE *fp;

			fopen_s(&fp, OutFileName, "wb");

			for (int h = 0; h < nHeight_in; h++)
			{
				fwrite(Out[h], sizeof(unsigned char), nWidth_in, fp);
			}

			fclose(fp);
		}
	}
}

void ArithmeticMeanFilter(unsigned char **Img_in, unsigned char** Out, int nHeight, int nWidth, int nFilterSize)
{
	int nTemp, nPadSize = (int)(nFilterSize / 2);

	unsigned char** Img_pad = Padding(Img_in, nHeight, nWidth, nFilterSize);

	for (int h = 0; h < nHeight; h++)
	{
		for (int w = 0; w < nWidth; w++)
		{
			nTemp = 0;

			for (int m = 0; m < nFilterSize; m++)
			{
				for (int n = 0; n < nFilterSize; n++)
				{
					nTemp += Img_pad[h + m][w + n];
				}
			}

			Out[h][w] = static_cast<unsigned char>(nTemp / (nFilterSize*nFilterSize));
		}
	}

	MemFree2D(Img_pad, nHeight + 2 * nPadSize);
}

void GeometricMeanFilter(unsigned char **Img_in, unsigned char** Out, int nHeight, int nWidth, int nFilterSize)
{
	int nPadSize = (int)(nFilterSize / 2);
	unsigned char** In_Pad = Padding(Img_in, nHeight, nWidth, nFilterSize);
	double Temp;
	double result;

	for (int h = 0; h < nHeight; h++)
	{
		for (int w = 0; w < nWidth; w++)
		{
			Temp = 1;
			for (int n = 0; n < nFilterSize; n++)
			{
				for (int m = 0; m < nFilterSize; m++)
				{
					Temp *= In_Pad[h + n][w + m];
				}
			}

			result = pow(Temp, (double)1 / (nFilterSize*nFilterSize));
			Out[h][w] = static_cast<unsigned char>(result);
		}
	}
	MemFree2D(In_Pad, nHeight + 2 * nPadSize);

}

void HarmonicMeanFilter(unsigned char** Img_in, unsigned char** Out, int nHeight, int nWidth, int nFilterSize)
{
	int nPadSize = (int)(nFilterSize / 2);
	unsigned char** In_Pad = Padding(Img_in, nHeight, nWidth, nFilterSize);
	double Temp;

	for (int h = 0; h < nHeight; h++)
	{
		for (int w = 0; w < nWidth; w++)
		{
			Temp = 0;
			for (int n = 0; n < nFilterSize; n++)
			{
				for (int m = 0; m < nFilterSize; m++)
				{
					Temp += ((double)1 / In_Pad[h + n][w + m]);
				}
			}

			Temp = (double)(nFilterSize*nFilterSize) / Temp;
			Out[h][w] = static_cast<unsigned char>(Temp);
		}
	}
	MemFree2D(In_Pad, nHeight + 2 * nPadSize);
}

void ContraharmonicMeanFilter(unsigned char **Img_in, unsigned char** Out, int nHeight, int nWidth, int nFilterSize, int Q)
{
	int nPadSize = (int)(nFilterSize / 2);
	unsigned char** In_Pad = Padding(Img_in, nHeight, nWidth, nFilterSize);

	double up = 0;
	double down = 0;

	for (int h = 0; h < nHeight; h++)
	{
		for (int w = 0; w < nWidth; w++)
		{
			up = 0;
			down = 0;
			for (int n = 0; n < nFilterSize; n++)
			{
				for (int m = 0; m < nFilterSize; m++)
				{
					up += pow(In_Pad[h + n][w + m], Q + 1);
					down += pow(In_Pad[h + n][w + m], Q);
				}
			}
			Out[h][w] = static_cast<unsigned char>(up / down);
		}
	}
	MemFree2D(In_Pad, nHeight + 2 * nPadSize);

}


void CViewerDlg::OnNoisefilteringOrderStatisticClicked()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	COrderStatisticsDlg OSFDlg;

	if (isImageOpened == true && nFormat == FORMAT_GRAY)
	{
		if (OSFDlg.DoModal() == IDOK)
		{
			int nFilterSize = OSFDlg.GetFilterSize1();
			//int MaxFilterSize = OSFDlg.GetFilterSize2();
			int nFilterType = OSFDlg.GetOSFType();

			if (nFilterSize < 3)
			{
				MessageBox("필터 크기가 잘못되었습니다.");
				return;
			}

			if (nFilterSize % 2 == 0)
			{
				nFilterSize++;
			}

			CString OutFileName = InpFileName;
			unsigned char** Out = MemAlloc2D(nHeight_in, nWidth_in, 0);

			switch (nFilterType)
			{
			case NF_MEDIAN:
				MedianFilter(ch_in_gray, Out, nHeight_in, nWidth_in, nFilterSize);
				OutFileName += "_MEDF.raw";
				break;

			case NF_ADAPTIVEMEDIAN:
				int MaxFilterSize = OSFDlg.GetFilterSize2();
				AdaptiveMedianFilter(ch_in_gray, Out, nHeight_in, nWidth_in, nFilterSize, MaxFilterSize);
				OutFileName += "_AMEDF.raw";
				break;
			}

			DisplayImage(GrayToMat(Out, nHeight_in, nWidth_in), false);

			FILE* fp;

			fopen_s(&fp, OutFileName, "wb");

			for (int h = 0; h < nHeight_in; h++)
			{
				fwrite(Out[h], sizeof(unsigned char), nWidth_in, fp);
			}

			fclose(fp);
		}
	}
}

void q_sort(unsigned char numbers[], int left, int right)
{
	int pivot, l_hold, r_hold;
	l_hold = left;
	r_hold = right;
	pivot = numbers[left]; // 0번째 원소를 피봇으로 선택
	while (left < right)
	{
		// 값이 선택한 피봇과 같거나 크다면, 이동할 필요가 없다
		while ((numbers[right] >= pivot) && (left < right))
			right--;

		// 그렇지 않고 값이 피봇보다 작다면,
		// 피봇의 위치에 현재 값을 넣는다.
		if (left != right)
		{
			numbers[left] = numbers[right];
		}
		// 왼쪽부터 현재 위치까지 값을 읽어들이면서
		// 피봇보다 큰 값이 있다면, 값을 이동한다.
		while ((numbers[left] <= pivot) && (left < right))
			left++;
		if (left != right)
		{
			numbers[right] = numbers[left];
			right--;
		}
	}
	// 모든 스캔이 끝났다면, 피봇값을 현재 위치에 입력한다.
	// 이제 피봇을 기준으로 왼쪽에는 피봇보다 작거나 같은 값만 남았다.
	numbers[left] = pivot;
	pivot = left;
	left = l_hold;
	right = r_hold;

	// 재귀호출을 수행한다.
	if (left < pivot)
		q_sort(numbers, left, pivot - 1);
	if (right > pivot)
		q_sort(numbers, pivot + 1, right);
}

void quickSort(unsigned char numbers[], int array_size)
{
	q_sort(numbers, 0, array_size - 1);
}

void MedianFilter(unsigned char **Img_in, unsigned char** Out, int nHeight, int nWidth, int nFilterSize)
{
	int nPadSize = (int)(nFilterSize / 2);

	unsigned char *arr;
	arr = new unsigned char[nFilterSize*nFilterSize];

	unsigned char** Img_pad = Padding(Img_in, nHeight, nWidth, nFilterSize);

	for (int h = 0; h < nHeight; h++)
	{
		for (int w = 0; w < nWidth; w++)
		{
			int i = 0;

			for (int m = 0; m < nFilterSize; m++)
			{
				for (int n = 0; n < nFilterSize; n++)
				{
					arr[i] = Img_pad[h + n][w + m];
					i++;
				}
			}
			
			quickSort(arr, nFilterSize*nFilterSize);

			//sort(arr, arr + nFilterSize*nFilterSize);

			Out[h][w] = arr[(int)(nFilterSize*nFilterSize / 2)];
		}
	}

	MemFree2D(Img_pad, nHeight + 2 * nPadSize);
}

void AdaptiveMedianFilter(unsigned char **Img_in, unsigned char** Out, int nHeight, int nWidth, int nFilterSize_min, int nFilterSize_Max)
{
	int nPadSize = (int)(nFilterSize_min / 2);
	unsigned char** In_Pad = Padding(Img_in, nHeight, nWidth, nFilterSize_min);
	unsigned char* sorting = new unsigned char[nFilterSize_Max*nFilterSize_Max];//sorting 범위 최대 지정

	int nFilter_Change;
	int A1, A2, Zxy, Zmed, B1, B2, Result;

	for (int h = 0; h < nHeight; h++)
	{
		for (int w = 0; w < nWidth; w++)
		{
			A1 = 0, A2 = 0, Zmed = 0, B1 = 0, B2 = 0, Result = 0;
			nFilter_Change = nFilterSize_min;

			Zxy = In_Pad[h + 1][w + 1];
			//Level A
			while (nFilter_Change <= nFilterSize_Max)
			{
				//mask sorting
				int sort_num = 0;
				memset(sorting, 0, nFilterSize_Max*nFilterSize_Max);
				for (int n = 0; n < nFilter_Change; n++)
					for (int m = 0; m < nFilter_Change; m++)
					{
						//Out of Range
						if (h + n <= nHeight + 2 - 1 && h + m <= nWidth + 2 - 1)
						{
							sorting[sort_num] = In_Pad[h + n][w + m];
							sort_num++;
						}
					}
				quickSort(sorting, sort_num);
				Zmed = sorting[(sort_num) / 2];

				A1 = Zmed - sorting[0];
				A2 = Zmed - sorting[nFilter_Change*nFilter_Change - 1];

				//Level A → Level B
				if (A1 > 0 && A2 < 0)
					break;
				//Level A Repeat
				else
					nFilter_Change += 2;
			}

			//Level B
			B1 = Zxy - sorting[0];
			B2 = Zxy - sorting[nFilter_Change*nFilter_Change - 1];
			if (B1 > 0 && B2 < 0)
			{
				Result = Zxy;
			}
			else
			{
				Result = Zmed;
			}

			Out[h][w] = (unsigned char)Result;
		}
	}
	delete[] sorting;
	MemFree2D(In_Pad, nHeight + 2 * nPadSize);
}

void CViewerDlg::OnGetpsnr()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CPSNRDlg PSNRDlg;

	PSNRDlg.DoModal();
}
