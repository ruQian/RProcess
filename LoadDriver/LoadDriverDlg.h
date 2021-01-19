
// LoadDriverDlg.h : ͷ�ļ�
//

#pragma once
#include <winsvc.h>

// CLoadDriverDlg �Ի���
class CLoadDriverDlg : public CDialogEx
{
// ����
public:
	CLoadDriverDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_LOADDRIVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

private:
	SC_HANDLE m_hServiceMgr;
	SC_HANDLE m_hServiceDDK;
	bool m_isDriverFile;
	//�ļ�·��
	CString m_szPath_w;
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonOpenfile();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedLoadDriver();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};
