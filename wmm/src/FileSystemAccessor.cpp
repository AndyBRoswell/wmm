#include "FileSystemAccessor.h"

namespace WritingMaterialsManager {
    std::shared_ptr<QFile> FileSystemAccessor::Open(const QString& FileName, QIODeviceBase::OpenMode Mode) {
        std::shared_ptr<QFile> File = std::make_shared<QFile>(FileName);
        if (File->open(Mode)) {
            throw std::runtime_error(("Open file " + FileName + " failed.").toUtf8().constData());
        }
        return File;
    }
    QByteArray FileSystemAccessor::ReadAllRawContents(const std::shared_ptr<QFile>& File) {
        return File->readAll();
    }
}