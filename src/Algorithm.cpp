#include "Algorithm.h"

namespace WritingMaterialsManager {
/// struct CaseInsensitiveHasher

    size_t CaseInsensitiveHasher::operator()(const QByteArray& Str) const noexcept {
        return *reinterpret_cast<const size_t*>(QCryptographicHash::hash(Str.toUpper(), DefaultHashAlgorithm).right(sizeof(size_t)).constData());
    }
    size_t CaseInsensitiveHasher::operator()(const QString& Str) const noexcept {
        return *reinterpret_cast<const size_t*>(QCryptographicHash::hash(Str.toUpper().toUtf8(), DefaultHashAlgorithm).right(sizeof(size_t)).constData());
    }

/// struct CaseInsensitiveStringComparator
}
