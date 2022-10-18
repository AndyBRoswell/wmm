#include "Algorithm.h"

namespace WritingMaterialsManager {
    size_t CaseInsensitiveHasher::operator()(const QByteArray& Str) const noexcept {
        return QCryptographicHash::hash(Str.toUpper(), DefaultHashAlgorithm).last(8).toHex().toULongLong(nullptr, 16);
    }
    size_t CaseInsensitiveHasher::operator()(const QString& Str) const noexcept {
        //return QCryptographicHash::hash(reinterpret_cast<const char* const>(Str.toUpper().constData()), DefaultHashAlgorithm).last(8).toHex().toULongLong(nullptr, 16);
        return QCryptographicHash::hash(Str.toUpper().toUtf8(), DefaultHashAlgorithm).last(8).toHex().toULongLong(nullptr, 16);
    }

/// ----------------------------------------------------------------

    bool CaseInsensitiveStringComparator::operator()(const QString& LHS, const QString& RHS) const {
        return LHS.size() == RHS.size()
        && std::equal(LHS.cbegin(), LHS.cend(), RHS.cbegin(), [](QChar a, QChar b) { return a.toUpper() == b.toUpper(); });
    }
}
  