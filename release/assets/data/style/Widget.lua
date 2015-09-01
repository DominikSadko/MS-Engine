newMetatable("Widget", "UIWidget")

function Widget.Create()
	local self = Widget.new()
	self:setMetatable("Widget")
	return self
end