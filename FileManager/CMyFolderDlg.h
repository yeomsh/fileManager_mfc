#pragma once


// CMyFolderDlg 대화 상자

class CMyFolderDlg : public CDialog
{
	DECLARE_DYNAMIC(CMyFolderDlg)

public:
	CMyFolderDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CMyFolderDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	CEdit m_edit;
	CString folderName;
};
