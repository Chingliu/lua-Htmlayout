module('ui.util', package.seeall);

--������ת����table�б�
--���������table����ôֱ�ӷ��أ�����
--��������table�ĵ�һ��������
function get_param_list(param)
	local t = nil;
	if type(param) == 'table' then
		t = param;
	else
		t = {};
		table.insert(t, param);
	end
	return t;
end
