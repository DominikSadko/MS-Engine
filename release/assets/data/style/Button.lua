newMetatable("Button", "UIWidget")

function Button.Create()
	local self = Button.new()
	self:setMetatable("Button")
	self:setResourceManager(g_resourceManager)
	self:addLayout(g_resourceManager:getLayout("Button"))

	-- title
	local title = self:getChildById("Title")

	g_event.mouse.add(Button.onMouseClick, nil, self)
	g_event.hover.add(Button.onMouseHover, self)
	return self
end

function Button.onJson(self, json)
	local resource = self:getResourceManager()
	local node = nil

	node = toauto(Json.toString(json, "title"))
	if(node) then self:setTitle(node) end

	node = toauto(Json.toString(json, "title-font"))
	if(node) then self:setTitleFont(resource:getFont(node)) end
end

---   event handlers   ---
function Button.onMouseClick(self, executed, id, pos)
	if(executed and self:isOn() == false) then
		self:setOn(true)
	elseif(not executed and self:isOn()) then
		self:setOn(false)

		if(self.click) then
			self.click(self, id, pos)
		end
	end

	if(self.mouseClick) then
		self.mouseClick(self, executed, id, pos)
	end
end

function Button.onMouseHover(self, id, hovered)
	if(hovered == false and self:isOn()) then
		self:setOn(false)
	end

	if(self.mouseHover) then
		self.mouseHover(self, id, hovered)
	end
end

---   usefull functions   ---
function Button.setTitle(self, text)
	local title = self:getChildById("Title")
	local font = title:getTextFont()
	local size = font:getTextSize(text)
	if(size.w + 24 > self:getWidth()) then
		self:setWidth(size.w + 24)
	end

	if(size.h + 4 > self:getHeight()) then
		self:setHeight(size.h + 4)
	end

	title:setText(text)
end

function Button.getTitle(self)
	return self:getChildById("Title"):getText()
end

function Button.setTitleFont(self, font)
	self:getChildById("Title"):setTextFont(font)
	self:setTitle(self:getTitle(self))
end