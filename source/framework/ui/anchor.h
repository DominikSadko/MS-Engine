#ifndef __ANCHOR_H__
#define __ANCHOR_H__

#include <map>

class UIWidget;
struct Anchor
{
    enum Edge
    {
        None = 0,
        Top = 1,
        Bottom = 2,
        Left = 3,
        Right = 4,
        VerticalCenter = 5,
        HorizontalCenter = 6,
        Width = 11,
        Height = 12
    };

    Anchor()
    {
        fromEdge = toEdge = Anchor::None;
        margin = 0;
        scale = false;
    }

    uint8_t fromEdge, toEdge;
    int16_t margin;
    bool scale;
};

struct UIAnchorLayout
{
    public:
        void add(Anchor::Edge fromEdge, UIWidget* anchored, Anchor::Edge toEdge);
        void clear();
        void removeByWidget(UIWidget* anchored);
        void removeByEdge(Anchor::Edge edge);

        void update(UIWidget* widget);

        void setOffset(const Point& offset) { m_offset = offset; };
        Point getOffset() const { return m_offset; };
        void setMargin(Anchor::Edge fromEdge, int16_t margin);
        int16_t getMargin(Anchor::Edge fromEdge) const;
        void setScale(Anchor::Edge fromEdge, int16_t scale);
        int16_t getScale(Anchor::Edge fromEdge) const;

    private:
        int32_t getHookedEdge(Anchor::Edge edge, const UIWidget* widget, const UIWidget* anchored) const;

    private:
        std::multimap<UIWidget*, Anchor*> m_anchors;
        Point m_offset;
};

#endif // __ANCHOR_H__ //
