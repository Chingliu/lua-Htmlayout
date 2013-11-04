#ifndef _WEBCTRL_H_
#define _WEBCTRL_H_
#pragma once
//IE �ؼ���װ

#include "ExtendWidget.h"
#include "SimpleBrowser.h"

class CWebCtrl;
//����ע������
struct IEWebBrowserType : public CControlTypeT<IEWebBrowserType, CWebCtrl>
{
public:
	IEWebBrowserType()
	{		
	};
	virtual const char* GetTypeName() {
		return "iebrowser";
	}
	//���ɿؼ�
	virtual HWND Create(lua_State* L, HELEMENT h, HWND hParent, void*& pControl);	

	//���ٿؼ�
	virtual bool  Destroy(void* pControl);			

	//���º�����pContrl����Create�������ɵĿؼ�
	virtual int Attribute(void* pControl, lua_State* L, BOOL bGet,  BOOL& bHandled);
};

class CWebCtrl : public SimpleBrowser {
public:
	CWebCtrl();
	DECLARE_LUA_SUPPORT_CTRL(CWebCtrl);

	virtual void OnTitleChange(CString text);

	virtual void OnDocumentComplete(CString	URL);

	virtual bool OnBeforeNavigate2(CString	URL,
		CString	frame,
		void		*post_data,int post_data_size,
		CString	headers);

	virtual bool OnNewWindow3(CString strURL, CString strURLContext, DWORD dwFlags);

protected:
	//ת��ָ����url
	int NavigateL(lua_State* L);

	//��ǰҳ������ڴ�ҳ��
	int WriteL(lua_State* L);

	//��յ�ǰҳ��
	int ClearL(lua_State* L);

	int GoBackL(lua_State* L);

	int GoForwardL(lua_State *L);

	int GoHomeL(lua_State *L);

	int RefreshL(lua_State *L);

	int StopL(lua_State *L);
};

#endif //_WEBCTRL_H_