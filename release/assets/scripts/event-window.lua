g_event.resizedWindow = {
	m_data = {} -- w = widget, f = function
}

function onWindowResize(w, h)
	for i, v in ipairs(g_event.resizedWindow.m_data) do
		v.f(w, h, v.w)
	end
end

function g_event.resizedWindow.clear()
	table.clear(g_event.resizedWindow.m_data)
end

function g_event.resizedWindow.add(f, w)
	table.insert(g_event.resizedWindow.m_data, {f = f, w = w})
end

function g_event.resizedWindow.remove(f)
	for i, v in ipairs(g_event.resizedWindow.m_data) do
		if(v.f == f) then
			table.remove(g_event.resizedWindow.m_data, i)
			g_event.resizedWindow.remove(f)
			return
		end
	end
end

function g_event.resizedWindow.removeByWidget(widget)
	for i, v in ipairs(g_event.resizedWindow.m_data) do
		if((v.w and v.w:getUniqueId() == widget:getUniqueId())
		or (v.p and v.p:getUniqueId() == widget:getUniqueId())) then
			table.remove(g_event.resizedWindow.m_data, i)
			g_event.resizedWindow.removeByWidget(widget)
			return
		end
	end
end