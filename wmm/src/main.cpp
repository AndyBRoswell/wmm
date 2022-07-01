#include "predefined.h"

#include <QApplication>
#include <QQmlApplicationEngine>

#include <QLocale>
#include <QTranslator>

#include <QFile>
#include <QQuickStyle>
#include <QStyle>
#include <QStyleFactory>

// headers of wmm
#include "DataSourceManagerWindow.h"
#include "EditorWindow.h"
#include "ExtraFunctionWindow.h"

// tests
#include "test/Test.h"

// NOTE: Meta object features are NOT supported for nested classes.

using namespace std;
using namespace std::chrono_literals;
using namespace WritingMaterialsManager;

int main(int argc, char* argv[]) {
    QApplication App(argc, argv);

    QTranslator Translator;
    const QStringList UILanguages = QLocale::system().uiLanguages();
    for (const QString& Locale: UILanguages) {
        const QString baseName = "wmm_" + QLocale(Locale).name();
        if (Translator.load(":/i18n/" + baseName)) {
            App.installTranslator(&Translator);
            break;
        }
    }

    // default theme
    {
        QQuickStyle::setStyle("Universal"); // default theme of the start window

//        QFile DarkThemeStyleSheetFile("src/thm/dark/style.qss");
//        DarkThemeStyleSheetFile.open(QIODevice::ReadOnly);
//        App.setStyleSheet(DarkThemeStyleSheetFile.readAll());
    }
    {
//        qApp->setStyle(QStyleFactory::create("fusion"));
        qApp->setStyle(QStyleFactory::create("windows"));

        QPalette palette;
        palette.setColor(QPalette::Window, Qt::black);
        palette.setColor(QPalette::WindowText, Qt::white);
        palette.setColor(QPalette::Base, QColor(15,15,15));
        palette.setColor(QPalette::AlternateBase, QColor(53,53,53));
        palette.setColor(QPalette::ToolTipBase, Qt::white);
        palette.setColor(QPalette::ToolTipText, Qt::white);
        palette.setColor(QPalette::Text, Qt::white);
        palette.setColor(QPalette::Button, QColor(53,53,53));
        palette.setColor(QPalette::ButtonText, Qt::white);
        palette.setColor(QPalette::BrightText, Qt::red);

        palette.setColor(QPalette::Highlight, QColor(142,45,197).lighter());
        palette.setColor(QPalette::HighlightedText, Qt::black);

        palette.setColor(QPalette::Disabled, QPalette::Text, Qt::darkGray);
        palette.setColor(QPalette::Disabled, QPalette::ButtonText, Qt::darkGray);

        qApp->setPalette(palette);
    }

//    QQmlApplicationEngine QMLEngine;
////    const QUrl MainQMLFile(u"qrc:/wmm/src/main.qml"_qs);
//    const QUrl MainQMLFile = QUrl::fromLocalFile(u"src/main.qml"_qs);
//    QObject::connect(&QMLEngine, &QQmlApplicationEngine::objectCreated, &App,
//                     [MainQMLFile](QObject* Obj, const QUrl& ObjURL) { if (!Obj && MainQMLFile == ObjURL) QCoreApplication::exit(-1); },
//                     Qt::QueuedConnection);
//    QMLEngine.load(MainQMLFile);

    int ret = Test::Start();

    auto DataSourceManagerWnd = new class DataSourceManagerWindow;
    DataSourceManagerWnd->showMaximized();
    auto EditorWnd = new class EditorWindow;
    EditorWnd->showMaximized();
    auto ExtFnWnd = new class ExtraFunctionWindow;
    ExtFnWnd->showMaximized();

    return App.exec();
}
