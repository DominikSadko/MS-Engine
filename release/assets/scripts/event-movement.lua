g_event.movement = {
	m_data = {} -- w = widget, k = key, f = function, p = for parent
}

function onMouseMove(widget, executed, key, pos)
	for i, v in ipairs(g_event.movement.m_data) do
		-- todo: check key
		if(v.p) then
			local parent = widget:getParent()
			if(parent and parent:getUniqueId() == v.p:getUniqueId()) then
				v.f(parent, executed, key, pos)
			end
		elseif(not v.w or widget:getUniqueId() == v.w:getUniqueId()) then
			v.f(widget, executed, key, pos)
		end
	end
end

function g_event.movement.clear()
	table.clear(g_event.movement.m_data)
end

function g_event.movement.add(f, key, widget, parent)
	table.insert(g_event.movement.m_data, {w = widget, k = key, f = f, p = parent})
end

function g_event.movement.remove(f)
	for i, v in ipairs(g_event.movement.m_data) do
		if(v.f == f) then
			table.remove(g_event.movement.m_data, i)
			g_event.movement.remove(f)
			return
		end
	end
end

function g_event.movement.removeByWidget(widget)
	for i, v in ipairs(g_event.movement.m_data) do
		if((v.w and v.w:getUniqueId() == widget:getUniqueId())
		or (v.p and v.p:getUniqueId() == widget:getUniqueId())) then
			table.remove(g_event.movement.m_data, i)
			g_event.movement.removeByWidget(widget)
			return
		end
	end
end