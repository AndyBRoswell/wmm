// Qt
#include <QSignalSpy>
#include <QTest>

// files to be tested
#include "src/MongoDBConsole.h"

class test : public QObject {
    Q_OBJECT
private slots:
    void initTestCase() {
        qDebug("GUI Test Cat. 3");
    }

    void MongoDBConsole__basic() {
        namespace wmm = WritingMaterialsManager;

        wmm::MongoDBConsole MongoDB_console;
        //wmm::TreeEditor tree_editor;
        //MongoDB_console.AddAssociatedEditor(&tree_editor);
        { // show dbs
            //MongoDB_console.ExecuteShellCommand();
            //QSignalSpy spy(&MongoDB_console, &wmm::MongoDBConsole::NewShellCommand);
            //QVERIFY(spy.isValid());
        }
        { // JSON operations

        }
    }

    void cleanupTestCase() {
        qDebug("End GUI Test Cat. 3");
    }
};

QTEST_MAIN(test)
#include "test.moc"
