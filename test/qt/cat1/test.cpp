#include <QLineEdit>
#include <QObject>
#include <QSignalSpy>
#include <QTest>

// files to be tested
#include <src/TreeView.h>
#include <src/TextArea.h>

class test : public QObject {
    Q_OBJECT
private:

private slots:
    void initTestFCase() {
        qDebug("GUI Test Cat. 1"); // cat.1 for relatively simpler GUI components
    }

    void Signal() {
        namespace wmm = WritingMaterialsManager;

        // signals are emitted when this TreeView is focused
        QTestEventList events;
        constexpr Qt::MouseButton mouse_keys[] = { Qt::MouseButton::LeftButton, Qt::MouseButton::RightButton, Qt::MouseButton::MiddleButton };
        for (const auto& m : mouse_keys) {
            events.addMouseClick(m);
            events.addMouseDClick(m);
            events.addMousePress(m);
            events.addMouseRelease(m);
        }

        // begin signal test
        constexpr qsizetype signal_count = 12;

        wmm::TreeView tree_view;
        wmm::TextField text_field;
        wmm::TextArea text_area;
        {
            qDebug("Testing TreeView");
            QSignalSpy spy(&tree_view, &wmm::TreeView::MouseDown);
            events.simulate(&tree_view);
            QCOMPARE(spy.count(), signal_count);
        }        {
            qDebug("Testing TextField");
            QSignalSpy spy(&text_field, &wmm::TextField::MouseDown);
            events.simulate(&text_field);
            QCOMPARE(spy.count(), signal_count);
        }        {
            qDebug("Testing TextArea");
            QSignalSpy spy(&text_area, &wmm::TextArea::MouseDown);
            events.simulate(&text_area);
            QCOMPARE(spy.count(), signal_count);
        }
    }

    void cleanupTestCase() {
        qDebug("End GUI Test Cat. 1");
    }
};

QTEST_MAIN(test)
#include "test.moc"
