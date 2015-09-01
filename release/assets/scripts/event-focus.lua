g_event.focusChange = {
	m_data = {} -- w = widget, f = function, p = parent
}

function onFocusChange(widget, focus)
	for i, v in ipairs(g_event.focusChange.m_data) do
		if(v.p) then
			local parent = widget:getParent()
			if(parent and parent:getUniqueId() == v.p:getUniqueId()) then
				v.f(widget, focus)
			end
		elseif(not v.w or widget:getUniqueId() == v.w:getUniqueId()) then
			v.f(widget, focus)
		end
	end
end

function g_event.focusChange.clear()
	table.clear(g_event.focusChange.m_data)
end

function g_event.focusChange.add(f, widget, parent)
	table.insert(g_event.focusChange.m_data, {f = f, w = widget, p = parent})
end

function g_event.focusChange.remove(f)
	for i, v in ipairs(g_event.focusChange.m_data) do
		if(v.f == f) then
			table.remove(g_event.focusChange.m_data, i)
			g_event.focusChange.remove(f)
			return
		end
	end
end

function g_event.focusChange.removeByWidget(widget)
	for i, v in ipairs(g_event.focusChange.m_data) do
		if((v.w and v.w:getUniqueId() == widget:getUniqueId())
		or (v.p and v.p:getUniqueId() == widget:getUniqueId())) then
			table.remove(g_event.focusChange.m_data, i)
			g_event.focusChange.removeByWidget(widget)
			return
		end
	end
end