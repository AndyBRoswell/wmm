// Qt
#include <QSignalSpy>
#include <QTest>

// files to be tested
#include "src/ExtraFunctionWindow.h"

class test : public QObject {
    Q_OBJECT
private slots:
    void initTestCase() {
        qDebug("GUI Test Cat. 3");
    }

    void DocumentExtractPage__view_docx() {

    }

    void cleanupTestCase() {
        qDebug("End GUI Test Cat. 3");
    }
};

QTEST_MAIN(test)
#include "test.moc"
