// CVariableDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "FileManager.h"
#include "CVariableDlg.h"
#include "afxdialogex.h"
#include "FileManagerDlg.h"

// CVariableDlg 대화 상자

IMPLEMENT_DYNAMIC(CVariableDlg, CDialog)

CVariableDlg::CVariableDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG1, pParent)
{
	m_edit = "";
}

CVariableDlg::~CVariableDlg()
{
}

void CVariableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_combo);
	DDX_Text(pDX, IDC_EDIT2, m_edit);
}


BEGIN_MESSAGE_MAP(CVariableDlg, CDialog)
END_MESSAGE_MAP()


// CVariableDlg 메시지 처리기


BOOL CVariableDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_combo.AddString(_T("숫자"));                   //텍스트 추가
	m_combo.AddString(_T("날짜"));
	m_combo.AddString(_T("장소"));
	m_combo.AddString(_T("가수"));
	m_combo.AddString(_T("노래제목"));
	m_combo.SetCurSel(0);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CVariableDlg::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	int m_index = m_combo.GetCurSel();
	//AfxMessageBox(m_index);
	if (m_index != CB_ERR) {
		m_num = m_index;
	}
	CDialog::OnOK();
}