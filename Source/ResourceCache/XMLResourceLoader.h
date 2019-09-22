#ifndef XMLRESOURCELOADER_H
#define XMLRESOURCELOADER_H

#include "GameEngineStd.h"
#include "ResCache.h"
//#include <tinyxml2.h>
//using namespace tinyxml2;

class XmlResourceExtraData : public IResourceExtraData
{
    QDomDocument m_xmlDocument;

public:
    virtual QString VToString() { return "XmlResourceExtraData"; }
    void ParseXml(char *pRawBuffer);
    QDomElement GetRoot(void) { return m_xmlDocument.documentElement(); }
};

class XmlResourceLoader : public IResourceLoader
{
public:
    virtual bool VUseRawFile() {return false; }
    virtual bool VDiscardRawBufferAfterLoad() { return true; }
    virtual unsigned int VGetLoadedResourceSize(char *rawBuffer, unsigned int rawSize) {Q_UNUSED(rawBuffer); return rawSize; }
    virtual bool VLoadResource(char *rawBuffer, unsigned int rawSize, QSharedPointer<ResHandle> handle);
    virtual QString VGetPattern() { return "*.xml"; }

    //convenience function
    static QDomElement LoadAndReturnXmlElement(QString resourceString);

};

#endif // XMLRESOURCELOADER_H
