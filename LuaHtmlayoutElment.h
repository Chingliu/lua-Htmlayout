#ifndef _LUA_HTMLAYOUT_ELEMENT_H
#define _LUA_HTMLAYOUT_ELEMENT_H
/************************************************************************/
/* lua��װhtmlayout element�������                                     */
/************************************************************************/

#include <htmlayout.h>
#include <htmlayout_dom.hpp>
#include <aux-cvt.h>
#include <Luna.h>
#include "exwidget.h"

class CLuaHtmlayoutElement
{
public:
	static const char className[];					//����
	static Lunar<CLuaHtmlayoutElement>::RegType methods[];	//��Ҫ�����ĺ���
	
protected:
	HWND  m_hSelfHwnd;				//�Զ���ؼ��Ĵ��ھ��
	void* m_pContext;				//�Զ���ؼ��������ľ��
	CControlType* m_pControlType;	//�Զ��ؼ�������
	int   m_nBindTableRef;			//��ǰԪ�ذ󶨵�lua table����
	htmlayout::dom::element  m_domelement;	//htmlayout�е�Dom��������
	lua_State*	m_L;
	CLuaHtmlayoutElement(){};
public:
	CLuaHtmlayoutElement(HELEMENT h)
		:m_domelement(h)
	{		
		check_widget();
	}

	CLuaHtmlayoutElement(lua_State *L)
		:m_domelement((HELEMENT)PAI(L, 1))
		{
			check_widget();
		};
	
	~CLuaHtmlayoutElement()
	{
		if(m_nBindTableRef != LUA_REFNIL)
		{	
			lua_unref(m_L, m_nBindTableRef);
			m_nBindTableRef = LUA_REFNIL;
		}		
	}

public:
	static void PushElement(lua_State* L, HELEMENT h)
	{
		if(h == NULL)
		{
			lua_pushnil(L);
			return;
		}

		//CLuaHtmlayoutElement* ph = new CLuaHtmlayoutElement(h);
		//Lunar<CLuaHtmlayoutElement>::push(L, ph, true);		
		lua_pushnumber(L, (long)h);
	}

	static CLuaHtmlayoutElement* ToElement(lua_State* L, int nIndex)
	{
		return Lunar<CLuaHtmlayoutElement>::check(L, nIndex);
	}

	static HELEMENT ToElementHanle(lua_State* L, int nIndex)
	{
		return (HELEMENT)PAI(L, nIndex);
	}
public:
	void detach(htmlayout::event_handler* pHandle)
	{
		m_domelement.detach(pHandle);
	}

	//����Զ���ؼ����������
	void check_widget()
	{
		m_hSelfHwnd = NULL;
		m_pContext  = NULL;
		m_pControlType  = NULL;
		m_nBindTableRef = LUA_REFNIL;

		const wchar_t* pValue = m_domelement.get_attribute("_self_hwnd_");
		if(pValue)
			m_hSelfHwnd = (HWND)wcstol(pValue, NULL, 16);

		pValue = m_domelement.get_attribute("_self_widget_");
		if(pValue)
			m_pContext =  (void *)wcstol(pValue, NULL, 16);

		if(m_hSelfHwnd)
		{
			pValue = m_domelement.get_attribute("type");
			m_pControlType = GetWidgetType(aux::w2a(pValue ? pValue : L""));
		}
	}
public:
	//�������ֵ
	//value = e:get_attribute(name)
	//���Բ����ڵ�ʱ�򷵻�nil
	int get_attribute(lua_State* L);

	//��������ֵ
	//e:set_attribute(string name, [string value])
	//value Ϊnilʱ��ɾ������
	int set_attribute(lua_State* L);

	//����Style����
	//e:set_style_attribute(string name, [string value])
	//value Ϊnilʱ��ɾ������
	int set_style_attribute(lua_State* L);

	//���Style����
	//value = e:get_style_attribute(string name)
	//���Բ����ڵ�ʱ�򷵻�nil
	int get_style_attribute(lua_State* L);

	//����domԪ�أ������ҵ��ĵ�һ��
	//PatterString �ο�cssѡ����
	//e:find_element(selector)
	int find_element(lua_State* L);

