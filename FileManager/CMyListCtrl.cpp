// CMyListCtrl.cpp: 구현 파일
//

#include "stdafx.h"
#include "FileManager.h"
#include "CMyListCtrl.h"
#include<algorithm>
#include <utility>
#include "FileManagerDlg.h"//  !!부모 다이얼로그 헤더
#include "filenameInfo.h"
#include "tag.h"  //  !!taglib용
#include "fileref.h"  //  !!taglib용

#define MAX_FILE_NUM 14  //  !!우측 종스크롤바 생성전 최대 파일수

using namespace Gdiplus;  //  !!GDI+ 사용

// CMyListCtrl

IMPLEMENT_DYNAMIC(CMyListCtrl, CListCtrl)

CMyListCtrl::CMyListCtrl() : indexItem(0), indexSubitem(0), totFileNum(0)
{

}

CMyListCtrl::~CMyListCtrl()
{

}


BEGIN_MESSAGE_MAP(CMyListCtrl, CListCtrl)
	ON_WM_DROPFILES()
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CMyListCtrl::OnNMDblclk)
	ON_NOTIFY(HDN_BEGINTRACKA, 0, &CMyListCtrl::OnHdnBegintrack)
	ON_NOTIFY(HDN_BEGINTRACKW, 0, &CMyListCtrl::OnHdnBegintrack)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, &CMyListCtrl::OnLvnColumnclick)
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_NOTIFY_REFLECT(NM_CLICK, &CMyListCtrl::OnNMClick)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CMyListCtrl::OnNMCustomdraw)
END_MESSAGE_MAP()



// CMyListCtrl 메시지 처리기

