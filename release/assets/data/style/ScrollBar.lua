newMetatable("ScrollBar", "UIWidget")

function ScrollBar.Create()
	local self = ScrollBar.new()
	self:setMetatable("ScrollBar")
	self:setFocusable(false)
	self:setMovable(false)

	self:setImageSource(g_resourceManager:getImage("gui"))
	self:setSize({ w = 90, h = 8 })
	self:setImageRect({ x = 30, y = 25, w = 80, h = 8 })
	self:setBorder({ x = 2, y = 2, w = 2, h = 2 })

	local indicator = self:doCreateChild()
	indicator:setId("Indicator")
	indicator:setFocusable(false)
	indicator:setPhantom(true)
	indicator:setMovable(false)

	indicator:setImageSource(g_resourceManager:getImage("gui"))
	indicator:setSize({ w = 10, h = 10 })
	indicator:setImageRect({ x = 20, y = 24, w = 10, h = 10 })
	indicator:setBorder({ x = 2, y = 2, w = 2, h = 2 })

	g_event.mouse.add(ScrollBar.onMouse, nil, self)
	g_event.movement.add(ScrollBar.onMouse, nil, self)
	g_event.resizedWindow.add(ScrollBar.onWindowResize, self)

	self:setDirection(Anchor.Left)
	self:setPercent(0, true)

	return self
end

---   event handlers   ---

function ScrollBar.onMouse(self, executed, id, pos)
	if(executed) then
		local indicator = self:getChildById("Indicator")
		local dir = self.direction
		local percent = 0

		if(dir == Anchor.Left) then
			percent = (pos.x - indicator:getWidth()) * 1000 / (self:getWidth() - indicator:getWidth() * 2)
		elseif(dir == Anchor.Top) then
			percent = (pos.y - indicator:getHeight()) * 1000 / (self:getHeight() - indicator:getHeight() * 2)
		elseif(dir == Anchor.Right) then
			percent = (self:getWidth()  - pos.x - indicator:getWidth()) * 1000 / (self:getWidth() - indicator:getWidth() * 2)
		elseif(dir == Anchor.Bottom) then
			percent = (self:getHeight()  - pos.y - indicator:getHeight()) * 1000 / (self:getHeight() - indicator:getHeight() * 2)
		end

		if(self:setPercent(percent)) then
			g_tooltip:show(math.closer(self:getPercent() / 10) .."%", Mouse.getPosition(id))
		end
	end
end

function ScrollBar.onWindowResize(width, height, self)
	self:setPercent(self:getPercent(), true)
end

---   usefull functions   ---

function ScrollBar.getPercent(self)
	return self.percent
end

function ScrollBar.setPercent(self, percent, safe)
	local indicator = self:getChildById("Indicator")
	self.percent = math.min(math.max(math.closer(percent), 0), 1000)

	if(self.direction == Anchor.Left or self.direction == Anchor.Right) then
		indicator:setAnchorMargin(self.direction, (self:getPercent() * (self:getWidth() - indicator:getWidth()) / 1000))
	else
		indicator:setAnchorMargin(self.direction, (self:getPercent() * (self:getHeight() - indicator:getHeight()) / 1000))
	end

	if(not safe and self.onUpdate and self.onUpdate(self, self:getPercent()) == false) then
		return false
	end

	return not safe
end

function ScrollBar.getDirection(self)
	return self.direction
end

function ScrollBar.setDirection(self, direction)
	local indicator = self:getChildById("Indicator")
	indicator:removeAnchors()
	indicator:setAnchor(direction, indicator:getParent(), direction)

	if(direction == Anchor.Left or direction == Anchor.Right) then
		indicator:setAnchor(Anchor.VerticalCenter, indicator:getParent(), Anchor.VerticalCenter)
		indicator:setAnchor(Anchor.Top, indicator:getParent(), Anchor.Top)
		indicator:setAnchor(Anchor.Bottom, indicator:getParent(), Anchor.Bottom)
		indicator:setSize({ w = 10, h = 15 })
	else
		indicator:setAnchor(Anchor.HorizontalCenter, indicator:getParent(), Anchor.HorizontalCenter)
		indicator:setAnchor(Anchor.Left, indicator:getParent(), Anchor.Left)
		indicator:setAnchor(Anchor.Right, indicator:getParent(), Anchor.Right)
		indicator:setSize({ w = 15, h = 10 })
	end

	self.direction = direction
	self:setPercent(self:getPercent() or 0)
end