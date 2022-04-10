#include <QApplication>
#include <QQmlApplicationEngine>

#include <QLocale>
#include <QTranslator>

#include <QQuickStyle>
#include <QFile>

// headers of wmm
#include "DataSourceManager.h"

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
    QQuickStyle::setStyle("Universal"); // default theme of the start window
    QFile DarkThemeStyleSheetFile("src/thm/dark/style.qss");
    DarkThemeStyleSheetFile.open(QIODevice::ReadOnly);
    App.setStyleSheet(DarkThemeStyleSheetFile.readAll());

    QQmlApplicationEngine QMLEngine;
//    const QUrl MainQMLFile(u"qrc:/wmm/src/main.qml"_qs);
    const QUrl MainQMLFile = QUrl::fromLocalFile(u"src/main.qml"_qs);
    QObject::connect(&QMLEngine, &QQmlApplicationEngine::objectCreated, &App,
                     [MainQMLFile](QObject* Obj, const QUrl& ObjURL) { if (!Obj && MainQMLFile == ObjURL) QCoreApplication::exit(-1); },
                     Qt::QueuedConnection);
    QMLEngine.load(MainQMLFile);

    int ret = Test::Start();

    auto DataSourceManager = new class DataSourceManager();
    DataSourceManager->showMaximized();

    return App.exec();
}
