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
	CString beforeDirect;  //  !!������ ��ġ�ϴ� ���丮
	bool first_in_direct = false;  //  !!��ġ ���丮 �� ù ���Ͽ���
	void changeBeforePath();
	~filenameInfo();
};