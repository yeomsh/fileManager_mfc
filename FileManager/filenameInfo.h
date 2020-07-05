#pragma once
#pragma once
class filenameInfo
{
public:
	CString strArtist, strTitle;
	CString strDate, strLocation;

	filenameInfo();
	filenameInfo(LPCTSTR b_name);
	bool select = false;
	bool changeCheck = false;
	bool folderChangeCheck = false;
	CString afterName;
	CString beforeName;
	CString type;
	CString beforePath;
	CString afterPath;
	CString beforeDirect;  //  !!파일이 위치하는 디렉토리
	bool first_in_direct = false;  //  !!위치 디렉토리 상 첫 파일여부
	void changeBeforePath();
	~filenameInfo();
};