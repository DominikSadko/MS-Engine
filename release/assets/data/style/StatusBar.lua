newMetatable("StatusBar", "UIWidget")

function StatusBar.Create()
	local self = StatusBar.new()
	self:setMetatable("StatusBar")
	self:setResourceManager(g_resourceManager)
	self:addLayout(g_resourceManager:getLayout("StatusBar"))

	self:setProgress(0)
	return self
end

---   usefull functions   ---

function StatusBar.setProgress(self, percent)
	local progress = self:getChildById("Progress")
	local percent = math.min(math.max(percent, 0), 100)

	progress:setAnchorScale(Anchor.Width, percent)
	self.percent = percent
end

function StatusBar.getProgress(self)
	return self.percent
end

function StatusBar.setProgressColor(self, color)
	self:getChildById("Progress"):setImageColor(color)
end