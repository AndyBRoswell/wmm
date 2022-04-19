#ifndef WRITING_MATERIALS_MANAGER_ALGORITHM_H
#define WRITING_MATERIALS_MANAGER_ALGORITHM_H

#include <QCryptographicHash>
#include <QString>

namespace WritingMaterialsManager {
    struct CaseInsensitiveHasher {
        static const auto DefaultSimpleHashAlgorithm = QCryptographicHash::Md4;
        size_t operator()(const QByteArray& Str) const;
        size_t operator()(const QString& Str) const;
    };

    struct CaseInsensitiveStringComparator {
        bool operator()(const QString& LHS, const QString& RHS) const;
    };
}

#endif //WMM_ALGORITHM_H
