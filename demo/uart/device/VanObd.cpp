#include "VanObd.h"


VanObd* VanObd::m_pInstance = NULL;

VanObd::VanObd()
{
}

VanObd::~VanObd()
{
}

VanObd* VanObd::Instance()
{
	if (NULL == m_pInstance) {
		m_pInstance = new VanObd();
	}

	return m_pInstance;
}

void VanObd::OnRecv(const byte* pBuf, int length)
{

}
