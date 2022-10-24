#include "Algorithm.h"

namespace WritingMaterialsManager {
    size_t CaseInsensitiveHasher::operator()(const QByteArray& Str) const noexcept {
        return *reinterpret_cast<const size_t*>(QCryptographicHash::hash(Str.toUpper(), DefaultHashAlgorithm).right(sizeof(size_t)).constData());
    }
    size_t CaseInsensitiveHasher::operator()(const QString& Str) const noexcept {
        const QString STR = Str.toUpper();
        const QByteArrayView v(reinterpret_cast<const char*>(STR.constData()), STR.size() * 2);
        return *reinterpret_cast<const size_t*>(QCryptographicHash::hash(v, DefaultHashAlgorithm).right(sizeof(size_t)).constData());
    }
    size_t CaseInsensitiveHasher::operator()(const QAnyStringView Str) const noexcept {
        if (Str.size_bytes() / Str.size() == 1) { return this->operator()(QByteArray(static_cast<const char*>(Str.data()), Str.size_bytes())); }
        else { return this->operator()(Str.toString()); }
    }
    size_t CaseInsensitiveHasher::operator()(const QStringView Str) const noexcept {
        const QString STR = QString::fromRawData(Str.constData(), Str.size()).toUpper();
        const QByteArrayView v(reinterpret_cast<const char*>(STR.constData()), STR.size() * 2);
        return *reinterpret_cast<const size_t*>(QCryptographicHash::hash(v, DefaultHashAlgorithm).right(sizeof(size_t)).constData());
    }

    bool CaseInsensitiveStringComparator::operator()(const QAnyStringView LHS, const QAnyStringView RHS) const noexcept {
        return QAnyStringView::compare(LHS, RHS, Qt::CaseInsensitive) == 0;
    }
}
