// HtmlayoutWDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HtmlayoutWDlg.h"
#include "LuaHtmlayoutElment.h"
#include "ExtendWidget.h"
#include "util.h"
#include "mfc.h"
#if _MSC_VER >= 0x1500
#include <dwmapi.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
HWND HtmalyoutType::Create(lua_State* L, HELEMENT h,  HWND hParent, void*& pControl)
{
	CHtmlayoutWDlg* pDlg = new CHtmlayoutWDlg(L, "", CWnd::FromHandle(hParent), TRUE, 9103);
	if(pDlg == NULL)
		return NULL;
	pDlg->SetControl(TRUE);
	pControl = pDlg;
	return pDlg->GetSafeHwnd();
}


bool HtmalyoutType::Destroy(void* pControl)
{
	CHtmlayoutWDlg* pDlg = ChangeControlType(pControl);
	if(pDlg)
	{
		pDlg->SendMessage(WM_CLOSE);		
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////

const char CHtmlayoutWDlg::className[] = "LuaViewWindow";
Lunar<CHtmlayoutWDlg>::RegType CHtmlayoutWDlg::methods[] =
{	
	method(CHtmlayoutWDlg, set_title),
	method(CHtmlayoutWDlg, get_title),
	method(CHtmlayoutWDlg, close),	
	method(CHtmlayoutWDlg, resize),	
	method(CHtmlayoutWDlg, show_window),
	method(CHtmlayoutWDlg, modify_style),
	method(CHtmlayoutWDlg, set_icon),
	method(CHtmlayoutWDlg, get_size),
	method(CHtmlayoutWDlg, get_pos),
	method(CHtmlayoutWDlg, find_element),
	method(CHtmlayoutWDlg, get_cursor_pos),
	method(CHtmlayoutWDlg, move),
	method(CHtmlayoutWDlg, center),
	method(CHtmlayoutWDlg, transparent),
	method(CHtmlayoutWDlg, transparent_color),
	method(CHtmlayoutWDlg, set_size_limit),
	method(CHtmlayoutWDlg, drag_anywhere),
	method(CHtmlayoutWDlg, drag_rect),	
	method(CHtmlayoutWDlg, load_url),	
	method(CHtmlayoutWDlg, can_drag_file),	
	method(CHtmlayoutWDlg, message_box),	
	method(CHtmlayoutWDlg, get_hwnd),
	method(CHtmlayoutWDlg, register_bind),
	method(CHtmlayoutWDlg, update),
	method(CHtmlayoutWDlg, set_glass),
	method(CHtmlayoutWDlg, set_option),
	method(CHtmlayoutWDlg, get_window_state),

	{0, 0}
};

/////////////////////////////////////////////////////////////////////////////
// CHtmlayoutWDlg dialog
CHtmlayoutWDlg::CHtmlayoutWDlg(lua_State* L, LPCTSTR lpszUrl, CWnd* pParent /*=NULL*/, BOOL bCreate/*= FALSE*/, UINT uDLGID/*=0*/, bool bSkinWin, int nParmas/*=0*/)
	: m_L(L), CDialog(CHtmlayoutWDlg::IDD, pParent), m_nParams(nParmas),m_bSkinWindow(bSkinWin)
{
	//{{AFX_DATA_INIT(CHtmlayoutWDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_nEventRef = LUA_REFNIL;
	m_nErrorRef = LUA_REFNIL;
	m_nHandleLoadRef = LUA_REFNIL;
	m_nHandleNcHitRef = LUA_REFNIL;
	m_nDestroyCtrlRef = LUA_REFNIL;
	m_nBindViewRef = LUA_REFNIL;
	m_bDragAnywhere = FALSE;
	m_DragRect = CRect(0, 0, 0, 0);

	m_MinSize = CSize(0, 0);
	m_MaxSize = CSize(0, 0);

	m_bIsControl = FALSE;

	m_strURL = lpszUrl;
	if(bCreate)
	{
		this->Create(uDLGID > 0 ? uDLGID : CHtmlayoutWDlg::IDD, pParent);
	}
	m_bModal = !bCreate;

}

//ע����������
#define  UNREF(L, ref) \
	if(ref != LUA_REFNIL)\
	{\
		lua_unref(L, ref);\
		ref = LUA_REFNIL;\
	}

CHtmlayoutWDlg::~CHtmlayoutWDlg()
{
	UNREF(m_L, m_nEventRef);
	UNREF(m_L, m_nErrorRef);
	UNREF(m_L, m_nHandleLoadRef);
	UNREF(m_L, m_nHandleNcHitRef);
	UNREF(m_L, m_nDestroyCtrlRef);
	UNREF(m_L, m_nBindViewRef);
}
void CHtmlayoutWDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHtmlayoutWDlg)
	//DDX_Control(pDX, IDC_EDIT1, m_ctrlEdit);
	//}}AFX_DATA_MAP

}

