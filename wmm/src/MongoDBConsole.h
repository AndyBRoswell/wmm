#ifndef MONGODBCONSOLE_H
#define MONGODBCONSOLE_H

#include <QListView>
#include <QSplitter>
#include <QWidget>

namespace WritingMaterialsManager {
    class MongoDBConsole : public QWidget {
    Q_OBJECT

    public:
        explicit MongoDBConsole(QWidget* const Parent = nullptr);
        ~MongoDBConsole();

        std::shared_ptr<QSplitter> FunctionArea;


    private:
    };
}

#endif // MONGODBCONSOLE_H
