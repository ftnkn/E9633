#pragma once
#include "stdafx.h"

class VanUart
{
private:
	VanUart();
	~VanUart();
public:
	static VanUart* Instance();
	void Start();
	void OnRecv(int id, const byte* pBuf, int length);
private:
	static void* ThreadProc(void* lpParam);
	void* ThreadFunc();
private:
	static VanUart* m_pInstance;
	pthread_t m_hThread;
};
