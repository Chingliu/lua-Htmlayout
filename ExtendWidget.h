#ifndef _EXTEND_CONTROL_H
#define _EXTEND_CONTROL_H
/************************************************************************/
/* ��չ�ؼ�����ͷ�ļ�                                                   */
/************************************************************************/
#include <Luna.h>
#include <string>
#include <map>
#include <htmlayout.h>

class CControlType
{
public:
	virtual const char*   GetTypeName() = 0;
	//���ɿؼ�
	virtual HWND  Create(lua_State* L, HELEMENT h, HWND hParent, void*& pControl) = 0;	

	//���ٿؼ�
	virtual bool  Destroy(void* pControl) = 0;			

	//���º�����pContrl����Create�������ɵĿؼ�

	//���Բ���
	//bGetָ���ǻ�����ԣ�������������
	//bGet = TRUE, �൱��  get(������)
	//bGet = FALSE,�൱��, set(������������ֵ)
	//��������ˣ���ôbHandled��ֵΪTRUE,����ΪFALSE
	virtual int Attribute(void* pControl, lua_State* L, BOOL bGet, BOOL& bHandled) {return 0;}

	//����֧�ֵĺ�������,lua���鷽ʽ
	virtual int GetMethods(void* pControl, lua_State* L){return 0;}

	//���÷��� e:call_ctrl_method(name, arg1, arg2, ...);
	virtual int CallMethod(void* pControl, lua_State* L){return 0;}	//���÷���	

	//��ؼ�ע�����Ϣ
	virtual int BindLuaTable(void* pControl, lua_State* L, int nTableRef) {return 0;}
};

template <typename T, typename CTRLT>
class CControlTypeT : public CControlType
{
public:
	CControlTypeT()
	{
	}	

	CTRLT*  ChangeControlType(void* pContext)
	{
		return (CTRLT*)pContext;
	}

	virtual int GetMethods(void* pControl, lua_State* L)
	{
		CTRLT*  pCtrl = ChangeControlType(pControl);
		if(pCtrl == NULL)
		{			
			return 0;
		}		
		return pCtrl->GetMethods(L);
	}

	//���÷��� e:call_method(name, arg1, arg2, ...);
	virtual int CallMethod(void* pControl, lua_State* L)
	{
		CTRLT*  pCtrl = ChangeControlType(pControl);
		if(pCtrl == NULL)
			return 0;

		if(lua_isnoneornil(L, 1) || PASL(L, 1) <= 0)
			return 0;

		const char* pszName = PAS(L, 1);
		lua_remove(L, 1);

		return pCtrl->CallMethod(pszName, L);
	}
	virtual int BindLuaTable(void* pControl, lua_State* L, int nTableRef)
	{
		CTRLT*  pCtrl = ChangeControlType(pControl);
		if(pCtrl == NULL)
			return 0;
		pCtrl->BindLuaTable(L, nTableRef);
		return 0;
	}
};


//�ؼ���Ϣ֪ͨ�ű�����
//L, nBindTableRef,�ǿؼ���Lua���л�����Ԫ�ض�Ӧ��Lua table
//ͨ��BindLuaTable�������ݸ��ؼ����ؼ���Ҫ����������ֵ
//pszFuncName���ص���������
//�ص������������ص��Ĳ���Ӧ���ڵ��ô˺���֮ǰ��ͨ��lua_pushXXXX��غ�������LuaState��ջ��
inline BOOL SendCallbackMsg(lua_State* L, int nBindTableRef, const char* pszFuncName, int nParams = 0)
{
	if(L == NULL || nBindTableRef == LUA_REFNIL)
		return FALSE;;
	
	int nError = 0;
	int nTop = lua_gettop(L);


	do
	{
		lua_getglobal(L, "errormsg");
		if(!lua_isnil(L, -1))
		{					
			lua_insert(L, nTop - (nParams - 1));
			nTop = lua_gettop(L);
			nError = nTop - nParams;
		}
		else
			lua_remove(L, -1);

		lua_getref(L,	nBindTableRef);
		lua_getfield(L, -1, pszFuncName);
		if(!lua_isfunction(L, -1))
		{
			break;
		}		
		nTop = lua_gettop(L);

		lua_insert(L, nTop - (nParams + 1));
		lua_insert(L, nTop - nParams);
		if(0 == lua_pcall(L, nParams + 1, -1, nError))
		{
			return TRUE;
		}
	}
	while(0);	

	return FALSE;
}

//���ûص�����ֵǮʹ�øú꣬���ܸ������κη���
#define BEGIN_LUA_CALLBACK \
	if(m_L != NULL) \
	{\
		Lua_SaveState ss(m_L);

//�����ص�ʱʹ�øú꣬���ܸ������κη���
#define END_LUA_CALLBACK }

//�ص�ʹ�÷�ʽ
/*
BEGIN_LUA_CALLBACK
//���ò���
lua_pushstring(m_L, ...);
...
//���ú���
if(CallLuaMsg(�ص���������, ��������))
{
	�ص��ɹ��Ĵ���
	...
}
*/

//��չ�ؼ�֧��lua
#define DECLARE_LUA_SUPPORT_CTRL(TClass)\
	lua_State*  m_L;\
	int         m_nBindTableRef;\
	typedef Lunar<TClass>::RegType RegType;\
	static RegType methods[];\
	void InitLuaEnv()\
	{\
		m_L = NULL;\
		m_nBindTableRef = LUA_REFNIL;\
	}\
	int GetMethods(lua_State* L)\
	{\
		lua_newtable(L);\
		int nIndex = 1;\
		for (RegType *l = methods; l->name; l++) \
		{\
			lua_pushstring(L, l->name);\
			lua_rawseti(L, -2, nIndex++);\
		}\
		return 1;\
	}\
	int CallMethod(const char* pszName, lua_State* L)\
	{\
		for (RegType *l = methods; l->name; l++) \
		{\
			if(0 == strcmp(l->name, pszName))\
			{\
				return (this->*(l->mfunc))(L); 			\
			}\
		}\
		return 0;\
	}\
	void BindLuaTable(lua_State* L, int nBindTableRef)\
	{\
		m_L = L;\
		m_nBindTableRef = nBindTableRef;\
	}\
	BOOL CallLuaMsg(const char* pszFuncName, int nParams = 0)\
	{\
		return SendCallbackMsg(m_L, m_nBindTableRef, pszFuncName, nParams);\
	}

#endif //_EXTEND_CONTROL_H