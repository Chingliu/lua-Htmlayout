--scintilla�ؼ�������չ
--��װscintilla�ؼ�����

local ex = {};
luaex_scintilla	= ex;	--ȫ�ַ���table

--��ʼ��
function ex:init()
	self:SendMsg(2037, 936);
	self:set_font('����', 11);
	self:set_tab_width(4);
--	self:show_line_number(1, 4);
	
end
--�����ı�
--txt ��Ҫ���õ��ı�����
--return ��
function ex:set_text(txt) 
	self:SendMsg(SCI_SETTEXT or 2181,  0, txt or '');
end

--�����ı�
--pos ��Ҫ�����λ�� -1�ǵ�ǰ��괦
--txt ��Ҫ������ı�
--return ��
function ex:insert_text(pos, txt)
	self:SendMsg(SCI_INSERTTEXT or 2003, pos, txt);
end

--��ʾ��ʾ��Ϣ
function ex:show_tip(pos, tip)
	self:SendMsg(SCI_CALLTIPSHOW or 2200, pos, tip);
end

--������ʾ
function ex:highlight_tip(hStart, hEnd) 
	self:SendMsg(SCI_CALLTIPSETHLT or 2204, hStart, hEnd);
end

function ex:hide_tip()
	self:SendMsg(SCI_CALLTIPCANCEL or 2201);
end
--������ʾǰ��ɫ
--back ����ɫ ��fore ǰ��ɫ highlight����ɫ
function ex:set_tip_color(fore, back, highlight)
	if back then
		self:SendMsg(SCI_CALLTIPSETBACK or 2205, back);
	end
	if fore then
		self:SendMsg(SCI_CALLTIPSETFORE or 2206, fore);
	end
	if highlight then
		self:SendMsg(SCI_CALLTIPSETFOREHLT or 2207, highlight);
	end

end

--����

--��ʾ�Զ����ѡ��
--entered �Ѿ�������ַ���
--list �Զ����ѡ����ѡ��֮���ÿո�ָ�
function ex:autocshow(entered, list) 
	self:SendMsg(SCI_AUTOCSHOW or 2100, entered or 0, list);
end

function ex:isautocshow()
	return self:SendMsg(SCI_AUTOCACTIVE or 2102) ~= 0;
end
--��������
--fontname ��������
--stylenumber �ض��������壬��ָ��ΪĬ������
function ex:set_font(fontname, size, bold, italic,  stylenumber) 
	stylenumber = stylenumber or (STYLE_DEFAULT or 32);
	if fontname then
		self:SendMsg(SCI_STYLESETFONT or 2056, stylenumber, fontname);
	end

	if size then
		self:SendMsg(SCI_STYLESETSIZE or 2055, stylenumber, size);
	end

	if bold then bold = 1; else bold = 0; end
	self:SendMsg(SCI_STYLESETBOLD or 2053, stylenumber, bold);

	if italic then italic = 1; else italic = 0; end
	self:SendMsg(SCI_STYLESETITALIC or 2054, stylenumber, italic);
end

--�﷨����
--set �﷨��������
--����
--[[
{	
	language = 'cpp' or 'lua';	--��������
	keywords = {'if then', ''} or string;		--���Թؼ���,֧�ֶ���ؼ���

	color = {
		[key] = color;
	};
}
]]
local COLOR_CHG = 
{
	['default'] = 0;	--Ĭ��
	['comment'] = 1;	--ע��
	['commentline'] = 2;	--��ע��
	['commentdoc']  = 3;
	['number']  = 4;	--����
	['keyword'] = 5;	--�ؼ���
	['string']  = 6;	--�ַ���
	['character'] = 7;	--�ַ�
	['operator']  = 10;	--������
}
function ex:syntax_light(set)
	self:SendMsg(SCI_SETLEXERLANGUAGE or 4006, 0, set.language);
	if type(set.keywords) == 'table' then
		for k, v in ipairs(set.keywords) do
			self:SendMsg(SCI_SETKEYWORDS or 4005, k - 1, v);	
		end		
	else
		self:SendMsg(SCI_SETKEYWORDS or 4005, 0, set.keywords);	
	end

	--������ɫ
	for k, v in pairs(set.color or {}) do
		local kn = COLOR_CHG[k] or tonumber(k);
		if kn  then
			self:SendMsg(SCI_STYLESETFORE or 2051, kn, v);
		end
	end
