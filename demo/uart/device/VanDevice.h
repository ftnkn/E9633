#pragma once
#include "stdafx.h"
#include "uni_api.h"
#include "VanUart.h"
#include "VanCan.h"
#include "VanObd.h"
#include "Lock.h"

class VanDevice
{
private:
	enum { IO_INPUT_COUNT = 3 };
	VanDevice();
	~VanDevice();
public:
	static VanDevice* Instance();
	void Open();
	void Close();
	BOOL IsOpen();

	int GetAccState();
	int GetIoInput(int id);
	int GetDialState();
	int GetCollide();
	int GetRollover();

	string GetSn();
private:
	BOOL BeginThread();
	static void* ThreadProc(void* lpParam);
	void* ThreadFunc();
	static void ApiCallback(int type, const byte* pBuf, int length);
	void OnRecvCallback(int type, const byte* pBuf, int length);
	void Init();
	void OnAccState(int state);
	void OnIoInput(int id, int value); // id 1-3
	void OnDialState(int state);
	void OnCollide(int value);
	void OnRollover(int angle);
	void OnPullPlug(int value);
private:
	static VanDevice* m_pInstance;
	BOOL   m_bOpen;
	CLock  m_Lock;
	VanUart* m_pUart;
	VanCan*  m_pCan;
	VanObd*  m_pObd;
	int m_BitState;		// 更新哪一项
	int m_AccState;		// ACC 0-OFF, 1-ON
	int m_IoInput[IO_INPUT_COUNT];	// GPIO
	int m_DialState;	// 拨号状态
	int m_Collide;		// 碰撞值
	int m_Rollover;		// 侧翻角度

	string m_strSN;
	string m_strIMEI;
};
