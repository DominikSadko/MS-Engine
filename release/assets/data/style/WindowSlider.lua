newMetatable("WindowSlider", "UIWidget")

function WindowSlider.Create()
	local self = WindowSlider.new()
	self:setMetatable("WindowSlider")
	self:setResourceManager(g_resourceManager)
	self:addLayout(g_resourceManager:getLayout("WindowSlider"))
	self.m_page = {}
	self:setPagePosition({ x = 0, y = 0 })

	local widgetsContainer = self:getChildById("VirtualContainer"):getChildById("WidgetsContainer")
	g_event.slider.add(WindowSlider.onSlide, nil, nil, widgetsContainer)
	return self
end

function WindowSlider.onSlide(widgetsContainer, angle, distance, key)
	if(widgetsContainer:isLocked()) then
		return
	end

	local self = widgetsContainer:getParent():getParent()
	local direction = angleToAnchor(angle, 20)

	if(distance > 50) then
		local pos = self:getPagePosition()

		if(direction == Anchor.Right) then
			pos.x = pos.x - 1
		elseif(direction == Anchor.Left) then
			pos.x = pos.x + 1
		elseif(direction == Anchor.Top) then
			pos.y = pos.y - 1
		elseif(direction == Anchor.Bottom) then
			pos.y = pos.y + 1
		else
			return
		end
		
		local page = self:getPageByPosition(self:getPagePosition())
		local nextPage = self:getPageByPosition(pos)

		if(not page or not nextPage) then
			return
		end

		page:setVisible(true)
		nextPage:setVisible(true)

		page:setOffset({x = 0, y = 0})
		nextPage:setOffset({ x = pos.x * nextPage:getWidth(), y = pos.y * nextPage:getHeight() })

		local eventor = nil
		if(direction == Anchor.Left) then
			eventor = function(speed)
				local offset = page:getOffset()
				if(offset.x > -widgetsContainer:getWidth()) then
					offset.x = math.max(-widgetsContainer:getWidth(), offset.x - 15)
					page:setOffset({ x = offset.x, y = 0 })
					nextPage:setOffset({ x = widgetsContainer:getWidth() + offset.x, y = 0 })
					scheduleEvent(eventor, speed)
				else
					self:setPagePosition(pos)
					page:setVisible(false)
					nextPage:focus()
				end
			end
		elseif(direction == Anchor.Top) then
			eventor = function(speed)
				local offset = page:getOffset()
				if(offset.y > -widgetsContainer:getHeight()) then
					offset.y = math.max(-widgetsContainer:getHeight(), offset.y - 15)
					page:setOffset({ x = 0, y = offset.y })
					nextPage:setOffset({ x = 0, y = widgetsContainer:getHeight() + offset.y })
					scheduleEvent(eventor, speed)
				else
					self:setPagePosition(pos)
					page:setVisible(false)
					nextPage:focus()
				end
			end
		elseif(direction == Anchor.Right) then
			eventor = function(speed)
				local offset = page:getOffset()
				if(offset.x < widgetsContainer:getWidth()) then
					offset.x = math.min(widgetsContainer:getWidth(), offset.x + 15)
					page:setOffset({ x = offset.x, y = 0 })
					nextPage:setOffset({ x = -widgetsContainer:getWidth() + offset.x, y = 0 })
					scheduleEvent(eventor, speed)
				else
					self:setPagePosition(pos)
					page:setVisible(false)
					nextPage:focus()
				end
			end
		elseif(direction == Anchor.Bottom) then
			eventor = function(speed)
				local offset = page:getOffset()
				if(offset.y < widgetsContainer:getHeight()) then
					offset.y = math.min(widgetsContainer:getHeight(), offset.y + 15)
					page:setOffset({ x = 0, y = offset.y })
					nextPage:setOffset({ x = 0, y = -widgetsContainer:getHeight() + offset.y })
					scheduleEvent(eventor, speed)
				else
					self:setPagePosition(pos)
					page:setVisible(false)
					nextPage:focus()
				end
			end
		else
			return
		end

		eventor(math.max(2, 8 - widgetsContainer:getWidth() / 100))
		widgetsContainer:setLocked(true)
	end
end

---   usefull functions   ---

function WindowSlider.addPage(self, pos)
	local widgetsContainer = self:getChildById("VirtualContainer"):getChildById("WidgetsContainer")
	local page = widgetsContainer:doCreateChild()
	page:fill("parent")
	page:setMovable(false)

	if(self:getPagePosition().x == pos.x and self:getPagePosition().y == pos.y) then
		page:setVisible(true)
	else
		page:setVisible(false)
	end

	table.insert(self.m_page, {pos, page})

	return page
end

function WindowSlider.getPageByPosition(self, pos)
	for i, v in ipairs(self.m_page) do
		if(v[1].x == pos.x and v[1].y == pos.y) then
			return v[2]
		end
	end

	return nil
end

function WindowSlider.setPagePosition(self, pos)
	self.m_currentPage = {x = pos.x, y = pos.y}

	local widgetsContainer = self:getChildById("VirtualContainer"):getChildById("WidgetsContainer")
	widgetsContainer:setLocked(false)
end

function WindowSlider.getPagePosition(self)
	return {x = self.m_currentPage.x, y = self.m_currentPage.y}
end