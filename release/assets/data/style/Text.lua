newMetatable("Text", "UIWidget")

function Text.Create()
	local self = Text.new()
	self:setMetatable("Text")
	self:setResourceManager(g_resourceManager)
	self:addLayout(g_resourceManager:getLayout("Text"))
	return self
end