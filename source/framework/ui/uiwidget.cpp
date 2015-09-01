#include "framework/ui/uiwidget.h"

int32_t UIWidget::AutoId = 0;

UIWidget::UIWidget()
{
    setMetatable("UIWidget");

    m_uniqueId = ++UIWidget::AutoId;
    m_id = stdex::toString("UIWidget", m_uniqueId);

    m_rect = {0, 0, 0, 0};
    m_vertices.clear();
    m_coords.clear();
    m_polygonWidth = m_polygonHeight = 0;
    m_rotation = 0;
    m_opacity = 255;

    setVisible(true);
    setFocusable(true);
    setMovable(true);
    setPhantom(false);
    setSelected(false);
    setOn(false);
    setLocked(false);
    setVirtualArea(false);
    setTextAutoResize(false);
    setFixedRotation(false);
    setCircle(false);

    m_parent = nullptr;

    m_imageSource = nullptr;
    m_imageRect = {0, 0, 0, 0};
    m_imageColor = {255, 255, 255, 255};
    setHorizontalMirror(false);
    setVerticalMirror(false);

    m_border = {0, 0, 0, 0};
    m_borderRect = {0, 0, 0, 0};

    m_text = "";
    m_textFont = nullptr;
    m_textCache = nullptr;
    m_textPosition = {0, 0};
    m_textColor = {255, 255, 255, 255};

    m_shader = nullptr;

    m_anchorLayout.clear();
    setOffset({0, 0});

    m_resourceManager = nullptr;
}

UIWidget::~UIWidget() {}

void UIWidget::remove()
{
    g_lua["onWidgetRemove"].call(this);

    if(g_platform.getRootWidget() == this)
        g_platform.setRootWidget(nullptr);

    if(g_platform.getFocusedWidget() == this)
        g_platform.setFocusedWidget(nullptr);

    if(g_platform.getGrabbedWidget() == this)
        g_platform.setGrabbedWidget(nullptr);

    for(auto child : m_childs)
        child->remove();

    m_childs.clear();

    if(m_parent)
    {
        m_parent->m_anchorLayout.removeByWidget(this);
        for(auto child : m_parent->m_childs)
            child->m_anchorLayout.removeByWidget(this);

        m_parent->eraseChild(this);
    }

    if(m_textCache)
    {
        delete m_textCache;
        m_textCache = nullptr;
    }

    m_anchorLayout.clear();
    m_jsonLayout.clear();

    for(auto condition : m_conditions)
		delete condition;

	m_conditions.clear();
}

void UIWidget::draw(const Point& offset, float scale)
{
	for(auto it = m_conditions.begin(); it != m_conditions.end(); it++)
	{
		if(!(*it)->onThink())
		{
			m_conditions.erase(it);
			break;
		}
	}

    if(!isVisible() || !m_rect.w || !m_rect.h)
        return;

    if(isVirtualArea())
        g_painter.setDrawArea(getVirtualRect());

    if(m_imageSource || (m_textFont && !m_text.empty()))
    {
        g_painter.useShader(m_shader);

        if(isHorizontalMirror())
            g_painter.setHorizontalMirror(true);

        if(isVerticalMirror())
            g_painter.setVerticalMirror(true);

        if(!m_vertices.empty())
        	drawPolygon(offset);
        else
        {
        	drawImage(offset);
        	drawBorder(offset);
        }

       	drawText(offset);

        if(isHorizontalMirror())
            g_painter.setHorizontalMirror(false);

        if(isVerticalMirror())
            g_painter.setVerticalMirror(false);

        g_painter.clear();
    }

	for(auto it = m_childs.rbegin(); it != m_childs.rend(); it++)
		(*it)->draw({offset.x + m_rect.x, offset.y + m_rect.y}, scale);

    if(isVirtualArea())
        g_painter.resetDrawArea();
}

