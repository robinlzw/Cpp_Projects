
// gaoyuan.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CgaoyuanApp: 
// �йش����ʵ�֣������ gaoyuan.cpp
//

class CgaoyuanApp : public CWinApp
{
public:
	CgaoyuanApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CgaoyuanApp theApp;