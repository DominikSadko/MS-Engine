#include "framework/core/platform.h"
#include "framework/ui/anchor.h"
#include "framework/ui/uiwidget.h"

void UIAnchorLayout::add(Anchor::Edge fromEdge, UIWidget* anchored, Anchor::Edge toEdge)
{
    // Just update fromEdge
	removeByEdge(fromEdge);

    Anchor* anchor = new Anchor();
    anchor->fromEdge = fromEdge;
    anchor->toEdge = toEdge;
    anchor->scale = false;
    m_anchors.insert(std::pair<UIWidget*, Anchor*>(anchored, anchor));
}

void UIAnchorLayout::clear()
{
    for(auto it : m_anchors)
        delete it.second;

    m_anchors.clear();
}

void UIAnchorLayout::removeByWidget(UIWidget* anchored)
{
    if(m_anchors.empty())
        return;

    auto ret = m_anchors.equal_range(anchored);
    for(auto it = ret.first; it != ret.second; ++it)
        delete it->second;

    m_anchors.erase(anchored);
}

void UIAnchorLayout::removeByEdge(Anchor::Edge edge)
{
    if(m_anchors.empty())
        return;

    for(auto it = m_anchors.begin(); it != m_anchors.end(); ++it)
    {
        if(it->second->fromEdge == edge)
        {
            delete it->second;
            m_anchors.erase(it);
            break;
        }
    }
}

void UIAnchorLayout::update(UIWidget* widget)
{
    bool horizontalResize = false, verticalResize = false;
    int32_t edge = 0;

    for(const auto& it : m_anchors)
    {
    	edge = getHookedEdge((Anchor::Edge)it.second->toEdge, widget, it.first);
        switch(it.second->fromEdge)
        {
            case Anchor::Left:
            {
                if(!horizontalResize)
                {
                    widget->setX((!it.second->scale ? edge + it.second->margin : edge * it.second->margin / 100) + m_offset.x);
                    horizontalResize = true;
                }
                else
                    widget->moveLeftBorder(widget->getX() - (!it.second->scale ? edge + it.second->margin : edge * it.second->margin / 100) + m_offset.x);

                break;
            }
            case Anchor::Right:
            {
                if(!horizontalResize)
                {
                    widget->setX((!it.second->scale ? edge - it.second->margin : edge - edge * it.second->margin / 100) - widget->getWidth() + m_offset.x);
                    horizontalResize = true;
                }
                else
                    widget->setWidth((!it.second->scale ? edge - it.second->margin : edge - edge * it.second->margin / 100) - widget->getX() + m_offset.x);

                break;
            }
            case Anchor::Top:
            {
                if(!verticalResize)
                {
                    widget->setY((!it.second->scale ? edge + it.second->margin : edge * it.second->margin / 100) + m_offset.y);
                    verticalResize = true;
                }
                else
                    widget->moveTopBorder(widget->getY() - (!it.second->scale ? edge + it.second->margin : edge * it.second->margin / 100) + m_offset.y);

                break;
            }
            case Anchor::Bottom:
            {
                if(!verticalResize)
                {
                    widget->setY((!it.second->scale ? edge - it.second->margin : edge - edge * it.second->margin / 100) - widget->getHeight() + m_offset.y);
                    verticalResize = true;
                }
                else
                    widget->setHeight((!it.second->scale ? edge - it.second->margin : edge - edge * it.second->margin / 100) - widget->getY() + m_offset.y);

                break;
            }
            case Anchor::HorizontalCenter:
            {
                if(!horizontalResize)
                {
                    widget->setX((!it.second->scale ? edge + it.second->margin : edge * it.second->margin / 100) - widget->getWidth() / 2 + m_offset.x);
                    horizontalResize = true;
                }
                else
                    widget->setWidth((!it.second->scale ? edge + it.second->margin : edge * it.second->margin / 100) - widget->getX() + m_offset.x);

                break;
            }
            case Anchor::VerticalCenter:
            {
                if(!verticalResize)
                {
                    widget->setY((!it.second->scale ? edge + it.second->margin : edge * it.second->margin / 100) - widget->getHeight() / 2 + m_offset.y);
                    verticalResize = true;
                }
                else
                    widget->setHeight((!it.second->scale ? edge + it.second->margin : edge * it.second->margin / 100) - widget->getY() + m_offset.y);

                break;
            }
            case Anchor::Width:
            {
                widget->setWidth((!it.second->scale ? edge + it.second->margin : edge * it.second->margin / 100) + m_offset.y);
                break;
            }
            case Anchor::Height:
            {
                widget->setHeight((!it.second->scale ? edge + it.second->margin : edge * it.second->margin / 100) + m_offset.y);
                break;
            }
            default:
                break;
        }
    }
}

int32_t UIAnchorLayout::getHookedEdge(Anchor::Edge edge, const UIWidget* widget, const UIWidget* anchored) const
{
    switch(edge)
    {
        case Anchor::Left:
            return widget->getParent() == anchored ? 0 : anchored->getX();
        case Anchor::Right:
            return anchored->getWidth() + getHookedEdge(Anchor::Left, widget, anchored);
        case Anchor::Top:
            return widget->getParent() == anchored ? 0 : anchored->getY();
        case Anchor::Bottom:
            return anchored->getHeight() + getHookedEdge(Anchor::Top, widget, anchored);
        case Anchor::HorizontalCenter:
            return anchored->getWidth() / 2 + getHookedEdge(Anchor::Left, widget, anchored);
        case Anchor::VerticalCenter:
            return anchored->getHeight() / 2 + getHookedEdge(Anchor::Top, widget, anchored);
        case Anchor::Width:
            return anchored->getWidth();
        case Anchor::Height:
            return anchored->getHeight();
        default:
            break;
    }

    return 0;
}

void UIAnchorLayout::setMargin(Anchor::Edge fromEdge, int16_t margin)
{
    for(auto it : m_anchors)
    {
        if(it.second->fromEdge == fromEdge)
        {
            it.second->margin = margin;
            it.second->scale = false;
            break;
        }
    }
}

int16_t UIAnchorLayout::getMargin(Anchor::Edge fromEdge) const
{
    for(auto it : m_anchors)
    {
        if(it.second->fromEdge == fromEdge)
            return it.second->margin;
    }

    return 0;
}

void UIAnchorLayout::setScale(Anchor::Edge fromEdge, int16_t scale)
{
    for(auto it : m_anchors)
    {
        if(it.second->fromEdge == fromEdge)
        {
            it.second->margin = scale;
            it.second->scale = true;
            break;
        }
    }
}

int16_t UIAnchorLayout::getScale(Anchor::Edge fromEdge) const
{
    for(auto it : m_anchors)
    {
        if(it.second->fromEdge == fromEdge)
            return it.second->margin;
    }

    return 0;
}