	//��������ƥ���Ԫ�أ���������table
	//t = e:find_all(selector)
	int find_all(lua_State* L);

	//����Ԫ��ֵ
	//e:set_value(string value)
	int set_value(lua_State* L);

	//���Ԫ��ֵ
	//value = e:get_avlue();
	int get_value(lua_State* L);

	//������Ԫ��
	//ne = e:create_element(string tagName, [string text]);
	int create_element(lua_State* L);

	//����Ԫ��html����
	//e:set_html(string htmlstring, [number mode])
	int set_html(lua_State* L);

	//����Ԫ��html����,utf-8��ʽ
	//e:set_html_utf8(string htmlstring)
	int set_html_utf8(lua_State* L);

	//��ȡԪ��html����
	//e:get_html([boolean bouter]);
	int get_html(lua_State* L);
	int get_html_utf8(lua_State* L);

	//����ǰԪ�غ��ƶ�Ԫ�ؽ���
	//e:swap(e2)
	int swap(lua_State* L);

	//��õ�ǰԪ���ڸ�Ԫ���е����
	//index = e:index()
	int index(lua_State* L);

	//���ظ�Ԫ��
	//he = e:parent();
	int parent(lua_State* L);

	//�ƶ�Ԫ��
	//e:move(x, y, [w], [h]);
	int move(lua_State* L);

	//�����Ԫ�ظ���
	//count = e:children_count();
	int children_count(lua_State* L);

	//��õ�n����Ԫ��,��0��ʼ
	//he = e:child(number n)
	int child(lua_State* L);

	//��һ���ֵ�Ԫ��
	//he = e:next_sibling();
	int next_sibling(lua_State* L);

	//ǰһ���ֵ�Ԫ��
	//he = e:prev_sibling();
	int prev_sibling(lua_State* L);

	//��һ���ֵ�Ԫ��
	//he = e:first_sibling();
	int first_sibling(lua_State* L);

	//���һ���ֵ�Ԫ��
	//he = e:last_sibling();
	int last_sibling(lua_State* L);

	//ɾ����ǰԪ��
	//e:destroy();
	int destroy(lua_State* L);

	//������Ԫ��
	//e:insert(ne, index);
	int insert(lua_State* L);

	//׷��Ԫ��
	//e:append(ne);
	int append(lua_State* L);

	//������ʾԪ��
	//e:show_popup(x, y, banimate);
	int show_popup(lua_State* L);

	//���ص�����ʾ
	//e:hide_popup();
	int hide_popup(lua_State* L);

	//���Ƶ�ǰԪ��
	//ne = e:clone();
	int clone(lua_State* L);

	//����ǰԪ�شӸ�Ԫ��������
	//e:detach()
	int detach(lua_State* L);

	//���غʹ�Ԫ����صĴ���
	int hide_element_hwnd(lua_State* L);

	//����
	//e:update();
	int update(lua_State* L);

	//ˢ��
	//e:refresh([bool bNeedMeasure]);
	int refresh(lua_State* L);

	//����ҳ���ļ�
	// e:load_html(url);
	int load_html(lua_State* L);

	//���Ԫ���µ�HTML����
	// e:clear();
	int clear(lua_State* L);

	//�ж�Ԫ���Ƿ���Ч
	//boo e:is_valid();
	int is_valid(lua_State* L);

	//�����Ա���ʾ��ǰԪ��
	//e:scroll_to_view(bTopOfView = false, bSmooth = false)
	int scroll_to_view(lua_State* L);

	//���ù�����λ��
	//e:set_scroll_pos(x, y);
	int set_scroll_pos(lua_State* L);

	//��ù�������Ϣ
	//x, y = e:get_scroll_info();
	int get_scroll_info(lua_State* L);

	//���Ԫ�ص�λ����Ϣ
	//left,top, right, bottom = e:get_location(type)
	int get_location(lua_State* L);

	//���������Ϣ
	//e:set_capture();
	int set_capture(lua_State* L);

	//�ͷ���겶��
	//e:release_capture();
	int release_capture(lua_State* L);

	//�������ͨ��set_style_attribute���õ�����
	//e:cleare_all_style_attributes();
	int clear_all_style_attributes(lua_State* L);

	//��ø�Ԫ��
	//e:root();
	int root(lua_State* L);

