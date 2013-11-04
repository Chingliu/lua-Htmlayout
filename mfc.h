// mfc.h : main header file for the mfc DLL
//

#ifndef _MFC_HTMLAYOUT_H
#define _MFC_HTMLAYOUT_H
#pragma once

#include "lua.hpp"


//����htmlayout�ؼ���Ƕ�뵽���д�����
CWnd* CreateHtmlayoutControl(lua_State* L, CWnd* pParent, const char* pszUrl);

//ɾ���ؼ�,�����ڹرյ�ʱ�����
int   DestroyHtmlayoutControl(CWnd* pControl);

//��ؼ�������Ϣ
int   SendMessage2Control(CWnd* pControl,  const char* pszName,   const char* pszMsg, int nLen = 0);

//ֱ�ӵ��ÿؼ��ű�����view:funcName(...)
//�˺���ֻ�������߳��е��ã�������ܵ��³������
int   CallControlFunc(CWnd* pControl, lua_State* L, const char* pszFuncName, int nParams);

//����mfcģ��
extern "C" int luaopen_mfc(lua_State* L);

//������ʼ��
int mfc_init(lua_State* L);

#endif