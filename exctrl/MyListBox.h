#pragma once

#include "ExtendWidget.h"
class CMyListBox;

struct ListBoxType : public CControlTypeT<ListBoxType, CMyListBox>
{
public:
	ListBoxType()
	{
	};
	virtual const char* GetTypeName() {
		return "mylist";
	}
	//���ɿؼ�
	virtual HWND Create(lua_State* L, HELEMENT h, HWND hParent, void*& pControl);	

	//���ٿؼ�
	virtual bool  Destroy(void* pControl);			

	//���º�����pContrl����Create�������ɵĿؼ�
	virtual int Attribute(void* pControl, lua_State* L, BOOL bGet,  BOOL& bHandled);
};

class CMyListBox : public CListBox
{
	DECLARE_DYNAMIC(CMyListBox)

public:
	DECLARE_LUA_SUPPORT_CTRL(CMyListBox);	//Lua֧��
public:
	CMyListBox();
	virtual ~CMyListBox();


protected:
	int AddStringL(lua_State* L);
	int SetCurSelL(lua_State* L);
	int DeleteItemL(lua_State* L);
	int EmptyL(lua_State* L);	
	int GetItemTextL(lua_State* L);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnSelchange();
	afx_msg void OnLbnDblclk();
};