void UIWidget::drawPolygon(const Point& offset) const
{
    Rect rect = {offset.x + m_rect.x, offset.y + m_rect.y, m_rect.w, m_rect.h};
    float scaleX = (float)m_rect.w / m_polygonWidth;
    float scaleY = (float)m_rect.h / m_polygonHeight;

    g_painter.setDrawMode(Shader::TriangleFan);
    g_painter.setImage(m_imageSource);
    g_painter.setColor(m_imageColor, m_opacity);
	g_painter.setPosition({(int32_t)(rect.x + m_polygonWidth / 2 * scaleX), (int32_t)(rect.y + m_polygonHeight / 2 * scaleY)});
	g_painter.scale(scaleX, scaleY);
	g_painter.setRotation(rect, m_parent->getFixedRotation());
	g_painter.setVertex(m_vertices);
	g_painter.setCoord(m_coords);

    g_painter.draw();
    g_painter.setPosition({0, 0});
}

void UIWidget::drawImage(const Point& offset) const
{
    if(!m_imageSource)
        return;

    Rect rect = {offset.x + m_rect.x, offset.y + m_rect.y, m_rect.w, m_rect.h};

    g_painter.setImage(m_imageSource);
    g_painter.setVertex(rect, !stdex::empty(m_borderRect) ? m_borderRect : m_border);
    g_painter.setCoord(m_imageSource, m_imageRect, m_border);
    g_painter.setColor(m_imageColor, m_opacity);
	g_painter.setRotation(rect, m_parent->getFixedRotation());

    g_painter.draw();
}

void UIWidget::drawBorder(const Point& offset) const
{
	// todo cached border
    if(!m_imageSource || (!m_border.x && !m_border.y && !m_border.w && !m_border.h))
        return;

    Rect rect = {offset.x + m_rect.x, offset.y + m_rect.y, m_rect.w, m_rect.h};
    const Rect& border = !stdex::empty(m_borderRect) ? m_borderRect : m_border;

    // left top
    g_painter.setVertex({rect.x,      rect.y,      border.x, border.y});
    g_painter.setCoord(m_imageSource, {m_imageRect.x, m_imageRect.y, m_border.x, m_border.y});
    g_painter.setRotation(rect, getFixedRotation());
    g_painter.draw();

    // top
    g_painter.setVertex({rect.x      + border.x, rect.y,      rect.w      - border.w - border.x, border.y});
    g_painter.setCoord(m_imageSource, {m_imageRect.x + m_border.x, m_imageRect.y, m_imageRect.w - m_border.w - m_border.x, m_border.y});
    g_painter.setRotation(rect, getFixedRotation());
    g_painter.draw();

    // right top
    g_painter.setVertex({rect.x      + rect.w      - border.w,  rect.y,      border.w, border.y});
    g_painter.setCoord(m_imageSource, {m_imageRect.x + m_imageRect.w - m_border.w,  m_imageRect.y, m_border.w, m_border.y});
    g_painter.setRotation(rect, getFixedRotation());
    g_painter.draw();

    // left
    g_painter.setVertex({rect.x,      rect.y      + border.y, border.x, rect.h      - border.y - border.h});
    g_painter.setCoord(m_imageSource, {m_imageRect.x, m_imageRect.y + m_border.y, m_border.x, m_imageRect.h - m_border.y - m_border.h});
    g_painter.setRotation(rect, getFixedRotation());
    g_painter.draw();

    // right
    g_painter.setVertex({rect.x      + rect.w      - border.w, rect.y      + border.y, border.w, rect.h      - border.y - border.h});
    g_painter.setCoord(m_imageSource, {m_imageRect.x + m_imageRect.w - m_border.w, m_imageRect.y + m_border.y, m_border.w, m_imageRect.h - m_border.y - m_border.h});
    g_painter.setRotation(rect, getFixedRotation());
    g_painter.draw();

    // bottom
    g_painter.setVertex({rect.x      + border.x, rect.y      + rect.h      - border.h, rect.w      - border.w - border.x, border.h});
    g_painter.setCoord(m_imageSource, {m_imageRect.x + m_border.x, m_imageRect.y + m_imageRect.h - m_border.h, m_imageRect.w - m_border.w - m_border.x, m_border.h});
    g_painter.setRotation(rect, getFixedRotation());
    g_painter.draw();

   // left bottom
    g_painter.setVertex({rect.x,      rect.y      + rect.h      - border.h, border.x, border.h});
    g_painter.setCoord(m_imageSource, {m_imageRect.x, m_imageRect.y + m_imageRect.h - m_border.h, m_border.x, m_border.h});
    g_painter.setRotation(rect, getFixedRotation());
    g_painter.draw();

    // right bottom
    g_painter.setVertex({rect.x      + rect.w      - border.w, rect.y      + rect.h      - border.h, border.w, border.h});
    g_painter.setCoord(m_imageSource, {m_imageRect.x + m_imageRect.w - m_border.w, m_imageRect.y + m_imageRect.h - m_border.h, m_border.w, m_border.h});
    g_painter.setRotation(rect, getFixedRotation());
    g_painter.draw();
}