BEGIN_MESSAGE_MAP(CHtmlayoutWDlg, CDialog)
	//{{AFX_MSG_MAP(CHtmlayoutWDlg)	
	ON_WM_CREATE()
	ON_WM_NCHITTEST()
	ON_WM_NCCALCSIZE()
	ON_WM_DESTROY()
	ON_WM_GETMINMAXINFO()
	ON_WM_DROPFILES()
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
	ON_MESSAGE(WM_HTMLAYOUTCTRL, OnCtrlMsg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHtmlayoutWDlg message handlers

BOOL CHtmlayoutWDlg::OnInitDialog()
{
	CDialog::OnInitDialog();		

	m_nEventRef = FindFunction("HandleHtmlayoutEvent");
	m_nHandleLoadRef = FindFunction("HandleDataLoad");
	m_nErrorRef = FindFunction("errormsg");
	m_nHandleNcHitRef = FindFunction("HandleNcHitTest");
	m_nDestroyCtrlRef = FindFunction("onDestroyControl");

	if(m_bSkinWindow)
	{
		ModifyStyle(WS_BORDER | WS_CAPTION, 0);
		::SetWindowLong(m_hWnd, GWL_EXSTYLE, WS_EX_LAYERED);
	}

	HTMLayoutSetOption(m_hWnd, HTMLAYOUT_SMOOTH_SCROLL, FALSE);
	
	Load(m_strURL);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

LRESULT CHtmlayoutWDlg::OnCtrlMsg(WPARAM wParam, LPARAM lParam)
{
	Lua_SaveState ss(m_L);
	int nError = BeginCallGlobalFunc("onReceiveViewMsg");
	if(nError < 0)
		return -1;
	
	MsgData* pName = (MsgData*)wParam;
	MsgData* pData = (MsgData*)lParam;

	Lunar<CHtmlayoutWDlg>::push(m_L, this, false);	
	lua_pushlstring(m_L, pName->m_pszData, pName->m_nLen);
	if(pData->m_nLen >= 0 && pData->m_pszData)
		lua_pushlstring(m_L, pData->m_pszData, pData->m_nLen);
	else
		lua_pushnil(m_L);
	
	lua_pcall(m_L, 3, -1, nError);
	return 0;
}

//��õ��ú���
int CHtmlayoutWDlg::GetTableFunction(LPCTSTR lpszFuncname)
{
	if(m_nBindViewRef == LUA_REFNIL)
	{
		return -1;
	}

	do
	{
		int nError = 0;
		if(m_nErrorRef != LUA_REFNIL)
		{
			lua_getref(m_L, m_nErrorRef);
			nError = lua_gettop(m_L);
		}

		lua_getref(m_L,		  m_nBindViewRef);
		lua_getfield(m_L, -1, lpszFuncname);
		if(!lua_isfunction(m_L, -1))
		{
			break;
		}		
		lua_pushvalue(m_L, -2);
		lua_remove(m_L, -3);			

		return nError;
	}
	while(0);		
	return -1;
}

//���ö�����
int CHtmlayoutWDlg::CallFunction(int narg, int nError/* = 0*/)
{
	return lua_pcall(m_L, 1 + narg, -1, nError);
}

int   CHtmlayoutWDlg::CallViewFunction(LPCTSTR lpszFuncname, int narg)
{
	int nTop = lua_gettop(m_L);
	int nError = GetTableFunction(lpszFuncname);
	if(nError < 0)
		return -1;

	int i = 0;
	for(i = narg - 1; i >= 0; --i)
	{
		lua_pushvalue(m_L, nTop - i);
	}
	//ɾ������Ĳ���
	for(i = 0; i < narg; ++i)
	{
		lua_remove(m_L, nTop - (narg - 1));
	}

	return CallFunction(narg, nError);
}

int   CHtmlayoutWDlg::FindFunction(const char* pszFuncName)
{
	lua_getglobal(m_L, pszFuncName);
	if(!lua_isfunction(m_L, -1))
	{
		lua_pop(m_L, 1);
		return LUA_REFNIL;
	}

	return lua_ref(m_L, this);
}

int CHtmlayoutWDlg::BeginCallFunc(int nFunRef)
{
	if(nFunRef == LUA_REFNIL)
		return -1;

	int nError = 0;
	if(m_nErrorRef != LUA_REFNIL)
	{
		lua_getref(m_L, m_nErrorRef);
		nError = lua_gettop(m_L);
	}
	lua_getref(m_L, nFunRef);
	return nError;
}

int  CHtmlayoutWDlg::BeginCallGlobalFunc(const char* pszFuncName)
{
	if(pszFuncName == NULL)
		return -1;

	int nError = 0;
	if(m_nErrorRef != LUA_REFNIL)
	{
		lua_getref(m_L, m_nErrorRef);
		nError = lua_gettop(m_L);
	}
	lua_getglobal(m_L, pszFuncName);
	if(lua_isfunction(m_L, -1))
		return nError;

	lua_pop(m_L, 1 + (m_nErrorRef != LUA_REFNIL ? 1 : 0));
	return -1;
}
BOOL CHtmlayoutWDlg::Load(LPCTSTR lpszURL)
{
	if(!LoadUrl(lpszURL))
		return FALSE;
	
	do
	{
		Lua_SaveState ss(m_L);
		int nError = BeginCallGlobalFunc("onHtmlayoutDocumentComplete");
		if(nError < 0)
			break;

		Lunar<CHtmlayoutWDlg>::push(m_L, this, false);
		CLuaHtmlayoutElement::PushElement(m_L, htmlayout::dom::element::root_element(m_hWnd));
		if(lua_isnil(m_L, -1))
			return FALSE;
		
		for(int i = 1; i <= m_nParams; ++i)
		{
			lua_pushvalue(m_L, i);
		}
		lua_pcall(m_L, 2 + m_nParams, 0, nError);

	}while(0);

	return TRUE;
}

int CHtmlayoutWDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	OnCreateCall();
	return 0;
}

#define BORDER_SIZE  6
HITRTN CHtmlayoutWDlg::OnNcHitTest(CPoint point)
{	
	LRESULT nRet = CDialog::OnNcHitTest(point);
	if(m_bDragAnywhere)
		return HTCAPTION;
	else 
	{
		CRect rect;
		this->GetClientRect(&rect);

		ScreenToClient(&point);

		CRect dragRect(m_DragRect);

		int nRight = dragRect.right;
		if(nRight < 0)
		{
			dragRect.right = rect.right + nRight;
		}
		if(dragRect.PtInRect(point))
		{
			return HTCAPTION;
		}
		if(!m_bSkinWindow)
			return nRet;

		int x = point.x;
		int y = point.y;
	
		if(x <= rect.left + BORDER_SIZE && y <= rect.top + BORDER_SIZE)
		{
			return HTTOPLEFT;
		}
		else if(x <= rect.left + BORDER_SIZE && y >= rect.bottom - BORDER_SIZE)
		{
			return HTBOTTOMLEFT;
		}
		else if(x >= rect.right - BORDER_SIZE && y >= rect.bottom - BORDER_SIZE)
		{
			return HTBOTTOMRIGHT;
		}
		else if(x >= rect.right - BORDER_SIZE && y <= rect.top + BORDER_SIZE)
		{
			return HTTOPRIGHT;
		}
		else if(x >= rect.right - BORDER_SIZE)
		{
			return HTRIGHT;
		}
		else if(x <= rect.left + BORDER_SIZE)
		{
			return HTLEFT;
		}
		else if(y >= rect.bottom - BORDER_SIZE)
		{
			return HTBOTTOM;
		}
		else if(y <= rect.top + BORDER_SIZE)
		{
			return HTTOP;
		}
	}
	return nRet;
}

void CHtmlayoutWDlg::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	if(m_bSkinWindow)
		return;
	CDialog::OnNcCalcSize(bCalcValidRects, lpncsp);
}

