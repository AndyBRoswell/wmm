// std
#include <iostream>

// Qt
#include <QObject>
#include <QSignalSpy>
#include <QTest>

// this software
#include "tiny_random.h"
#include "util.h"

// files to be tested
#include "src/global.h"
#include "src/TextArea.h"

void test_message_handler(const QtMsgType type, const QMessageLogContext& context, const QString& msg) {
    //static const QString filter_msg = QString::fromUtf8("Mouse event \"Mouse");
    //if (msg.indexOf(filter_msg) == 0) { return; }
    //std::cout << msg.toUtf8().constData() << std::endl;
}

class test : public QObject {
    Q_OBJECT
private:
    static constexpr Qt::MouseButton mouse_keys[] = { Qt::MouseButton::LeftButton, Qt::MouseButton::RightButton, Qt::MouseButton::MiddleButton };

private slots:
    void initTestCase() {
        qDebug("GUI Test Cat. 1: Text Area");
        util::no_sync_with_stdio();
    }

    void TextField__basic() {
        namespace wmm = WritingMaterialsManager;

        constexpr size_t n = 1e6; // test count

        wmm::TextField text_field;

        // default settings
        QCOMPARE(text_field.font(), wmm::DefaultCodeFont);

        // signals
        QTestEventList events;
        for (const auto m : mouse_keys) {
            events.addMouseClick(m);
            events.addMouseDClick(m);
            events.addMousePress(m);
            events.addMouseRelease(m);
        }
        QSignalSpy spy(&text_field, &wmm::TextField::MouseDown);
        for (size_t i = 0; i < n; ++i) { events.simulate(&text_field); }
        QCOMPARE(spy.count(), 6 * n); // left & right & middle; click & press
    }

    void TextArea__basic() {
        namespace wmm = WritingMaterialsManager;

        constexpr size_t n = 1e6; // test count

        wmm::TextArea text_area;

        // default settings
        QCOMPARE(text_area.font(), wmm::DefaultCodeFont);

        // signals
        qRegisterMetaType<wmm::TextArea>();
        QSignalSpy spy(&text_area, SIGNAL(MouseDown()));
        qInstallMessageHandler(test_message_handler); // ignore warning
        for (size_t i = 0; i < n; ++i) {
            for (const auto m : mouse_keys) {
                QTest::mouseClick(text_area.viewport(), m);
                QTest::mouseDClick(text_area.viewport(), m);
                QTest::mousePress(text_area.viewport(), m);
                QTest::mouseRelease(text_area.viewport(), m);
            }
        }
        QCOMPARE(spy.count(), 6 * n); // left & right & middle; click & press
    }

    void cleanupTestCase() {
        qDebug("End GUI Test Cat. 1: Text Area");
    }
};

QTEST_MAIN(test)
#include "test.moc"
