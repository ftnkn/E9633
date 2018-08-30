#include "VanLocation.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "Utils.h"


VanLocation* VanLocation::m_pInstance = NULL;

VanLocation::VanLocation()
{
	m_bOpen = FALSE;
	memset(&m_LocInfo, 0, sizeof(UNI_LOCATION_INFO));
}

VanLocation::~VanLocation()
{
	Close();
}

VanLocation* VanLocation::Instance()
{
	if (NULL == m_pInstance) {
		m_pInstance = new VanLocation();
	}

	return m_pInstance;
}

void VanLocation::Open()
{
	Close();
	m_bOpen = TRUE;
	UNI_GPS_SetCallback(GpsCallback, GPS_LOCATION);
}

void VanLocation::Close()
{
	m_bOpen = FALSE;
	UNI_GPS_SetCallback(NULL, 0);
	memset(&m_LocInfo, 0, sizeof(UNI_LOCATION_INFO));
}

BOOL VanLocation::IsOpen()
{
	return m_bOpen;
}

void VanLocation::GpsCallback(int type, const byte* pBuf, int length)
{
    if (GPS_LOCATION == type) {
        UNI_LOCATION_INFO* pLocInfo = (UNI_LOCATION_INFO*)pBuf;
		m_pInstance->SetLocation(pLocInfo);
    }
}

void VanLocation::SetLocation(const UNI_LOCATION_INFO* pLocInfo)
{
	memcpy(&m_LocInfo, pLocInfo, sizeof(UNI_LOCATION_INFO));

	if (0 != m_LocInfo.timestamp) {
		//ReportMng::Instance()->OnLocation(m_LocInfo);
	}
}
