module('ui.view', package.seeall)
require('ui.comm');

local V = {
	__index=function(t, k)
		local obj = rawget(t, 'obj');

		if k == '##' then
			return obj:dosomething();
		end

		local v = rawget(t, k);
		if v  then
			return v;
		end

		local mt  = getmetatable(obj);
		if mt and mt[k] then
			return function(self, ...)									
				local obj = self.obj;
				return obj[k](obj, ...);
			end
		end

		return nil;
	end;
};

function V:new (obj)
	local o = {};
	o['__view_flag__'] = true;	
	o.obj = obj;	
	setmetatable(o, V)
	return o
end

--view 扩展功能
local ExV = {};
function ExV:hide_border()
	self:modify_style(0x00C40000, 0, 0x20);
end

--是否允许改变窗口大小
function ExV:can_resize(can) 
	if can == false then
		self:modify_style(0x00050000, 0, 0x20);
	else
		self:modify_style(0, 0x00050000, 0x20);
	end
end

--是否允许最小化窗口
function ExV:can_minsize(can)
	if can == false then
		self:modify_style(0x00020000, 0, 0x20);
	else
		self:modify_style(0, 0x00020000, 0x20);
	end
end


local view_map = {};
--v 程序中的view对象
--forcecreate 是否强制生成新对象，如果是，会删除老的对象
function CreateView(v, forcecreate)
	local h     = v:get_hwnd();
	local value = view_map[h];
	if  value  then
		if not forcecreate then
			return value;
		end
		view_map[h] = nil;
	end

	local nv = V:new(v);
	local nc  = ui.createClass(ExV, nv);
	value = nc:new();
	value:register_bind(value);
	view_map[h] = value;
	return value;
end

--删除当前view对象v
function DeleteView(v)
	if v == nil then return end;
	view_map[v:get_hwnd()] = nil;
end