#define G_VALUE(x,y) if(x > 0) y = x;
void CHtmlayoutWDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	CDialog::OnGetMinMaxInfo(lpMMI);
	G_VALUE(m_MinSize.cx, lpMMI->ptMinTrackSize.x);
	G_VALUE(m_MinSize.cy, lpMMI->ptMinTrackSize.y);
	G_VALUE(m_MaxSize.cx, lpMMI->ptMaxTrackSize.x);
	G_VALUE(m_MaxSize.cy, lpMMI->ptMaxTrackSize.y);
}

LRESULT CHtmlayoutWDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	BOOL bHandled = FALSE;
	LRESULT lResult = 0;
	lResult = OnWindowProcCall(message, wParam, lParam, bHandled);
	if(bHandled)
		return lResult;

	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CHtmlayoutWDlg::PreTranslateMessage(MSG* pMsg) 
{	
	BOOL bRtn = CDialog::PreTranslateMessage(pMsg);	
	if(pMsg->message == WM_KEYDOWN 
		&& (GetKeyState(VK_CONTROL) < 0 || GetKeyState(VK_MENU) < 0)
		&& GetFocus() != this)
	{		
		BOOL bHandled = FALSE;
		LRESULT lResult = 0;
		lResult = OnWindowProcCall(pMsg->message, pMsg->wParam, pMsg->lParam, bHandled);
		if(bHandled)
			return TRUE;
	}

	if(pMsg->message == WM_CHAR)
	{
		CWnd* pWnd = CWnd::GetFocus();
		if(pWnd && pWnd->GetSafeHwnd() == m_hWnd)
			return FALSE;
	}
	return bRtn;
}
	
