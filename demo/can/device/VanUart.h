#pragma once
#include "stdafx.h"

class VanUart
{
private:
	VanUart();
	~VanUart();
public:
	static VanUart* Instance();
	void OnRecv(int id, const byte* pBuf, int length);
private:
	static VanUart* m_pInstance;
};
