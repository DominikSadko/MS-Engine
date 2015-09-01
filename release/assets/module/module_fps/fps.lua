local Fps = nil

function updateFps()
	Fps:setText("Fps: ".. getFps())
	scheduleEvent(updateFps, 1000)
end

function init()
	Fps = UIWidget.new()
	Fps:setParent(module_menu.getMenu())

	Fps:setTextAutoResize(true)
	Fps:setPhantom(true)
	Fps:setTextFont(g_resourceManager:getFont("default"))

	Fps:setAnchor(Anchor.Left, Fps:getParent(), Anchor.Left)
	Fps:setAnchor(Anchor.Top, Fps:getParent(), Anchor.Top)

	scheduleEvent(updateFps, 1000)
end

function terminate()
	Fps:delete()
end