#pragma once


// CMyHelpDlg 대화 상자

class CMyHelpDlg : public CDialog
{
	DECLARE_DYNAMIC(CMyHelpDlg)

public:
	CMyHelpDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CMyHelpDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CBitmapButton m_okBtn;
};
