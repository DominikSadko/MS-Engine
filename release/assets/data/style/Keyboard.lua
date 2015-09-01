newMetatable("Keyboard", "UIWidget")
Keyboard.useKeyboard = true

g_keyboard = nil
local m_textEdit = nil

local config = {
	buttonSize = { w = 40, h = 40 },
	keys = {"qwertyuiop", "asdfghjkl", "zxcvbnm"}
}

function Keyboard.Create(parent)
	local self = Keyboard.new()
	self:setMetatable("Keyboard")

	self:setParent(getRootWidget())
	self:setFocusable(false)
	self:setImageSource(g_resourceManager:getImage("keyboard"))
	self:setSize({ w = 35 + (#config.keys[1] * (config.buttonSize.w + 2)), h = 16 + (#config.keys + 1) * (config.buttonSize.h + 2) })
	self:setBorder({ x = 3, y = 3, w = 3, h = 3 })

	self:setAnchor(Anchor.HorizontalCenter, self:getParent(), Anchor.HorizontalCenter)
	self:setAnchor(Anchor.Bottom, self:getParent(), Anchor.Bottom)
	self:setAnchorMargin(Anchor.Bottom, -self:getHeight())
	self:setVisible(false)
	self.event = 0
	self.eventWrite = 0
	self.cursor = 0

	g_event.focusChange.add(Keyboard.onFocusChange)

	for i, v in ipairs(config.keys) do
		for stack = 1, #v do
			local button = Button.Create()
			button:setId(v:sub(stack, stack))
			button:setParent(self)
			button:setFocusable(false)

			button:setAnchor(Anchor.Left, button:getParent(), Anchor.Left)
			button:setAnchor(Anchor.Top, button:getParent(), Anchor.Top)
			button:setAnchorMargin(Anchor.Left, 15 + (i * 2) + ((i - 1) * config.buttonSize.h) + ((stack - 1) * (config.buttonSize.w + 2)))
			button:setAnchorMargin(Anchor.Top, 4 + ((i - 1) * (config.buttonSize.h + 2)))

			button:setTitleFont(g_resourceManager:getFont("verdana_22px"))
			button:setTitle(v:sub(stack, stack))

			button:setSize({w = config.buttonSize.w, h = config.buttonSize.h})

			button.mouseClick = Keyboard.onMouseClick
			button.mouseHover = Keyboard.onMouseHover
		end
	end

	-- Space
	local space = Button.Create()
	space:setParent(self)
	space:setFocusable(false)
	space:setAnchor(Anchor.Left, space:getParent(), Anchor.Left)
	space:setAnchor(Anchor.Right, space:getParent(), Anchor.Right)
	space:setAnchor(Anchor.Bottom, space:getParent(), Anchor.Bottom)
	space:setAnchorMargin(Anchor.Left, 64)
	space:setAnchorMargin(Anchor.Right, 64)
	space:setAnchorMargin(Anchor.Bottom, 4)
	space:setHeight(config.buttonSize.h)

	space:setTitleFont(g_resourceManager:getFont("verdana_22px"))
	space:setTitle(" ")

	space.mouseClick = Keyboard.onMouseClick
	space.mouseHover = Keyboard.onMouseHover

	-- Back
	local back = Button.Create()
	back:setParent(self)
	back:setFocusable(false)
	back:setAnchor(Anchor.Right, back:getParent(), Anchor.Right)
	back:setAnchor(Anchor.Bottom, back:getParent(), Anchor.Bottom)
	back:setAnchorMargin(Anchor.Right, 10)
	back:setAnchorMargin(Anchor.Bottom, config.buttonSize.h + 15)
	back:setHeight(config.buttonSize.h)

	back:setTitleFont(g_resourceManager:getFont("verdana_22px"))
	back:setTitle("<-")

	back.mouseClick = Keyboard.onMouseClick
	back.mouseHover = Keyboard.onMouseHover

	-- Back
	local shift = Button.Create()
	shift:setParent(self)
	shift:setFocusable(false)
	shift:setAnchor(Anchor.Left, back:getParent(), Anchor.Left)
	shift:setAnchor(Anchor.Bottom, back:getParent(), Anchor.Bottom)
	shift:setAnchorMargin(Anchor.Left, 10)
	shift:setAnchorMargin(Anchor.Bottom, config.buttonSize.h + 15)
	shift:setHeight(config.buttonSize.h)

	shift:setTitleFont(g_resourceManager:getFont("verdana_22px"))
	shift:setTitle("Shift")
	shift.pressed = false

	g_event.mouse.add(function(widget, executed)
		if(executed) then
			local pressed = not widget.pressed
			widget.pressed = pressed

			for i, v in ipairs(config.keys) do
				for stack = 1, #v do
					local c = v:sub(stack, stack)
					local button = self:getChildById(c)
					if(pressed) then
						button:getChildById("Title"):setText(string.char((string.byte(c) - 32)))
					else
						button:getChildById("Title"):setText(c)
					end
					button:setSize({w = config.buttonSize.w, h = config.buttonSize.h})
				end
			end
		end
	end, nil, shift)

	-- Cursor
	local cursor = Widget.new()
	cursor:setId("cursor")
	cursor:setParent(self)
	cursor:setFocusable(false)
	cursor:setPhantom(true)
	cursor:setImageSource(g_resourceManager:getImage("gui"))
	cursor:setSize({ w = 2, h = 13 })
	cursor:setBorder({ x = 0, y = 2, w = 0, h = 2 })
	cursor:setImageRect({ x = 32, y = 65, w = 1, h = 13 })
	cursor:setVisible(false)

	return self
end

-------------------------------------------------------

function Keyboard.onMouseHover(self, id, hovered)
	if(hovered == false) then
		local keyboard = self:getParent()
		stopEvent(keyboard.eventWrite)
		keyboard.eventWrite = 0
	end
end

function Keyboard.onMouseClick(self, executed)
	if(g_keyboard.eventWrite ~= 0) then
		stopEvent(g_keyboard.eventWrite)
		g_keyboard.eventWrite = 0
	end

	if(not executed or not m_textEdit) then
		return
	end

	local text = m_textEdit:getText()
	local c = self:getTitle()

	if(c == "<-") then
		if(#text == 0 or g_keyboard.cursor == 0) then
			return
		end

		text = string.sub(text, 0, g_keyboard.cursor - 1) .. string.sub(text, g_keyboard.cursor + 1, #text)
		g_keyboard.cursor = g_keyboard.cursor - 1
	else
		if(m_textEdit.maxSize > 0 and #text >= m_textEdit.maxSize) then
			return
		end

		text = (string.sub(text, 0, g_keyboard.cursor)) .. c .. (string.sub(text, g_keyboard.cursor + 1, #text))
		g_keyboard.cursor = g_keyboard.cursor + 1
	end

	m_textEdit:setText(text)

	local font = m_textEdit:getTextFont()
	local cursor = m_textEdit:getChildById("cursor")
	if(cursor) then
		cursor:setAnchorMargin(Anchor.Left, font:getTextSize(string.sub(text, 0, g_keyboard.cursor)).w)
	end

	g_keyboard.eventWrite = scheduleEvent(function() Keyboard.onMouseClick(self, executed) end, 200)
end

function Keyboard.onMouseTextEdit(widget, executed, id, pos)
	if(not executed) then
		return
	end

	local text = widget:getText()
	local font = widget:getTextFont()
	local cursor = widget:getChildById("cursor")

	if(not font or not cursor) then
		return
	end

	local size = { x = 0, y = 0, w = 0, h = 0 }
	local i = 1
	while(true) do
		if(i > #text) then
			break
		end

		local _size = font:getTextSize(string.sub(text, 0, i))
		if(_size.w > pos.x) then
			break
		end

		size = _size
		i = i + 1
	end

	cursor:setAnchorMargin(Anchor.Left, size.w)
	g_keyboard.cursor = i - 1
end

function Keyboard.onRemoveWidget(widget)
	if(m_textEdit and widget:getUniqueId() == m_textEdit:getUniqueId()) then
		g_keyboard:hide()
		m_textEdit = nil
	end
end

function Keyboard.onFocusChange(widget, focus)
	if(widget and (widget.useKeyboard or (widget:getParent() and widget:getParent().useKeyboard))) then
		if(focus == false and m_textEdit) then
			local cursor = widget:getChildById("cursor")
			if(cursor) then
				cursor:setParent(g_keyboard)
				cursor:setVisible(false)
				cursor:removeCondition(Condition.Type.Blink)
			end
		end

		if(focus) then
			m_textEdit = widget
			g_keyboard:show()
		end
	else
		g_keyboard:hide()
		m_textEdit = widget
	end
end

---   usefull functions   ---
local function slider(widget, value)
	local margin = widget:getAnchorMargin(Anchor.Bottom)
	if((value > 0 and margin < 0) or (value < 0 and margin > -widget:getHeight())) then
		widget:setAnchorMargin(Anchor.Bottom, math.max(-widget:getHeight(), math.min(0, margin + value)))
		widget.event = scheduleEvent(function() slider(widget, value) end, 10)
		return
	elseif(value < 0) then
		widget:setVisible(false)
	end

	widget.event = 0
end

function Keyboard.show(self)
	if(not m_textEdit) then
		return
	end

	stopEvent(self.eventWrite)
	stopEvent(self.event)

	self:setVisible(true)

	local cursor = self:getChildById("cursor")
	if(cursor) then
		cursor:setParent(m_textEdit)
		cursor:setVisible(true)
		Condition.Blink(cursor, 1000)

		cursor:setAnchor(Anchor.Top, cursor:getParent(), Anchor.Top)
		cursor:setAnchor(Anchor.Bottom, cursor:getParent(), Anchor.Bottom)
		cursor:setAnchorMargin(Anchor.Top, 4)
		cursor:setAnchorMargin(Anchor.Bottom, 4)
		cursor:setAnchor(Anchor.Left, cursor:getParent(), Anchor.Left)
	end

	g_event.mouse.add(Keyboard.onMouseTextEdit, nil, m_textEdit)
	g_event.removeWidget.add(Keyboard.onRemoveWidget, m_textEdit)

	local parent = self:getParent()
	if(parent) then
		parent:popChild(self)
	end

	slider(self, 5)
end

function Keyboard.hide(self)
	if(self:isVisible() == false) then
		return
	end

	if(not m_textEdit) then
		return
	end

	local cursor = m_textEdit:getChildById("cursor")
	if(cursor) then
		cursor:setParent(self)
		cursor:setVisible(false)
		cursor:removeCondition(Condition.Type.Blink)
	end

	g_event.mouse.remove(Keyboard.onMouseTextEdit, m_textEdit)
	g_event.removeWidget.remove(Keyboard.onRemoveWidget, m_textEdit)

	stopEvent(self.eventWrite)
	stopEvent(self.event)

	slider(self, -5)
end