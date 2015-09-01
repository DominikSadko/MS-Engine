g_event.keyboard = {
	m_data = {} -- w = widget, f = function, k = key
}

function onKeyPress(key, executed)
	local widget = getFocusedWidget()
	if(not widget) then return end

	for i, v in ipairs(g_event.keyboard.m_data) do
		if((not v.k or key == v.k) and (not v.w or widget:getUniqueId() == v.w:getUniqueId())) then
			v.f(key, executed, widget)
		end
	end
end

function g_event.keyboard.clear()
	table.clear(g_event.keyboard.m_data)
end

function g_event.keyboard.add(f, key, widget)
	table.insert(g_event.keyboard.m_data, {f = f, w = widget, k = key})
end

function g_event.keyboard.remove(f)
	for i, v in ipairs(g_event.keyboard.m_data) do
		if(v.f == f) then
			table.remove(g_event.keyboard.m_data, i)
			g_event.keyboard.remove(f)
			return
		end
	end
end

function g_event.keyboard.removeByWidget(widget)
	for i, v in ipairs(g_event.keyboard.m_data) do
		if((v.w and v.w:getUniqueId() == widget:getUniqueId())
		or (v.p and v.p:getUniqueId() == widget:getUniqueId())) then
			table.remove(g_event.keyboard.m_data, i)
			g_event.keyboard.removeByWidget(widget)
			return
		end
	end
end