#ifndef _EXCTRL_WRAPPERWND_H
#define _EXCTRL_WRAPPERWND_H
/************************************************************************/
/* ��չ�ؼ���װ����                                                      */
/* �ṩ��������Ҫ������Ϣ�Ĵ�����������ʹ��								*/
/* Date:    2012-03-05													*/
/************************************************************************/

#define EXCTRL_WRAPPED_HWND  "ExCtrlWrappedWindow"
class CExctrlWrappedWnd: public CWnd
{
public:
	CExctrlWrappedWnd(){};
	~CExctrlWrappedWnd(void){};
	
public:
	static BOOL		Register()
	{
		HINSTANCE hInstance;
		hInstance = AfxGetInstanceHandle();
		WNDCLASS wc;	

		if (!(::GetClassInfo(hInstance, EXCTRL_WRAPPED_HWND, &wc)))  {
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
			wc.hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
			wc.hIcon   = NULL;
			wc.hInstance = hInstance;
			wc.lpfnWndProc = ::DefWindowProc;
			wc.lpszClassName = EXCTRL_WRAPPED_HWND;
			wc.lpszMenuName = NULL;
			wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;

			if (!AfxRegisterClass(&wc))  {			
				return FALSE;
			}
		}
		return TRUE;
	}

	//���ɿؼ�ID
	static long    GetCtrlID()
	{
		static long nCtrlID = 1999;
		InterlockedIncrement(&nCtrlID);
		return nCtrlID;
	}

	//���ɰ�װ����
	virtual BOOL	Create(HWND hParent)
	{
		BOOL bCreated = CWnd::Create(
			EXCTRL_WRAPPED_HWND,
			_T(""),
			WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,
			CRect(0, 0, 0, 0),
			CWnd::FromHandle(hParent),
			GetCtrlID()
			);
		return bCreated;
	}

	//ɾ������
	virtual void Destroy()
	{
		this->DestroyWindow();
		delete this;
	}
};
#endif //_EXCTRL_WRAPPERWND_H