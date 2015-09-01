newMetatable("MessageBox", "UIWidget")
local g_messageBox = nil

function MessageBox.Create(titleText, messageText)
	if(g_messageBox) then
		g_messageBox:setVisible(true)
		g_messageBox:getChildById("Title"):setText(titleText)
		g_messageBox:getChildById("Message"):setText(messageText)
		
		local size = g_messageBox:getChildById("Title"):getTextFont():getTextSize(messageText)
		g_messageBox:setSize({ w = size.w + 50, h = size.h + 75 })

		return
	end

	local self = MessageBox.new()
	self:setMetatable("MessageBox")

	self:setParent(getRootWidget())
	self:focus()

	self:setImageSource(g_resourceManager:getImage("window"))
	self:setBorder({x = 3, y = 28, w = 3, h = 3})
	self:setAnchor(Anchor.HorizontalCenter, self:getParent(), Anchor.HorizontalCenter)
	self:setAnchor(Anchor.VerticalCenter, self:getParent(), Anchor.VerticalCenter)

	-- Title
	local title = self:doCreateChild()
	title:setId("Title")
	title:setPhantom(true)
	title:setAnchor(Anchor.HorizontalCenter, self, Anchor.HorizontalCenter)
	title:setAnchor(Anchor.Top, self, Anchor.Top)
	title:setAnchorMargin(Anchor.Top, 3)

	title:setTextAutoResize(true)
	title:setTextFont(g_resourceManager:getFont("default"))

	-- Message
	local message = self:doCreateChild()
	message:setId("Message")
	message:setPhantom(true)
	message:setAnchor(Anchor.HorizontalCenter, self, Anchor.HorizontalCenter)
	message:setAnchor(Anchor.Top, self, Anchor.Top)
	message:setAnchorMargin(Anchor.Top, 28)

	message:setTextAutoResize(true)
	message:setTextFont(g_resourceManager:getFont("default"))

	-- Button: ok
	local ok = Button.Create()
	ok:setParent(self)
	ok:setTitle("Ok")
	ok:setAnchor(Anchor.HorizontalCenter, ok:getParent(), Anchor.HorizontalCenter)
	ok:setAnchor(Anchor.Bottom, ok:getParent(), Anchor.Bottom)
	ok:setAnchorMargin(Anchor.Bottom, 20)
	ok.click = function() self:setVisible(false) end

	title:setText(titleText)
	message:setText(messageText)
	local size = title:getTextFont():getTextSize(messageText)
	self:setSize({ w = size.w + 50, h = size.h + 75 })

	g_messageBox = self

	return self
end