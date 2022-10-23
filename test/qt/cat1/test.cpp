#include <QLineEdit>
#include <QObject>
#include <QSignalSpy>
#include <QTest>

// files to be tested
#include "src/global.h"
#include "src/TreeView.h"
#include "src/TextArea.h"

class test : public QObject {
    Q_OBJECT
private:

private slots:
    void initTestFCase() {
        qDebug("GUI Test Cat. 1"); // cat. 1 for relatively simpler GUI components
    }

    void SignalAndCtor() {
        namespace wmm = WritingMaterialsManager;

        // signals are emitted when this TreeView is focused
        QTestEventList events;
        constexpr Qt::MouseButton mouse_keys[] = { Qt::MouseButton::LeftButton, Qt::MouseButton::RightButton, Qt::MouseButton::MiddleButton };
        for (const auto m : mouse_keys) {
            events.addMouseClick(m);
            events.addMouseDClick(m);
            events.addMousePress(m);
            events.addMouseRelease(m);
        }

        // begin signal test
        constexpr qsizetype signal_count = 6; // click, press

        wmm::TreeView tree_view;
        wmm::TextField text_field;
        wmm::TextArea text_area;
        {
            qRegisterMetaType<wmm::TreeView>();
            QSignalSpy spy(&tree_view, SIGNAL(MouseDown()));
            for (const auto m : mouse_keys) {
                QTest::mouseClick(tree_view.viewport(), m);
                QTest::mouseDClick(tree_view.viewport(), m);
                QTest::mousePress(tree_view.viewport(), m);
                QTest::mouseRelease(tree_view.viewport(), m);
            }
            QCOMPARE(spy.count(), signal_count);
        }
        {
            QSignalSpy spy(&text_field, &wmm::TextField::MouseDown);
            events.simulate(&text_field);
            QCOMPARE(spy.count(), signal_count);
        }
        {
            qRegisterMetaType<wmm::TextArea>();
            QSignalSpy spy(&text_area, SIGNAL(MouseDown()));
            for (const auto m : mouse_keys) {
                QTest::mouseClick(text_area.viewport(), m);
                QTest::mouseDClick(text_area.viewport(), m);
                QTest::mousePress(text_area.viewport(), m);
                QTest::mouseRelease(text_area.viewport(), m);
            }
            QCOMPARE(spy.count(), signal_count);
        }

        // verify default font
        QCOMPARE(text_field.font(), wmm::DefaultCodeFont);
        QCOMPARE(text_area.font(), wmm::DefaultCodeFont);
    }

    void cleanupTestCase() {
        qDebug("End GUI Test Cat. 1");
    }
};

QTEST_MAIN(test)
#include "test.moc"
