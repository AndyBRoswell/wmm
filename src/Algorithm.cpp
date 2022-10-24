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
}
