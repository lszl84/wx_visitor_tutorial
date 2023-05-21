#pragma once

class Circle;
class Rect;
class Path;

class ShapeVisitor
{
public:
    virtual void Visit(const Circle &circle) = 0;
    virtual void Visit(const Rect &rectangle) = 0;
    virtual void Visit(const Path &rectangle) = 0;
};