LRESULT CHtmlayoutWDlg::OnCreateControl( LPNMHL_CREATE_CONTROL nm)
{
	htmlayout::dom::element e = nm->helement;
	CString strType = e.get_attribute("type");

	void* pContext = NULL;
	nm->outControlHwnd = CreateWidget(m_L, nm->helement, strType, nm->inHwndParent, pContext);

	if(pContext)
	{
		CString strPrototype = "luaex_" + strType;
		e.set_attribute("widget_prototype", aux::a2w(strPrototype));
		
		CString strValue;
		strValue.Format("%X", pContext);
		e.set_attribute("_self_widget_", aux::a2w(strValue));

		strValue.Format("%X", nm->outControlHwnd);
		e.set_attribute("_self_hwnd_",  aux::a2w(strValue));		
	}

	return 0L;
}

LRESULT CHtmlayoutWDlg::OnDestroyControl( LPNMHL_DESTROY_CONTROL nm)
{
	htmlayout::dom::element e = nm->helement;
	Lua_SaveState ss(m_L);
	do
	{
		int nError = BeginCallFunc(m_nDestroyCtrlRef);
		if(nError < 0)
			break;

		CLuaHtmlayoutElement::PushElement(m_L, nm->helement);
		lua_pcall(m_L, 1, 0, nError);
	}while(0);

	return 0L;
}


LRESULT CHtmlayoutWDlg::OnAttachBehavior(LPNMHL_ATTACH_BEHAVIOR lpab )
{
	htmlayout::dom::element e = lpab->element;

	//����ǰ��Lua����������ݸ���ǰԪ��
	CString strLuaHandle;
	strLuaHandle.Format("%X", m_L);
	e.set_attribute("_lua_handle_", aux::a2w(strLuaHandle));
	//

	return basehtmalyout::OnAttachBehavior(lpab);
}

LRESULT CHtmlayoutWDlg::OnLoadData( LPNMHL_LOAD_DATA pnmld)
{
	if(m_nHandleLoadRef == LUA_REFNIL)
		return LOAD_OK;

	Lua_SaveState ss(m_L);
	do
	{
		int nError = BeginCallFunc(m_nHandleLoadRef);
		if(nError < 0)
			break;
		
		lua_pushnumber(m_L, 0);
		lua_pushnumber(m_L, pnmld->dataType);
		lua_pushstring(m_L, aux::w2a(pnmld->uri));
		
		if(0 != lua_pcall(m_L, 3, 1, nError))
		{
			break;
		}

		if(lua_isstring(m_L, -1))
		{
			pnmld->outData = (LPVOID)PAS(m_L, -1);
			pnmld->outDataSize = PASL(m_L, -1);
		}
	}while(0);

	return LOAD_OK;
}

