// mfc.h : main header file for the mfc DLL
//

#ifndef _MFC_HTMLAYOUT_H
#define _MFC_HTMLAYOUT_H
#pragma once

#include "lua.hpp"


//生成htmlayout控件，嵌入到现有窗口中
CWnd* CreateHtmlayoutControl(lua_State* L, CWnd* pParent, const char* pszUrl);

//删除控件,父窗口关闭的时候调用
int   DestroyHtmlayoutControl(CWnd* pControl);

//向控件发送消息
int   SendMessage2Control(CWnd* pControl,  const char* pszName,   const char* pszMsg, int nLen = 0);

//直接调用控件脚本函数view:funcName(...)
//此函数只能在主线程中调用，否则可能导致程序崩溃
int   CallControlFunc(CWnd* pControl, lua_State* L, const char* pszFuncName, int nParams);

//加载mfc模块
extern "C" int luaopen_mfc(lua_State* L);

//环境初始化
int mfc_init(lua_State* L);

#endif