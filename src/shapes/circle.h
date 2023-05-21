#pragma once

#include <wx/graphics.h>

#include "shape.h"

struct Circle : Shape
{
    double radius;
    wxPoint2DDouble center;
    wxColour color;

    Circle() = default;
    Circle(const wxColour &color, const wxPoint2DDouble &center) : radius{0.0}, center{center}, color{color}
    {
    }

    ~Circle() noexcept = default;

    void Draw(wxGraphicsContext &gc) const override
    {
        gc.SetPen(wxPen(color));
        gc.SetBrush(wxBrush(color));
        gc.DrawEllipse(center.m_x - radius, center.m_y - radius, radius * 2, radius * 2);
    }

    void HandleCreationByMouseDrag(wxPoint currentDragPoint) override
    {
        radius = std::sqrt(std::pow(currentDragPoint.x - center.m_x, 2) + std::pow(currentDragPoint.y - center.m_y, 2));
    }

    wxXmlNode *Serialize() const override
    {
        wxXmlNode *mainNode = new wxXmlNode(wxXML_ELEMENT_NODE, SerializationNodeName());
        mainNode->AddAttribute("type", SerializationNodeType());
        mainNode->AddAttribute("color", color.GetAsString(wxC2S_HTML_SYNTAX));
        mainNode->AddAttribute("radius", wxString::FromDouble(radius));

        wxXmlNode *centerNode = new wxXmlNode(wxXML_ELEMENT_NODE, "Center");
        centerNode->AddAttribute("x", wxString::FromDouble(center.m_x));
        centerNode->AddAttribute("y", wxString::FromDouble(center.m_y));

        mainNode->AddChild(centerNode);

        return mainNode;
    }

    void Deserialize(const wxXmlNode *node) override
    {
        color.Set(node->GetAttribute("color"));
        radius = wxAtof(node->GetAttribute("radius"));

        const wxXmlNode *centerNode = node->GetChildren();
        center.m_x = wxAtof(centerNode->GetAttribute("x"));
        center.m_y = wxAtof(centerNode->GetAttribute("y"));
    }

    static wxString SerializationNodeType()
    {
        return "Circle";
    }
};