// TestProcess.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#define IOCTL_BASE 0x800
#define MY_CTL_CODE(code)       \
		CTL_CODE( \
		FILE_DEVICE_UNKNOWN, \
		IOCTL_BASE+(code), \
		METHOD_OUT_DIRECT, \
		0 \
		)
#define DF_GET_PROC_HIDE MY_CTL_CODE(7)

	
//隐藏进程
void HideProc(HANDLE hSys, DWORD size)
{
	//要隐藏进程的字符
	char pBuff[128] = { 0 };
	int inputSize = strlen("notepad++.exe") + 1;	
	memcpy(pBuff, "notepad++.exe", inputSize);
	
	//调用设备控制码函数
	BOOL nRet = DeviceIoControl(
		hSys,
		DF_GET_PROC_HIDE,
		(LPVOID)pBuff,
		inputSize,
		NULL,
		0,
		&size,
		NULL);
	if (!nRet)
	{
		printf("调用隐藏程序控制码失败%d\n", GetLastError());
		return;
	}
	printf("隐藏程序成功！\n");
}

int _tmain(int argc, _TCHAR* argv[])
{
	//打开该驱动
	HANDLE hSys = CreateFile(
		L"..\\driver\\RProcessDriver.sys",
		GENERIC_ALL,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hSys==INVALID_HANDLE_VALUE)
	{
		printf("打开驱动失败 %d\n", GetLastError());
		getchar();
		return 0;
	}
	else printf("打开驱动成功\n");
	DWORD size = 0;

	//调用隐藏进程函数
	HideProc(hSys, size);

	getchar();
	CloseHandle(hSys);
}

