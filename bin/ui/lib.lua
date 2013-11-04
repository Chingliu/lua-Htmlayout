require('ui');

--控件消息中转处理
function onReceiveViewMsg(view, msg, msgtxt)
	view = ui.view.CreateView(view);

	if msg == "open_url" then
		view:load_url(msgtxt);
		return ;
	end

	local f = view.onReceiveMessage;
	if f then
		f(view, msg, msgtxt);
	end
end


print = printf or print;