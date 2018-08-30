#include "VanUart.h"
#include "uni_api.h"
#include <unistd.h>
#include "Utils.h"

VanUart* VanUart::m_pInstance = NULL;

VanUart::VanUart()
{
	m_hThread = 0;
}

VanUart::~VanUart()
{
}

VanUart* VanUart::Instance()
{
	if (NULL == m_pInstance) {
		m_pInstance = new VanUart();
	}

	return m_pInstance;
}

void VanUart::OnRecv(int id, const byte* pBuf, int length)
{
	// id- RS485, 2-RS232
	Utils::PrintHexBuf(pBuf, length);
}

void VanUart::Start()
{
	pthread_create(&m_hThread, NULL, ThreadProc, this);
}

void* VanUart::ThreadProc(void* lpParam)
{
	return ((VanUart *)lpParam)->ThreadFunc();
}

void* VanUart::ThreadFunc()
{
	const char* buf = "1234567890";

	while (true) {
		UNI_API_DataTransfer(1, (const byte*)buf, strlen(buf));

		sleep(5);
	}

	return NULL;
}
