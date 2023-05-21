#include "drawingdocument.h"
#include "streamutils.h"

wxIMPLEMENT_DYNAMIC_CLASS(DrawingDocument, wxDocument);

std::ostream &DrawingDocument::SaveObject(std::ostream &stream)
{
    auto doc = serializer.SerializePaths(squiggles);

    auto wrapper = OStreamWrapper(stream);
    serializer.CompressXml(doc, wrapper);
    return stream;
}

std::istream &DrawingDocument::LoadObject(std::istream &stream)
{
    auto wrapper = IStreamWrapper(stream);
    auto doc = serializer.DecompressXml(wrapper);

    squiggles = serializer.DeserializePaths(doc);

    // workaround for wxWidgets problem: https://github.com/wxWidgets/wxWidgets/issues/23479
    stream.clear();

    return stream;
}