bool compare(const filenameInfo &s1, const filenameInfo& s2) {

	return s1.beforePath < s2.beforePath;
}
void CMyListCtrl::OnDropFiles(HDROP hDropInfo)
{

	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	TRACE(_T("ASDASDF"));
	m_pDlg = (CFileManagerDlg*)GetParent();  //  !!부모 지정
	setlocale(LC_ALL, "korean"); // 인코딩 관련

	int FileNum;
	char FilePath[MAX_PATH]; // 파일 경로명
	CString FileName;

	// 드래그 앤 드롭 된 파일 수
	FileNum = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, MAX_PATH);

	for (int i = 0; i < FileNum; i++) {
		bool flag = true;
		DragQueryFileA(hDropInfo, i, FilePath, MAX_PATH);
		CString str = CString(FilePath);
		for (int j = 0; j < f_info.size(); j++) {
			if (str == f_info[j].beforePath) {
				flag = false;
				break;
			}
		}
		if (flag == true) { //기존의 것과 겹치는 것이 없을 때

			f_info.push_back(filenameInfo(str));

			CString strTok, temp = str, insert;
			int iSub = 0;
			while (AfxExtractSubString(strTok, temp, iSub++, '\\'))
			{
				if (strTok != _T("\r\n"))
					insert = strTok;
			}
			this->InsertItem(0, insert, 0);
		}
	}
	std::sort(f_info.begin(), f_info.end(), compare);

	m_pDlg->makeDirectory(f_info);

	totFileNum = f_info.size();  //  !!우측 종스크롤 생성여부 판단위한 리스트목록수

								 /////////////////////////////////////////////////////////////////////////
								 //  !!파일항목 정렬
	for (int i = 0; i < totFileNum; i++) {
		CString strTok, temp = f_info[i].beforePath, insert;
		int iSub = 0;
		while (AfxExtractSubString(strTok, temp, iSub++, '\\'))
		{
			if (strTok != _T("\r\n"))
				insert = strTok;
		}
		f_info[i].beforeName = insert;
		if (!f_info[i].changeCheck)
			this->SetItemText(i, 0, insert);
		else
			this->SetItemText(i, 0, f_info[i].afterName);
	}
	//  !!파일별 위치 디렉토리 저장
	for (int i = 0; i < totFileNum; i++) {
		CString strTok, temp = f_info[i].beforePath, insert, direct = _T("");
		int iSub = 0;
		while (AfxExtractSubString(strTok, temp, iSub++, '\\'))
		{
			if (strTok == f_info[i].beforeName) {
				direct += _T("\r\n");
				break;
			}
			direct += strTok;
			direct += _T("\\");
		}
		f_info[i].beforeDirect = direct;
	}
	CString beforeDir = _T("");
	for (int i = 0; i < totFileNum; i++) {
		if (beforeDir != f_info[i].beforeDirect) {
			beforeDir = f_info[i].beforeDirect;
			f_info[i].first_in_direct = true;
		}
		else
			f_info[i].first_in_direct = false;
	}
	/////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////
	//  !!사운드파일 메타데이터
	TagLib::FileRef empty;
	TagLib::String artist, title;
	CString strArtist, strTitle;
	CFile file;

	for (int i = 0; i < totFileNum; i++) {
		if (1) {
			// 사운드 태그 출력
			TagLib::FileName pathSound(f_info[i].beforePath);
			TagLib::FileRef sndFile(pathSound);

			artist.clear();
			title.clear();
			strArtist.Empty();
			strTitle.Empty();

			if (sndFile.tag() == NULL) {  //  !!사운드 메타데이터가 없으면 통과
				this->SetItemText(i, ARTIST, strArtist);
				this->SetItemText(i, TITLE, strTitle);
				continue;
			}

			artist = sndFile.tag()->artist();
			if (artist.isLatin1())
				strArtist = artist.toCString();
			else
				strArtist = artist.toCWString();

			title = sndFile.tag()->title();
			if (title.isLatin1())
				strTitle = title.toCString();
			else
				strTitle = title.toCWString();

			f_info[i].strArtist = strArtist;
			f_info[i].strTitle = strTitle;

			this->SetItemText(i, ARTIST, strArtist);
			this->SetItemText(i, TITLE, strTitle);
		}
	}
	//////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////
	//  !!이미지파일 메타데이터
	for (int i = 0; i < totFileNum; i++) {
		Image img(f_info[i].beforePath);
		UINT size, count;
		PropertyItem *arPro;
		arPro = NULL;
		TCHAR str[1024];

		img.GetPropertySize(&size, &count);
		arPro = (PropertyItem*)malloc(size);
		img.GetAllPropertyItems(size, count, arPro);
		double lat = 0, lon = 0;
		CString date, latitude, longitude, location = _T("");

		if (arPro == NULL) {  //  !!이미지 메타데이터가 없으면 통과
			::free(arPro);
			this->SetItemText(i, MOMENT, date);
			continue;
		}
		CString test;
		double sec = 0;
		for (int j = 0; j < count; j++) {
			switch (arPro[j].id) {
			case PropertyTagDateTime:
				date.Format(_T("%S"), (TCHAR *)arPro[j].value);
				date.Replace(':', '-');
				//wsprintf(str, TEXT("촬영날짜 : %S"), (TCHAR *)arPro[j].value);
				break;
			case PropertyTagGpsLatitude:
				if (*((LONG *)arPro[j].value + 4) >= 6000)
					sec = *((LONG *)arPro[j].value + 4) / 36000000.0;
				else if (*((LONG *)arPro[j].value + 4) >= 60)
					sec = *((LONG *)arPro[j].value + 4) / 360000.0;
				else if (*((LONG *)arPro[j].value + 4) < 60)
					sec = *((LONG *)arPro[j].value + 4) / 3600.0;
				lat = *((LONG *)arPro[j].value) + *((LONG *)arPro[j].value + 2) / 60.0 + sec;
				//latitude.Format(_T("위도 : %d도 %d분 %d초 = %f도"), *((LONG *)arPro[j].value), *((LONG *)arPro[j].value + 2), *((LONG *)arPro[j].value + 4), lat);
				//wsprintf(str, latitude);
				break;
			case PropertyTagGpsLongitude:
				if (*((LONG *)arPro[j].value + 4) >= 6000)
					sec = *((LONG *)arPro[j].value + 4) / 36000000.0;
				else if (*((LONG *)arPro[j].value + 4) >= 60)
					sec = *((LONG *)arPro[j].value + 4) / 360000.0;
				else if (*((LONG *)arPro[j].value + 4) < 60)
					sec = *((LONG *)arPro[j].value + 4) / 3600.0;
				lon = *((LONG *)arPro[j].value) + *((LONG *)arPro[j].value + 2) / 60.0 + sec;
				//longitude.Format(_T("경도 : %d도 %d분 %d초 = %f도"), *((LONG *)arPro[j].value), *((LONG *)arPro[j].value + 2), *((LONG *)arPro[j].value + 4), lon);
				//wsprintf(str, longitude);
				break;
			default:
				continue;
			}
		}

		location = GPStoLocation(lat, lon);
		::free(arPro);
		f_info[i].strDate = date;
		f_info[i].strLocation = location;

		this->SetItemText(i, MOMENT, date);
		this->SetItemText(i, LOCATION, location);
	}
	///////////////////////////////////////////////////////////////////////
	//for (int i = 0; i < totFileNum; i++)
	//AfxMessageBox(f_info[i].beforeDirect);

	CListCtrl::OnDropFiles(hDropInfo);
}