end

--����tab���
function ex:set_tab_width(width)
	self:SendMsg(SCI_SETTABWIDTH or 2036 ,width);
end

--��ʾ�к�
--margin ���0-4�������к���ʾ��˳��
--width  �к����ڵĿ�ȣ����ִ����кŵ��ֽ���
function ex:show_line_number(margin, width)
	self:SendMsg(SCI_SETMARGINTYPEN or 2240, margin or 0,  SC_MARGIN_NUMBER or 1);
	self:SendMsg(SCI_SETMARGINMASKN or 2244, margin or 0, bit.rol(1, margin));
	local pixwidth = self:SendMsg(SCI_TEXTWIDTH or 2276, STYLE_LINENUMBER or 33, '_' .. string.rep('9', width));
	self:SendMsg(SCI_SETMARGINWIDTHN or 2242, margin or 0, pixwidth);
end

--����margin����ʾ���
--margin ���
--width  ���
--iconidex ͼ����ʽ
--canclick �Ƿ�����Ӧ�����Ϣ 0-���� 1-��
function ex:set_margin_symbol(margin, width, iconindex, canclick)
	self:SendMsg(SCI_SETMARGINTYPEN  or 2240,  margin, SC_MARGIN_SYMBOL or 0);	
	self:SendMsg(SCI_SETMARGINWIDTHN or 2242, margin, width);
	self:SendMsg(SCI_MARKERDEFINE or 2040, margin, iconindex);
	self:SendMsg(SCI_SETMARGINMASKN or 2244, margin, bit.rol(1, margin));
	self:SendMsg(SCI_SETMARGINSENSITIVEN or 2246, margin, canclick);
end

--����margin�����״
--cursor ȡֵ 2-arrow 4-wait 7-�����ͷ
function ex:set_margin_cursor(margin, cursor)
	self:SendMsg(SCI_SETMARGINCURSORN or 2248, margin, cursor); 
end

--��xpm��ʾmarker
function ex:set_marker_xpm(markernumber, xpmdata)
	self:SendMsg(SCI_MARKERDEFINEPIXMAP or 2049, markernumber, xpmdata);
end
--���ñ����ʾ��ɫ
function ex:margin_color(markernumber, forecolor, backcolor)
	if forecolor then
		self:SendMsg(SCI_MARKERSETFORE or 2041, markernumber, forecolor);
	end

	if backcolor then
		self:SendMsg(SCI_MARKERSETBACK or 2042, markernumber, backcolor);
	end
end

--���õ�line�б��λ
function ex:marker_add(line, markernumber)
	self:SendMsg(SCI_MARKERADD or 2043, line, markernumber);
end

--ɾ����line�б��λ
function ex:marker_delete(line, markernumber)
	self:SendMsg(SCI_MARKERDELETE or 2044, line, markernumber);
end

--�ж�ĳһ��lineָ���ı�־markernumber�Ƿ�ѡ��
function ex:marker_ischecked(line, markernumber)
	local value = self:SendMsg(SCI_MARKERGET or 2046, line);
	local mask  = bit.rol(1, markernumber);
	return bit.band(value, mask) > 0;
end

--�ı�״̬��״̬ת��
function ex:marker_change(line, markernumber)	
	if self:marker_ischecked(line, markernumber) then
		self:marker_delete(line, markernumber);
		return false;
	else
		self:marker_add(line, markernumber);
		return true;
	end
end

--�������ָ���ı�־λ
function ex:marker_clearall(markernumber)
	self:SendMsg(SCI_MARKERDELETEALL or 2045, markernumber);
