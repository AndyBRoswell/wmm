#include <QObject>
#include <QTest>

class test : public QObject {
    Q_OBJECT
private:

private slots:
    void initTestCase() {
        qDebug("GUI Test Cat. 2");
    }

    void cleanupTestCase() {
        qDebug("End GUI Test Cat. 2");
    }
};

QTEST_MAIN(test)
#include "test.moc"
