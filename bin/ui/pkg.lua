--�����pkg���м���ҳ�����Դ
module('ui.pkg', package.seeall)
require('ui');
local ui = ui;
ui.saferequire('zip');

local zip = zip;
local open_pkg = nil;
if zip then
	open_pkg = zip.open;
end

--����URL
local function parse_url(url) 
	local _, _, protocol, pkg, path, param = string.find(url, '([%w]+)://([^/]+)/([^?]+)(.*)');
	protocol = string.lower(protocol or '');
	return protocol, pkg, path, param;
end


--��pkg���л���ļ�����
local function get_file_from_pkg(pkg, path) 
	if open_pkg == nil then return ''; end;

	local p = nil;
	local data  = nil;
	while(1) do
		p = open_pkg(mfc.workdir() .. '\\' .. pkg);
		if p == nil then break;	end;
		
		local f = p:open(path);
		if f == nil then break; end;

		data = f:read('*a');
		f:close();
		break;
	end
	if p  then p:close(); p=nil; end

	return data;
end

function LoadData(url)
	if string.find(url, '%%') then
		url = htmlayout.url_unescape(url);
	end

	local protocol, pkg, path = parse_url(url);
	if pkg == nil or path == nil then return end;


	if protocol == 'ftp' then	--��ȡpkg�е��ļ�����
		return get_file_from_pkg(pkg, path);

	elseif protocol == 'file' then	--��ȡ�����ļ�
		local f = io.open(pkg ..'/' .. path, 'rb');
		if f  then
			local data = f:read('*a');
			f:close();
			return data;
		end
	end
	
end