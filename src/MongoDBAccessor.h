#ifndef WRITING_MATERIALS_MANAGER_MONGODBACCESSOR_H
#define WRITING_MATERIALS_MANAGER_MONGODBACCESSOR_H

// mongocxx
#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>

// Qt
#include <QString>

namespace WritingMaterialsManager {
    class MongoDBAccessor {
    public:
        static constexpr char LocalMongoDBURI[] = "mongodb://localhost:27017/?directConnection=true&serverSelectionTimeoutMS=2000";

        MongoDBAccessor(const char* const MongoDBURI = LocalMongoDBURI);
        // return string 'cause different document DBs use different internal data structures.
        QByteArray GetDatabasesInformation();
        QByteArray GetCollectionsInformation(const QByteArray& DatabaseName);
        QByteArray GetDBsAndCollsInfo();
    private:
        inline static const mongocxx::instance mongocxxDriver{}; // This represents the mongocxx driver instance hence should be done only once.
        mongocxx::uri DBURI;
        mongocxx::client Client;

        QByteArray GetCollectionsInformation(mongocxx::database& Database);
    };
}

#endif //WRITING_MATERIALS_MANAGER_MONGODBACCESSOR_H