CString CMyListCtrl::GPStoLocation(double lat, double lon)
{
	CRect Seoul(CPoint(37688761, 126816484), CPoint(37454743, 127166673));
	CRect Incheon(CPoint(37859579, 125651581), CPoint(37334233, 126794770));
	CRect Gyeonggi(CPoint(38122494, 125806442), CPoint(37146537, 127547623));
	CRect GangWon(CPoint(38424319, 127547623), CPoint(37146537, 129276391));
	CRect ChungN(CPoint(37146537, 127485462), CPoint(36105798, 127807093));
	CRect ChungS(CPoint(37146537, 126137015), CPoint(36105798, 127485462));
	CRect JunN(CPoint(36105798, 126544986), CPoint(35351375, 127807093));
	CRect JunS(CPoint(35371375, 125177564), CPoint(34122616, 127792310));
	CRect GyungN(CPoint(37146537, 127807093), CPoint(35686623, 129492093));
	CRect GyungS(CPoint(35686623, 127807093), CPoint(34786891, 129492093));
	CRect JeJu(CPoint(33567611, 126145904), CPoint(33092066, 126968850));

	CRgn SeoulRgn;
	CRgn IncheonRgn;
	CRgn GyeonggiRgn;
	CRgn GangWonRgn;
	CRgn ChungNRgn; //충북
	CRgn ChungSRgn; //충남
	CRgn JunNRgn;
	CRgn JunSRgn;
	CRgn GyungNRgn;
	CRgn GyungSRgn;
	CRgn JeJuRgn;

	GyeonggiRgn.CreateRectRgnIndirect(Gyeonggi);
	GangWonRgn.CreateRectRgnIndirect(GangWon);
	SeoulRgn.CreateRectRgnIndirect(Seoul);
	IncheonRgn.CreateRectRgnIndirect(Incheon);
	ChungNRgn.CreateRectRgnIndirect(ChungN);
	ChungSRgn.CreateRectRgnIndirect(ChungS);
	JunNRgn.CreateRectRgnIndirect(JunN);
	JunSRgn.CreateRectRgnIndirect(JunS);
	GyungNRgn.CreateRectRgnIndirect(GyungN);
	GyungSRgn.CreateRectRgnIndirect(GyungS);
	JeJuRgn.CreateRectRgnIndirect(JeJu);

	CPoint point = CPoint(lat * 1000000, lon * 1000000); // lat : 위도 , lon : 경도
	CString area = _T("");

	if (SeoulRgn.PtInRegion(point)) {
		area = _T("서울");
	}
	else if (IncheonRgn.PtInRegion(point)) {
		area = _T("인천");
	}
	else if (GangWonRgn.PtInRegion(point)) {
		area = _T("강원");
	}
	else if (GyeonggiRgn.PtInRegion(point)) {
		area = _T("경기");
	}
	else if (ChungNRgn.PtInRegion(point)) {
		area = _T("충북");
	}
	else if (ChungSRgn.PtInRegion(point)) {
		area = _T("충남");
	}
	else if (JunNRgn.PtInRegion(point)) {
		area = _T("전북");
	}
	else if (JunSRgn.PtInRegion(point)) {
		area = _T("전남");
	}
	else if (GyungNRgn.PtInRegion(point)) {
		area = _T("경북");
	}
	else if (GyungSRgn.PtInRegion(point)) {
		area = _T("경남");
	}
	else if (JeJuRgn.PtInRegion(point)) {
		area = _T("제주");
	}
	else
		area = _T("");

	return area;
}

