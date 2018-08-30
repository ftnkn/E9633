#pragma once
#include "stdafx.h"
#include "uni_gps.h"
#include "Lock.h"

class VanLocation
{
private:
	VanLocation();
	~VanLocation();
public:
	static VanLocation* Instance();
	void Open();
	void Close();
	BOOL IsOpen();
	void SetLocation(const UNI_LOCATION_INFO* pLocInfo);
private:
	static void GpsCallback(int type, const byte* pBuf, int length);
private:
	static VanLocation* m_pInstance;
	UNI_LOCATION_INFO m_LocInfo;
	BOOL   m_bOpen;
};
