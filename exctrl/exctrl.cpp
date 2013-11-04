// exctrl.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "exctrl.h"
#include "ExtendWidget.h"
#include <vector>


#include "MyListBox.h"
#include "ScintillaCtrl.h"
#include "webctrl.h"
#include "flashctrl.h"

#include <lua.h>
static std::vector<CControlType*>  s_ControlList;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(CExCtrlApp, CWinApp)
END_MESSAGE_MAP()


// CtsApp ����

CExCtrlApp::CExCtrlApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CtsApp ����

CExCtrlApp theApp;


// CtsApp ��ʼ��

BOOL CExCtrlApp::InitInstance()
{
	CWinApp::InitInstance();

	//��ʼ���ؼ�
	s_ControlList.push_back( new ListBoxType);

	//Scintilla�ؼ�
	HMODULE hmod = LoadLibrary("SciLexer.dll");
	if(hmod)
		s_ControlList.push_back(new ScintillaType);

	//IE�ؼ�
	s_ControlList.push_back(new IEWebBrowserType);

	//Flash�ؼ�
	s_ControlList.push_back(new FlashCtrlType);

	//���һ��һ��Ҫ����NULL
	s_ControlList.push_back((CControlType*)NULL);

	return TRUE;
}



extern "C"
CControlType** WINAPI open_mfc_exctrl(lua_State* L)
{	
	if(s_ControlList.empty())
		return NULL;

	CExctrlWrappedWnd::Register();
	lua_getglobal(L,  "require");
	lua_pushstring(L, "exctrl");
	lua_pcall(L, 1, -1, 0);

	return &s_ControlList[0];
	
}
