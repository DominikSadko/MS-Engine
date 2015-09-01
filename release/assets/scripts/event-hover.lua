g_event.hover = {
	m_data = {} -- w = widget, f = function
}

function onMouseHover(widget, id, hovered)
	for i, v in ipairs(g_event.hover.m_data) do
		if(not v.w or widget:getUniqueId() == v.w:getUniqueId()) then
			v.f(widget, id, hovered)
		end
	end
end

function g_event.hover.clear()
	table.clear(g_event.hover.m_data)
end

function g_event.hover.add(foo, widget)
	table.insert(g_event.hover.m_data, {w = widget, f = foo})
end

function g_event.hover.remove(f)
	for i, v in ipairs(g_event.hover.m_data) do
		if(v.f == f) then
			table.remove(g_event.hover.m_data, i)
			g_event.hover.remove(f)
			return
		end
	end
end

function g_event.hover.removeByWidget(widget)
	for i, v in ipairs(g_event.hover.m_data) do
		if((v.w and v.w:getUniqueId() == widget:getUniqueId())
		or (v.p and v.p:getUniqueId() == widget:getUniqueId())) then
			table.remove(g_event.hover.m_data, i)
			g_event.hover.removeByWidget(widget)
			return
		end
	end
end