void UIWidget::drawText(const Point& offset) const
{
    Rect rect = {offset.x + m_rect.x, offset.y + m_rect.y, m_rect.w, m_rect.h};

    if(m_text.size() == 1)
    {
        g_painter.setColor(m_textColor, m_opacity);
        g_painter.setRotation(rect, getFixedRotation());
        m_textFont->drawGlyph(m_text[0], {m_textPosition.x + rect.x, m_textPosition.y + rect.y});
    }
    else if(m_textCache && !m_textCache->empty())
    {
        g_painter.setColor(m_textColor, m_opacity);
        g_painter.setPosition({m_textPosition.x + rect.x, m_textPosition.y + rect.y});
        g_painter.setRotation(rect, getFixedRotation());

        m_textFont->draw(m_textCache);

        g_painter.setPosition({0, 0});
    }
}

void UIWidget::setPolygon(const stdex::Vertices& vertices)
{
	m_coords.clear();
	m_vertices.clear();
	m_polygonWidth = m_polygonHeight = 0;

	m_vertices = vertices;

	for(int32_t i = 0; i < m_vertices.size * 2; i += 2)
	{
		if(m_imageSource)
		{
			m_coords.add(m_vertices.data[i] / (float)m_imageSource->getTextureWidth());
			m_coords.add(m_vertices.data[i + 1] / (float)m_imageSource->getTextureHeight());
		}

		if(m_vertices.data[i] > m_polygonWidth)
			m_polygonWidth = m_vertices.data[i];

		if(m_vertices.data[i + 1] > m_polygonHeight)
			m_polygonHeight = m_vertices.data[i + 1];
	}

	m_coords.size = m_vertices.size;
	m_polygonWidth *= 2;
	m_polygonHeight *= 2;
}

void UIWidget::setImageCoords(const stdex::Vertices& vertices)
{
	if(!m_imageSource)
	{
		LOGE("Image source not found.");
		return;
	}

	m_coords.clear();

	for(int32_t i = 0; i < m_vertices.size * 2; i += 2)
	{
		m_coords.add(vertices.data[i] / (float)m_imageSource->getTextureWidth());
		m_coords.add(vertices.data[i + 1] / (float)m_imageSource->getTextureHeight());
	}

	m_coords.size = vertices.size;
}

Rect UIWidget::getVirtualRect() const
{
    Rect rect = m_rect;
    UIWidget* parent = getParent();
    while(parent)
    {
        rect.x += parent->m_rect.x;
        rect.y += parent->m_rect.y;

        parent = parent->getParent();
    }

    return rect;
}

bool UIWidget::focus()
{
    if(isPhantom() || !isVisible() || !isFocusable())
        return false;

    if(m_parent)
        m_parent->popChild(this);

    g_platform.setFocusedWidget(this);
    return true;
}

bool UIWidget::grab()
{
    if(isPhantom() || !isVisible() || !isMovable())
        return false;

    if(m_parent)
        m_parent->popChild(this);

    g_platform.setGrabbedWidget(this);
    g_platform.setOffsetPosition(getPosition());
    return true;
}

void UIWidget::setText(std::string text)
{
    m_text = text;
    if(!m_textFont)
    {
        LOGE("m_font not found.");
        return;
    }

    if(!m_textCache && m_text.size() > 1)
        m_textCache = new TextCache();
    else if(m_textCache && m_text.size() <= 1)
    {
        delete m_textCache;
        m_textCache = nullptr;
    }

    if(m_textCache)
       m_textCache->setText(m_textFont, m_text);

    if(isTextAutoResize())
    	setSize(m_textFont->getTextSize(m_text));
}

UIWidget* UIWidget::doCreateChild()
{
    UIWidget* widget = new UIWidget();
    m_childs.push_back(widget);
    widget->m_parent = this;
    return widget;
}

