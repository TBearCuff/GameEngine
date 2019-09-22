#include "GameEngineStd.h"
#include "XMLResourceLoader.h"

void XmlResourceExtraData::ParseXml(char* pRawBuffer)
{
//    m_xmlDocument.Parse(pRawBuffer);
    Q_UNUSED(pRawBuffer);
}

bool XmlResourceLoader::VLoadResource(char* rawBuffer, unsigned int rawSize, QSharedPointer<ResHandle> handle)
{
    if(rawSize <= 0)
    {
        return false;
    }
    QSharedPointer<XmlResourceExtraData> pExtraData = QSharedPointer<XmlResourceExtraData> (GCC_NEW XmlResourceExtraData());
    pExtraData->ParseXml(rawBuffer);
    handle->SetExtra(QSharedPointer<XmlResourceExtraData>(pExtraData));

    return true;
}

QSharedPointer<IResourceLoader> CreateXmlResourceLoader()
{
    return QSharedPointer<IResourceLoader>(new XmlResourceLoader());
}

QDomElement XmlResourceLoader::LoadAndReturnXmlElement(QString resourceString)
{
    Resource resource(resourceString);
    QSharedPointer<ResHandle> pResourceHandle = g_pApp->m_ResCache->GetHandle(&resource); //this actually loads the XML file from the zip file
    QSharedPointer<XmlResourceExtraData> pExtraData = qSharedPointerCast<XmlResourceExtraData>(pResourceHandle->GetExtra());
    return pExtraData->GetRoot();
}