LRESULT CHtmlayoutWDlg::OnDataLoaded( LPNMHL_DATA_LOADED nm)
{
	if(m_nHandleLoadRef == LUA_REFNIL)
		return LOAD_OK;

	Lua_SaveState ss(m_L);
	do
	{
		int nError = BeginCallFunc(m_nHandleLoadRef);
		if(nError < 0)
			break;

		lua_pushnumber(m_L, 1);
		lua_pushnumber(m_L, nm->dataType);
		lua_pushstring(m_L, aux::w2a(nm->uri));
		lua_pushlstring(m_L, (const char*)nm->data, nm->dataSize);

		lua_pcall(m_L, 4, -1, nError);
	}while(0);

	return LOAD_OK;
}

BOOL CHtmlayoutWDlg::on_event (HELEMENT he, HELEMENT target, BEHAVIOR_EVENTS type, UINT_PTR reason )
{	
	if(m_nEventRef == LUA_REFNIL)
		return FALSE;
	
	if((type & HANDLED))
		return FALSE;

	unsigned int nType = type;
	nType &= ~SINKING;	
	BOOL bHandled = FALSE;
	Lua_SaveState ss(m_L);
	do
	{
		int nError = BeginCallFunc(m_nEventRef);
		if(nError < 0)
			break;

		CLuaHtmlayoutElement::PushElement(m_L, he);
		if(he == target)
			lua_pushvalue(m_L, -1);
		else
			CLuaHtmlayoutElement::PushElement(m_L, target);
		
		lua_pushnumber(m_L, nType);
		lua_pushnumber(m_L, reason);
		lua_pushboolean(m_L, type & SINKING);

		if(0 != lua_pcall(m_L, 5, -1, nError))
		{
			bHandled = TRUE;	//��ֹ�ű������ʱ����Ϣ���ε���
			break;
		}
		bHandled = lua_toboolean(m_L, -1);
	}while(0);
	return bHandled;
}

void CHtmlayoutWDlg::WindowClose()
{
	//���ýű��еĹرպ������ͷŽű��е���Դ
	Lua_SaveState ss(m_L);
	int nError = BeginCallGlobalFunc("onWindowClose");
	if(nError < 0)
		return;

	CLuaHtmlayoutElement::PushElement(m_L, htmlayout::dom::element::root_element(m_hWnd));
	if(lua_isnil(m_L, -1))
		return;
	
	Lunar<CHtmlayoutWDlg>::push(m_L, this, false);
	lua_pcall(m_L, 2, 0, nError);

	return;
}

void CHtmlayoutWDlg::OnClose()
{
	do
	{
		Lua_SaveState ss(m_L);
		if(0 == CallViewFunction("onClose", 0))
		{
			if(!lua_toboolean(m_L, -1))
				return;
		}
	}while(0);

	WindowClose();
		
	CDialog::OnClose();	
	
	if(!m_bModal)
	{
		this->DestroyWindow();
		delete this;
	}	
}

void CHtmlayoutWDlg::OnDestroy()
{
	CDialog::OnDestroy();	
}

void CHtmlayoutWDlg::OnDropFiles(HDROP hDropInfo)
{
	char szPath[MAX_PATH + 1] = {0};

	Lua_SaveState ss(m_L);
	int nError = GetTableFunction("onDropFiles");
	if(nError >= 0)
	{
		lua_newtable(m_L);
		POINT pt;
		DragQueryPoint(hDropInfo, &pt);
		lua_pushnumber(m_L, pt.x);
		lua_setfield(m_L, -2, "x");
		lua_pushnumber(m_L, pt.y);
		lua_setfield(m_L, -2, "y");

		UINT uNumFiles = DragQueryFile(hDropInfo, -1, NULL, 0);
		for(UINT nIndex = 0; nIndex < uNumFiles; ++nIndex)
		{
			DragQueryFile(hDropInfo, nIndex, szPath, MAX_PATH);
			lua_pushstring(m_L, szPath);
			lua_rawseti(m_L, -2, nIndex + 1);
		}
		CallFunction(1, nError);
		DragFinish(hDropInfo);
	}
	CDialog::OnDropFiles(hDropInfo);
}

//���ñ���
//view:set_title([string]title);
int CHtmlayoutWDlg::set_title(lua_State* L)
{	
	SetWindowText(PAS(L, 1));
	return 0;
}

//��ñ���
//view:get_title():string
int CHtmlayoutWDlg::get_title(lua_State* L)
{
	CString strText;
	GetWindowText(strText);
	lua_pushstring(L, strText);
	return 1;
}

