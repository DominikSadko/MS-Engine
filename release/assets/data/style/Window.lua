newMetatable("Window", "UIWidget")

function Window.Create()
	local self = Window.new()
	self:setMetatable("Window")
	self:setResourceManager(g_resourceManager)
	self:addLayout(g_resourceManager:getLayout("Window"))
	return self
end

function Window.onJson(self, json)
	local resource = self:getResourceManager()
	local node = nil

	node = toauto(Json.toString(json, "title"))
	if(node) then self:setTitle(node) end
end

---   usefull functions   ---

function Window.setTitle(self, text)
	self:getChildById("Title"):setText(text)
end

function Window.setTitleColor(self, color)
	self:getChildById("Title"):setTextColor(color)
end