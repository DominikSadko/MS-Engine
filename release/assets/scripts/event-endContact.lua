g_event.endContact = {
	m_data = {} --f = function, w = widget
}

function onEndContact(fixtureA, fixtureB)
	for i, v in ipairs(g_event.endContact.m_data) do
		v.f(fixtureA, fixtureB, v.w)
	end
end

function g_event.endContact.clear()
	table.clear(g_event.endContact.m_data)
end

function g_event.endContact.add(f, widget)
	table.insert(g_event.endContact.m_data, {f = f, w = widget})
end

function g_event.endContact.remove(f)
	for i, v in ipairs(g_event.endContact.m_data) do
		if(v.f == f) then
			table.remove(g_event.endContact.m_data, i)
			g_event.endContact.remove(f)
			return
		end
	end
end
