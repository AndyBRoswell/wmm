#ifndef WRITING_MATERIALS_MANAGER_FILESYSTEMACCESSOR_H
#define WRITING_MATERIALS_MANAGER_FILESYSTEMACCESSOR_H

#include <QFile>

namespace WritingMaterialsManager {
    class FileSystemAccessor {
    public:
        static std::shared_ptr<QFile> Open(const QString& FileName, QIODeviceBase::OpenMode Mode = QIODevice::ReadOnly);
        static QByteArray ReadAllRawContents(const std::shared_ptr<QFile>& File);
    };
}

#endif //WRITING_MATERIALS_MANAGER_FILESYSTEMACCESSOR_H
