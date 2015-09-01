#ifndef __WIDGET_H__
#define __WIDGET_H__

#include "framework/core/platform.h"
#include "framework/graphic/graphic.h"
#include "framework/font/font.h"
#include "framework/ui/uiroot.h"
#include "framework/ui/anchor.h"
#include "framework/ui/uicondition.h"
#include "framework/core/resourceManager.h"
#include "framework/lua/lua.h"

#include <json/json11.hpp>
#include <stdex/flag.h>

enum WidgetFlags
{
    // Core: uint16_t
    Visible          = (1 << 0),
    Focusable        = (1 << 1),
    Movable          = (1 << 2),
    Phantom          = (1 << 3),
    Selected         = (1 << 4),
    On               = (1 << 5),
    Lock             = (1 << 6),
    VirtualArea      = (1 << 7),
    TextAutoResize   = (1 << 8),
    FixedRotation    = (1 << 9),
    Circle           = (1 << 10),

    // Image: uint8_t
    HorizontalMirror = (1 << 0),
    VerticalMirror   = (1 << 1)
};

class UIWidget : public UIRoot
{
    public:
        static int32_t AutoId;


        /////////////////       Core       /////////////////

    public:
        UIWidget();
        virtual ~UIWidget();
        virtual void remove();

        virtual void draw(const Point& offset, float scale);
        void drawPolygon(const Point& offset)  const;
        void drawImage(const Point& offset)    const;
        void drawBorder(const Point& offset)   const;
        void drawText(const Point& offset)     const;

        void setId(const std::string& id)  { m_id = id; }
        void setRect(const Rect& rect)     { m_rect = rect; }
        void setPosition(const Point& pos) { m_rect.x = pos.x; m_rect.y = pos.y; }
        void setSize(const Rect& rect)     { m_rect.w = rect.w; m_rect.h = rect.h; }
        void setX(int32_t x)               { m_rect.x = x; }
        void setY(int32_t y)               { m_rect.y = y; }
        void setWidth(int32_t w)           { m_rect.w = w; }
        void setHeight(int32_t h)          { m_rect.h = h; }
        void moveLeftBorder(int32_t x)     { m_rect.x -= x; m_rect.w += x; }
        void moveTopBorder(int32_t y)      { m_rect.y -= y; m_rect.h += y; }
        void setPolygon(const stdex::Vertices& vertices);
        void setRotation(uint16_t angle)   { m_rotation = angle % 360; }
        void setOpacity(uint8_t opacity)   { m_opacity = opacity; }

        std::string  getId()               const { return m_id; }
        int32_t      getUniqueId()         const { return m_uniqueId; }
        Point        getPosition()         const { return {m_rect.x, m_rect.y}; }
        Point        getCenterPosition()   const { return {m_rect.x + m_rect.w / 2, m_rect.y + m_rect.h / 2}; }
        Rect         getSize()             const { return {0, 0, m_rect.w, m_rect.h}; }
        Rect         getRect()             const { return m_rect; }
        Rect         getVirtualRect()      const;
        int32_t      getX()                const { return m_rect.x; }
        int32_t      getY()                const { return m_rect.y; }
        int32_t      getWidth()            const { return m_rect.w; }
        int32_t      getHeight()           const { return m_rect.h; }
        uint16_t     getRotation()         const { return m_rotation; }
        uint16_t     getFixedRotation()    const { return (!isFixedRotation() || !m_parent) ? m_rotation : m_parent->getRotation() + m_rotation; }
        uint8_t      getOpacity()          const { return m_opacity; }

        // Flags
        void setVisible(bool value)
        {
            m_flags.set(WidgetFlags::Visible, value);
            if(value == true && m_opacity == 0)
                m_opacity = 0xFF;

            value ? onLayout("@visible") : onLayout("!visible");
        }

        void setFocusable(bool value)      { m_flags.set(WidgetFlags::Focusable, value); }
        void setMovable(bool value)        { m_flags.set(WidgetFlags::Movable, value); }
        void setPhantom(bool value)        { m_flags.set(WidgetFlags::Phantom, value);       value ? onLayout("@phantom")  : onLayout("!phantom"); }
        void setSelected(bool value)       { m_flags.set(WidgetFlags::Selected, value);      value ? onLayout("@selected") : onLayout("!selected"); }
        void setOn(bool value)             { m_flags.set(WidgetFlags::On, value);            value ? onLayout("@on")       : onLayout("!on"); }
        void setLocked(bool value)         { m_flags.set(WidgetFlags::Lock, value);          value ? onLayout("@locked")   : onLayout("!locked"); }
        void setVirtualArea(bool value)    { m_flags.set(WidgetFlags::VirtualArea, value); }
        void setTextAutoResize(bool value) { m_flags.set(WidgetFlags::TextAutoResize, value); }
        void setFixedRotation(bool value)  { m_flags.set(WidgetFlags::FixedRotation, value); }
        void setCircle(bool value)         { m_flags.set(WidgetFlags::Circle, value);        value ? onLayout("@circle")   : onLayout("!circle"); }

