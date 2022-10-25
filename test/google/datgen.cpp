// Qt
#include <QFile>
#include <QDir>
#include <QJsonDocument>

// googletest
#include <gtest/gtest.h>

TEST(TreeEditor, ConvertCharsetForArtificialData) {
    QFile file_list("test/TreeEditor/in.json");
    if (file_list.open(QIODevice::OpenModeFlag::ReadOnly) == false) { return; }

}
