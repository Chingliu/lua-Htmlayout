require('mfc');

mfc.register('exctrl.dll');--������չ�ؼ�
mfc.init();	--��ʼ��

local fn = arg[1] or  string.format('file://%s/demo.htm', mfc.workdir());
local w = mfc.NewWindow(fn, true);	--����ģ̬����

