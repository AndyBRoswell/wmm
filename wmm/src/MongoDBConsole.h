#ifndef MONGODBCONSOLE_H
#define MONGODBCONSOLE_H

#include <QTable
#include <QSplitter>
#include <QWidget>

namespace WritingMaterialsManager {
    class MongoDBConsole : public QWidget {
    Q_OBJECT

    public:
        explicit MongoDBConsole(QWidget* const Parent = nullptr);
        ~MongoDBConsole();

        std::shared_ptr<QSplitter> FunctionArea;
        std::shared_ptr<>

    private:
    };
}

#endif // MONGODBCONSOLE_H
