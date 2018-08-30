#include "VanUart.h"
#include "Utils.h"

VanUart* VanUart::m_pInstance = NULL;

VanUart::VanUart()
{
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
	if (1 == id) {
		//m_pObdManager->OnRecv(pBuf, length);
	}
}
