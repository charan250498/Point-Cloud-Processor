// MainDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Point Cloud Processor.h"
#include "MainDialog.h"
#include "afxdialogex.h"


// MainDialog dialog

IMPLEMENT_DYNAMIC(MainDialog, CDialog)

MainDialog::MainDialog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_MainDialog, pParent)
{

}

MainDialog::~MainDialog()
{
}

void MainDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(MainDialog, CDialog)
	ON_BN_CLICKED(IDCANCEL, &MainDialog::OnBnClickedCancel)
END_MESSAGE_MAP()


// MainDialog message handlers


void MainDialog::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}
