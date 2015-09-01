newMetatable("MainWindow", "UIWidget")

local config =
{
	headerSize = 22,
	sliderSize = 16,
	border = 10,
}

function MainWindow.Create()
	local self = MainWindow.new()
	self:setMetatable("MainWindow")
	self:setResourceManager(g_resourceManager)
	self:addLayout(g_resourceManager:getLayout("MainWindow"))

	self:getChildById("moveBarBack"):setSize({ w = config.sliderSize, h = config.sliderSize})
	self:getChildById("moveBarNext"):setSize({ w = config.sliderSize, h = config.sliderSize})

	local moveBar = self:getChildById("moveBar")
	moveBar:setWidth(config.sliderSize)

	local indicator = moveBar:getChildById("Indicator")
	indicator:setImageSource(g_resourceManager:getImage("gui"))
	indicator:setImageRect({ x = 0, y = 124, w = 12, h = 12 })
	indicator:setBorder({ x = 1, y = 1, w = 1, h = 1 })
	indicator:setSize({ w = config.sliderSize, h = config.sliderSize })

	moveBar.onUpdate = MainWindow.onUpdate

	self:setDirection(Anchor.Top, Anchor.Right)
	return self
end

---   event handlers   ---

function MainWindow.onUpdate(self, percent)
	self = self:getParent()

	local widgetsContainer = self:getChildById("VirtualContainer"):getChildById("WidgetsContainer")
	widgetsContainer:setAnchorMargin(self.direction, -percent)
	return true
end

---   usefull functions   ---

function MainWindow.setTitle(self, text)
	self:getChildById("Title"):setText(text)
end

function MainWindow.setTitleColor(self, color)
	self:getChildById("Title"):setTextColor(color)
end

function MainWindow.setDirection(self, direction, barSide)
	local moveBarBack 		= self:getChildById("moveBarBack")
	local moveBarNext 		= self:getChildById("moveBarNext")
	local moveBar 			= self:getChildById("moveBar")
	local indicator			= moveBar:getChildById("Indicator")
	local virtualContainer  = self:getChildById("VirtualContainer")

	moveBarBack:removeAnchors()
	moveBarNext:removeAnchors()
	moveBarNext:removeAnchors()
	indicator:removeAnchors()
	
	self.direction = direction
	if(direction == Anchor.Top or direction == Anchor.Bottom) then
		moveBarBack:setAnchor(Anchor.Top, self, Anchor.Top)
		moveBarBack:setAnchorMargin(Anchor.Top, config.headerSize)
		moveBarBack:setRotation(0)

		moveBarNext:setAnchor(Anchor.Right, moveBarBack, Anchor.Right)
		moveBarNext:setAnchor(Anchor.Bottom, self, Anchor.Bottom)
		moveBarNext:setRotation(180)

		moveBar:setAnchor(Anchor.Right, self, Anchor.Right)
		moveBar:setAnchor(Anchor.Top, moveBarBack, Anchor.Bottom)
		moveBar:setAnchor(Anchor.Bottom, moveBarNext, Anchor.Top)
		moveBar:setAnchor(Anchor.Left, moveBarBack, Anchor.Left)
		moveBar:setAnchor(Anchor.Right, moveBarBack, Anchor.Right)
		moveBar:setDirection(direction)

		indicator:setAnchor(Anchor.Right, moveBar, Anchor.Right)
		indicator:setAnchor(Anchor.Left, moveBar, Anchor.Left)
		indicator:setAnchor(Anchor.Top, moveBar, Anchor.Top)

		virtualContainer:setAnchorMargin(Anchor.Top, config.headerSize + config.border)
		virtualContainer:setAnchorMargin(Anchor.Bottom, config.border)

		if(barSide == Anchor.Left) then
			moveBarBack:setAnchor(Anchor.Left, self, Anchor.Left)
			virtualContainer:setAnchorMargin(Anchor.Left, moveBar:getWidth())
			virtualContainer:setAnchorMargin(Anchor.Right, config.border)
		else
			moveBarBack:setAnchor(Anchor.Right, self, Anchor.Right)
			virtualContainer:setAnchorMargin(Anchor.Left, config.border)
			virtualContainer:setAnchorMargin(Anchor.Right, moveBar:getWidth())
		end

	elseif(direction == Anchor.Left or direction == Anchor.Right) then
		moveBarBack:setAnchor(Anchor.Left, self, Anchor.Left)
		moveBarBack:setRotation(270)

		moveBarNext:setAnchor(Anchor.Right, self, Anchor.Right)
		moveBarNext:setAnchor(Anchor.Bottom, moveBarBack, Anchor.Bottom)
		moveBarNext:setRotation(90)

		moveBar:setAnchor(Anchor.Bottom, self, Anchor.Bottom)
		moveBar:setAnchor(Anchor.Left, moveBarBack, Anchor.Right)
		moveBar:setAnchor(Anchor.Right, moveBarNext, Anchor.Left)
		moveBar:setAnchor(Anchor.Top, moveBarBack, Anchor.Top)
		moveBar:setAnchor(Anchor.Bottom, moveBarBack, Anchor.Bottom)
		moveBar:setDirection(direction)

		indicator:setAnchor(Anchor.Top, moveBar, Anchor.Top)
		indicator:setAnchor(Anchor.Bottom, moveBar, Anchor.Bottom)
		indicator:setAnchor(Anchor.Left, moveBar, Anchor.Left)

		virtualContainer:setAnchorMargin(Anchor.Left, config.border)
		virtualContainer:setAnchorMargin(Anchor.Right, config.border)

		if(barSide == Anchor.Top) then
			moveBarBack:setAnchor(Anchor.Top, self, Anchor.Top)
			moveBarBack:setAnchorMargin(Anchor.Top, config.headerSize)
			virtualContainer:setAnchorMargin(Anchor.Top, config.headerSize + moveBar:getHeight())
			virtualContainer:setAnchorMargin(Anchor.Bottom, config.border)
		else
			moveBarBack:setAnchor(Anchor.Bottom, self, Anchor.Bottom)
			virtualContainer:setAnchorMargin(Anchor.Bottom, moveBar:getHeight())
			virtualContainer:setAnchorMargin(Anchor.Top, config.headerSize + config.border)
		end
	end
end

function MainWindow.addWidget(self, widget)
	local widgetsContainer = self:getChildById("VirtualContainer"):getChildById("WidgetsContainer")
	widget:setParent(widgetsContainer)
end