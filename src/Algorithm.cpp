#include "Algorithm.h"

namespace WritingMaterialsManager {
    size_t CaseInsensitiveHasher::operator()(const QByteArray& Str) const noexcept {
        return *reinterpret_cast<const size_t*>(QCryptographicHash::hash(Str.toUpper(), DefaultHashAlgorithm).right(sizeof(size_t)).constData());
    }
    size_t CaseInsensitiveHasher::operator()(const QString& Str) const noexcept {
        return *reinterpret_cast<const size_t*>(QCryptographicHash::hash(Str.toUpper().toUtf8(), DefaultHashAlgorithm).right(sizeof(size_t)).constData());
    }

/// ----------------------------------------------------------------

    bool CaseInsensitiveStringComparator::operator()(const QString& LHS, const QString& RHS) const {
        return LHS.size() == RHS.size() && std::equal(LHS.cbegin(), LHS.cend(), RHS.cbegin(), [](QChar a, QChar b) { return a.toUpper() == b.toUpper(); });
    }
}