	//����ı�
	//text = e:text();
	int text(lua_State* L);

	//�����ı�
	//e:set_text(txt);
	int set_text(lua_State* L);

	//���Ԫ������
	//t = e:type();
	int get_type(lua_State* L);

	//�ϲ�url
	//url = e:combine_url(url)
	int combine_url(lua_State* L);
	
	//��ȡ״̬
	//bool  e:get_state(state);
	int get_state(lua_State* L);

	//����״̬
	//e:set_state(state, on[off]);
	int set_state(lua_State* L);

	//���Ԫ�ؾ��
	//handle = e:get_handle();
	int get_handle(lua_State* L);

	//��������htmlԪ��
	//e:get_html_element();
	int get_html_element(lua_State* L);

	//������ʱ��
	//e:start_timer(millisecond, [tid])
	int start_timer(lua_State* L);

	//�رն�ʱ��
	//e:stop_timer([tid])
	int stop_timer(lua_State* L);

	//ҳ�����¼���
	//e:reload(url);
	int reload(lua_State* L);

	//��������ķ��������ĸ�Ԫ��
	//he = e:find_nearest_parent(selector);
	int find_nearest_parent(lua_State* L);

	//���ÿؼ�����
	int call_ctrl_method(lua_State* L);

	//��ÿؼ��ķ���
	int get_ctrl_methods(lua_State* L);

	//��ÿؼ�����
	int get_ctrl_attribute(lua_State* L);

	//���ÿؼ�����
	int set_ctrl_attribute(lua_State* L);

	//����post��Ϣ
	int post_event(lua_State* L);

	//����send��Ϣ
	int send_event(lua_State* L);

	//�����ע��󶨵�lua table��Ϣ
	//e:register_bind_table(t);
	int register_bind_table(lua_State* L)
	{
		if(!lua_istable(L, 1))
			return 0;

		m_L = L;
		if(m_nBindTableRef != LUA_REFNIL)
		{
			lua_unref(L, m_nBindTableRef);
			m_nBindTableRef = LUA_REFNIL;
		}

		m_nBindTableRef = lua_ref(L, true);
		if(m_nBindTableRef != LUA_REFNIL && m_pControlType && m_pContext)
		{
			m_pControlType->BindLuaTable(m_pContext, L, m_nBindTableRef);
		}
		return 0;	
	}

	//�����������
	int get_uid(lua_State* L)
	{
		static long sNO = 1;
		InterlockedIncrement(&sNO);
		lua_pushnumber(L, sNO);
		return 1;
	}

	//ɾ���Զ��崰��
	int destroy_ctrl(lua_State* L)
	{
		if(m_pControlType == NULL || m_pContext == NULL)
			return 0;
		
		lua_pushboolean(L, m_pControlType->Destroy(m_pContext));
		return 1;
	}

	//����������ھ��
	int get_root_hwnd(lua_State* L)
	{
		lua_pushnumber(L, (long)m_domelement.get_element_hwnd(true));
		return 1;
	}

	//�жϵ�ǰԪ���Ƿ�ɼ�
	//he:visible():bool
	int is_visible(lua_State* L)
	{
		lua_pushboolean(L, m_domelement.visible());
		return 1;
	}

	//
	int is_enabled(lua_State* L)
	{
		lua_pushboolean(L, m_domelement.enabled());		
		return 1;
	}

	int scroll_set_values(lua_State* L)
	{
		htmlayout::dom::scrollbar s = m_domelement;
		s.set_values(PAI(L, 1), PAI(L, 2),PAI(L, 3),PAI(L, 4),PAI(L, 5));
		return 0;
	}

	int scroll_get_values(lua_State* L)
	{
		htmlayout::dom::scrollbar s = m_domelement;
		int  v[5];
		s.get_values(v[0], v[1], v[2], v[3], v[4]);
		lua_pushnumber(L, v[0]);
		lua_pushnumber(L, v[1]);
		lua_pushnumber(L, v[2]);
		lua_pushnumber(L, v[3]);
		lua_pushnumber(L, v[4]);
		return 5;
	}

	int scroll_set_value(lua_State* L)
	{
		htmlayout::dom::scrollbar s = m_domelement;
		s.set_value(PAI(L, 1));
		return 0;
	}

