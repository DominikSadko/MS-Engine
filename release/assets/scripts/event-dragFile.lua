g_event.dragFile = {
	m_data = {} -- f = function, w = widget, p = parent
}

function onDragFile(path, widget)
	for i, v in ipairs(g_event.dragFile.m_data) do
		if(v.p) then
			local parent = widget:getParent()
			if(parent and parent:getUniqueId() == v.p:getUniqueId()) then
				v.f(path, parent)
			end
		elseif(not v.w or widget:getUniqueId() == v.w:getUniqueId()) then
			v.f(path, widget)
		end
	end
end

function g_event.dragFile.clear()
	table.clear(g_event.dragFile.m_data)
end

function g_event.dragFile.add(f, widget, parent)
	table.insert(g_event.dragFile.m_data, {f = f, w = widget})
end

function g_event.dragFile.remove(f)
	for i, v in ipairs(g_event.dragFile.m_data) do
		if(v.f == f) then
			table.remove(g_event.dragFile.m_data, i)
			g_event.dragFile.remove(f)
			return
		end
	end
end

function g_event.dragFile.removeByWidget(widget)
	for i, v in ipairs(g_event.dragFile.m_data) do
		if((v.w and v.w:getUniqueId() == widget:getUniqueId())
		or (v.p and v.p:getUniqueId() == widget:getUniqueId())) then
			table.remove(g_event.dragFile.m_data, i)
			g_event.dragFile.removeByWidget(widget)
			return
		end
	end
end