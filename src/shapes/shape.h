#pragma once

#include <wx/graphics.h>
#include <wx/xml/xml.h>

struct Shape
{
    virtual void Draw(wxGraphicsContext &gc) const = 0;
    virtual void HandleCreationByMouseDrag(wxPoint currentDragPoint) = 0;
    virtual ~Shape() noexcept {};

    virtual wxXmlNode *Serialize() const = 0;
    virtual void Deserialize(const wxXmlNode *node) = 0;

    static wxString SerializationNodeName()
    {
        return "Object";
    }
};