	int scroll_get_value(lua_State* L)
	{
		htmlayout::dom::scrollbar s = m_domelement;
		lua_pushnumber(L, s.get_value());
		return 1;
	}

	//����Ԫ������
	int sort(lua_State* L);

	//������д�name���Ե���Ԫ�ص�ֵ
	int get_child_values(lua_State* L)
	{
		htmlayout::named_values vals;
		if(!htmlayout::get_values(m_domelement, vals))
			return 0;

		lua_newtable(L);
		htmlayout::named_values_iterator iter = vals.begin();
		while(iter != vals.end())
		{
			lua_pushstring(L, aux::w2a(iter->second.to_string().c_str()));
			lua_setfield(L, -2, aux::w2a(iter->first.c_str()));
			++iter;
		}
		return 1;
	}

	json::value  StringToDate(const char* pszValue)
	{
		int wYear  = 2011;
		int wMonth = 12;
		int wDay   = 7;
		int wHour  = 14;
		int wMin   = 0;
		int wSecond = 0;

		bool bDate = false;
		bool bTime = false;
		do
		{
			if(sscanf(pszValue, "%4d-%2d-%2d", &wYear, &wMonth, &wDay) == 3)
			{
				bDate = true;
				break;
			}

			if(sscanf(pszValue, "%4d%2d%2d", &wYear, &wMonth, &wDay) == 3)
			{
				bDate = true;
				break;
			}

			if(sscanf(pszValue, "%2d:%2d:%2d", &wHour, &wMin, &wSecond) == 3)
			{
				bTime = true;
				break;
			}

			if(sscanf(pszValue, "%2d:%2d", &wHour, &wMin) == 2)
			{
				bTime = true;
				break;
			}

		}while(0);
		SYSTEMTIME st;
		FILETIME   ft;
		GetSystemTime(&st);
		if(bDate)
		{
			st.wYear = wYear;
			st.wMonth = wMonth;
			st.wDay = wDay;
		}
		if(bTime)
		{
			st.wHour = wHour;
			st.wMinute = wMin;
			st.wSecond = wSecond;
		}
		SystemTimeToFileTime(&st, &ft);
		return json::value::date(ft);
	}

	//����Ԫ��ֵ����ͬ���ƿ���ͬʱ����
	inline void _set_value(const char* pszName, const char* pszValue)
	{
		char szSelector[256] = {0};
		_snprintf(szSelector, sizeof(szSelector), "[name='%s']", pszName);

		json::value value = json::value::from_string((const wchar_t*)aux::a2w(pszValue));			

		htmlayout::selected_cb selected;
		m_domelement.find_all(&selected, szSelector);

		bool bCheckedSet = false;
		bool bRadioSet   = false;
		for( unsigned int n = 0; n < selected.elements.size(); ++n )
		{			
			htmlayout::dom::element& t = selected.elements[n];
			if( !t.get_style_attribute("behavior") )
			{
				const wchar_t* pszData = aux::a2w(pszValue);
				aux::w2utf utf8(pszData);
				t.set_html( utf8, utf8.length() );				
				continue;
			}
			CTL_TYPE ty = t.get_ctl_type();
			if(ty == CTL_CHECKBOX)
			{
				if(bCheckedSet)
					continue;
				bCheckedSet = true;
			}
			else if(ty == CTL_RADIO)
			{
				if(bRadioSet)
					continue;
				bRadioSet = true;
			}	
			else if(ty == CTL_DATE || ty == CTL_TIME)
			{				
				htmlayout::set_value(t, StringToDate(pszValue));
				continue;
			}
			htmlayout::set_value(t, value);
		}
		return ;
	}
	int set_child_values(lua_State* L)
	{					
		int nIndex = 0;
		lua_pushnil(L);  /* first key */
		while (lua_next(L, 1) != 0) {
			_set_value(PAS(L, -2), PAS(L,-1));
			lua_pop(L, 1);  /* removes `value'; keeps `key' for next iteration */
		}
		lua_pop(L, 1);		
		return 0;
	}

	//����Ԫ�ط���
	int xcall(lua_State* L);

	//�첽����http����
	int http_request(lua_State* L);
};
#endif
