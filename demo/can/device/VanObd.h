#pragma once
#include "stdafx.h"

class VanObd
{
private:
	VanObd();
	~VanObd();
public:
	static VanObd* Instance();
	void OnRecv(const byte* pBuf, int length);
private:
private:
	static VanObd* m_pInstance;
};
