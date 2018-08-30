#include "Utils.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/file.h>


void Utils::Log(const char* szPath, const char* szMsg)
{
	/*FILE* fp = fopen(szPath, "a");
	if(NULL == fp)
	{
		return;
	}

	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);

	char szTime[32];
	sprintf(szTime, "%d-%02d-%02d %02d:%02d:%02d.%d - ", sysTime.wYear, sysTime.wMonth, sysTime.wDay,
		sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);

	fwrite(szTime, strlen(szTime), 1, fp);
	fwrite(szMsg, strlen(szMsg), 1, fp);
      
	fclose(fp);*/
}

string Utils::GetHexString(const byte* pData, int length)
{
	char HEX[] = {'0', '1', '2', '3', '4', '5', '6', '7', 
		'8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

	string str;

	for (int i = 0; i < length; ++i) 
	{
		int value = pData[i] & 0xff;
		str.append(1, HEX[value/16]);
		str.append(1, HEX[value%16]);
	}

	return str;
}

int Utils::GetHexData(const char* pSrc, char* pBuf, int length)
{
	if (length < (int)strlen(pSrc)/2)
	{
		return 0;
	}

	int nRet = strlen(pSrc)/2;

	char temp[3];
	memset(temp, 0, 3);

	for (int i=0; i<nRet; ++i)
	{
		temp[0] = pSrc[i*2];
		temp[1] = pSrc[i*2+1];

		pBuf[i] = (char)strtol(temp, 0, 16);
	}

	return nRet;
}

void Utils::PrintHexBuf(const byte* pData, int length)
{
	char HEX[] = { '0', '1', '2', '3', '4', '5', '6', '7',
		'8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

	char* pBuf = new char[length * 3];

	for (int i = 0; i < length; ++i)
	{
		int value = pData[i] & 0xff;
		pBuf[i * 3] = HEX[value / 16];
		pBuf[i * 3 + 1] = HEX[value % 16];
		pBuf[i * 3 + 2] = 0x20;
	}

	pBuf[length * 3 - 1] = 0;
	printf("%s\n", pBuf);
	delete[] pBuf;
}

string Utils::GetBcdStr(byte* pData, int length)
{
	string str;

	for (int i = 0; i < length; i++)
	{
		int value = pData[i] & 0xff;
		str.append(1, (char)((value >> 4) + 0x30));
		str.append(1, (char)((value & 0x0F) + 0x30));
	}

	return str;
}

byte* Utils::WriteInt(byte* pBuf, int value, int bytes)
{
	unsigned long temp = value & 0xFFFFFFFF;

	if (4 == bytes) {
		*pBuf++ = temp >> 24;
		*pBuf++ = (temp >> 16) & 0xFF;
		*pBuf++ = (temp >> 8) & 0xFF;
		*pBuf++ = temp & 0xFF;
	} else if (2 == bytes) {
		*pBuf++ = (temp >> 8) & 0xFF;
		*pBuf++ = temp & 0xFF;
	} else if (1 == bytes) {
		*pBuf++ = temp & 0xFF;
	}

	return pBuf;
}

byte* Utils::WriteBCD(byte* pBuf, const byte* pSrc, int length)
{
	length /= 2;

	byte bcd;
	for (int i = 0; i < length; ++i) {
		bcd = *pSrc++ - 0x30;
		bcd <<= 4;
		bcd |= *pSrc++ - 0x30;

		*pBuf++ = bcd;
	}

	return pBuf;
}

byte* Utils::WriteBuf(byte* pBuf, const void* pSrc, int length)
{
	memcpy(pBuf, pSrc, length);
	return pBuf + length;
}

int Utils::ReadInt(const byte* pBuf, int bytes)
{
	int temp = 0;

	if (4 == bytes) {
		temp |= *pBuf++;
		temp <<= 8;

		temp |= *pBuf++;
		temp <<= 8;

		temp |= *pBuf++;
		temp <<= 8;

		temp |= *pBuf++;
	} else if (2 == bytes) {
		temp |= *pBuf++;
		temp <<= 8;

		temp |= *pBuf++;
	} else if (1 == bytes) {
		temp |= *pBuf++;
	}

	return temp;
}

long Utils::GetUpTime()
{
	timespec tp;
	clock_gettime(CLOCK_MONOTONIC_RAW, &tp);

	return (long)tp.tv_sec;
}

long Utils::GetUpMSec()
{
	timespec tp;
	clock_gettime(CLOCK_MONOTONIC_RAW, &tp);

	return (long)(tp.tv_sec*1000 + tp.tv_nsec/1000000);
}

string Utils::GetModuleFilePath()
{
	char buf[512];
	int size = readlink("/proc/self/exe", buf, 512);

	if (size<=0 || size>=512) {
    	return "";
	}
	buf[size] = 0;

	return string(buf);
}

int Utils::ProcessExists()
{
	string path = GetModuleFilePath();
	if (path.empty()) {
		return -1;
	}

    int pos = path.find_last_of("/");
    if (pos == -1) {
        return -1;
    }

	string name = path.substr(pos + 1);
	string filePath = string("/var/run/") + name + ".pid";

	int fd = open(filePath.c_str(), O_RDWR | O_CREAT, 0666);
    if (fd < 0) {
        return -1;
    }

	struct flock fl;
    fl.l_type = F_WRLCK; // 写文件锁定
    fl.l_start = 0;
    fl.l_whence = SEEK_SET;
    fl.l_len = 0;
    int ret = fcntl(fd, F_SETLK, &fl);
    if (ret < 0) {
        if (errno==EACCES || errno==EAGAIN) {
            //printf("%s already locked, error: %s\n", filePath.c_str(), strerror(errno));
            close(fd);
            return 1;
        }
    }

    // 锁定文件后，将该进程的pid写入文件
    char buf[16] = {'\0'};
    sprintf(buf, "%d", getpid());
    ftruncate(fd, 0);
    ret = write(fd, buf, strlen(buf));
    if (ret < 0) {
        //printf("Write file failed, file: %s, error: %s\n", filePath.c_str(), strerror(errno));
        close(fd);
		return -1;
    }

    // 函数返回时不需要调用close(fd)
    // 不然文件锁将失效
    // 程序退出后kernel会自动close
    return 0;
}

bool Utils::StartupApp(const char* path)
{
	if (NULL == path) {
		return false;
	}

	if (access(path, F_OK | X_OK)) {
		return false;
	}

	string name = path;
	int pos = name.find_last_of("/");
    if (pos != -1) {
        name = name.substr(pos + 1);
    }

	pid_t pid = fork();

    switch(pid)
    {
    case -1:
        return false;
        break;
    case 0:
        // 这是在子进程中，调用execlp切换为ps进程
        execlp(path, name.c_str(), NULL);
        break;
    default:
        // 这是在父进程中，输出相关提示信息
		return true;
        break;
    }

	return true;
}

bool Utils::PathFileExists(const char* path)
{
	if (NULL == path) {
		return false;
	}

	if (access(path, F_OK)) {
		return false;
	}

	return true;
}

string Utils::GetAppVersion()
{
	FILE* fp = fopen("/data/QuecOpen/app/version", "r");
	if (NULL == fp) {
		return "";
	}

	char buf[64];
	memset(buf, 0, sizeof(buf));
	fread(buf, 63, 1, fp);
	fclose(fp);

	int length = strlen(buf);
	for (int i=length-1; i>=0; --i) {
		if (buf[i] > 0x20) {
			break;
		}

		buf[i] = '\0';
	}

	return string(buf);
}

void Utils::Split(vector<string>& items, 
			const char* pLine, const char* DELIMITER)
{
	string str(pLine);

	size_t pos1 = 0;
	size_t pos2 = str.find(DELIMITER);

	while (string::npos != pos2)
	{
		string item = str.substr(pos1, pos2 - pos1);
		items.push_back(item);
	
		pos1 = pos2 + strlen(DELIMITER);
		pos2 = str.find(DELIMITER, pos1);
	}

	if (pos1 != 0) {
		string item = str.substr(pos1);
		items.push_back(item);
	}
}
