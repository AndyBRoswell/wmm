// Qt
#include <QSignalSpy>
#include <QTest>

// files to be tested
#include "src/MongoDBConsole.h"
#include "src/PythonInteractor.h"

class test : public QObject {
    Q_OBJECT
private slots:
    void initTestCase() {
        qDebug("GUI Test Cat. 3");
    }

    void MongoShAccessor__basic() {
        namespace wmm = WritingMaterialsManager;
        using namespace std::chrono_literals;

        wmm::MongoShAccessor mongosh_accessor;
        { // show dbs
            QSignalSpy MoreResult_signal_spy(&mongosh_accessor, &wmm::MongoShAccessor::MoreResult);
            QSignalSpy NoMoreResult_signal_spy(&mongosh_accessor, &wmm::MongoShAccessor::NoMoreResult);
            

            mongosh_accessor.Execute("show dbs");
        }
    }

    void MongoDBConsole__basic() {
        namespace wmm = WritingMaterialsManager;

        //wmm::MongoDBConsole MongoDB_console;
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

    void PythonInteractor__basic() {
        namespace wmm = WritingMaterialsManager;

        wmm::PythonInteractor Python_interactor;
    }

    void cleanupTestCase() {
        qDebug("End GUI Test Cat. 3");
    }
};

QTEST_MAIN(test)
#include "test.moc"