void CMyListCtrl::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_pDlg = (CFileManagerDlg*)GetParent();  //  !!부모 지정
	HWND hEdit;  //  !!에디트컨트롤 핸들러
	hEdit = ::GetDlgItem(m_pDlg->GetSafeHwnd(), IDC_EDIT1);

	//Invalidate(FALSE);
	m_pDlg->m_ctrEdit.ShowWindow(SW_HIDE);

	indexItem = pNMItemActivate->iItem;
	indexSubitem = pNMItemActivate->iSubItem;

	if (indexItem < 0 || indexSubitem <= 0)  //  !!태그지역 외 클릭시 무시
		return;

	CString strText = GetItemText(indexItem, indexSubitem);  //  !!선택항목 텍스트 저장

	CRect rtSubitem, rtListCtrl, rtDlg;  //  !!항목 Rect 저장
	if (GetSubItemRect(pNMItemActivate->iItem, pNMItemActivate->iSubItem, LVIR_BOUNDS, rtSubitem) == FALSE)
		return;

	::GetWindowRect(GetSafeHwnd(), &rtListCtrl);  //  !!리스트컨트롤 Rect 저장

	::GetWindowRect(m_pDlg->GetSafeHwnd(), &rtDlg);  //  !!부모 다이얼로그 Rect 저장

													 //  !!리스트컨트롤의 좌상단 위치 계산
	int nLeft = rtListCtrl.left - rtDlg.left;
	int nTop = rtListCtrl.top - rtDlg.top;

	//  !!리스트컨트롤 영역 내 여유길이
	int cornerWidth = rtListCtrl.Width() - rtSubitem.left;
	int cornerHeight = rtListCtrl.Height() - rtSubitem.top;



	//  !!에디트컨트롤 위치 변경
	int width, height;
	if (totFileNum >= MAX_FILE_NUM) {
		if (cornerWidth < rtSubitem.Width() + 21 && cornerHeight >= rtSubitem.Height()) {
			width = cornerWidth - 21;
			height = rtSubitem.Height();
		}
		else if (cornerWidth >= rtSubitem.Width() + 21 && cornerHeight < rtSubitem.Height()) {
			width = rtSubitem.Width();
			height = cornerHeight - 21;
		}
		else if (cornerWidth < rtSubitem.Width() + 21 && cornerHeight < rtSubitem.Height()) {
			width = cornerWidth - 21;
			height = cornerHeight - 21;
		}
		else {
			width = rtSubitem.Width();
			height = rtSubitem.Height();
		}
	}
	else {
		if (cornerWidth < rtSubitem.Width() && cornerHeight >= rtSubitem.Height()) {
			width = cornerWidth - 5;
			height = rtSubitem.Height();
		}
		else if (cornerWidth >= rtSubitem.Width() && cornerHeight < rtSubitem.Height()) {
			width = rtSubitem.Width();
			height = cornerHeight - 5;
		}
		else if (cornerWidth < rtSubitem.Width() && cornerHeight < rtSubitem.Height()) {
			width = cornerWidth - 5;
			height = cornerHeight - 5;
		}
		else {
			width = rtSubitem.Width();
			height = rtSubitem.Height();
		}
	}
	height += 3;  //  !!크기조절
	::SetWindowPos(hEdit, HWND_TOP,
		nLeft + rtSubitem.left - 5,
		nTop + rtSubitem.top - 30,
		width,
		height,
		NULL);

	//  !!에디트컨트롤 출력
	m_pDlg->m_ctrEdit.ShowWindow(SW_SHOW);
	m_pDlg->m_ctrEdit.SetFocus();
	m_pDlg->m_ctrEdit.SetWindowTextW(strText);
	int nSel = strText.GetLength();
	::SendMessage(hEdit, EM_SETSEL, LOWORD(nSel), HIWORD(nSel));

	*pResult = 0;
}


