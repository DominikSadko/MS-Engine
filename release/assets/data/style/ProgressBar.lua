newMetatable("ProgressBar", "UIWidget")

function ProgressBar.Create()
	local self = ProgressBar.new()
	self:setMetatable("ProgressBar")
	self:setResourceManager(g_resourceManager)
	self:addLayout(g_resourceManager:getLayout("ProgressBar"))

	self:setProgress(0)
	return self
end

---   usefull functions   ---

function ProgressBar.setProgress(self, percent)
	local progress = self:getChildById("Progress")
	local percent = math.min(math.max(percent, 0), 100)

	progress:setAnchorScale(Anchor.Width, percent)
	self.percent = percent
end

function ProgressBar.getProgress(self)
	return self.percent
end

function ProgressBar.setProgressColor(self, color)
	self:getChildById("Progress"):setImageColor(color)
end