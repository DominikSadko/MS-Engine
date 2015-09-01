newMetatable("TextEdit", "UIWidget")

function TextEdit.Create()
	local self = TextEdit.new()
	self:setMetatable("TextEdit")
	self:setResourceManager(g_resourceManager)
	self:addLayout(g_resourceManager:getLayout("TextEdit"))

	self.maxSize = 0
	return self
end

---   usefull functions   ---

function TextEdit.setMaxSize(self, size)
	self.maxSize = size
end