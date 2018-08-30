#include "VanCan.h"
#include "uni_api.h"
#include <unistd.h>

VanCan* VanCan::m_pInstance = NULL;

VanCan::VanCan()
{
	m_hThread = 0;
}

VanCan::~VanCan()
{
}

VanCan* VanCan::Instance()
{
	if (NULL == m_pInstance) {
		m_pInstance = new VanCan();
	}

	return m_pInstance;
}

void VanCan::Start()
{
	pthread_create(&m_hThread, NULL, ThreadProc, this);
}

void* VanCan::ThreadProc(void* lpParam)
{
	return ((VanCan *)lpParam)->ThreadFunc();
}

void* VanCan::ThreadFunc()
{
	int id = 0x12345678;
	byte data[] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38};

	while (true) {
		Write(1, id, data, sizeof(data), true, false);

		sleep(5);
	}

	return NULL;
}

void VanCan::OnRecv(const byte* pData, int length)
{
	if (14 != length) {
		printf("CAN Recv Wrong Length\n");
		return;
	}

	int channel = pData[0];				// 1-通道1, 2-通道2
	int idType = pData[4] & 0x04;		// ID类型 !0-扩展帧，0-标准帧
	int frameType = pData[4] & 0x02;	// 帧类型 !0-远程帧，0-数据帧
	DWORD id = 0;

	if (idType) {
		// 扩展帧
		id = (pData[1] << 21) | (pData[2] << 13) | (pData[3] << 5) | (pData[4] >> 3);
	} else {
		// 标准帧
		id = (pData[1] << 3) | (pData[2] >> 5);
	}

	int dataSize = pData[5];

	char msg[256];
	if (idType) {
		sprintf(msg, "Channel=%d, Ext/%s, ID=%08X, Length=%d, Data=%02X %02X %02X %02X %02X %02X %02X %02X",
			channel, frameType ? "Remote" : "Data", id, dataSize, pData[6], pData[7], pData[8], pData[9],
			pData[10], pData[11], pData[12], pData[13]);
	} else {
		sprintf(msg, "Channel=%d, Std/%s, ID=%04X, Length=%d, Data=%02X %02X %02X %02X %02X %02X %02X %02X",
			channel, frameType ? "Remote" : "Data", id, dataSize, pData[6], pData[7], pData[8], pData[9],
			pData[10], pData[11], pData[12], pData[13]);
	}

	printf("Can Recv: %s\n", msg);
}

/*
 * FullName:	写CAN
 * @param [in]: buf : 数据,长度必须是14
 * @return:		1-成功， 0-失败
 */
bool VanCan::Write(int channel, int id, const byte* pData, int length,
						bool bExtFrame, bool bRemoteFrame)
{
	if (channel<1 || channel>2) {
		return false;
	}

	if (length<0 || length>8) {
		return false;
	}

	if (bExtFrame) {
		id <<= 3;
		id |= 0x04;
	} else {
		id <<= 21;
	}

	if (bRemoteFrame) {
		id |= 0x02;
	}

	byte buf[14];
	memset(buf, 0, 14);

	buf[0] = (byte)channel;
	buf[1] = (byte)((id>>24) & 0xFF);
	buf[2] = (byte)((id>>16) & 0xFF);
	buf[3] = (byte)((id>>8) & 0xFF);
	buf[4] = (byte)(id & 0xFF);
	buf[5] = (byte)length;

	if (NULL != pData) {
		memcpy(buf + 6, pData, length);
	}
		
	return UNI_API_CanWrite(buf);
}
