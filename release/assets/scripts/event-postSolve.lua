g_event.postSolve = {
	m_data = {} -- f = function, w = widget
}

function onPostSolve(fixtureA, fixtureB)
	for i, v in ipairs(g_event.postSolve.m_data) do
		v.f(fixtureA, fixtureB, v.w)
	end
end

function g_event.postSolve.clear()
	table.clear(g_event.postSolve.m_data)
end

function g_event.postSolve.add(f, widget)
	table.insert(g_event.postSolve.m_data, {f = f, w = widget})
end

function g_event.postSolve.remove(f)
	for i, v in ipairs(g_event.postSolve.m_data) do
		if(v.f == f) then
			table.remove(g_event.postSolve.m_data, i)
			g_event.postSolve.remove(f)
			return
		end
	end
end