        bool isVisible()             const { return m_flags.has(WidgetFlags::Visible) && m_opacity; }
        bool isFocusable()           const { return m_flags.has(WidgetFlags::Focusable); }
        bool isMovable()             const { return m_flags.has(WidgetFlags::Movable); }
        bool isPhantom()             const { return m_flags.has(WidgetFlags::Phantom); }
        bool isSelected()            const { return m_flags.has(WidgetFlags::Selected); }
        bool isOn()                  const { return m_flags.has(WidgetFlags::On); }
        bool isLocked()              const { return m_flags.has(WidgetFlags::Lock); }
        bool isVirtualArea()         const { return m_flags.has(WidgetFlags::VirtualArea); }
        bool isTextAutoResize()      const { return m_flags.has(WidgetFlags::TextAutoResize); }
        bool isFixedRotation()       const { return m_flags.has(WidgetFlags::FixedRotation); }
        bool isCircle()              const { return m_flags.has(WidgetFlags::Circle); }

        bool focus();
        bool grab();

    protected:
        int32_t m_uniqueId;
        std::string m_id;

        Rect m_rect;
        stdex::Vertices m_vertices;
        int32_t m_polygonWidth, m_polygonHeight;

        uint16_t m_rotation;
        uint8_t m_opacity;
        stdex::flag<uint16_t> m_flags;


        /////////////////       Events       ////////////////
    public:
        virtual bool onMouseHover(uint8_t id, bool hovered);
        virtual bool onMousePress(uint8_t id, const Point& pos, bool executed);

        /////////////////       Childs       ////////////////

    public:
        UIWidget* doCreateChild();
        UIWidget* doCreateJsonChild(const json11::Json* json);
        void setParent(UIWidget* parent);
        void addChild(UIWidget* child);
        void eraseChild(UIWidget* child);
        void removeChilds();
        void removeChild(UIWidget* child);
        void popChild(UIWidget* child);

        bool hasChild(UIWidget* child);
        UIWidget* getChildById(const std::string& id) const;
        UIWidget* getChildByRecursiveId(const std::string& id) const;
        UIWidget* getChildByPoint(const Point& point);
        UIWidget* getChildByIndex(uint32_t index) const;
        int32_t   getChildIndex(UIWidget* child) const;
        uint32_t  getChildsCount() const;
        UIWidget* getParent() const { return m_parent; }
        UIWidget* getParentByRecursiveId(const std::string& id) const;
        const std::vector<UIWidget*>& getChilds() const { return m_childs; }

    protected:
        UIWidget* m_parent;
        std::vector<UIWidget*> m_childs;


        /////////////////       Image       /////////////////

    public:
        void setImageSource(Image* image)
        {
            if(!m_imageSource && image && (!m_imageRect.w || !m_imageRect.h))
                m_imageRect = image->getRect();

            m_imageSource = image;
        }
        void setImageRect(const Rect& rect)       { m_imageRect = rect; }
        void setImageRectX(int32_t x)             { m_imageRect.x = x; }
        void setImageRectY(int32_t y)             { m_imageRect.y = y; }
        void setImageRectW(int32_t w)             { m_imageRect.w = w; }
        void setImageRectH(int32_t h)             { m_imageRect.h = h; }
        void setImageColor(const Color& color)    { m_imageColor = color; }
        void setColor(const Color& color)         { m_imageColor = color; }
        void setImageOpacity(uint8_t alpha)       { m_imageColor.a = alpha; }
        void setImageCoords(const stdex::Vertices& vertices);

        Image*  getImageSource()            const { return m_imageSource; }
        Rect    getImageRect()              const { return m_imageRect; }
        int32_t getImageRectX()             const { return m_imageRect.x; }
        int32_t getImageRectY()             const { return m_imageRect.y; }
        int32_t getImageRectW()             const { return m_imageRect.w; }
        int32_t getImageRectH()             const { return m_imageRect.h; }
        Color   getImageColor()             const { return m_imageColor; }
        Color   getColor()                  const { return m_imageColor; }
        uint8_t getImageOpacity()           const { return m_imageColor.a; }

    protected:
        Image* m_imageSource;
        Rect m_imageRect;
        Color m_imageColor;
        stdex::Vertices m_coords;

