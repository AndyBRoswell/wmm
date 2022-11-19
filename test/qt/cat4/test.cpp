// duckx
#include <duckx.hpp>

// Qt
#include <QSignalSpy>
#include <QTest>

// this software
#include "tiny_random.h"
#include "util.h"

// files to be tested
#include "src/ExtraFunctionWindow.h"

class test : public QObject {
    Q_OBJECT
private slots:
    void initTestCase() {
        qDebug("GUI Test Cat. 4");
    }

    void DocumentExtractPage__view_docx() {
        namespace wmm = WritingMaterialsManager;

        const QDir wd("test/ExtraFunctionWindow");
        if (wd.exists() == false) { wd.mkpath("."); }

        constexpr size_t nf = 1000;     // file count
        constexpr size_t np = 100;      // paragraph count of each file
        constexpr size_t nr = 100;      // run count of each paragraph
        constexpr size_t lmax = 100;    // max length of random strings

        wmm::DocumentExtractPage doc_extract_page;
        QSignalSpy spy(&doc_extract_page, SIGNAL(DocumentOpened(const QString&)));
        QFile blank_source(wd.path() + "/blank.docx");
        for (size_t i = 0; i < nf; ++i) {
            const auto file_name = wd.path() + "/" + QString::number(util::get_tick_count()) + ".docx";
            QVERIFY(blank_source.copy(file_name));

            duckx::Document test_doc(file_name.toStdString());
            test_doc.open(); // at present this test will crash at here
            QString expected_text; {
                for (size_t j = 0; j < np; ++j) {
                    duckx::Paragraph p = test_doc.paragraphs();
                    for (size_t k = 0; k < nr; ++k) {
                        const auto s = tiny_random::chr::ASCII_string(tiny_random::number::integer(1ull, lmax));
                        p.add_run(s);
                        expected_text.append(s.c_str()).append("\n");
                    }
                }
                test_doc.save();
                if (*expected_text.crbegin() == '\n') { expected_text.remove(expected_text.size() - 1, 1); }
            }

            doc_extract_page.OpenFile(file_name);
            QCOMPARE(spy.count(), i + 1);
            //const auto doc_text_with_special_char_removed = [&]() {
            //    const auto s = doc_extract_page.GetPlainTextFromOpenDocument();
            //    QString ret;
            //    ret.reserve(s.size());
            //    for (auto c : s) {
            //        //constexpr QChar::Category cats_to_be_removed[] = {
            //        //    QChar::Separator_Line, QChar::Separator_Paragraph, 
            //        //    QChar::Other_Control, QChar::Other_Format, QChar::Other_Surrogate, QChar::Other_PrivateUse, QChar::Other_NotAssigned,
            //        //};
            //        constexpr char16_t chrs_to_be_removed[] = { '\uFFFD', '\uFFFE', '\uFFFF', '\u0001', };
            //        bool remove = false;
            //        //for (const auto cat : cats_to_be_removed) { if (c.category() == cat) { remove = true; break; } }
            //        for (const auto chr : chrs_to_be_removed) { if (c == chr) { remove = true; break; } }
            //        if (remove == false) { ret.push_back(c); }
            //    }
            //    return ret;
            //};
            //const auto actual_text = doc_text_with_special_char_removed();
            const auto actual_text = doc_extract_page.GetPlainTextFromOpenDocument();
            QCOMPARE(actual_text, expected_text);
            //const bool ok = actual_text == expected_text; // this line is for conditional breakpoints
            //QVERIFY(ok);
        }
    }

    void cleanupTestCase() {
        qDebug("End GUI Test Cat. 4");
    }
};

QTEST_MAIN(test)
#include "test.moc"
