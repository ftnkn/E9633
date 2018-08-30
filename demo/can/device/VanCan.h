#pragma once
#include "stdafx.h"

class VanCan
{
private:
	VanCan();
	~VanCan();
public:
	static VanCan* Instance();
	void Start();
	void OnRecv(const byte* pBuf, int length);

	
	/*
 	 * FullName:	写CAN数据包
	 * @param [in]: channel : 通道,1或2
	 * @param [in]: id : CAN ID
	 * @param [in]: pData : 数据内容
	 * @param [in]: length : 数据长度 0-8
	 * @param [in]: bExtFrame : true-扩展帧 false-标准帧
 	 * @param [in]: bRemoteFrame : true-远程帧 false-数据帧
 	 * @return:		1-成功， 0-失败
 	 */
	bool Write(int channel, int id, const byte* pData, int length,
						bool bExtFrame, bool bRemoteFrame);
private:
	static void* ThreadProc(void* lpParam);
	void* ThreadFunc();
private:
	static VanCan* m_pInstance;
	pthread_t m_hThread;
};
