#include "stdafx.h"
#include "filenameInfo.h"


filenameInfo::filenameInfo()
{

}

filenameInfo::filenameInfo(LPCTSTR b_name) {
	this->beforePath = b_name;
	afterPath = _T("");
	strArtist = _T("");
	strTitle = _T("");
	strDate = _T("");
	strLocation = _T("");
	afterPath = _T("");
}

filenameInfo::~filenameInfo()
{
}
void filenameInfo::changeBeforePath() {
	this->beforePath = this->afterPath;
}