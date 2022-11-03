// duckx
#include <duckx.hpp>

// Qt
#include <QSignalSpy>
#include <QTest>

// this software
#include "tiny_random.h"

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
        if (wd.exists() == false) { QDir("test").mkdir("ExtraFunctionWindow"); }

        constexpr size_t nf = 100;      // file count
        constexpr size_t np = 100;      // paragraph count of each file
        constexpr size_t nr = 100;      // run count of each paragraph
        constexpr size_t lmax = 100;    // max length of random strings

        wmm::DocumentExtractPage doc_extract_page;
        for (size_t i = 0; i < nf; ++i) {
            QString expected_text;
            const auto file_name = wd.path() + "/" + QString::number(get_high_resolution_tick_count()) + ".docx";
            duckx::Document test_doc(file_name.toStdString());
            for (size_t j = 0; j < np; ++j) {
                duckx::Paragraph p = test_doc.paragraphs().insert_paragraph_after("");
                for (size_t k = 0; k < nr; ++k) {
                    const auto s = tiny_random::chr::ASCII_string(tiny_random::number::integer(1ull, lmax));
                    p.add_run(s);
                    expected_text.append(s.c_str()).append("\n");
                }
            }
            test_doc.save();
            expected_text.remove(expected_text.size() - 1, 1); // remove the trailing newline char

            doc_extract_page.OpenFile(file_name);
            QCOMPARE(doc_extract_page.GetPlainTextFromOpenDocument(), expected_text);
        }
    }

    void cleanupTestCase() {
        qDebug("End GUI Test Cat. 3");
    }
};

QTEST_MAIN(test)
#include "test.moc"
