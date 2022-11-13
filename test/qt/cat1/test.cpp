#include <QLineEdit>
#include <QObject>
#include <QSignalSpy>
#include <QTest>

// files to be tested
#include "src/global.h"
#include "src/TextArea.h"

class test : public QObject {
    Q_OBJECT
private:
    static constexpr Qt::MouseButton mouse_keys[] = { Qt::MouseButton::LeftButton, Qt::MouseButton::RightButton, Qt::MouseButton::MiddleButton };

private slots:
    void initTestCase() {
        qDebug("GUI Test Cat. 1"); // cat. 1 for relatively simpler GUI components
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
        qDebug("End GUI Test Cat. 1");
    }
};

QTEST_MAIN(test)
#include "test.moc"