UIWidget* UIWidget::doCreateJsonChild(const json11::Json* json)
{
    UIWidget* child = nullptr;

    json11::Json layout = *json;
    if((*json)["Layout"])
        layout = (*json)["Layout"];
    else if((*json)["child"])
        layout = (*json)["child"];

    std::string className = "";
    if(auto classNode = layout["class"])
        className = stdex::toString(classNode);

    if(!className.empty() && g_lua[className] && g_lua[className]["Create"])
    {
        child = g_lua[className]["Create"].call<UIWidget*>();
        child->setParent(this);
    }
    else
        child = doCreateChild();

	child->setResourceManager(m_resourceManager);
    child->setParent(this);
    child->addLayout(json);
    return child;
}

void UIWidget::setParent(UIWidget* parent)
{
    if(m_parent)
        m_parent->eraseChild(this);

    m_parent = parent;
    m_parent->m_childs.push_back(this);
}

void UIWidget::addChild(UIWidget* child)
{
    if(child->m_parent)
        child->m_parent->eraseChild(child);

    child->m_parent = this;
    m_childs.push_back(child);
}

void UIWidget::eraseChild(UIWidget* child)
{
    auto it = std::find(std::begin(m_childs), std::end(m_childs), child);
    if(it != m_childs.end())
    {
        m_childs.erase(it);
        child->m_parent = nullptr;
    }
}

void UIWidget::removeChilds()
{
    for(auto child : m_childs)
    	child->remove();

    m_childs.clear();
}

void UIWidget::removeChild(UIWidget* child)
{
    eraseChild(child);
    child->remove();
}

void UIWidget::popChild(UIWidget* child)
{
    auto it = std::find(std::begin(m_childs), std::end(m_childs), child);
    if(it != m_childs.end())
    {
        m_childs.erase(it);
        m_childs.insert(std::begin(m_childs), child);
    }
}

bool UIWidget::hasChild(UIWidget* child)
{
    return child && m_childs.end() != std::find(std::begin(m_childs), std::end(m_childs), child);
}

UIWidget* UIWidget::getChildById(const std::string& id) const
{
    for(auto child : m_childs)
    {
        if(child->m_id == id)
            return child;
    }

    return nullptr;
}

UIWidget* UIWidget::getChildByRecursiveId(const std::string& id) const
{
    for(auto child : m_childs)
    {
        if(child->m_id == id)
            return child;

           if(UIWidget* widget = child->getChildByRecursiveId(id))
               return widget;
    }

    return nullptr;
}

UIWidget* UIWidget::getChildByPoint(const Point& point)
{
    if(!isVisible() || isLocked())
        return nullptr;

    for(auto it = m_childs.begin(); it != m_childs.end(); it++)
    {
        if((*it)->isCircle() && !stdex::inCircle({point.x - m_rect.x, point.y - m_rect.y}, (*it)->m_rect))
        	continue;
        else if(!stdex::inRange({point.x - m_rect.x, point.y - m_rect.y}, (*it)->m_rect))
        	continue;
        else if(UIWidget* next = (*it)->getChildByPoint({point.x - m_rect.x, point.y - m_rect.y}))
        {
            if(!next->isPhantom())
                return next;
        }
    }

    if(!isPhantom() && ( (isCircle() && stdex::inCircle(point, m_rect)) || stdex::inRange(point, m_rect)) )
        return this;

    return nullptr;
}

UIWidget* UIWidget::getChildByIndex(uint32_t index) const
{
    if(index >= m_childs.size())
        return nullptr;

    return m_childs[index];
}

int32_t UIWidget::getChildIndex(UIWidget* child) const
{
    auto it = std::find(m_childs.begin(), m_childs.end(), child);
    if(it == m_childs.end())
        return -1;

    return it - m_childs.begin();
}

uint32_t UIWidget::getChildsCount() const
{
    return m_childs.size();
}

UIWidget* UIWidget::getParentByRecursiveId(const std::string& id) const
{
    UIWidget* parent = getParent();
    while(parent)
    {
        if(parent->getId() == id)
            return parent;

        parent = parent->getParent();
    }

    return nullptr;
}

bool UIWidget::onMouseHover(uint8_t id, bool hovered)
{
    g_lua["onMouseHover"].call(this, id, hovered);
	hovered ? onLayout("@hovered")   : onLayout("!hovered");
    return true;
}

