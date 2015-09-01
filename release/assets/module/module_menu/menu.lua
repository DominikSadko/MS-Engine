local config = {
	modules = {
		{"module_fps", "assets/module/module_fps/fps.lua"},
		{"module_scene", "assets/module/module_scene/scene.lua"}
	}
}


local MainMenu = nil
local LoadingProgressBar = nil

local function raiserProgressBar()
	local progress = LoadingProgressBar:getProgress()
	if(progress < 100) then
		if(progress == 0) then
			LoadingProgressBar:setVisible(true)
			progress = 90
		end

		LoadingProgressBar:setProgress(progress + 1)
		scheduleEvent(raiserProgressBar, (progress + 1 < 100) and 2 or 500)
	else
		LoadingProgressBar = LoadingProgressBar:delete()

		for i, v in ipairs(config.modules) do
			g_resourceManager:addModule(v[1], v[2])
		end

		Show()
		
		MainMenu:getChildByRecursiveId("PlayGame").click = function()
			Hide()
			
			local scene = UIScene.Create()
			scene:show()
			scene:play()
		end

		MainMenu:getChildByRecursiveId("Exit").click = function()
			closeApplication()
		end
	end
end

function init()
	-- Resource
	g_resourceManager:addImage("gui-2", "assets/module/gui-2.png")
	g_resourceManager:addImage("wallpaper", "assets/data/images/wallpaper.png")
	g_resourceManager:addLayout("Menu", "assets/module/module_menu/menuWindow.json")

	-- MainMenu
	MainMenu = loadUI("Menu", getRootWidget())

	-- LoadingProgressBar
	LoadingProgressBar = ProgressBar.Create()
	LoadingProgressBar:setParent(MainMenu)
	LoadingProgressBar:setHeight(40)
	LoadingProgressBar:setProgressColor({r = 0x64, g = 0x64, b = 0xFF, a = 0xFF})

	LoadingProgressBar:setAnchor(Anchor.Left, LoadingProgressBar:getParent(), Anchor.Left)
	LoadingProgressBar:setAnchor(Anchor.Right, LoadingProgressBar:getParent(), Anchor.Right)
	LoadingProgressBar:setAnchor(Anchor.Bottom, LoadingProgressBar:getParent(), Anchor.Bottom)
	LoadingProgressBar:setAnchorMargin(Anchor.Left, 100)
	LoadingProgressBar:setAnchorMargin(Anchor.Right, 100)
	LoadingProgressBar:setAnchorMargin(Anchor.Bottom, 20)

	LoadingProgressBar:setVisible(false)
	scheduleEvent(raiserProgressBar, 500)
	Hide()
end

function terminate()
	MainMenu = MainMenu:delete()
	g_resourceManager:removeLayout("Menu")
	g_resourceManager:removeImage("gui-2")
	g_resourceManager:removeImage("wallpaper")
end

function getMenu()
	return MainMenu
end

function Show()
	MainMenu:getChildByRecursiveId("PlayGame"):setVisible(true)
	MainMenu:getChildByRecursiveId("TestA"):setVisible(true)
	MainMenu:getChildByRecursiveId("TestB"):setVisible(true)
	MainMenu:getChildByRecursiveId("Exit"):setVisible(true)
end

function Hide()
	MainMenu:getChildByRecursiveId("PlayGame"):setVisible(false)
	MainMenu:getChildByRecursiveId("TestA"):setVisible(false)
	MainMenu:getChildByRecursiveId("TestB"):setVisible(false)
	MainMenu:getChildByRecursiveId("Exit"):setVisible(false)
end