#ifndef _SCINTILLA_H_
#define _SCINTILLA_H_
#pragma once

#include "ExtendWidget.h"
#include "Scintilla.h"
#include "wrapperwnd.h"

class CScintillaCtrl;
//����ע������
struct ScintillaType : public CControlTypeT<ScintillaType, CScintillaCtrl>
{
public:
	ScintillaType()
	{		
	};
	virtual const char* GetTypeName() {
		return "scintilla";
	}
	//���ɿؼ�
	virtual HWND Create(lua_State* L, HELEMENT h, HWND hParent, void*& pControl);	

	//���ٿؼ�
	virtual bool  Destroy(void* pControl);			

	//���º�����pContrl����Create�������ɵĿؼ�
	virtual int Attribute(void* pControl, lua_State* L, BOOL bGet,  BOOL& bHandled);
};

//////////////////////////////////////////////////////////////////////////
class CScintillaCtrl: public CExctrlWrappedWnd
{
public:
	CScintillaCtrl();
	~CScintillaCtrl(void);

	DECLARE_LUA_SUPPORT_CTRL(CScintillaCtrl);
protected:
	HWND	    m_hScintillaWnd;
	SciFnDirect m_pFnDirect;	//ֱ�ӵ��ù��ܺ���
	sptr_t	    m_ptr;			//������
	int			m_nCtrlID;
	
	typedef std::map<long, long> CNotifyCodeMap;
	CNotifyCodeMap	m_NotifyCodeMap;
public:

	virtual BOOL	Create(HWND hParent);
	//ɾ������
	virtual void Destroy();

	//��ؼ�������Ϣ
	sptr_t  SendEditor(unsigned int iMessage, uptr_t wParam = 0, sptr_t lParam = 0)
	{
		if(m_pFnDirect == NULL || m_ptr == NULL)
			return NULL;

		return (*m_pFnDirect)(m_ptr, iMessage, wParam, lParam);
	}

	//ע����Ҫ�����֪ͨ����
	void RegisterNotify(long nNotifyCode);

protected:
	int  SendMsg(lua_State* L);			//������Ϣ
	int  RegisterNotify(lua_State* L);	//ע��notify��Ϣ
	int  GetText(lua_State* L);			//�����������
	int  GetTextRange(lua_State* L);	//���ָ��λ�õ��ַ���
	int  GetLine(lua_State* L);			//���ĳ�е��ַ�
protected:
	void    Notify(SCNotification* pNotify);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEarseBackground();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	DECLARE_MESSAGE_MAP()
};

#endif //_SCINTILLA_H_