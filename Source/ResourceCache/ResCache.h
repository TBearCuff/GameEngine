#ifndef RESCACHE_H
#define RESCACHE_H

#include "GameEngineStd.h"
#include <QFileInfo>
#include <QFile>
#include <QDir>

class ResHandle;
class ResCache;

#include "ZipFile.h"        //Needed for ZipContentsMap

//
// class IResourceExtraData		- Chapter 8, page 224 (see notes below)
//
//   This isn't mentioned specifically on that page, but it is a class that can attach extra data to
//   a particular resource. Best example is storing the length and format of a sound file.
//   There's a great discussion of this in Chapter 13, "Game Audio"
//
class IResourceExtraData
{
public:
    virtual QString VToString()=0;
};

//
//  class Resource			- Chapter 8, page 220
//
class Resource
{
public:
    QString m_name;
    Resource(const QString &name);
};

//
// class ResourceZipFile
//
//  This class implements the IResourceFile interface with a ZipFile
class ResourceZipFile : public IResourceFile
{
    ZipFile *m_pZipFile;
    QString m_resFileName;

public:
    ResourceZipFile(const QString resFileName ) {m_pZipFile = NULL; m_resFileName=resFileName; }
    virtual ~ResourceZipFile();

    virtual bool VOpen();
    virtual int VGetRawResourceSize(const Resource &r);
    virtual int VGetRawResource(const Resource &r, char *buffer);
    virtual int VGetNumResources() const;
    virtual QString VGetResourceName(int num) const;
    virtual bool VIsUsingDevelopmentDirectories(void) const { return false; }
};

//
//class DevelopmentalResourceZipFile
//
//This class fakes a ZIP file from a normal directory, and is used in the editor.
//
class DevelopmentResourceZipFile : public ResourceZipFile
{
public:
    enum Mode
    {
        Development, //this mode checks the original asset directory for changes - helps during development
        Editor      //this mode only checks the original asset directory - the ZIP file is left unopened
    };

    Mode m_mode;
    QString m_AssetsDir;
    std::vector<QFileInfo> m_AssetFileInfo;
    ZipContentsMap m_DirectoryContentsMap;

    DevelopmentResourceZipFile(const QString resFileName, const Mode mode);

    virtual bool VOpen();
    virtual int VGetRawResourceSize(const Resource &r);
    virtual int VGetRawResource(const Resource &r, char *buffer);
    virtual int VGetNumResources() const;
    virtual QString VGetResourceName(int num) const;
    virtual bool VIsUsingDevelopmentDirectories(void) const { return true; }

    int Find(const QString &name);

protected:
    void ReadAssetsDirectory(QString fileSpec);

};

//
//  class ResHandle			- Chapter 8, page 222
//
class ResHandle
{
    friend class ResCache;

protected:
    Resource m_resource;
    char *m_buffer;
    unsigned int m_size;
    QSharedPointer<IResourceExtraData> m_extra;
    ResCache *m_pResCache;

public:
    ResHandle(Resource & resource, char *buffer, unsigned int size, ResCache *pResCache);
    virtual ~ResHandle();

    const QString GetName() { return m_resource.m_name; }
    unsigned int Size() const { return m_size; }
    char *Buffer() const { return m_buffer; }
    char *WritableBuffer() { return m_buffer; }

    QSharedPointer<IResourceExtraData> GetExtra() { return m_extra; }
    void SetExtra(QSharedPointer<IResourceExtraData> extra) { m_extra = extra; }
};

//
// class DefaultResourceLoader							- Chapter 8, page 225
//
class DefaultResourceLoader : public IResourceLoader
{
public:
    virtual bool VUseRawFile() { return true; }
    virtual bool VDiscardRawBufferAfterLoad() { return true; }
    virtual unsigned int VGetLoadedResourceSize(char *, unsigned int rawSize) { return rawSize; }
    virtual bool VLoadResource(char *, unsigned int , QSharedPointer<ResHandle> ) { return true; }
    virtual QString VGetPattern() { return "*"; }

};

typedef QList< QSharedPointer <ResHandle> > ResHandleList;
typedef QMap<QString, QSharedPointer < ResHandle > > ResHandleMap;  //maps identifiers to resource data
typedef QList< QSharedPointer < IResourceLoader > > ResourceLoaders;


class ResCache
{
    friend class ResHandle;

    ResHandleList m_lru;    //lru list
    ResHandleMap m_resources;
    ResourceLoaders m_resourceLoaders;

    IResourceFile *m_file;

    unsigned int    m_cacheSize;    //total memory size
    unsigned int    m_allocated;    //total memory allocated

protected:

    bool MakeRoom(unsigned int size);
    char *Allocate(unsigned int size);
    void Free(QSharedPointer<ResHandle> gonner);

    QSharedPointer<ResHandle> Load(Resource *r);
    QSharedPointer<ResHandle> Find(Resource *r);
    void Update(QSharedPointer<ResHandle> handle);

    void FreeOneResource();
    void MemoryHasBeenFreed(unsigned int size);

public:
    ResCache(const unsigned int sizeInMb, IResourceFile *resFile );
    virtual ~ResCache();

    bool Init();

    void RegisterLoader(QSharedPointer<IResourceLoader> loader);

    QSharedPointer<ResHandle> GetHandle(Resource *r);

    int Preload(const std::string pattern, void (*progressCallback)(int, bool&));

    std::vector<std::string> Match(const std::string pattern);

    void Flush(void);

    bool IsUsingDevelopmentDirectories(void) const { Q_ASSERT(m_file); return m_file->VIsUsingDevelopmentDirectories(); }
};

#endif // RESCACHE_H