end

--�����markernumber��ָ����־�������У�����һ��table
function ex:marker_getall(markernumber)
	local t = {};
	local mask = bit.rol(1, markernumber);
	local bg = 0;
	while(true) do
		local line = self:SendMsg(SCI_MARKERNEXT or 2047, bg, mask);
		if line < 0 then
			break;
		end
		table.insert(t, line);
		bg = line + 1;
	end
	return t;
end

--���posλ�����ڵĵ���
function ex:get_word(pos, bWordCharacters)
	local wb = math.min(pos, self:SendMsg(SCI_WORDSTARTPOSITION or 2266, pos, bWordCharacters));
	local we = math.min(pos, self:SendMsg(SCI_WORDENDPOSITION or 2267,   pos, bWordCharacters));

	--print('get_word', pos, wb, we);
	return self:GetTextRange(wb, we), wb, we;
end

--��õ�ǰλ��
function ex:get_current_pos() 
	return self:SendMsg(SCI_GETCURRENTPOS or 2008);
end

--�����Զ�����������
function ex:set_autoc_maxheight(m)
	return self:SendMsg(SCI_AUTOCSETMAXHEIGHT or 2210, m);
end

function ex:set_autoc_maxwidth(m)
	return self:SendMsg(SCI_AUTOCSETMAXWIDTH or 2208, m);
end

--����pos�����
function ex:get_line_from_pos(pos)
	return self:SendMsg(SCI_LINEFROMPOSITION or 2166, pos);
end

--�����л���������ʼλ��
function ex:get_pos_from_line(line) 
	return self:SendMsg(SCI_POSITIONFROMLINE or 2167, line);
end

--��ú�����λ��
function ex:get_endpos_from_line(line)
	return self:SendMsg(SCI_GETLINEENDPOSITION or 2136, line);
end
--��õ�ǰѡ�е��ı�
function ex:get_sel_text()
	local b = self:SendMsg(SCI_GETSELECTIONSTART or 2143);
	local e = self:SendMsg(SCI_GETSELECTIONEND or 2145);

	return self:GetTextRange(b, e);
end

--���������ʾʱ��
function ex:set_mouse_dwell_time(millisec)
	self:SendMsg(SCI_SETMOUSEDWELLTIME or 2264, millisec);
end

--����ָ����
function ex:goto_line(line)
	self:SendMsg(SCI_GOTOLINE or 2024, line);
end

--ѡ���ı�
function ex:select_text(posstart, posend) 
	self:SendMsg(SCI_SETSELECTIONSTART or 2142, posstart);
	self:SendMsg(SCI_SETSELECTIONEND or 2144, posend);
end

--ѡ��ĳ��
function ex:select_line(line)
	local linestart = self:get_pos_from_line(line);
	local lineend   = self:get_endpos_from_line(line);

	self:select_text(linestart, lineend);
end

--���ÿɼ��ĵ�һ��
function ex:set_first_visible_line(line) 
	self:SendMsg(SCI_SETFIRSTVISIBLELINE or 2613, line);
end

--�����Ļ�ϵ���ʾ������
function ex:get_screen_lines()
	return self:SendMsg(SCI_LINESONSCREEN or 2370);
end

--�õ�line������ʾ
function ex:set_center_line(line) 
	local n = self:get_screen_lines();
	local fn = line - math.floor(n/2);
	self:set_first_visible_line(fn);
end

--ע��֪ͨ��Ϣ
--code ֪ͨ��Ϣ����
--�ص�����
function ex:register_notify(code, func)
	if code == nil or type(func) ~= 'function' then
		return;
	end

	self.msg_map = self.msg_map or {};
	self.msg_map[code] = func;
	self:RegisterNotify(code);
end

--��Ϣ֪ͨ
function ex:onNotify(msg)
	local code = msg.code;
	local fn   = self.msg_map[code];
	if fn then
		fn(self, msg);
	end
end