//�رմ���
//view:close([nCode])
int CHtmlayoutWDlg::close(lua_State* L)
{
	if(m_bModal)
	{
		int nID = IDCANCEL;
		if(lua_toboolean(L, 1))
			nID = IDOK;

		WindowClose();
		EndDialog(nID);
	}
	else
	{
		PostMessage(WM_CLOSE);
	}
	return 0;
}

//�������ڴ�С
//view:resize(x, y);
//view:resize([auto,max,min]);
int CHtmlayoutWDlg::resize(lua_State* L)
{
	if(lua_isnumber(L, 1))
	{
		lua_pushboolean(L, ::SetWindowPos(m_hWnd, 0, 0, 0, PAI(L, 1), PAI(L, 2), SWP_NOMOVE | SWP_NOZORDER));
	}
	else if(lua_isstring(L, 1))
	{
		CString strType = PAS(L, 1);
		if(strType == "auto")
		{
			UINT nWidth = HTMLayoutGetMinWidth(m_hWnd);
			UINT nHeight = HTMLayoutGetMinHeight(m_hWnd, nWidth);
			lua_pushboolean(L, ::SetWindowPos(m_hWnd, 0, 0, 0, nWidth + 20, nHeight + 55, SWP_NOMOVE | SWP_NOZORDER));
		}
		else if(strType == "max")
		{
			lua_pushboolean(L, ShowWindow(SW_SHOWMAXIMIZED));
		}
		else if(strType == "min")
		{
			lua_pushboolean(L, ShowWindow(SW_SHOWMINIMIZED));
		}
	}
	return 1;
}

//��ʾ����
//view:show_window(show_type[int])
int CHtmlayoutWDlg::show_window(lua_State* L)
{
	int nCmd = SW_SHOW;

	if(lua_isnumber(L, 1))
		nCmd = PAI(L, 1);
	else if(lua_isnoneornil(L,1) || lua_toboolean(L, 1))
		nCmd = SW_SHOW;
	else if(lua_isboolean(L, 1) && !lua_toboolean(L, 1))
		nCmd = SW_HIDE;

	lua_pushboolean(L, ShowWindow(nCmd));
	return 1;
}

//�޸Ĵ�������
int CHtmlayoutWDlg::modify_style(lua_State* L)
{					
	lua_pushboolean(L, ModifyStyle(PAI(L, 1), PAI(L, 2), PAI(L, 3)));		
	return 1;
}

