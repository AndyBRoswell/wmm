// Qt
#include <QSignalSpy>
#include <QTest>

// files to be tested
#include "src/ExtraFunctionWindow.h"

auto get_high_resolution_tick_count() { return std::chrono::high_resolution_clock::now().time_since_epoch().count(); }

class test : public QObject {
    Q_OBJECT
private slots:
    void initTestCase() {
        qDebug("GUI Test Cat. 3");
    }

    void DocumentExtractPage__view_docx() {
        namespace wmm = WritingMaterialsManager;
        
        const QDir wd("test/ExtraFunctionWindow");

        constexpr size_t nf = 100;      // file count
        constexpr size_t np = 100;      // paragraph count of each file
        constexpr size_t nr = 100;      // run count of each paragraph
        constexpr size_t lmax = 100;    // max length of random strings

        wmm::DocumentExtractPage doc_extract_page;

    }

    void cleanupTestCase() {
        qDebug("End GUI Test Cat. 3");
    }
};

QTEST_MAIN(test)
#include "test.moc"
