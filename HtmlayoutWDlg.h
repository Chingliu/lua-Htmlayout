// HtmlayoutWDlg.h : header file
//

#if !defined(AFX_HTMLAYOUTWDLG_H__D1707EB0_2419_4B57_8064_4221320E0AD5__INCLUDED_)
#define AFX_HTMLAYOUTWDLG_H__D1707EB0_2419_4B57_8064_4221320E0AD5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CHtmlayoutWDlg dialog
#include "luna.h"
#include <htmlayoutwrapper.h>
#include "afxwin.h"
#include "ExtendWidget.h"

#define WM_HTMLAYOUTCTRL	(WM_USER + 913)

#if _MSC_VER > 1200
#define HITRTN  LRESULT
#else
#define HITRTN  UINT
#endif

//��htmlayout�Ի�����Ϊ��չ�ؼ�ʹ�ô���
class CHtmlayoutWDlg;
struct HtmalyoutType : public CControlTypeT<HtmalyoutType, CHtmlayoutWDlg>
{ 
public:
	HtmalyoutType()
	{		
	};
	virtual const char* GetTypeName() {
		return "htmlayoutw";
	}
	//���ɿؼ�
	virtual HWND Create(lua_State* L, HELEMENT h,  HWND hParent, void*& pControl);	

	//���ٿؼ�
	virtual bool  Destroy(void* pControl);			

	//���º�����pContrl����Create�������ɵĿؼ�
	virtual int Attribute(void* pControl, lua_State* L, BOOL bGet,  BOOL& bHandled){return 0;}

	virtual int BindLuaTable(void* pControl, lua_State* L, int nTableRef){return 0;}	//֪ͨ�ؼ���lua ��Ӧtable
};


struct  MsgData
{
	long   m_nLen;
	const char* m_pszData;
	MsgData(const char* pszData, long nLen = 0)
		:m_nLen(nLen), m_pszData(pszData)
	{
		if(m_pszData && m_nLen <= 0)
			m_nLen = strlen(m_pszData);
	}
};
//////////////////////////////////////////////////////////////////////////
class CHtmlayoutWDlg : public CDialog, public CHtmalyoutWrapper<CHtmlayoutWDlg>
{
// Construction
public:
	static const char className[];						//����	
	DECLARE_LUA_SUPPORT_CTRL(CHtmlayoutWDlg);
	CHtmlayoutWDlg(lua_State* L){};
	CHtmlayoutWDlg(lua_State* L,  LPCTSTR lpszUrl, CWnd* pParent = NULL, BOOL bCreate= FALSE, UINT uDLGID = 0, bool bSkinWin = false, int nParams = 0);	// standard constructor
	~CHtmlayoutWDlg();
	typedef CHtmalyoutWrapper<CHtmlayoutWDlg>  basehtmalyout;
// Dialog Data
	//{{AFX_DATA(CHtmlayoutWDlg)
	enum { IDD = 9102 };	
	//}}AFX_DATA

	CString m_strURL;
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHtmlayoutWDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	

public:
	virtual BOOL on_event (HELEMENT he, HELEMENT target, BEHAVIOR_EVENTS type, UINT_PTR reason );
	virtual LRESULT OnCreateControl( LPNMHL_CREATE_CONTROL nm);
	virtual LRESULT OnDestroyControl( LPNMHL_DESTROY_CONTROL nm);
	virtual LRESULT OnAttachBehavior( LPNMHL_ATTACH_BEHAVIOR lpab );
	virtual LRESULT OnLoadData( LPNMHL_LOAD_DATA pnmld);
	virtual LRESULT OnDataLoaded( LPNMHL_DATA_LOADED nm);
	BOOL Load(LPCTSTR lpszURL);

	void    SetControl(BOOL isControl)
	{
		m_bIsControl = isControl;
	}

	//ֱ�ӵ��ú��� view:funcname(...)
	int CallViewFunction(LPCTSTR lpszFuncname, int narg);
protected:
	//���Һ�������
	int    FindFunction(const char* pszFuncName);

	//���ú���ǰ׼��������Error������������С��0,��ʾ�������ܵ���
	int	   BeginCallFunc(int nFunRef);

	//����ȫ�ֺ���׼��������ֵС��0�����ܵ��ã����򷵻ش�����������	
	int	   BeginCallGlobalFunc(const char* pszFuncName);

	//��õ��ð󶨺���
	BOOL GetTableFunction(LPCTSTR lpszFuncname);

	//���ð󶨺���
	int CallFunction(int narg, int nError = 0);

	//���ڹر�ʱ����ű��������
	void WindowClose();

// Implementation
protected:	
	//��������ʹ�þ�̬��������Ȼ����֧�ֶ��߳�
	int		m_nEventRef;		//Event handle function ref
	int     m_nErrorRef;		//������Ϣ����ref
	int		m_nHandleLoadRef;	//�������ݼ���
	int		m_nHandleNcHitRef;	//����NcHitTest����
	int     m_nDestroyCtrlRef;	//ɾ���ؼ�����

	BOOL	m_bModal;			//�Ƿ���ģ̬�Ի���

	int		m_nParams;			//������������

	int		m_nBindViewRef;		//���ڵ�View Table

	int		m_bSkinWindow;		//�Ƿ����Զ��崰�ڣ�������ʾ�߿�

	BOOL	m_bIsControl;		//��ǰ�����Ƿ��ǿؼ�
	// Generated message map functions
	//{{AFX_MSG(CHtmlayoutWDlg)
	virtual BOOL OnInitDialog();		
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	
	afx_msg HITRTN OnNcHitTest(CPoint point);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	//}}AFX_MSG
	afx_msg LRESULT OnCtrlMsg(WPARAM wParam, LPARAM lParam); 
	DECLARE_MESSAGE_MAP()
public:	
	afx_msg void OnClose();
	afx_msg void OnDestroy();


	//������Lua�еĺ���
public:	
	int load_url(lua_State* L);
	int set_title(lua_State* L);
	int get_title(lua_State* L);
	int close(lua_State* L);	
	int resize(lua_State* L);	
	int show_window(lua_State* L);
	int modify_style(lua_State* L);
	int set_icon(lua_State* L);
	int get_size(lua_State* L);
	int get_pos(lua_State* L);
	int find_element(lua_State* L);
	int get_cursor_pos(lua_State* L);
	int move(lua_State* L);
	int center(lua_State* L);
	int transparent(lua_State* L);
	int transparent_color(lua_State* L);
	int set_size_limit(lua_State* L);
	int drag_anywhere(lua_State* L);
	int drag_rect(lua_State* L);	
	int can_drag_file(lua_State* L);
	int message_box(lua_State* L);
	int get_hwnd(lua_State* L);
	int register_bind(lua_State* L);
	int update(lua_State* L);
	int set_glass(lua_State* L);
	int set_option(lua_State* L);
	int get_window_state(lua_State* L);
protected:
	CSize  m_MinSize;
	CSize  m_MaxSize;
	BOOL   m_bDragAnywhere;	//����κ�λ���϶�����
	CRect  m_DragRect;		//�˷�Χ����϶�����
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HTMLAYOUTWDLG_H__D1707EB0_2419_4B57_8064_4221320E0AD5__INCLUDED_)
