--����sysģ��
module('ui.sys', package.seeall);

--ȫ�ִ�������
function _G.errormsg(msg)
	print(debug.traceback(string.format('%s �ű�����: %s', os.date('%Y%m%d %H:%M:%S'),msg)));
end

