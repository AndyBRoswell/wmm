#ifndef WRITING_ASSISTANT_MONGODBACCESSOR_H
#define WRITING_ASSISTANT_MONGODBACCESSOR_H

// mongocxx
#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>

// Qt
#include <QString>

namespace WritingMaterialsManager {
    class MongoDBAccessor {
    public:
        MongoDBAccessor(const char* const MongoDBURI = "mongodb://localhost:27017/?directConnection=true&serverSelectionTimeoutMS=2000");
        // return string 'cause different document DBs use different internal data structures.
        QByteArray GetDatabasesInformation();
        QByteArray GetCollectionsInformation(mongocxx::database& Database);
        QByteArray GetDBsAndCollsInfo();

    private:
        static const mongocxx::instance mongocxxDriver; // This represents the mongocxx driver instance hence should be done only once.
        mongocxx::uri DBURI;
        mongocxx::client Client;
    };
}

#endif //WRITING_ASSISTANT_MONGODBACCESSOR_H
