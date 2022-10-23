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
        const std::shared_ptr<QWidget> widgets[] = { 
            std::make_shared<wmm::TreeView>(),
            std::make_shared<wmm::TextField>(),
            std::make_shared<wmm::TextArea>(),
        };

        for (const auto& widget : widgets) {
            QSignalSpy signal_spy(widget.get(), &wmm::TreeView::MouseDown);
            events.simulate(&tree_view);
            QCOMPARE(signal_spy.count(), 12);
        }
    }

    void cleanupTestCase() {
        qDebug("End GUI Test Cat. 1");
    }
};

QTEST_MAIN(test)
#include "test.moc"