void CMyListCtrl::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_pDlg = (CFileManagerDlg*)GetParent();  //  !!부모 지정
	m_pDlg->m_ctrEdit.ShowWindow(SW_HIDE);
	HWND hEdit;  //  !!에디트컨트롤 핸들러
	hEdit = ::GetDlgItem(m_pDlg->GetSafeHwnd(), IDC_EDIT1);
	::SendMessage(hEdit, WM_KILLFOCUS, 0, 0);
	m_pDlg->m_list.SetFocus();

	//  !!선택 인덱스 반환
	int index = pNMItemActivate->iItem;
	if (index < 0)
		return;

	//  !!파일 길이
	CString str;
	if (!f_info[index].changeCheck)
		str = f_info[index].beforePath;
	else
		str = f_info[index].afterPath;
	CString strRes, strByte;
	CFile file;
	file.Open(str, CFile::modeRead);
	ULONGLONG file_size = file.GetLength();
	file.Close();

	//  !!선택한 항목(이미지) - 썸네일 출력
	float constant;
	float width = 0, height = 0;
	Image image(str);

	if (image.GetFrameDimensionsCount() == 1) {
		if (image.GetWidth() > image.GetHeight()) {
			constant = image.GetWidth() / (float)PROP_LENGTH;
			width = PROP_LENGTH;
			height = image.GetHeight() / constant;
		}
		else if (image.GetWidth() <= image.GetHeight()) {
			constant = image.GetHeight() / (float)PROP_LENGTH;
			width = image.GetWidth() / constant;
			height = PROP_LENGTH;
		}
		else
			return;

		int nWidth = image.GetWidth();
		int nHeight = image.GetHeight();
		strRes.Format(_T("이미지 크기 : %d * %d"), nWidth, nHeight);
	}
	if (file_size < 1024)
		strByte.Format(_T("파일 크기 : %d byte"), file_size);
	else if (file_size < 1048576)
		strByte.Format(_T("파일 크기 : %.1f KB"), file_size / 1024.0);
	else if (file_size < 1073741824)
		strByte.Format(_T("파일 크기 : %.1f MB"), file_size / 1048576.0);
	else
		strByte.Format(_T("파일 크기 : %.1f GB"), file_size / 1073741824.0);
	UpdateThumbNail(str, width, height, strRes, strByte);  //  !!썸네일 출력



														   //  !!선택한 항목(사운드) - 앨범아트 출력
	TagLib::FileRef empty;
	TagLib::String albumArt;
	CString strAlbumArt;

	if (1) {
		// 사운드 태그 출력
		TagLib::FileName pathSound(str);
		TagLib::FileRef sndFileRef(pathSound);
		albumArt.clear();
		strAlbumArt.Empty();

		if (sndFileRef.tag() == NULL)
			return;

		TagLib::AudioProperties *properties = sndFileRef.audioProperties();

		int seconds = properties->length() % 60;
		int minutes = (properties->length() - seconds) / 60;
		CString strPlyTime;
		strPlyTime.Format(_T("재생 시간 : %d분 %d초"), minutes, seconds);
		m_pDlg->m_propDlg->m_text.SetWindowTextW(strPlyTime + _T("\r\n") + strByte);

		//#include <mpeg/mpegfile.h>
		//#include <mpeg/id3v2/frames/attachedpictureframe.h>
		//#include <mpeg/id3v2/id3v2tag.h>
		//#include <mpeg/id3v2/id3v2frame.h>
		//#include <mpeg/id3v2/id3v2header.h>
		//
		//      static const char *idPicture = "APIC";
		//
		//sndFile.

		//artist = sndFile.tag()->artist();
		//if (artist.isLatin1())
		//   strArtist = artist.toCString();
		//else
		//   strArtist = artist.toCWString();

		//title = sndFile.tag()->title();
		//if (title.isLatin1())
		//   strTitle = title.toCString();
		//else
		//   strTitle = title.toCWString();
		//this->SetItemText(i, ARTIST, strArtist);
		//this->SetItemText(i, TITLE, strTitle);
	}


	*pResult = 0;
}


