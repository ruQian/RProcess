
// LoadDriverDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LoadDriver.h"
#include "LoadDriverDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLoadDriverDlg �Ի���




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


// CLoadDriverDlg ��Ϣ�������

BOOL CLoadDriverDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_isDriverFile = true;

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CLoadDriverDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CLoadDriverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CLoadDriverDlg::OnBnClickedOk()
{
		// TODO: �ڴ���ӿؼ�֪ͨ����������
	//���������ļ�,ֱ�ӷ���,������
	if (m_isDriverFile == FALSE)
	{
		return;
	}
	//1. ʹ��OpenSCManager������SCM
	m_hServiceMgr = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (m_hServiceMgr == NULL)
	{
		MessageBox(_T("OpenSCManager Error"), _T("Error"));
		CloseServiceHandle(m_hServiceMgr);
		return;
	}
	//2.ʹ��CreateService��������SCM�������һ������
	m_hServiceDDK = ::CreateService(
		m_hServiceMgr,//SMC���
		_T("HadesService"),//������������(�����������ע����е�����)
		_T("HadesService"),//����������ʾ����(ע������������DisplayNameֵ)
		SERVICE_ALL_ACCESS,//Ȩ��(���з���Ȩ��)
		SERVICE_KERNEL_DRIVER,//��������(��������)
		SERVICE_DEMAND_START,//������ʽ(��Ҫʱ����,ע������������Startֵ)
		SERVICE_ERROR_IGNORE,//�������(����,ע������������ErrorControlֵ)
		m_szPath_w,//����Ķ������ļ�·��(���������ļ�·��, ע������������ImagePathֵ)
		NULL,//����������
		NULL,//TagId(ָ��һ������˳��ı�ǩֵ)
		NULL,//�����ϵ
		NULL,//����������
		NULL);//����
	if (m_hServiceDDK == NULL)
	{
		//�����������,�رվ��
		CloseServiceHandle(m_hServiceDDK);
		CloseServiceHandle(m_hServiceMgr);
		MessageBox(_T("Install Drive Fail"), _T("Error"));
		return;
	}
	CloseServiceHandle(m_hServiceDDK);
	CloseServiceHandle(m_hServiceMgr);
	GetDlgItem(IDC_STATIC_PROMPT)->SetWindowText(_T("������װ�ɹ�!"));//���ÿؼ��ı�����

}


void CLoadDriverDlg::OnBnClickedButtonOpenfile()
{
	//������	
	BOOL isOpen = TRUE;		//�Ƿ��(����Ϊ����)
	CString fileName = L"";			//Ĭ�ϴ򿪵��ļ���
	CString filter = L"�ļ� (*.sys)|*.sys||";	//�ļ����ǵ�����
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
	//������������
	if (m_isDriverFile == FALSE)
	{
		return;
	}
	//ʹ��OpenSCManager������SCM
	m_hServiceMgr = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (m_hServiceMgr == NULL)
	{
		MessageBox(_T("OpenSCManager Error"), _T("Error"));
		CloseServiceHandle(m_hServiceMgr);
		return;
	}
	//�򿪷����þ��
	m_hServiceDDK = OpenService(m_hServiceMgr, _T("HadesService"), SERVICE_START);
 
	//�����ոմ����ķ���
	BOOL bRet = StartService(m_hServiceDDK, NULL, NULL);
	if (bRet == FALSE)
	{
		CloseServiceHandle(m_hServiceDDK);
		CloseServiceHandle(m_hServiceMgr);
		CString str;
		str.Format(L"��������ʧ�� - %d", GetLastError());
		GetDlgItem(IDC_STATIC_PROMPT)->SetWindowText(str);//���ÿؼ��ı�����
		//MessageBox(_T("Start Service Fail"), _T("Error"));
		return;
	}
	CloseServiceHandle(m_hServiceDDK);
	CloseServiceHandle(m_hServiceMgr);
	GetDlgItem(IDC_STATIC_PROMPT)->SetWindowText(_T("The driver has started"));//���ÿؼ��ı�����
}


void CLoadDriverDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//���������ļ�,ֱ�ӷ���,������
	if (m_isDriverFile == FALSE)
	{
		return;
	}
	//ʹ��OpenSCManager������SCM
	m_hServiceMgr = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (m_hServiceMgr == NULL)
	{
		MessageBox(_T("OpenSCManager Error"), _T("Error"));
		CloseServiceHandle(m_hServiceMgr);
		return;
	}
	//�򿪷����þ��
	m_hServiceDDK = OpenService(m_hServiceMgr, _T("HadesService"), SERVICE_STOP);
 
	//ֹͣ��������
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
	GetDlgItem(IDC_STATIC_PROMPT)->SetWindowText(_T("������ֹͣ!"));//���ÿؼ��ı�����
}


void CLoadDriverDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//���������ļ�,ֱ�ӷ���,������
	if (m_isDriverFile == FALSE)
	{
		return;
	}
	//ʹ��OpenSCManager������SCM
	m_hServiceMgr = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (m_hServiceMgr == NULL)
	{
		MessageBox(_T("OpenSCManager Error"), _T("Error"));
		CloseServiceHandle(m_hServiceMgr);
		return;
	}
	//�򿪷����þ��
	m_hServiceDDK = OpenService(m_hServiceMgr, _T("HadesService"), SERVICE_STOP | DELETE);
	//ɾ����������
	BOOL bRet = DeleteService(m_hServiceDDK);
	if (bRet == FALSE)
	{
		MessageBox(_T("UnInstall Service Fail"), _T("Error"));
		CloseServiceHandle(m_hServiceDDK);
		CloseServiceHandle(m_hServiceMgr);
		return;
	}
	GetDlgItem(IDC_STATIC_PROMPT)->SetWindowText(_T("����ж�سɹ�!"));//���ÿؼ��ı�����
	CloseServiceHandle(m_hServiceDDK);
	CloseServiceHandle(m_hServiceMgr);

}
