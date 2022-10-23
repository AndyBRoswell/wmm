#include <QLineEdit>
#include <QObject>
#include <QSignalSpy>
#include <QTest>

// files to be tested
#include <src/TreeView.h>

class test : public QObject {
    Q_OBJECT
private:

private slots:
    void initTestFCase() {
        qDebug("GUI Test Cat. 1"); // cat.1 for relatively simpler GUI components
    }

    void TreeView() {
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
        WritingMaterialsManager::TreeView tree_view;
        QSignalSpy signal_spy(&tree_view, &WritingMaterialsManager::TreeView::MouseDown);
        events.simulate(&tree_view);
        //QCOMPARE(signal_spy.count(), 12);

        //QLineEdit lineEdit;
        //events.simulate(&lineEdit);
    }

    void cleanupTestCase() {
        qDebug("End GUI Test Cat. 1");
    }
};

QTEST_MAIN(test)
#include "test.moc"
