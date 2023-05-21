#pragma once

#include <wx/xml/xml.h>
#include <wx/fs_zip.h>
#include <wx/zipstrm.h>
#include <wx/wfstream.h>

#include <memory>

#include "shapes/shape.h"
#include "shapes/path.h"
#include "shapes/rect.h"
#include "shapes/circle.h"

struct XmlSerializer
{
    XmlSerializer()
    {
        wxFileSystem::AddHandler(new wxZipFSHandler);
    }

    wxXmlDocument SerializeShapes(const std::vector<std::unique_ptr<Shape>> &objects)
    {
        wxXmlDocument doc;

        wxXmlNode *docNode = new wxXmlNode(wxXML_ELEMENT_NODE, "PaintDocument");
        docNode->AddAttribute("version", "1.2");

        for (const auto &obj : objects)
        {
            docNode->AddChild(obj->Serialize());
        }

        doc.SetRoot(docNode);

        return doc;
    }

    std::vector<std::unique_ptr<Shape>> DeserializeShapes(const wxXmlDocument &doc)
    {
        wxXmlNode *root = doc.GetRoot();

        std::vector<std::unique_ptr<Shape>> objects;

        for (wxXmlNode *node = root->GetChildren(); node; node = node->GetNext())
        {
            if (node->GetName() != "Object")
                continue;

            if (node->GetAttribute("type") == Path::SerializationNodeType())
            {
                Path squiggle;
                squiggle.Deserialize(node);

                objects.emplace_back(std::make_unique<Path>(squiggle));
            }
            else if (node->GetAttribute("type") == Rect::SerializationNodeType())
            {
                Rect rect;
                rect.Deserialize(node);

                objects.emplace_back(std::make_unique<Rect>(rect));
            }
            else if (node->GetAttribute("type") == Circle::SerializationNodeType())
            {
                Circle circle;
                circle.Deserialize(node);

                objects.emplace_back(std::make_unique<Circle>(circle));
            }
        }

        return objects;
    }

    void CompressXml(const wxXmlDocument &doc, wxOutputStream &outStream)
    {
        wxZipOutputStream zip(outStream);

        zip.PutNextEntry("paintdocument.xml");
        doc.Save(zip);

        zip.CloseEntry();

        zip.Close();
    }

    void CompressXml(const wxXmlDocument &doc, const wxString &zipFile)
    {
        auto outStream = wxFileOutputStream(zipFile);

        CompressXml(doc, outStream);
        outStream.Close();
    }

    wxXmlDocument DecompressXml(wxInputStream &in)
    {
        wxXmlDocument doc;
        wxZipInputStream zipIn(in);
        std::unique_ptr<wxZipEntry> entry(zipIn.GetNextEntry());

        while (entry)
        {
            wxString entryName = entry->GetName();

            if (entryName == "paintdocument.xml" && zipIn.CanRead())
            {
                doc.Load(zipIn);
                zipIn.CloseEntry();

                break;
            }

            zipIn.CloseEntry();
            entry.reset(zipIn.GetNextEntry());
        }

        return doc;
    }

    wxXmlDocument DecompressXml(const wxString &in)
    {
        wxFileSystem fs;
        std::unique_ptr<wxFSFile> zip(fs.OpenFile(in + "#zip:paintdocument.xml"));

        wxXmlDocument doc;

        if (zip)
        {
            wxInputStream *in = zip->GetStream();

            if (in)
            {
                doc.Load(*in);
            }
        }

        return doc;
    }
};