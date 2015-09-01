g_event.removeWidget = {
	m_data = {} -- w = widget, f = function
}

function onWidgetRemove(widget)
	for i, v in ipairs(g_event.removeWidget.m_data) do
		if(not v.w or widget:getUniqueId() == v.w:getUniqueId()) then
			v.f(widget)
		end
	end

	g_event.removeWidget.removeByWidget(widget)
	g_event.resizedWindow.removeByWidget(widget)
	g_event.focusChange.removeByWidget(widget)
	g_event.hover.removeByWidget(widget)
	g_event.mouse.removeByWidget(widget)
	g_event.movement.removeByWidget(widget)
	g_event.slider.removeByWidget(widget)
	g_event.dragFile.removeByWidget(widget)
	g_event.keyboard.removeByWidget(widget)
end

function g_event.removeWidget.clear()
	table.clear(g_event.removeWidget.m_data)
end

function g_event.removeWidget.add(f, widget)
	table.insert(g_event.removeWidget.m_data, {f = f, w = widget})
end

function g_event.removeWidget.remove(f)
	for i, v in ipairs(g_event.removeWidget.m_data) do
		if(v.f == f) then
			table.remove(g_event.removeWidget.m_data, i)
			g_event.removeWidget.remove(f)
			return
		end
	end
end

function g_event.removeWidget.removeByWidget(widget)
	for i, v in ipairs(g_event.removeWidget.m_data) do
		if((v.w and v.w:getUniqueId() == widget:getUniqueId())
		or (v.p and v.p:getUniqueId() == widget:getUniqueId())) then
			table.remove(g_event.removeWidget.m_data, i)
			g_event.removeWidget.removeByWidget(widget)
			return
		end
	end
end