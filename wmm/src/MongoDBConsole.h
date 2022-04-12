#ifndef MONGODBCONSOLE_H
#define MONGODBCONSOLE_H

#include <QWidget>

namespace Ui {
class MongoDBConsole;
}

class MongoDBConsole : public QWidget
{
    Q_OBJECT

public:
    explicit MongoDBConsole(QWidget *parent = nullptr);
    ~MongoDBConsole();

private:
    Ui::MongoDBConsole *ui;
};

#endif // MONGODBCONSOLE_H
