#ifndef MONGODBCONSOLE_H
#define MONGODBCONSOLE_H

#include <QWidget>

namespace WritingMaterialsManager {
    class MongoDBConsole : public QWidget {
    Q_OBJECT

    public:
        explicit MongoDBConsole(QWidget* const Parent = nullptr);
        ~MongoDBConsole();

    private:
    };
}

#endif // MONGODBCONSOLE_H