//���ô���ͼ��
//view:set_item(icon_file, bBig[bool]);
int CHtmlayoutWDlg::set_icon(lua_State* L)
{			
	HICON hIcon = (HICON)::LoadImage(AfxGetInstanceHandle(), PAS(L, 1), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	if(hIcon == NULL)
		return 0;

	lua_pushboolean(L, SetIcon(hIcon, lua_toboolean(L, 2)) != NULL);
	return 1;
}

//��õ�ǰ���ڴ�С
int CHtmlayoutWDlg::get_size(lua_State* L)
{
	RECT rect;
	GetWindowRect(&rect);
	lua_pushnumber(L, rect.right - rect.left);
	lua_pushnumber(L, rect.bottom - rect.top);
	return 2;
}

//��õ�ǰ��������Ļ�ϵ�λ��
//view:get_pos():x, y
int CHtmlayoutWDlg::get_pos(lua_State* L)
{		
	RECT rect;
	GetWindowRect(&rect);
	POINT pt;
	pt.x = rect.left;
	pt.y = rect.top;
	ClientToScreen(&pt);
	lua_pushnumber(L, rect.left);
	lua_pushnumber(L, rect.top);
	return 2;
}

//�ƶ���ǰ����
//view:move(x, y[,cx, cy]);
int CHtmlayoutWDlg::move(lua_State* L)
{
	UINT uFlag = SWP_NOZORDER;
	if(lua_gettop(L) == 2)
		uFlag |= SWP_NOSIZE;

	SetWindowPos(NULL, PAI(L, 1), PAI(L, 2), PAI(L, 3),PAI(L, 4), uFlag);
	return 0;
}

//���Ҵ�����ĳ�����ϵ�Ԫ��
//view:find_element(x, y):helement
int CHtmlayoutWDlg::find_element(lua_State* L)
{		
	POINT pt;
	pt.x = PAI(L, 1);
	pt.y = PAI(L, 2);
	HELEMENT he;
	if(HLDOM_OK !=  HTMLayoutFindElement(m_hWnd, pt, &he))
		return 0;
	CLuaHtmlayoutElement::PushElement(L, he);
	return 1;
}

//�������ڵ�ǰ���ڵ�λ��
//view:get_cursor_pos(): x, y
int CHtmlayoutWDlg::get_cursor_pos(lua_State* L)
{
	POINT pt;
	if(!GetCursorPos(&pt))
		return 0;

	if(PAI(L, 1))
		ScreenToClient(&pt);

	lua_pushnumber(L, pt.x);
	lua_pushnumber(L, pt.y);
	return 2;
}

int CHtmlayoutWDlg::center(lua_State* L)
{
	CenterWindow();
	return 0;
}

//���ô���͸��
int CHtmlayoutWDlg::transparent(lua_State* L)
{	
	::SetWindowLong(m_hWnd, GWL_EXSTYLE, WS_EX_LAYERED);
	if(PAI(L, 1) < 0)
		return 0;

	::SetLayeredWindowAttributes(m_hWnd, 0, PAI(L, 1), LWA_ALPHA);	
	return 0;
}

int CHtmlayoutWDlg::transparent_color(lua_State* L)
{	
	::SetWindowLong(m_hWnd, GWL_EXSTYLE, WS_EX_LAYERED);
	::SetLayeredWindowAttributes(m_hWnd, RGB(PAI(L, 1), PAI(L, 2), PAI(L, 3)), PAI(L, 1), LWA_COLORKEY);
	return 0;
}

int CHtmlayoutWDlg::set_size_limit(lua_State* L)
{
	m_MinSize.cx = PAI(L, 1);
	m_MinSize.cy = PAI(L, 2);
	m_MaxSize.cx = PAI(L, 3);
	m_MaxSize.cy = PAI(L, 4);
	return 0;
}

int CHtmlayoutWDlg::drag_anywhere(lua_State* L)
{
	m_bDragAnywhere = lua_toboolean(L, 1);
	return 0;
}

int CHtmlayoutWDlg::drag_rect(lua_State* L)
{
	m_DragRect.left = PAI(L, 1);
	m_DragRect.top = PAI(L, 2);
	m_DragRect.right = PAI(L, 3);
	m_DragRect.bottom = PAI(L, 4);
	return 0;
}

int CHtmlayoutWDlg::load_url(lua_State* L)
{
	lua_pushboolean(L, Load(PAS(L, 1)));
	return 1;
}

int CHtmlayoutWDlg::can_drag_file(lua_State* L)
{
	DragAcceptFiles(lua_toboolean(L, 1));
	return 0;
}

int CHtmlayoutWDlg::message_box(lua_State* L)
{
	lua_pushnumber(L, MessageBox(PAS(L, 1), lua_isnoneornil(L, 2) ? 0 : PAS(L, 2),PAI(L, 3)));
	return 1;
}

int CHtmlayoutWDlg::get_hwnd(lua_State* L)
{
	lua_pushnumber(L, (long)GetSafeHwnd());
	return 1;
}

int CHtmlayoutWDlg::register_bind(lua_State* L)
{
	if(lua_istable(L, -1))
	{
		m_nBindViewRef = lua_ref(L, true);
	}
	return 0;
}
int CHtmlayoutWDlg::update(lua_State* L)
{
	this->UpdateWindow();
	return 0;
}

int CHtmlayoutWDlg::set_glass(lua_State* L)
{
#if _MSC_VER >= 0x1500
 	static MARGINS unlimited_margins = {-1};
 	static MARGINS zero_margins = {0};
 	bool _is_glassy = lua_toboolean(L, 1);
 	HRESULT hr = DwmExtendFrameIntoClientArea(GetSafeHwnd(),_is_glassy?&unlimited_margins:&zero_margins); 
 	HTMLayoutSetOption(GetSafeHwnd(), HTMLAYOUT_TRANSPARENT_WINDOW, _is_glassy ? 1 : 0);
#endif
 	return 0;
}

int CHtmlayoutWDlg::set_option(lua_State* L)
{
	HTMLayoutSetOption(m_hWnd, PAI(L, 1), PAI(L, 2));
	return 0;
}

int CHtmlayoutWDlg::get_window_state(lua_State* L)
{
	lua_pushboolean(L, IsIconic());
	lua_pushboolean(L, IsZoomed());
	return 2;
}
