g_event.slider = {
	m_data = {} -- w = widget, k = key, f = function
}

function onMouseSlide(widget, executed, key, pos)
	if(not executed) then return end

	local mousePosition = Mouse.getPosition(key)
	local clickPosition = Mouse.getMouseClickPosition(key)
	local distance = getDistanceBetween(mousePosition, clickPosition)
	local angle = getAngleBetweenVectors(clickPosition, mousePosition)

	for i, v in ipairs(g_event.slider.m_data) do
		-- todo: check key
		if(not v.w or widget:getUniqueId() == v.w:getUniqueId()) then
			v.f(widget, angle, distance, key)
		end
	end
end

function g_event.slider.clear()
	table.clear(g_event.slider.m_data)
end

function g_event.slider.add(f, key, widget, parent)
	table.insert(g_event.slider.m_data, {w = parent or widget, k = key, f = f})
	g_event.movement.add(onMouseSlide, key, widget, parent)
end

function g_event.slider.remove(f)
	for i, v in ipairs(g_event.slider.m_data) do
		if(v.f == f) then
			table.remove(g_event.slider.m_data, i)
			g_event.slider.remove(f)
			g_event.movement.remove(f)
			return
		end
	end
end

function g_event.slider.removeByWidget(widget)
	for i, v in ipairs(g_event.slider.m_data) do
		if((v.w and v.w:getUniqueId() == widget:getUniqueId())
		or (v.p and v.p:getUniqueId() == widget:getUniqueId())) then
			table.remove(g_event.slider.m_data, i)
			g_event.slider.removeByWidget(widget)
			return
		end
	end
end