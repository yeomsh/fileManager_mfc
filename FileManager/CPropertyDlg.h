#pragma once


// CPropertyDlg 대화 상자

class CPropertyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPropertyDlg)

public:
	CPropertyDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CPropertyDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PROP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_thumb;
	CStatic m_text;
};
