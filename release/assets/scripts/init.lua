g_resourceManager = nil
g_audioManager = nil
g_keyboard = nil
g_tooltip = nil
g_datatip = nil
g_config = nil

g_event = {}

local function doLoadStyle(name)
	local jsPath = "assets/data/style/".. name ..".json"
	local luaPath = "assets/data/style/".. name ..".lua"
	if(g_fileManager.fileExists(jsPath)) then
		g_resourceManager:addLayout(name, jsPath)
	end

	if(g_fileManager.fileExists(luaPath)) then
		g_fileManager.call(luaPath)
	end
end

function init()
	math.randomseed(getTicks())

	-- Load: basic scripts
	print("*** Loading: basic scripts")
		g_fileManager.call("assets/scripts/class.lua")
		g_fileManager.call("assets/scripts/table.lua")
		g_fileManager.call("assets/scripts/enums.lua")
		g_fileManager.call("assets/scripts/global.lua")

	-- Load: handlers
	print("*** Loading: handlers")
		g_fileManager.call("assets/scripts/event-removeWidget.lua")
		g_fileManager.call("assets/scripts/event-window.lua")
		g_fileManager.call("assets/scripts/event-focus.lua")
		g_fileManager.call("assets/scripts/event-hover.lua")
		g_fileManager.call("assets/scripts/event-mouse.lua")
		g_fileManager.call("assets/scripts/event-keyboard.lua")
		g_fileManager.call("assets/scripts/event-movement.lua")
		g_fileManager.call("assets/scripts/event-slider.lua")
		g_fileManager.call("assets/scripts/event-dragFile.lua")
		g_fileManager.call("assets/scripts/event-beginContact.lua")
		g_fileManager.call("assets/scripts/event-endContact.lua")
		g_fileManager.call("assets/scripts/event-preSolve.lua")
		g_fileManager.call("assets/scripts/event-postSolve.lua")

	-- Load: resources
	print("*** Loading: resources")
		g_audioManager = AudioManager.new()
		g_resourceManager = ResourceManager.new()
		g_config = Config.new("config.dat")

		getRootWidget():setResourceManager(g_resourceManager)
		g_resourceManager:addImage("panel", "assets/data/gui/panel.png")
		g_resourceManager:addImage("panel_side", "assets/data/gui/panel_side.png")
		g_resourceManager:addImage("window", "assets/data/gui/window.png")
		g_resourceManager:addImage("gui", "assets/data/gui/gui.png")
		g_resourceManager:addImage("keyboard", "assets/data/gui/keyboard.png")

		g_resourceManager:addFont("default", "assets/data/fonts/verdana-11px-rounded_cp1250.png", 11)
		g_resourceManager:addFont("verdana_22px", "assets/data/fonts/verdana_22px.png", 22)
		g_resourceManager:addFont("verdana_8px", "assets/data/fonts/verdana_8px.png", 8)
		g_resourceManager:addFont("verdana_10px", "assets/data/fonts/verdana_10px.png", 10)
		g_resourceManager:addFont("arial_22px", "assets/data/fonts/arial_22px.png", 22)
		g_resourceManager:addFont("arial_heavy_22px", "assets/data/fonts/arial_heavy_22px.png", 22)
		g_resourceManager:addFont("sagoe_script_18px", "assets/data/fonts/sagoe_script_18px.png", 18)
		g_resourceManager:addFont("scary_hallowed_18px", "assets/data/fonts/scary_hallowed_18px.png", 18)
		g_resourceManager:addFont("scary_hallowed_22px", "assets/data/fonts/scary_hallowed_22px.png", 22)
		g_resourceManager:addFont("scary_hallowed_30px", "assets/data/fonts/scary_hallowed_30px.png", 30)
		g_resourceManager:addFont("scary_hallowed_40px", "assets/data/fonts/scary_hallowed_40px.png", 40)
		g_resourceManager:addFont("scary_hallowed_60px", "assets/data/fonts/scary_hallowed_60px.png", 60)

		g_resourceManager:addShader("GrayScale", "assets/data/shaders/default.vert", "assets/data/shaders/grayscale.frag")
		g_resourceManager:addShader("Sepia", "assets/data/shaders/default.vert", "assets/data/shaders/sepia.frag")

		g_audioManager:init()
	-- Load: styles
	print("*** Loading: styles")	

		doLoadStyle("Widget")
		doLoadStyle("Text")
		doLoadStyle("Window")
		doLoadStyle("Panel")
		doLoadStyle("MainWindow")
		doLoadStyle("WindowSlider")
		doLoadStyle("Button")
		doLoadStyle("LabelContainer")
		doLoadStyle("CheckBox")
		doLoadStyle("ComboBox")
		doLoadStyle("TextEdit")
		doLoadStyle("ScrollBar")
		doLoadStyle("ProgressBar")
		doLoadStyle("StatusBar")
		doLoadStyle("Tooltip")
		doLoadStyle("Datatip") -- TODO
		doLoadStyle("Keyboard")
		doLoadStyle("MessageBox")
		doLoadStyle("Item")
		g_resourceManager:addLayout("Skin", "assets/module/Skin.json")

		g_keyboard = Keyboard.Create()
		g_tooltip = Tooltip.Create()
		g_datatip = Datatip.Create()

	print("*** Loading: config")
		g_config:load()
		g_audioManager:SetVolume(toauto(g_config:get("volume"), 80))
		g_audioManager:SetMuted(toauto(g_config:get("volumeMute"), false))

	-- Load: modules
	print("*** Loading: modules")
		g_resourceManager:addModule("module_menu", "assets/module/module_menu/menu.lua")

	print("*** Loading:   ===   Done   ===")
	--setDrawDimension({ w = 512, h = 512 })
end

function terminate()
	print("*** Terminating")

	g_config:set("volume", g_audioManager:GetVolume())
	g_config:set("volumeMute", g_audioManager:IsMuted())
	g_config:save()
	g_audioManager:terminate()
	g_audioManager:delete()
	g_resourceManager:delete()
end