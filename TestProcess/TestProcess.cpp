// TestProcess.cpp : �������̨Ӧ�ó������ڵ㡣
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

	
//���ؽ���
void HideProc(HANDLE hSys, DWORD size)
{
	//Ҫ���ؽ��̵��ַ�
	char pBuff[128] = { 0 };
	int inputSize = strlen("notepad++.exe") + 1;	
	memcpy(pBuff, "notepad++.exe", inputSize);
	
	//�����豸�����뺯��
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
		printf("�������س��������ʧ��%d\n", GetLastError());
		return;
	}
	printf("���س���ɹ���\n");
}

int _tmain(int argc, _TCHAR* argv[])
{
	//�򿪸�����
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
		printf("������ʧ�� %d\n", GetLastError());
		getchar();
		return 0;
	}
	else printf("�������ɹ�\n");
	DWORD size = 0;

	//�������ؽ��̺���
	HideProc(hSys, size);

	getchar();
	CloseHandle(hSys);
}

