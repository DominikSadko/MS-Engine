newMetatable("ComboBoxMenu", "UIWidget")
newMetatable("ComboBox", "UIWidget")

function ComboBoxMenu.Create()
	local self = Panel.Create()
	self:setMetatable("ComboBoxMenu")

	self.comboBoxList = {}
	return self
end

function ComboBoxMenu.add(self, text)
	local comboBox = ComboBox.Create(text)

	comboBox:setParent(self)
	if(#self.comboBoxList == 0) then
		comboBox:setAnchor(Anchor.Left, self, Anchor.Left)
		comboBox:setAnchor(Anchor.Right, self, Anchor.Right)
		comboBox:setAnchor(Anchor.Top, self, Anchor.Top)
	else
		local lastComboBox = self.comboBoxList[#self.comboBoxList]
		comboBox:setAnchor(Anchor.Left, lastComboBox, Anchor.Left)
		comboBox:setAnchor(Anchor.Right, lastComboBox, Anchor.Right)
		comboBox:setAnchor(Anchor.Top, lastComboBox, Anchor.Bottom)
		comboBox:setAnchorMargin(Anchor.Top, 6)
	end

	if(text) then
		local title = comboBox:getChildById("Title")
		title:setText(text)
	end

	self.comboBoxList[#self.comboBoxList + 1] = comboBox
	return comboBox
end

function ComboBox.Create(text)
	local self = ComboBox.new()
	self:setMetatable("ComboBox")

	local checkBox = self:doCreateChild()
	checkBox:setId("Box")

	checkBox:setImageSource(g_resourceManager:getImage("gui"))
	checkBox:setImageRect({ x = 20, y = 0, w = 12, h = 12 })
	checkBox:setSize({ w = 16, h = 16 })
	checkBox:setAnchor(Anchor.Right, self, Anchor.Right)
	checkBox:setAnchor(Anchor.Top, self, Anchor.Top)

	-- Title
	local title = self:doCreateChild()
	title:setId("Title")
	title:setPhantom(true)
	title:setTextAutoResize(true)

	title:setAnchor(Anchor.Left, self, Anchor.Left)
	title:setAnchor(Anchor.VerticalCenter, self, Anchor.VerticalCenter)

	title:setTextFont(g_resourceManager:getFont("default"))

	self:setSize(checkBox:getSize())

	g_event.mouse.add(ComboBox.onMouseClick, nil, checkBox)
	return self
end

---   event handlers   ---

function ComboBox.onMouseClick(self, executed)
	if(executed == false) then
		self:getParent():set(true)
	end
end

---   usefull functions   ---

function ComboBox.set(self, checked)
	local comboBoxMenu = self:getParent()
	local checkBox = self:getChildById("Box")

	if(not comboBoxMenu.comboBoxList) then
		return
	end

	for i, v in ipairs(comboBoxMenu.comboBoxList) do
		if(v ~= self and v:isOn()) then
			v:set(false)
		end
	end
	
	if(checked and self:isOn() == false) then
		checkBox:setImageRectY(checkBox:getImageRectY() + checkBox:getImageRectH())
		self:setOn(true)
	elseif(checked == false and self:isOn()) then
		checkBox:setImageRectY(checkBox:getImageRectY() - checkBox:getImageRectH())
		self:setOn(false)
	end
end