bool UIWidget::onMousePress(uint8_t id, const Point& pos, bool executed)
{
    g_lua["onMousePress"].call(this, executed, id, pos);
	executed ? onLayout("@pressed")   : onLayout("!pressed");
    return true;
}

void UIWidget::updateAnchors()
{
    m_anchorLayout.update(this);

    for(auto child : m_childs)
        child->updateAnchors();
}

void UIWidget::setAnchor(Anchor::Edge fromEdge, const std::string& anchored, Anchor::Edge toEdge)
{
    UIWidget* widget = nullptr;
    if(anchored == "self")
        widget = this;
    else if(anchored == "parent")
        widget = getParent();
    else if(anchored == "prev")
    {
        if(UIWidget* parent = getParent())
        {
            uint32_t index = parent->getChildIndex(this);
            if(index > 0) // found and is not first!
                widget = parent->getChildByIndex(index - 1);
        }
    }
    else if(anchored == "next")
    {
        if(UIWidget* parent = getParent())
        {
            uint32_t index = parent->getChildIndex(this);
            if(index > 0 && index < parent->getChildsCount()) // found and is not last!
                widget = parent->getChildByIndex(index + 1);
        }
    }
    else if(UIWidget* parent = getParent())
    {
        widget = parent->getChildById(anchored);
        if(!widget && parent->getId() == anchored)
            widget = parent;
    }

    if(widget && (widget == this || widget == m_parent || hasChild(widget) || (m_parent &&  m_parent->hasChild(widget))))
        m_anchorLayout.add(fromEdge, widget, toEdge);
    else if(!widget)
        LOGE("Anchored Widget not found: self: %s | anchored: %s", getId().c_str(), anchored.c_str());
    else
        LOGE("Anchored Widget is not parent or parents child: self: %s | anchored: %s", getId().c_str(), anchored.c_str());
}

void UIWidget::fill(std::string anchored)
{
    UIWidget* widget = nullptr;
    if(anchored == "self")
        widget = this;
    else if(anchored == "parent")
        widget = getParent();
    else if(anchored == "prev")
    {
        if(UIWidget* parent = getParent())
        {
            uint32_t index = parent->getChildIndex(this);
            if(index > 0) // found and is not first!
                widget = parent->getChildByIndex(index - 1);
        }
    }
    else if(anchored == "next")
    {
        if(UIWidget* parent = getParent())
        {
            uint32_t index = parent->getChildIndex(this);
            if(index > 0 && index < parent->getChildsCount()) // found and is not last!
                widget = parent->getChildByIndex(index + 1);
        }
    }
    else if(UIWidget* parent = getParent())
    {
        widget = parent->getChildById(anchored);
        if(!widget && parent->getId() == anchored)
            widget = parent;
    }

    if(widget && (widget == this || widget == m_parent || hasChild(widget) || (m_parent &&  m_parent->hasChild(widget))))
    {
        m_anchorLayout.removeByEdge(Anchor::VerticalCenter);
        m_anchorLayout.removeByEdge(Anchor::HorizontalCenter);

        m_anchorLayout.add(Anchor::Left, widget, Anchor::Left);
        m_anchorLayout.add(Anchor::Top, widget, Anchor::Top);
        m_anchorLayout.add(Anchor::Right, widget, Anchor::Right);
        m_anchorLayout.add(Anchor::Bottom, widget, Anchor::Bottom);
    }
    else if(!widget)
        LOGE("Anchored Widget not found: self: %s | anchored: %s", getId().c_str(), anchored.c_str());
    else
        LOGE("Anchored Widget is not parent or parents child: self: %s | anchored: %s", getId().c_str(), anchored.c_str());
}

void UIWidget::removeAnchor(Anchor::Edge fromEdge)
{
    m_anchorLayout.removeByEdge(fromEdge);
}

void UIWidget::removeAnchors()
{
    m_anchorLayout.clear();
}

void UIWidget::setAnchorMargin(Anchor::Edge fromEdge, int16_t margin)
{
    m_anchorLayout.setMargin(fromEdge, margin);
}