    public:
        void setHorizontalMirror(bool value)      { m_mirror.set(WidgetFlags::HorizontalMirror, value); }
        void setVerticalMirror(bool value)        { m_mirror.set(WidgetFlags::VerticalMirror, value); }

        bool isHorizontalMirror()           const { return m_mirror.has(WidgetFlags::HorizontalMirror); }
        bool isVerticalMirror()             const { return m_mirror.has(WidgetFlags::VerticalMirror); }

    protected:
        stdex::flag<uint8_t> m_mirror;

        /////////////////       Border       /////////////////

    public:
        void setBorder(const Rect& rect)     { m_border = rect; }
        void setBorderLeft(uint8_t border)   { m_border.x = border; }
        void setBorderTop(uint8_t border)    { m_border.y = border; }
        void setBorderRight(uint8_t border)  { m_border.w = border; }
        void setBorderBottom(uint8_t border) { m_border.h = border; }

        void setBorderRect(const Rect& rect)     { m_borderRect = rect; }
        void setBorderRectLeft(uint8_t border)   { m_borderRect.x = border; }
        void setBorderRectTop(uint8_t border)    { m_borderRect.y = border; }
        void setBorderRectRight(uint8_t border)  { m_borderRect.w = border; }
        void setBorderRectBottom(uint8_t border) { m_borderRect.h = border; }

        Rect    getBorder()            const { return m_border; }
        uint8_t getBorderLeft()        const { return m_border.x; }
        uint8_t getBorderTop()         const { return m_border.y; }
        uint8_t getBorderRight()       const { return m_border.w; }
        uint8_t getBorderBottom()      const { return m_border.h; }

    protected:
        Rect m_border;
        Rect m_borderRect;


        /////////////////       Text       /////////////////

    public:
        void setText(std::string text);
        void setTextFont(Font* font)             { m_textFont = font; }
        void setTextPosition(const Point& point) { m_textPosition = point; }
        void setTextColor(const Color& color)    { m_textColor = color; }
        void setTextOpacity(uint8_t alpha)       { m_textColor.a = alpha; }

        std::string getText()              const { return m_text; }
        Font* getTextFont()                const { return m_textFont; }
        Point getTextPosition()            const { return m_textPosition; }
        Color getTextColor()               const { return m_textColor; }
        uint8_t getTextOpacity()           const { return m_textColor.a; }

    protected:
        std::string m_text;
        Font* m_textFont;
        TextCache* m_textCache;
        Point m_textPosition;
        Color m_textColor;

        /////////////////       Shader      /////////////////

    public:
        void setShader(Shader* shader) { m_shader = shader; }
        Shader* getShader() const { return m_shader; }

    protected:
        Shader* m_shader;

        /////////////////       Anchor Layout      /////////////////

    public:
        void setAnchor(Anchor::Edge fromEdge, const std::string& anchored, Anchor::Edge toEdge);
        void fill(std::string anchored);
        void removeAnchor(Anchor::Edge fromEdge);
        void removeAnchors();

        void updateAnchors();

        void setOffset(const Point& offset) { m_anchorLayout.setOffset(offset); };
        Point getOffset() const { return m_anchorLayout.getOffset(); };
        void setAnchorMargin(Anchor::Edge fromEdge, int16_t margin);
        void setAnchorMargins(int16_t margin);
        int16_t getAnchorMargin(Anchor::Edge fromEdge) const;
        void setAnchorScale(Anchor::Edge fromEdge, int16_t margin);
        int16_t getAnchorScale(Anchor::Edge fromEdge) const;

    protected:
        UIAnchorLayout m_anchorLayout;

        /////////////////       Json Layout      /////////////////

    public:
        void setResourceManager(ResourceManager* resourceManager) { m_resourceManager = resourceManager; };
        ResourceManager* getResourceManager() const { return m_resourceManager; };

        void addLayout(const json11::Json* json);
        void onLayout(const std::string& event);
        void applyLayout(json11::Json json);

    protected:
        ResourceManager* m_resourceManager;
        std::vector<json11::Json> m_jsonLayout;

	public:
        void addCondition(UICondition* condition)
        {
        	removeCondition(condition->getType());
        	m_conditions.push_back(condition);
        }

        void removeCondition(uint8_t type)
        {
        	for(auto it = m_conditions.begin(); it != m_conditions.end(); it++)
        	{
        		if((*it)->getType() == type)
        		{
        			m_conditions.erase(it);
        			return;
        		}
        	}
        }

	private:
        std::vector<UICondition*> m_conditions;
};


#endif // __WIDGET_H__ //
