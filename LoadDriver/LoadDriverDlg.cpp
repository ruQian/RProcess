
// LoadDriverDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LoadDriver.h"
#include "LoadDriverDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLoadDriverDlg 对话框




CLoadDriverDlg::CLoadDriverDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLoadDriverDlg::IDD, pParent)
	, m_szPath_w(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLoadDriverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_szPath_w);
}

BEGIN_MESSAGE_MAP(CLoadDriverDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CLoadDriverDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_OPENFILE, &CLoadDriverDlg::OnBnClickedButtonOpenfile)
	ON_BN_CLICKED(IDCANCEL, &CLoadDriverDlg::OnBnClickedCancel)
	ON_BN_CLICKED(ID_LOAD_DRIVER, &CLoadDriverDlg::OnBnClickedLoadDriver)
	ON_BN_CLICKED(IDC_BUTTON2, &CLoadDriverDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CLoadDriverDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CLoadDriverDlg 消息处理程序

BOOL CLoadDriverDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_isDriverFile = true;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CLoadDriverDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CLoadDriverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CLoadDriverDlg::OnBnClickedOk()
{
		// TODO: 在此添加控件通知处理程序代码
	//不是驱动文件,直接返回,不处理
	if (m_isDriverFile == FALSE)
	{
		return;
	}
	//1. 使用OpenSCManager函数打开SCM
	m_hServiceMgr = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (m_hServiceMgr == NULL)
	{
		MessageBox(_T("OpenSCManager Error"), _T("Error"));
		CloseServiceHandle(m_hServiceMgr);
		return;
	}
	//2.使用CreateService函数利用SCM句柄创建一个服务
	m_hServiceDDK = ::CreateService(
		m_hServiceMgr,//SMC句柄
		_T("HadesService"),//驱动服务名称(驱动程序的在注册表中的名字)
		_T("HadesService"),//驱动服务显示名称(注册表驱动程序的DisplayName值)
		SERVICE_ALL_ACCESS,//权限(所有访问权限)
		SERVICE_KERNEL_DRIVER,//服务类型(驱动程序)
		SERVICE_DEMAND_START,//启动方式(需要时启动,注册表驱动程序的Start值)
		SERVICE_ERROR_IGNORE,//错误控制(忽略,注册表驱动程序的ErrorControl值)
		m_szPath_w,//服务的二进制文件路径(驱动程序文件路径, 注册表驱动程序的ImagePath值)
		NULL,//加载组命令
		NULL,//TagId(指向一个加载顺序的标签值)
		NULL,//依存关系
		NULL,//服务启动名
		NULL);//密码
	if (m_hServiceDDK == NULL)
	{
		//如果创建错误,关闭句柄
		CloseServiceHandle(m_hServiceDDK);
		CloseServiceHandle(m_hServiceMgr);
		MessageBox(_T("Install Drive Fail"), _T("Error"));
		return;
	}
	CloseServiceHandle(m_hServiceDDK);
	CloseServiceHandle(m_hServiceMgr);
	GetDlgItem(IDC_STATIC_PROMPT)->SetWindowText(_T("驱动安装成功!"));//设置控件文本内容

}


void CLoadDriverDlg::OnBnClickedButtonOpenfile()
{
	//打开驱动	
	BOOL isOpen = TRUE;		//是否打开(否则为保存)
	CString fileName = L"";			//默认打开的文件名
	CString filter = L"文件 (*.sys)|*.sys||";	//文件过虑的类型
	CFileDialog openFileDlg(isOpen, L"D:\\", fileName, OFN_HIDEREADONLY|OFN_READONLY, filter, NULL);
	INT_PTR result = openFileDlg.DoModal();
	if(result == 1)
	{
		//
		m_szPath_w = openFileDlg.GetPathName();
		UpdateData(false);
	}
}


void CLoadDriverDlg::OnBnClickedCancel()
{
	OnCancel();
}


void CLoadDriverDlg::OnBnClickedLoadDriver()
{
	//加载驱动代码
	if (m_isDriverFile == FALSE)
	{
		return;
	}
	//使用OpenSCManager函数打开SCM
	m_hServiceMgr = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (m_hServiceMgr == NULL)
	{
		MessageBox(_T("OpenSCManager Error"), _T("Error"));
		CloseServiceHandle(m_hServiceMgr);
		return;
	}
	//打开服务获得句柄
	m_hServiceDDK = OpenService(m_hServiceMgr, _T("HadesService"), SERVICE_START);
 
	//启动刚刚创建的服务
	BOOL bRet = StartService(m_hServiceDDK, NULL, NULL);
	if (bRet == FALSE)
	{
		CloseServiceHandle(m_hServiceDDK);
		CloseServiceHandle(m_hServiceMgr);
		CString str;
		str.Format(L"启动服务失败 - %d", GetLastError());
		GetDlgItem(IDC_STATIC_PROMPT)->SetWindowText(str);//设置控件文本内容
		//MessageBox(_T("Start Service Fail"), _T("Error"));
		return;
	}
	CloseServiceHandle(m_hServiceDDK);
	CloseServiceHandle(m_hServiceMgr);
	GetDlgItem(IDC_STATIC_PROMPT)->SetWindowText(_T("The driver has started"));//设置控件文本内容
}


void CLoadDriverDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	//不是驱动文件,直接返回,不处理
	if (m_isDriverFile == FALSE)
	{
		return;
	}
	//使用OpenSCManager函数打开SCM
	m_hServiceMgr = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (m_hServiceMgr == NULL)
	{
		MessageBox(_T("OpenSCManager Error"), _T("Error"));
		CloseServiceHandle(m_hServiceMgr);
		return;
	}
	//打开服务获得句柄
	m_hServiceDDK = OpenService(m_hServiceMgr, _T("HadesService"), SERVICE_STOP);
 
	//停止驱动服务
	SERVICE_STATUS svcsta = { 0 };
	BOOL bRet = ControlService(m_hServiceDDK, SERVICE_CONTROL_STOP, &svcsta);
	if (bRet == FALSE)
	{
		CloseServiceHandle(m_hServiceDDK);
		CloseServiceHandle(m_hServiceMgr);
		MessageBox(_T("Stop Service Fail"), _T("Error"));
		return;
	}
	CloseServiceHandle(m_hServiceDDK);
	CloseServiceHandle(m_hServiceMgr);
	GetDlgItem(IDC_STATIC_PROMPT)->SetWindowText(_T("驱动已停止!"));//设置控件文本内容
}


void CLoadDriverDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	//不是驱动文件,直接返回,不处理
	if (m_isDriverFile == FALSE)
	{
		return;
	}
	//使用OpenSCManager函数打开SCM
	m_hServiceMgr = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (m_hServiceMgr == NULL)
	{
		MessageBox(_T("OpenSCManager Error"), _T("Error"));
		CloseServiceHandle(m_hServiceMgr);
		return;
	}
	//打开服务获得句柄
	m_hServiceDDK = OpenService(m_hServiceMgr, _T("HadesService"), SERVICE_STOP | DELETE);
	//删除驱动服务
	BOOL bRet = DeleteService(m_hServiceDDK);
	if (bRet == FALSE)
	{
		MessageBox(_T("UnInstall Service Fail"), _T("Error"));
		CloseServiceHandle(m_hServiceDDK);
		CloseServiceHandle(m_hServiceMgr);
		return;
	}
	GetDlgItem(IDC_STATIC_PROMPT)->SetWindowText(_T("驱动卸载成功!"));//设置控件文本内容
	CloseServiceHandle(m_hServiceDDK);
	CloseServiceHandle(m_hServiceMgr);

}
