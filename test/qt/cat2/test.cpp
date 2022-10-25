#include <QDir>
#include <QFile>
#include <QObject>
#include <QTest>

#include "src/TreeEditor.h"

class test : public QObject {
    Q_OBJECT
private:

private slots:
    void initTestCase() {
        qDebug("GUI Test Cat. 2");
    }

    void TreeEditor__open_JSON() {
        const QDir wd("test/TreeEditor");
        const QStringList test_file_list = wd.entryList({ "*.json" });

    }

    void cleanupTestCase() {
        qDebug("End GUI Test Cat. 2");
    }
};

QTEST_MAIN(test)
#include "test.moc"
