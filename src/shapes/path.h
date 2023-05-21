#pragma once

#include <wx/wx.h>

#include <vector>

#include "shape.h"

struct Path : Shape
{
    std::vector<wxPoint2DDouble> points;
    wxColour color;
    int width;

    Path() = default;
    Path(const wxColour &color, int width, const wxPoint2DDouble firstPoint) : points{firstPoint}, color{color}, width{width}
    {
    }

    ~Path() noexcept = default;

    void Draw(wxGraphicsContext &gc) const override
    {
        if (points.size() > 1)
        {
            gc.SetPen(wxPen(color, width));
            gc.StrokeLines(points.size(), points.data());
        }
    }

    void HandleCreationByMouseDrag(wxPoint currentDragPoint) override
    {
        points.push_back(wxPoint2DDouble(currentDragPoint.x, currentDragPoint.y));
    }

    wxXmlNode *Serialize() const override
    {
        wxXmlNode *mainNode = new wxXmlNode(wxXML_ELEMENT_NODE, SerializationNodeName());

        mainNode->AddAttribute("type", SerializationNodeType());
        mainNode->AddAttribute("color", color.GetAsString(wxC2S_HTML_SYNTAX));
        mainNode->AddAttribute("width", wxString::FromDouble(width));

        for (const auto &point : points)
        {
            wxXmlNode *pointNode = new wxXmlNode(wxXML_ELEMENT_NODE, "Point");
            pointNode->AddAttribute("x", wxString::FromDouble(point.m_x));
            pointNode->AddAttribute("y", wxString::FromDouble(point.m_y));
            mainNode->AddChild(pointNode);
        }

        return mainNode;
    }

    void Deserialize(const wxXmlNode *node) override
    {
        color = wxColor(node->GetAttribute("color"));
        width = wxAtof(node->GetAttribute("width"));
        points = {};

        for (wxXmlNode *pointNode = node->GetChildren(); pointNode; pointNode = pointNode->GetNext())
        {
            if (pointNode->GetName() != "Point")
                continue;

            points.push_back(wxPoint2DDouble(wxAtof(pointNode->GetAttribute("x")),
                                             wxAtof(pointNode->GetAttribute("y"))));
        }
    }

    static wxString SerializationNodeType()
    {
        return "Path";
    }
};