void UIWidget::setAnchorMargins(int16_t margin)
{
    m_anchorLayout.setMargin(Anchor::Top, margin);
    m_anchorLayout.setMargin(Anchor::Bottom, margin);
    m_anchorLayout.setMargin(Anchor::Left, margin);
    m_anchorLayout.setMargin(Anchor::Right, margin);
    m_anchorLayout.setMargin(Anchor::VerticalCenter, margin);
    m_anchorLayout.setMargin(Anchor::HorizontalCenter, margin);
}

int16_t UIWidget::getAnchorMargin(Anchor::Edge fromEdge) const
{
    return m_anchorLayout.getMargin(fromEdge);
}

void UIWidget::setAnchorScale(Anchor::Edge fromEdge, int16_t margin)
{
    m_anchorLayout.setScale(fromEdge, margin);
}

int16_t UIWidget::getAnchorScale(Anchor::Edge fromEdge) const
{
    return m_anchorLayout.getScale(fromEdge);
}

void UIWidget::addLayout(const json11::Json* json)
{
    m_jsonLayout.push_back(*json);
    applyLayout(*json);
}

void UIWidget::onLayout(const std::string& event)
{
    json11::Json layout;
    for(const auto it : m_jsonLayout)
    {
        if((layout = it[event]) || (it["Layout"] && (layout = it["Layout"][event])))
            applyLayout(layout);
    }
}

