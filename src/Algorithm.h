#ifndef WRITING_MATERIALS_MANAGER_ALGORITHM_H
#define WRITING_MATERIALS_MANAGER_ALGORITHM_H

#include <QCryptographicHash>
#include <QString>

namespace WritingMaterialsManager {
    struct CaseInsensitiveHasher {
        static const auto DefaultHashAlgorithm = QCryptographicHash::Blake2b_160;
        size_t operator()(const QByteArray& Str) const noexcept;
        size_t operator()(const QString& Str) const noexcept;
    };

    struct CaseInsensitiveStringComparator {
        bool operator()(const QAnyStringView LHS, const QAnyStringView RHS) const noexcept; // Qt recommends pass string views by value
    };
}

#endif //WRITING_MATERIALS_MANAGER_ALGORITHM_H
