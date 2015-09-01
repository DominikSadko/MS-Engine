g_event.beginContact = {
	m_data = {} -- f = function, w = widget
}

function onBeginContact(fixtureA, fixtureB)
	for i, v in ipairs(g_event.beginContact.m_data) do
		v.f(fixtureA, fixtureB, v.w)
	end
end

function g_event.beginContact.clear()
	table.clear(g_event.beginContact.m_data)
end

function g_event.beginContact.add(f, widget)
	table.insert(g_event.beginContact.m_data, {f = f, w = widget})
end

function g_event.beginContact.remove(f)
	for i, v in ipairs(g_event.beginContact.m_data) do
		if(v.f == f) then
			table.remove(g_event.beginContact.m_data, i)
			g_event.beginContact.remove(f)
			return
		end
	end
end
