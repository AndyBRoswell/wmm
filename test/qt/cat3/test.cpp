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
            QString result;
            QObject::connect(&mongosh_accessor, &wmm::MongoShAccessor::MoreResult, [&](const QString& result_fragment) {
                result += result_fragment;
                });
            QObject::connect(&mongosh_accessor, &wmm::MongoShAccessor::NoMoreResult, [&]() {
                QCOMPARE_GT(MoreResult_signal_spy.count(), 0);
                QCOMPARE(NoMoreResult_signal_spy.count(), 1);
                QCOMPARE_GT(result.size(), 0);
                QCOMPARE_GT(result.indexOf("admin"), -1);
                QCOMPARE_GT(result.indexOf("config"), -1);
                QCOMPARE_GT(result.indexOf("local"), -1);
                });
            mongosh_accessor.Execute("show dbs\n");
        }
        { // JSON operations

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

        //wmm::PythonInteractor Python_interactor;
    }

    void cleanupTestCase() {
        qDebug("End GUI Test Cat. 3");
    }
};

QTEST_MAIN(test)
#include "test.moc"
