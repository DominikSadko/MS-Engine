newMetatable("Datatip", "UIWidget")

function Datatip.Create()
	local self = Datatip.new()
	self:setMetatable("Datatip")
	self:setResourceManager(g_resourceManager)
	self:addLayout(g_resourceManager:getLayout("Datatip"))

	self:setParent(getRootWidget())

	-- todo
	--scheduleEvent(function() self:show({{"aa", "bb"}, {"cc", "dd"}}, Mouse.getPosition(1), 2000) end, 2000)
	
	return self
end

function Datatip.show(self, data, pos, time)
	self:removeChilds()

	local font = self:getTextFont()
	
	local size = { w = 128, h = 128 }

	for i, v in ipairs(data) do
		local left = self:doCreateChild()
		local right = self:doCreateChild()

		left:setTextFont(font)
		right:setTextFont(font)

		left:setTextAutoResize(font)
		right:setTextAutoResize(font)

		left:setText(v[1])
		right:setText(v[2])

		left:setAnchor(Anchor.Left, "parent", Anchor.Left)
		right:setAnchor(Anchor.Right, "parent", Anchor.Right)
		right:setAnchor(Anchor.Top, left, Anchor.Top)

		if(i == 1) then
			left:setAnchor(Anchor.Top, "parent", Anchor.Top)
		else
			left:setAnchor(Anchor.Top, "prev", Anchor.Bottom)
			left:setAnchorMargin(Anchor.Top, v.margin or 2)
		end
	end

	self:setWidth(size.w + 24)
	self:setHeight(size.h + 4)
	self:setPosition({ x = pos.x - self:getWidth() / 2, y = pos.y - self:getHeight() / 2})

	if(not self:isVisible()) then
		self:setVisible(true)
	end

	self:setVisible(true)
	getRootWidget():popChild(self)
	
	if(self.event) then
		stopEvent(self.event)
	end

	self.event = scheduleEvent(function() self:setVisible(false) self.event = nil end, time)
end
