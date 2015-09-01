newMetatable("CheckBox", "UIWidget")

function CheckBox.Create()
	local self = CheckBox.new()
	self:setMetatable("CheckBox")
	self:setResourceManager(g_resourceManager)
	self:addLayout(g_resourceManager:getLayout("CheckBox"))

	g_event.mouse.add(CheckBox.onMouseClick, nil, self)
	return self
end

---   event handlers   ---

function CheckBox.onMouseClick(self, executed)
	if(executed == false) then
		self:set(not self:isOn())
	end
end

---   usefull functions   ---

function CheckBox.set(self, checked, safe)
	if(checked and self:isOn() == false) then
		self:setImageRectY(self:getImageRectY() + self:getImageRectH())
		self:setOn(true)
	elseif(checked == false and self:isOn()) then
		self:setImageRectY(self:getImageRectY() - self:getImageRectH())
		self:setOn(false)
	end

	if(not safe and self.click) then
		self.click(self, checked)
	end
end