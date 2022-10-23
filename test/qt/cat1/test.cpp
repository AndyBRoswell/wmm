#include <QObject>
#include <QTest>

// files to be tested
#include <src/TreeView.h>

class test : public QObject {
    Q_OBJECT
private:

private slots:
    void initTestFCase() {
        qDebug("GUI Test Cat. 1");
    }

    void TreeView() {
        WritingMaterialsManager::TreeView TreeView;

    }

    void cleanupTestCase() {
        qDebug("End GUI Test Cat. 1");
    }
};

QTEST_MAIN(test)
#include "test.moc"
