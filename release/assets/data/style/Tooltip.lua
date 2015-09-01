newMetatable("Tooltip", "UIWidget")

function Tooltip.Create()
	local self = Tooltip.new()
	self:setMetatable("Tooltip")
	self:setResourceManager(g_resourceManager)
	self:addLayout(g_resourceManager:getLayout("Tooltip"))

	self:setParent(getRootWidget())

	return self
end

function Tooltip.show(self, text, pos, time)
	if(not pos) then
		pos = Mouse.getPosition(1)
	end

	local font = self:getTextFont()
	local size = font:getTextSize(text)

	self:setWidth(size.w + 24)
	self:setHeight(size.h + 4)
	self:setPosition({ x = pos.x - self:getWidth(), y = pos.y - self:getHeight()})
	self:setText(text)

	if(not self:isVisible()) then
		self:setVisible(true)
	end

	self:setOpacity(0xFF)

	getRootWidget():popChild(self)
	Condition.FadeOut(self, 5, 25)
	Condition.Move(self, { x = 0, y = -1 }, 30, 16)
end