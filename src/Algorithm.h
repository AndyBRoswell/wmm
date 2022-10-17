#ifndef WRITING_MATERIALS_MANAGER_ALGORITHM_H
#define WRITING_MATERIALS_MANAGER_ALGORITHM_H

#include <QCryptographicHash>
#include <QString>

namespace WritingMaterialsManager {
    struct CaseInsensitiveHasher {
        static const auto DefaultSimpleHashAlgorithm = QCryptographicHash::Blake2b_160;
        size_t operator()(const QByteArray& Str) const noexcept;
        size_t operator()(const QString& Str) const noexcept;
    };

    struct CaseInsensitiveStringComparator {
        bool operator()(const QString& LHS, const QString& RHS) const;
    };
}

#endif //WRITING_MATERIALS_MANAGER_ALGORITHM_H
