newMetatable("Item", "UIWidget")

function Item.Create()
	local self = Item.new()
	self:setMetatable("Item")
	self:setResourceManager(g_resourceManager)
	self:addLayout(g_resourceManager:getLayout("Item"))

	self.slotType = 0
	self.itemType = 0
	self.slotId = 0
	self.data = nil

	g_event.mouse.add(Item.onMousePress, nil, self:getChildById("Source"))

	return self
end

function Item.setData(self, data)
	self.data = data
	
	local source = self:getChildById("Source")
	if(not data) then
		source:setImageRect({})
		self.data = nil
		self.itemType = nil
	else
		source:setImageRect(self.data[#self.data])
		self.itemType = self.data[#self.data - 1]
	end
end

function Item.onMousePress(source, executed, key, pos)
	if(executed) then
		source:getParent():getParent():popChild(source:getParent())
		source:removeAnchors()
		source:grab()
	else
		source:fill(source:getParent())

		local mousePos = Mouse.getPosition(key)
		local mouseClickPos = Mouse.getMouseClickPosition(key)

		if(mousePos.x == mouseClickPos.x and mousePos.y == mouseClickPos.y) then
			local self = source:getParent()
			return self.click and self.click(self, executed, key, pos)
		end

		local targetSource = getRootWidget():getChildByPoint(Mouse.getPosition(key))
		if(not targetSource) then
			return
		end

		local self = source:getParent()
		local targetParent = targetSource:getParent()

		if(self.onSetData and self.onSetData(self, targetParent)) then
			return
		end

		if(not targetParent or not targetParent.slotType) then
			return
		end

		self:internalMoveItem(targetParent)
	end
end

function Item.requestMoveItem(self, target)
	if(target.slotType ~= 0 and self.itemType ~= target.slotType) then
		return false
	end

	return true
end

function Item.internalMoveItem(self, target)
	if(self:requestMoveItem(target) == false) then
		return false
	end

	local data = self.data
	local targetData = target.data

	target:setData(data)
	self:setData(targetData)
	return true
end