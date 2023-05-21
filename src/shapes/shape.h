#pragma once

#include <wx/graphics.h>

struct Shape
{
    virtual void Draw(wxGraphicsContext &gc) const = 0;
    virtual void HandleCreationByMouseDrag(wxPoint currentDragPoint) = 0;
    virtual ~Shape() noexcept {};
};