#include "Algorithm.h"

namespace WritingMaterialsManager {
    size_t CaseInsensitiveHasher::operator()(const QByteArray& Str) const noexcept {
        return *reinterpret_cast<const size_t*>(QCryptographicHash::hash(Str.toUpper(), DefaultHashAlgorithm).right(sizeof(size_t)).constData());
    }
    size_t CaseInsensitiveHasher::operator()(const QString& Str) const noexcept {
        return *reinterpret_cast<const size_t*>(QCryptographicHash::hash(Str.toUpper().toUtf8(), DefaultHashAlgorithm).right(sizeof(size_t)).constData());
    }

/// ----------------------------------------------------------------

    bool CaseInsensitiveStringComparator::operator()(const QAnyStringView LHS, const QAnyStringView RHS) const noexcept {
        return QAnyStringView::compare(LHS, RHS, Qt::CaseInsensitive) == 0;
    }
}
