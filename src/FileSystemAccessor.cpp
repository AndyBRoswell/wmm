#include "FileSystemAccessor.h"

namespace WritingMaterialsManager {
    std::shared_ptr<QFile> FileSystemAccessor::Open(const QString& PathName, const QIODeviceBase::OpenMode Mode) {
        std::shared_ptr<QFile> File = std::make_shared<QFile>(PathName);
        if (File->open(Mode) == false) {
            throw std::runtime_error(("Open file " + PathName + " failed.").toUtf8().constData());
        }
        return File;
    }

    std::shared_ptr<QFileInfo> FileSystemAccessor::GetFileInfo(const std::shared_ptr<QFile>& File) {
        return std::make_shared<QFileInfo>(*File);
    }

    QByteArray FileSystemAccessor::GetAllRawContents(const std::shared_ptr<QFile>& File) {
        return File->readAll();
    }
}
