#ifndef WRITING_MATERIALS_MANAGER_FILESYSTEMACCESSOR_H
#define WRITING_MATERIALS_MANAGER_FILESYSTEMACCESSOR_H

#include <QFile>
#include <QFileInfo>

namespace WritingMaterialsManager {
    class FileSystemAccessor {
    public:
        static std::shared_ptr<QFile> Open(const QString& PathName, const QIODeviceBase::OpenMode Mode = QIODevice::ReadOnly);
        static std::shared_ptr<QFileInfo> GetFileInfo(const std::shared_ptr<QFile>& File);
        static QByteArray GetAllRawContents(const std::shared_ptr<QFile>& File);
    };
}

#endif //WRITING_MATERIALS_MANAGER_FILESYSTEMACCESSOR_H
