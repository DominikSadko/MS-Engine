g_event.preSolve = {
	m_data = {} -- f = function, w = widget
}

function onPreSolve(fixtureA, fixtureB)
	local deny = true
	for i, v in ipairs(g_event.preSolve.m_data) do
		if(not v.f(fixtureA, fixtureB, v.w)) then
			deny = false
		end
	end

	return deny
end

function g_event.preSolve.clear()
	table.clear(g_event.preSolve.m_data)
end

function g_event.preSolve.add(f, widget)
	table.insert(g_event.preSolve.m_data, {f = f, w = widget})
end

function g_event.preSolve.remove(f)
	for i, v in ipairs(g_event.preSolve.m_data) do
		if(v.f == f) then
			table.remove(g_event.preSolve.m_data, i)
			g_event.preSolve.remove(f)
			return
		end
	end
end
