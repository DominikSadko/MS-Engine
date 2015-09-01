newMetatable("LabelContainer", "UIWidget")

function LabelContainer.Create()
	g_resourceManager:addLayout("Label", "assets/data/style/Label.json")

	local self = Panel.new()
	self:setMetatable("LabelContainer")
	self:setResourceManager(g_resourceManager)
	self:addLayout(g_resourceManager:getLayout("LabelContainer"))

	self.labelHeight = 20

	g_event.movement.add(LabelContainer.onMove, nil, nil, self:getChildById("WidgetsContainer"))
	return self
end

function LabelContainer.onMove(widgetsContainer, executed, key, pos)
	if(not executed) then return end

	local self = widgetsContainer:getParent()
	if(self:getHeight() > widgetsContainer:getChildsCount() * self.labelHeight) then
		return
	end

	local lastPosition = Mouse.getLastPosition(key)
	local currentPosition = Mouse.getPosition(key)
	
	local angle = getAngleBetweenVectors(lastPosition, currentPosition)
	local direction = angleToAnchor(angle, 1)
	local offset = widgetsContainer:getOffset()

	local speed = 1 + (widgetsContainer:getChildsCount() / 100)
	if(direction == Anchor.Top) then
		widgetsContainer:setOffset({ x = 0, y = math.min(0, offset.y - speed * (currentPosition.y - lastPosition.y)) })
	elseif(direction == Anchor.Bottom) then
		widgetsContainer:setOffset({ x = 0, y = math.max(-widgetsContainer:getHeight() + self:getHeight(), offset.y + speed * (lastPosition.y - currentPosition.y)) })
	end
end

function LabelContainer.addLabel(self)
	local widgetsContainer = self:getChildById("WidgetsContainer")

	local label = widgetsContainer:doCreateChild()
	label:setResourceManager(self:getResourceManager())
	label:addLayout(g_resourceManager:getLayout("Label"))
	label:setHeight(self.labelHeight)

	label:setAnchorMargin(Anchor.Top, widgetsContainer:getChildIndex(label) * self.labelHeight)
	widgetsContainer:setHeight((widgetsContainer:getChildIndex(label) + 1) * self.labelHeight)
	return label
end