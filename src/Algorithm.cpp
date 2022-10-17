#include "Algorithm.h"

namespace WritingMaterialsManager {
    size_t CaseInsensitiveHasher::operator()(const QByteArray& Str) const noexcept {
        return QCryptographicHash::hash(Str.toUpper(), DefaultSimpleHashAlgorithm).right(8).toULongLong();
    }
    size_t CaseInsensitiveHasher::operator()(const QString& Str) const noexcept {
        return QCryptographicHash::hash(reinterpret_cast<const char* const>(Str.toUpper().constData()), DefaultSimpleHashAlgorithm).right(8).toULongLong();
    }

/// ----------------------------------------------------------------

    bool CaseInsensitiveStringComparator::operator()(const QString& LHS, const QString& RHS) const {
        return LHS.size() == RHS.size()
        && std::equal(LHS.cbegin(), LHS.cend(), RHS.cbegin(), [](QChar a, QChar b) { return a.toUpper() == b.toUpper(); });
    }
}
