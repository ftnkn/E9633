#include "VanDevice.h"
#include "VanLocation.h"
#include <string.h>
#include "Utils.h"

VanDevice* VanDevice::m_pInstance = NULL;

VanDevice::VanDevice()
{
	m_bOpen = FALSE;

	m_pUart = VanUart::Instance();
	m_pCan = VanCan::Instance();
	m_pObd = VanObd::Instance();

	m_BitState = 0;
	m_AccState = -1;

	for (int i=0; i<IO_INPUT_COUNT; ++i) {
		m_IoInput[i] = -1;
	}

	m_DialState = 0;
	m_Collide = 0;
	m_Rollover = 0;

	Init();
}

VanDevice::~VanDevice()
{
	Close();
}

VanDevice* VanDevice::Instance()
{
	if (NULL == m_pInstance) {
		m_pInstance = new VanDevice();
	}

	return m_pInstance;
}

void VanDevice::Open()
{
	Close();
	
	m_bOpen = TRUE;

	int filter = ACC | IO_INPUT | LTE_DIAL_STATE | COLLIDE 
				| ROLLOVER | DATA_TRANSFER | CAN_READ | OBD_READ 
				| RAPIDLY_SPEED | SHARP_CURVE | PULL_PLUG;
    UNI_API_SetCallback(ApiCallback, filter);

	printf("Set API Callback\n");
}

void VanDevice::Close()
{
	m_bOpen = FALSE;

	UNI_API_SetCallback(NULL, 0);
}

void VanDevice::Init()
{
	m_AccState = UNI_API_GetAccStatus();

	UNI_API_SetDataTransferParam(1, 115200);
	int nRet = UNI_API_SetCanSpeed(1, 0); // CAN 1, 500K
	printf("Can Speed Result = %d\n", nRet);

	VanUart::Instance()->Start();

	for (unsigned int i=0; i<IO_INPUT_COUNT; ++i) {
		m_IoInput[i] = UNI_API_GetIoInput(i + 1);
	}

	m_DialState = UNI_API_GetLteDialState();

    char buf[64] = { 0 };
    UNI_API_GetSN(buf, 64);
    m_strSN = buf;
}

BOOL VanDevice::IsOpen()
{
	return m_bOpen;
}

void VanDevice::ApiCallback(int type, const byte* pBuf, int length)
{
	if (NULL != m_pInstance) {
		m_pInstance->OnRecvCallback(type, pBuf, length);
	}
}

void VanDevice::OnRecvCallback(int type, const byte* pBuf, int length)
{
	if (ACC == type) {
		OnAccState(pBuf[0]);
	} else if (IO_INPUT == type) {
		OnIoInput(pBuf[0], pBuf[1]);
	} else if (LTE_DIAL_STATE == type) {
		OnDialState(pBuf[0]);
	} else if (COLLIDE == type) {
		OnCollide(pBuf[0]);
	} else if (ROLLOVER == type) {
		OnRollover(pBuf[0]);
	} else if (DATA_TRANSFER == type) {
		m_pUart->OnRecv(pBuf[0], pBuf+1, length-1);
	} else if (CAN_READ == type) {
		m_pCan->OnRecv(pBuf, length);
	} else if (OBD_READ == type) {
		m_pObd->OnRecv(pBuf, length);
	} else if (PULL_PLUG == type) {
		OnPullPlug(pBuf[0]);
	}
}

void VanDevice::OnAccState(int state)
{
	m_AccState = state;

	printf("******** OnAccState ===== %d\n", state);
	//ReportMng::Instance()->OnEvent(E_ACC, state);
}

int VanDevice::GetAccState()
{
	return m_AccState;
}

void VanDevice::OnIoInput(int id, int value)
{
	m_IoInput[id-1] = value;
}

int VanDevice::GetIoInput(int id)
{
	return m_IoInput[id-1];
}

void VanDevice::OnDialState(int state)
{
	m_DialState = state;
}

int VanDevice::GetDialState()
{
	return m_DialState;
}

void VanDevice::OnCollide(int value)
{
	m_Collide = value;
}

int VanDevice::GetCollide()
{
	return m_Collide;
}

void VanDevice::OnRollover(int angle)
{
	m_Rollover = angle;
}

int VanDevice::GetRollover()
{
	return m_Rollover;
}

string VanDevice::GetSn()
{
	return m_strSN;
}

void VanDevice::OnPullPlug(int value)
{
	printf("******** OnPullPlug ===== %d\n", value);
	//ReportMng::Instance()->OnEvent(E_PULL_PLUG, value);
}
