// CMyHelpDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "FileManager.h"
#include "CMyHelpDlg.h"
#include "afxdialogex.h"


// CMyHelpDlg 대화 상자

IMPLEMENT_DYNAMIC(CMyHelpDlg, CDialog)

CMyHelpDlg::CMyHelpDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG3, pParent)
{

}

CMyHelpDlg::~CMyHelpDlg()
{
}

void CMyHelpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_okBtn);
}


BEGIN_MESSAGE_MAP(CMyHelpDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CMyHelpDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CMyHelpDlg 메시지 처리기


void CMyHelpDlg::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CDialog::OnOK();
}


BOOL CMyHelpDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_okBtn.LoadBitmaps(IDB_BITMAP10, IDB_BITMAP10, IDB_BITMAP10, IDB_BITMAP10);
	m_okBtn.SizeToContent();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CMyHelpDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialog::OnOK();
}
