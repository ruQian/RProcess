
// LoadDriver.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CLoadDriverApp:
// �йش����ʵ�֣������ LoadDriver.cpp
//

class CLoadDriverApp : public CWinApp
{
public:
	CLoadDriverApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CLoadDriverApp theApp;