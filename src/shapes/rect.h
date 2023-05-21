#pragma once

#include <wx/graphics.h>

#include "shape.h"

struct Rect : Shape
{
    wxRect2DDouble rect;
    wxColour color;

    Rect() = default;
    Rect(const wxColour &color, const wxPoint2DDouble origin)
        : rect{origin.m_x, origin.m_y, 0.0, 0.0}, color{color}
    {
    }

    ~Rect() noexcept = default;

    void Draw(wxGraphicsContext &gc) const override
    {
        gc.SetPen(wxPen(color));
        gc.SetBrush(wxBrush(color));
        gc.DrawRectangle(rect.m_x, rect.m_y, rect.m_width, rect.m_height);
    }

    void HandleCreationByMouseDrag(wxPoint currentDragPoint) override
    {
        rect.SetRightBottom(currentDragPoint);
    }

    wxXmlNode *Serialize() const override
    {
        wxXmlNode *mainNode = new wxXmlNode(wxXML_ELEMENT_NODE, SerializationNodeName());
        mainNode->AddAttribute("type", SerializationNodeType());
        mainNode->AddAttribute("color", color.GetAsString(wxC2S_HTML_SYNTAX));

        wxXmlNode *rectNode = new wxXmlNode(wxXML_ELEMENT_NODE, "Rect");
        rectNode->AddAttribute("x", wxString::FromDouble(rect.m_x));
        rectNode->AddAttribute("y", wxString::FromDouble(rect.m_y));
        rectNode->AddAttribute("width", wxString::FromDouble(rect.m_width));
        rectNode->AddAttribute("height", wxString::FromDouble(rect.m_height));

        mainNode->AddChild(rectNode);

        return mainNode;
    }

    void Deserialize(const wxXmlNode *node) override
    {
        color.Set(node->GetAttribute("color"));

        const wxXmlNode *rectNode = node->GetChildren();
        rect.m_x = wxAtof(rectNode->GetAttribute("x"));
        rect.m_y = wxAtof(rectNode->GetAttribute("y"));
        rect.m_width = wxAtof(rectNode->GetAttribute("width"));
        rect.m_height = wxAtof(rectNode->GetAttribute("height"));
    }

    static wxString SerializationNodeType()
    {
        return "Rect";
    }
};