#ifndef WRITING_MATERIALS_MANAGER_FILESYSTEMACCESSOR_H
#define WRITING_MATERIALS_MANAGER_FILESYSTEMACCESSOR_H

#include <QFile>

namespace WritingMaterialsManager {
    class FileSystemAccessor {
        std::shared_ptr<QFile> Open(const QString& FileName) {
            std::shared_ptr<QFile> File = std::make_shared<QFile>(FileName);
            
            return File;
        }
    };
}

#endif //WRITING_MATERIALS_MANAGER_FILESYSTEMACCESSOR_H
