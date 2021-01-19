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

//卸载驱动
void UnLoad(DRIVER_OBJECT* obj)
{
	KdPrint(("驱动已卸载\n"));
	UNICODE_STRING userName;
	RtlInitUnicodeString(&userName,
		L"\\DosDevices\\MyFristDriver_User");
	//删除符号链接
	IoDeleteSymbolicLink(&userName);
	//删除设备对象
	IoDeleteDevice(obj->DeviceObject);
}

NTSTATUS InCreate(DEVICE_OBJECT *Device, IRP *irp);
NTSTATUS InClose(DEVICE_OBJECT *Device, IRP *irp);
NTSTATUS DeviceCtrl(DEVICE_OBJECT* device, IRP* irp);

//驱动入口
NTSTATUS DriverEntry(DRIVER_OBJECT* driver, UNICODE_STRING* path)
{
	path;
	KdPrint(("驱动已加载\n"));
	DbgBreakPoint();
	
	//1.创建一个设备对象
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
	driver->Flags |= DO_DIRECT_IO;//直接IO

	//2.绑定一个符号链接
	UNICODE_STRING userName;
	RtlInitUnicodeString(&userName,
		L"\\DosDevices\\MyFristDriver_User");
	IoCreateSymbolicLink(&userName, &deviceName);


	//调用派遣函数
	driver->MajorFunction[IRP_MJ_CREATE] = InCreate;
	driver->MajorFunction[IRP_MJ_CLOSE] = InClose;
	driver->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DeviceCtrl;

	driver->DriverUnload = UnLoad;
	return STATUS_SUCCESS;
}

NTSTATUS InCreate(DEVICE_OBJECT *Device, IRP *irp)
{
	Device;
	KdPrint(("调用了创建派遣函数\n"));
	irp->IoStatus.Status = STATUS_SUCCESS;
	irp->IoStatus.Information = 0;
	IoCompleteRequest(irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}
NTSTATUS InClose(DEVICE_OBJECT *Device, IRP *irp)
{
	Device;
	KdPrint(("调用了关闭派遣函数\n"));
	irp->IoStatus.Status = STATUS_SUCCESS;
	irp->IoStatus.Information = 0;
	IoCompleteRequest(irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}



//通过控制码方式通讯
NTSTATUS DeviceCtrl(DEVICE_OBJECT* device, IRP* irp)
{
	KdPrint(("已进入控制码通讯\n"));	
	device;
	NTSTATUS status = STATUS_SUCCESS;
	ULONG complateSize = 0;
	HANDLE hFile = NULL;
	irp->IoStatus.Status = STATUS_SUCCESS;

	//1.获取IO栈位置
	IO_STACK_LOCATION* pIoStack = 
		IoGetCurrentIrpStackLocation(irp);

	//2.获取传入的参数
	//2.1 IO设备控制码
	ULONG ctrlCode = 
		pIoStack->Parameters.DeviceIoControl.IoControlCode;

	//2.2 输入缓冲区的字节数
	ULONG inputSize = pIoStack->
			Parameters.DeviceIoControl.InputBufferLength;

	//2.3获取输入缓冲区
	PVOID pInputBuff = irp->AssociatedIrp.SystemBuffer;

	//选择控制码
    switch (ctrlCode)
	{
		//HideProcess
		case DF_GET_PROC_HIDE:
		{		
			//进程名称
			char* ProcName = NULL;
			ProcName = (char*)pInputBuff;		
			//获取当前进程对象
			PEPROCESS NowProc = PsGetCurrentProcess();
			//获取进程对象内的当前活动进程链表
			LIST_ENTRY* pNowList = (LIST_ENTRY*)((ULONG)NowProc + 0xb8);
			//临时链表
			LIST_ENTRY* pTempList = pNowList;
			//遍历链表
			while (pNowList != pTempList->Flink)
			{
				//相对进程对象偏移0x16c保存的是ImageFileName,
				//当前相对进程对象偏移是0xb8，需要减去			
				if (!strcmp(ProcName, (char*)
					((ULONG)pTempList - 0xb8 + 0x16c)))
				{
					//把找到的进程从链表中删除
					(pTempList->Blink)->Flink = pTempList->Flink;
					(pTempList->Flink)->Blink = pTempList->Blink;
				
					pTempList->Flink = pTempList->Blink = NULL;
					KdPrint(("成功删除程序链表节点\n"));				
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





