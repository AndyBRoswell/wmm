#include "predefined.h"

#include <QApplication>

#include <QLocale>
#include <QTranslator>

#include <QStyle>
#include <QStyleFactory>

// headers of this project wmm
#include "DataSourceManagerWindow.h"
#include "EditorWindow.h"
#include "ExtraFunctionWindow.h"

// tests of this project wmm
#include "test/Test.h"

// NOTE: Meta object features are NOT supported for nested classes.

using namespace std;
using namespace std::chrono_literals;
using namespace WritingMaterialsManager;

int main(int argc, char* argv[]) {
    QApplication App(argc, argv); // <only 1 instance> manages the Widgets app's control flow and main settings

    QTranslator Translator; // internationalization support for text output
    const QStringList UILanguages = QLocale::system().uiLanguages(); // A list of locale names for translation in preference order
    for (const QString& Locale: UILanguages) {
        const QString baseName = "wmm_" + QLocale(Locale).name();
        if (Translator.load(":/i18n/" + baseName)) { // load translation
            QApplication::installTranslator(&Translator);
            break;
        }
    }

    {// default theme
        qApp->setStyle(QStyleFactory::create("windows"));

        QPalette p; // contains color groups (active, disabled, inactive) for each widget state for Widgets, etc
        p.setColor(QPalette::Window, Qt::black);
        p.setColor(QPalette::WindowText, Qt::white);
        p.setColor(QPalette::Base, QColor(15, 15, 15));
        p.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
        p.setColor(QPalette::ToolTipBase, Qt::white);
        p.setColor(QPalette::ToolTipText, Qt::white);
        p.setColor(QPalette::Text, Qt::white);
        p.setColor(QPalette::Button, QColor(53, 53, 53));
        p.setColor(QPalette::ButtonText, Qt::white);
        p.setColor(QPalette::BrightText, Qt::red);

        p.setColor(QPalette::Highlight, QColor(142, 45, 197).lighter());
        p.setColor(QPalette::HighlightedText, Qt::black);

        p.setColor(QPalette::Disabled, QPalette::Text, Qt::darkGray);
        p.setColor(QPalette::Disabled, QPalette::ButtonText, Qt::darkGray);

        QApplication::setPalette(p);
    }

    int ret = Test::Start();

    auto DataSourceManagerWnd = new class DataSourceManagerWindow;
    DataSourceManagerWnd->showMaximized();
    auto EditorWnd = new class EditorWindow;
    EditorWnd->showMaximized();
    auto ExtFnWnd = new class ExtraFunctionWindow;
    ExtFnWnd->showMaximized();

    return QApplication::exec(); // Enters the main event loop and waits until exit() is called
}
