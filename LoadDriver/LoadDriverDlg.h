
// LoadDriverDlg.h : 头文件
//

#pragma once
#include <winsvc.h>

// CLoadDriverDlg 对话框
class CLoadDriverDlg : public CDialogEx
{
// 构造
public:
	CLoadDriverDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_LOADDRIVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	SC_HANDLE m_hServiceMgr;
	SC_HANDLE m_hServiceDDK;
	bool m_isDriverFile;
	//文件路径
	CString m_szPath_w;
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
