function init()
	g_resourceManager:addImage("Asteroid", "assets/module/images/Asteroid.png")
	g_resourceManager:addImage("Ship", "assets/module/images/redfighter0006.png")
	g_resourceManager:addImage("Planet", "assets/module/images/Planet.png")
	g_resourceManager:addLayout("Scene", "assets/module/module_scene/scene.json")
end

function terminate()
	g_resourceManager:removeImage("Asteroid")
	g_resourceManager:removeImage("Ship")
	g_resourceManager:removeImage("Planet")
	g_resourceManager:removeLayout("Scene")
end

function UIScene.Create()
	local self = UIScene.new()
	self:setParent(module_menu.getMenu())
	self:setResourceManager(g_resourceManager)
	self:addLayout(g_resourceManager:getLayout("Scene"))
	self:setScale(50)

	g_event.beginContact.add(UIScene.onBeginContact, self)
	g_event.endContact.add(UIScene.onEndContact, self)
	g_event.preSolve.add(UIScene.onPreSolve, self)
	g_event.postSolve.add(UIScene.onPostSolve, self)
	self:getChildByRecursiveId("ExitGame").click = function() self:onExitGame() end

	return self
end

function UIScene.show(self)
	self:setVisible(true)
	self:focus()
end

function UIScene.remove(self)
	self:sleep()
	self:delete()
end

function UIScene.onExitGame(self)
	self:remove()
	module_menu.Show()
end

function UIScene.onBeginContact(fixtureA, fixtureB, self)
	local bodyA = fixtureA:getBody()
	local bodyB = fixtureB:getBody()
end

function UIScene.onEndContact(fixtureA, fixtureB, self)
	local bodyA = fixtureA:getBody()
	local bodyB = fixtureB:getBody()
end

function UIScene.onPreSolve(fixtureA, fixtureB, self)
	return true
end

function UIScene.onPostSolve(fixtureA, fixtureB, self)

end

function UIScene.play(self)
	self:getChildById("TimeText"):setStartTime(getTicks())
	
	local ship = Body.new(self)
	ship:setImage(g_resourceManager:getImage("Ship"))

	local size = {x = 0, y = 0, w = 343, h = 383}
	local fixture = Fixture:Square(ship, {w = 256, h = 256})
	fixture:setFriction(500)
	fixture:setRestitution(10)
	fixture:setDensity(100)
	fixture:setImageCoords({
				size.x,					size.y,
				size.x + size.w,		size.y,
				size.x + size.w,		size.y + size.h,
				size.x,					size.y + size.h})


	self:setCamera(ship)
end