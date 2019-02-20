
// Point Cloud Processor.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CPointCloudProcessorApp:
// See Point Cloud Processor.cpp for the implementation of this class
//

class CPointCloudProcessorApp : public CWinApp
{
public:
	CPointCloudProcessorApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CPointCloudProcessorApp theApp;
