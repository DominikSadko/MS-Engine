function math.closer(n)
	return (math.floor(n) + 0.5) < n and math.ceil(n) or math.floor(n)
end

function math.round(v, n)
	while(v > n) do v = v - n end
	return v
end

function loadUI(name, parent, meta)
	if(not meta) then
		return parent:doCreateJsonChild(g_resourceManager:getLayout(name))
	end

	local self = meta.Create()
	if(parent) then
		self:setParent(parent)
	end

	self:setResourceManager(g_resourceManager)
	self:addLayout(g_resourceManager:getLayout(name))
	return self
end

function getDistanceBetween(fromPos, toPos)
	return math.floor(math.abs(math.sqrt((toPos.x - fromPos.x) ^ 2 + (toPos.y - fromPos.y) ^ 2)))
end

function getAngleBetweenVectors(fromPos, toPos)
	return math.atan2(toPos.y - fromPos.y, toPos.x - fromPos.x) * (180 / math.pi) + 180
end

function getVectorByAngle(pos, angle, distance)
	angle = angle - 180
	return {
		x = math.closer(pos.x + math.cos(angle * math.pi / 180) * (distance or 1)),
		y = math.closer(pos.y + math.sin(angle * math.pi / 180) * (distance or 1))
	}
end

function angleToAnchor(angle, disp)
	disp = disp or 0

	if((angle >= 0 and angle <= disp) or (angle <= 360 and angle >= 360 - disp)) then
		return Anchor.Left
	elseif(angle > disp and angle < 90 - disp) then
		return Anchor.TopLeft
	elseif(angle >= 90 - disp and angle <= 90 + disp) then
		return Anchor.Top
	elseif(angle > 90 + disp and angle < 180 - disp) then
		return Anchor.TopRight
	elseif(angle >= 180 - disp and angle <= 180 + disp) then
		return Anchor.Right
	elseif(angle > 180 + disp and angle < 270 - disp) then
		return Anchor.BottomRight
	elseif(angle >= 270 - disp and angle <= 270 + disp) then
		return Anchor.Bottom
	elseif(angle > 270 + disp and angle < 360 - disp) then
		return Anchor.BottomLeft
	end
end

function anchorToString(direction)
	if(direction == Anchor.Top)            		 then return "Anchor.Top"
	elseif(direction == Anchor.Bottom)     		 then return "Anchor.Bottom"
	elseif(direction == Anchor.Left)       		 then return "Anchor.Left"
	elseif(direction == Anchor.Right)      		 then return "Anchor.Right"
	elseif(direction == Anchor.Right)      		 then return "Anchor.Right"
	elseif(direction == Anchor.VerticalCenter)   then return "Anchor.VerticalCenter"
	elseif(direction == Anchor.HorizontalCenter) then return "Anchor.HorizontalCenter"
	elseif(direction == Anchor.TopLeft)    		 then return "Anchor.TopLeft"
	elseif(direction == Anchor.TopRight)   		 then return "Anchor.TopRight"
	elseif(direction == Anchor.BottomLeft) 		 then return "Anchor.BottomLeft"
	elseif(direction == Anchor.BottomRight)		 then return "Anchor.BottomRight"
	end

	return "Anchor.None"
end