void CMyListCtrl::UpdateThumbNail(CString str, UINT width, UINT height, CString strRes, CString strByte)
{
	Image image(str);
	Image* pThumb = image.GetThumbnailImage(width, height, NULL, NULL);  //  !!150*150 출력

	Bitmap* pBitmap = (Bitmap*)pThumb;    //Bitmap으로 변환
	HBITMAP hBitmap;
	pBitmap->GetHBITMAP(Color(255, 255, 255), &hBitmap);//HBITMAP으로 변환
	m_pDlg->m_propDlg->m_thumb.SetBitmap(hBitmap);    //뿌린다!!
	m_pDlg->m_propDlg->m_thumb.MoveWindow((PROP_WIDTH - width) / 2.0, (PROP_WIDTH - height) / 2.0, width, height);
	m_pDlg->m_propDlg->m_text.SetWindowTextW(strRes + _T("\r\n") + strByte);
	m_pDlg->m_propDlg->m_text.MoveWindow((PROP_WIDTH - PROP_LENGTH) / 2.0, (PROP_WIDTH + PROP_LENGTH) / 2.0 + 10, PROP_LENGTH, 30);

	delete pThumb;
}


void CMyListCtrl::OnHdnBegintrack(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_pDlg = (CFileManagerDlg*)GetParent();  //  !!부모 지정
	m_pDlg->m_ctrEdit.ShowWindow(SW_HIDE);
	HWND hEdit;  //  !!에디트컨트롤 핸들러
	hEdit = ::GetDlgItem(m_pDlg->GetSafeHwnd(), IDC_EDIT1);
	::SendMessage(hEdit, WM_KILLFOCUS, 0, 0);

	*pResult = 0;
}


void CMyListCtrl::OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_pDlg = (CFileManagerDlg*)GetParent();  //  !!부모 지정
	m_pDlg->m_ctrEdit.ShowWindow(SW_HIDE);
	HWND hEdit;  //  !!에디트컨트롤 핸들러
	hEdit = ::GetDlgItem(m_pDlg->GetSafeHwnd(), IDC_EDIT1);
	::SendMessage(hEdit, WM_KILLFOCUS, 0, 0);

	*pResult = 0;
}


void CMyListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	m_pDlg = (CFileManagerDlg*)GetParent();  //  !!부모 지정
	m_pDlg->m_ctrEdit.ShowWindow(SW_HIDE);
	HWND hEdit;  //  !!에디트컨트롤 핸들러
	hEdit = ::GetDlgItem(m_pDlg->GetSafeHwnd(), IDC_EDIT1);
	::SendMessage(hEdit, WM_KILLFOCUS, 0, 0);

	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CMyListCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	m_pDlg = (CFileManagerDlg*)GetParent();  //  !!부모 지정
	m_pDlg->m_ctrEdit.ShowWindow(SW_HIDE);
	HWND hEdit;  //  !!에디트컨트롤 핸들러
	hEdit = ::GetDlgItem(m_pDlg->GetSafeHwnd(), IDC_EDIT1);
	::SendMessage(hEdit, WM_KILLFOCUS, 0, 0);

	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}


BOOL CMyListCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	m_pDlg = (CFileManagerDlg*)GetParent();  //  !!부모 지정
	m_pDlg->m_ctrEdit.ShowWindow(SW_HIDE);
	HWND hEdit;  //  !!에디트컨트롤 핸들러
	hEdit = ::GetDlgItem(m_pDlg->GetSafeHwnd(), IDC_EDIT1);
	::SendMessage(hEdit, WM_KILLFOCUS, 0, 0);

	return CListCtrl::OnMouseWheel(nFlags, zDelta, pt);
}


void CMyListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
	*pResult = 0;

	if (CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage) {
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage) {
		CString beforeDir = _T("");
		for (int i = 0; i < totFileNum; i++) {
			if (f_info[pLVCD->nmcd.dwItemSpec].first_in_direct == true) {
				pLVCD->clrText = RGB(0, 0, 0);
				pLVCD->clrTextBk = RGB(200, 200, 255);
			}
			else {
				pLVCD->clrText = RGB(0, 0, 0);
				pLVCD->clrTextBk = RGB(255, 255, 255);
			}
		}
		*pResult = CDRF_DODEFAULT;
	}
}
