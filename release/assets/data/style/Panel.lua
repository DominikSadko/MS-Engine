newMetatable("Panel", "UIWidget")

function Panel.Create()
	local self = Panel.new()
	self:setMetatable("Panel")
	self:setResourceManager(g_resourceManager)
	self:addLayout(g_resourceManager:getLayout("Panel"))
	return self
end