void UIWidget::applyLayout(json11::Json json)
{
    if(!m_resourceManager)
    {
        LOGE("ResourceManager not found.");
        return;
    }

    json11::Json layout = json;
    if(json["Layout"])
        layout = json["Layout"];
    else if(json["child"])
        layout = json["child"];

    json11::Json node;

    if((node = layout["skin"]))
    {
        std::string style = stdex::toString(node);
        json11::Json* jsonStyle = m_resourceManager->getLayout("Skin");

        if(jsonStyle && ((node = (*jsonStyle)[style])))
        {
			m_jsonLayout.push_back(node);
			applyLayout(node);
        }
        else
            LOGE("Skin not found: %s", style.c_str());
    }
    if((node = layout["id"]))
        setId(stdex::toString(node));
    if((node = layout["rect"]))
        setRect(stdex::toRect(node));
    if((node = layout["position"]))
        setPosition(stdex::toPoint(node));
    if((node = layout["size"]))
        setSize(stdex::toRectSize(node));
    if((node = layout["x"]))
        setX(stdex::toInteger(node));
    if((node = layout["y"]))
        setY(stdex::toInteger(node));
    if((node = layout["w"]) || (node = layout["width"]))
        setWidth(stdex::toInteger(node));
    if((node = layout["h"]) || (node = layout["height"]))
        setHeight(stdex::toInteger(node));
    if((node = layout["rotation"]))
        setRotation(stdex::toInteger(node));
    if((node = layout["opacity"]))
        setOpacity(stdex::toInteger(node));

    if((node = layout["visible"]))
        setVisible(stdex::toBoolean(node));
    if((node = layout["focusable"]))
        setFocusable(stdex::toBoolean(node));
    if((node = layout["movable"]))
        setMovable(stdex::toBoolean(node));
    if((node = layout["phantom"]))
        setPhantom(stdex::toBoolean(node));
    if((node = layout["selected"]))
        setSelected(stdex::toBoolean(node));
    if((node = layout["on"]))
        setOn(stdex::toBoolean(node));
    if((node = layout["locked"]))
        setLocked(stdex::toBoolean(node));
    if((node = layout["virtual-area"]))
        setVirtualArea(stdex::toBoolean(node));
    if((node = layout["text-auto-resize"]))
        setTextAutoResize(stdex::toBoolean(node));
    if((node = layout["fixed-rotation"]))
    	setFixedRotation(stdex::toBoolean(node));
    if((node = layout["circle"]))
    	setCircle(stdex::toBoolean(node));

    if((node = layout["image-source"]))
        setImageSource(m_resourceManager->getImage(stdex::toString(node)));
    if((node = layout["image-rect"]))
        setImageRect(stdex::toRect(node));
    if((node = layout["image-rect.x"]))
        setImageRectX(stdex::toInteger(node));
    if((node = layout["image-rect.y"]))
        setImageRectY(stdex::toInteger(node));
    if((node = layout["image-rect.w"]))
        setImageRectW(stdex::toInteger(node));
    if((node = layout["image-rect.h"]))
        setImageRectH(stdex::toInteger(node));
    if((node = layout["image-color"]))
        setImageColor(stdex::toColor(node));
    if((node = layout["image-opacity"]))
        setImageOpacity(stdex::toInteger(node));

    if((node = layout["mirror-horizontal"]))
        setHorizontalMirror(stdex::toInteger(node));
    if((node = layout["mirror-vertical"]))
        setVerticalMirror(stdex::toInteger(node));

    if((node = layout["border"]))
        setBorder(stdex::toRect(node));
    if((node = layout["border-left"]))
        setBorderLeft(stdex::toInteger(node));
    if((node = layout["border-right"]))
        setBorderRight(stdex::toInteger(node));
    if((node = layout["border-top"]))
        setBorderTop(stdex::toInteger(node));
    if((node = layout["border-bottom"]))
        setBorderBottom(stdex::toInteger(node));

    if((node = layout["border-rect"]))
        setBorderRect(stdex::toRect(node));
    if((node = layout["border-rect-left"]))
        setBorderRectLeft(stdex::toInteger(node));
    if((node = layout["border-rect-right"]))
        setBorderRectRight(stdex::toInteger(node));
    if((node = layout["border-rect-top"]))
        setBorderRectTop(stdex::toInteger(node));
    if((node = layout["border-rect-bottom"]))
        setBorderRectBottom(stdex::toInteger(node));

    if((node = layout["text-font"]))
        setTextFont(m_resourceManager->getFont(stdex::toString(node)));
    if((node = layout["text-position"]))
        setTextPosition(stdex::toPoint(node));
    if((node = layout["text-color"]))
        setTextColor(stdex::toColor(node));
    if((node = layout["text-opacity"]))
        setTextOpacity(stdex::toInteger(node));
    if((node = layout["text"]))
        setText(stdex::toString(node));

    if((node = layout["shader"]))
    	setShader(m_resourceManager->getShader(stdex::toString(node)));

    if((node = layout["anchor.Left"]))
    {
        const auto& items = node.array_items();
        setAnchor(Anchor::Left, stdex::toString(items[0]), stdex::toEdge(items[1]));
    }
    if((node = layout["anchor.Right"]))
    {
        const auto& items = node.array_items();
        setAnchor(Anchor::Right, stdex::toString(items[0]), stdex::toEdge(items[1]));
    }
    if((node = layout["anchor.Top"]))
    {
        const auto& items = node.array_items();
        setAnchor(Anchor::Top, stdex::toString(items[0]), stdex::toEdge(items[1]));
    }
    if((node = layout["anchor.Bottom"]))
    {
        const auto& items = node.array_items();
        setAnchor(Anchor::Bottom, stdex::toString(items[0]), stdex::toEdge(items[1]));
    }
    if((node = layout["anchor.HorizontalCenter"]))
    {
        const auto& items = node.array_items();
        setAnchor(Anchor::HorizontalCenter, stdex::toString(items[0]), stdex::toEdge(items[1]));
    }
    if((node = layout["anchor.VerticalCenter"]))
    {
        const auto& items = node.array_items();
        setAnchor(Anchor::VerticalCenter, stdex::toString(items[0]), stdex::toEdge(items[1]));
    }
    if((node = layout["anchor.Height"]))
    {
        const auto& items = node.array_items();
        setAnchor(Anchor::Height, stdex::toString(items[0]), stdex::toEdge(items[1]));
    }
    if((node = layout["anchor.Width"]))
    {
        const auto& items = node.array_items();
        setAnchor(Anchor::Width, stdex::toString(items[0]), stdex::toEdge(items[1]));
    }
    if((node = layout["fill"]))
        fill(stdex::toString(node));
    if((node = layout["margin"]))
    {
        int32_t margin = stdex::toInteger(node);
        setAnchorMargin(Anchor::Left, margin);
        setAnchorMargin(Anchor::Right, margin);
        setAnchorMargin(Anchor::Top, margin);
        setAnchorMargin(Anchor::Bottom, margin);
    }
    if((node = layout["margin-Left"]))
    	setAnchorMargin(Anchor::Left, stdex::toInteger(node));
    if((node = layout["margin-Right"]))
    	setAnchorMargin(Anchor::Right, stdex::toInteger(node));
    if((node = layout["margin-Top"]))
    	setAnchorMargin(Anchor::Top, stdex::toInteger(node));
    if((node = layout["margin-Bottom"]))
    	setAnchorMargin(Anchor::Bottom, stdex::toInteger(node));
    if((node = layout["margin-VerticalCenter"]))
    	setAnchorMargin(Anchor::VerticalCenter, stdex::toInteger(node));
    if((node = layout["margin-HorizontalCenter"]))
        setAnchorMargin(Anchor::HorizontalCenter, stdex::toInteger(node));
    if((node = layout["margin-Width"]))
        setAnchorMargin(Anchor::Width, stdex::toInteger(node));
    if((node = layout["margin-Height"]))
        setAnchorMargin(Anchor::Height, stdex::toInteger(node));
    if((node = layout["scale-Left"]))
    	setAnchorScale(Anchor::Left, stdex::toInteger(node));
    if((node = layout["scale-Right"]))
    	setAnchorScale(Anchor::Right, stdex::toInteger(node));
    if((node = layout["scale-Top"]))
    	setAnchorScale(Anchor::Top, stdex::toInteger(node));
    if((node = layout["scale-Bottom"]))
    	setAnchorScale(Anchor::Bottom, stdex::toInteger(node));
    if((node = layout["scale-VerticalCenter"]))
    	setAnchorScale(Anchor::VerticalCenter, stdex::toInteger(node));
    if((node = layout["scale-HorizontalCenter"]))
    	setAnchorScale(Anchor::HorizontalCenter, stdex::toInteger(node));
    if((node = layout["scale-Width"]))
    	setAnchorScale(Anchor::Width, stdex::toInteger(node));
    if((node = layout["scale-Height"]))
    	setAnchorScale(Anchor::Height, stdex::toInteger(node));
    if((node = layout["offset"]))
        setOffset(stdex::toPoint(node));
    if((node = layout["offset.x"]))
        setOffset({ getOffset().x, (int32_t)stdex::toInteger(node) });
    if((node = layout["offset.y"]))
        setOffset({ (int32_t)stdex::toInteger(node), getOffset().y });

    if((node = layout["@visible"]) && isVisible())
        applyLayout(node);
    if((node = layout["!visible"]) && !isVisible())
        applyLayout(node);
    if((node = layout["@phantom"]) && isPhantom())
        applyLayout(node);
    if((node = layout["!phantom"]) && !isPhantom())
        applyLayout(node);
    if((node = layout["@selected"]) && isLocked())
        applyLayout(node);
    if((node = layout["!selected"]) && !isLocked())
        applyLayout(node);
    if((node = layout["@on"]) && isOn())
        applyLayout(node);
    if((node = layout["!on"]) && !isOn())
        applyLayout(node);
    if((node = layout["@locked"]) && isLocked())
        applyLayout(node);
    if((node = layout["!locked"]) && !isLocked())
        applyLayout(node);
    if((node = layout["@circle"]) && isCircle())
        applyLayout(node);
    if((node = layout["!circle"]) && !isCircle())
        applyLayout(node);

    if((node = layout["ex-child"]))
    {
        UIWidget* child = nullptr;

        for(auto it : node.array_items())
        {
            if(auto idNode = it["Layout"]["id"])
            {
				if((child = getChildById(stdex::toString(idNode))))
				{
					child->m_jsonLayout.push_back(it);
					child->applyLayout(it);
				}
            }
            else
            	LOGE("ex-child.Layout.id not found.");
        }
    }

    if((node = layout["child"]))
    {
        UIWidget* child = nullptr;

        for(auto it : node.array_items())
        {
            std::string className = "";
            if(auto classNode = it["Layout"]["class"])
                className = stdex::toString(classNode);

            if(!className.empty() && g_lua[className] && g_lua[className]["Create"])
            {
                child = g_lua[className]["Create"].call<UIWidget*>();
                child->setParent(this);
            }
            else
                child = doCreateChild();

            child->setResourceManager(getResourceManager());

			child->m_jsonLayout.push_back(it);
			child->applyLayout(it);
        }
    }

    std::string className = getMetatable();
    if(g_lua[className])
    {
        if(Lua::Object object = g_lua[className]["onJson"])
        {
            if(object.isFunction())
                g_lua[className]["onJson"].call(this, &layout);
        }
    }
}
