
#include "GameEngineStd.h"

#include <QSharedMemory>
#include <QStorageInfo>
#include <QDebug>
#include <sys/statvfs.h>

bool CheckStorage(const qint64 diskSpaceNeeded)
{
    //Check for enough free disk space on the current disk (/root)
    //check secondary storage
    QStorageInfo storage = QStorageInfo::root();

    qDebug() << storage.rootPath();
    if(storage.isReadOnly())
        qDebug() << "isReadOnly:" << storage.isReadOnly();

    qDebug() << "name: " << storage.name();
    qDebug() << "fileSystemType: " << storage.fileSystemType();
    qDebug() << "size: " << storage.bytesTotal() << "Bytes";
    qDebug() << "availableSize" << storage.bytesAvailable() << "Bytes";

    if(diskSpaceNeeded > storage.bytesAvailable())
    {
        //if we get here we don't have enough disk space
        //GCC_ERROR("CheckStorage Failure: Not enough physical storage.");
        return false;
    }

    return true;
}

bool IsOnlyInstance(QString gameTitle)
{
    //set up a shared memry area keyed to title
    QSharedMemory sharedMemory(gameTitle);

    //check for success on the creation
    if(sharedMemory.create(1) && sharedMemory.error() != QSharedMemory::AlreadyExists)
    {
        //TODO: Find the active game window??
        //Microsoft: Findwindow(gameTitle, NULL);
        //TO DO: Display to user that an instance is already runnning
        return true;
    }
    else
        return false;

    return true;
}
