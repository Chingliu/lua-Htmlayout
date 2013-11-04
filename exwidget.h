#ifndef _EXTEND_MANAGER_H
#define _EXTEND_MANAGER_H
/************************************************************************/
/* ��չ�ؼ�����                                                         */
/************************************************************************/
#include <Luna.h>
#include <string>
#include <map>

#include "ExtendWidget.h"
//���º������ó�����ͷ�ļ���
typedef std::map<std::string, CControlType*> CControlTypeMap;

inline CControlTypeMap& GetControlMap()
{
	static CControlTypeMap s_ControlMap;
	return s_ControlMap;
}

inline bool RegisterWidget(CControlType* pControlType)
{
	CControlTypeMap& cm = GetControlMap();
	cm[pControlType->GetTypeName()] = pControlType;
	return true;
}

inline CControlType* GetWidgetType(LPCTSTR lpszTypeName)
{
	CControlTypeMap& cm = GetControlMap();
	CControlTypeMap::iterator iter = cm.find(lpszTypeName);
	if(iter == cm.end())
		return NULL;
	return iter->second;
}

inline HWND CreateWidget(lua_State* L, HELEMENT h, LPCTSTR lpszTypeName, HWND hParent,  void*& pContext)
{
	CControlType* pType = GetWidgetType(lpszTypeName);
	if(pType == NULL)
		return NULL;
	return pType->Create(L, h, hParent, pContext);
}
//
#endif //_EXTEND_MANAGER_H