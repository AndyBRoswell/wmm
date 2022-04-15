#include "MongoDBAccessor.h"

// mongocxx
#include <bsoncxx/json.hpp>

namespace WritingMaterialsManager {
    MongoDBAccessor::MongoDBAccessor(const char* const MongoDBURI) :
        DBURI(mongocxx::uri(MongoDBURI)),
        Client(DBURI) {}

    QByteArray MongoDBAccessor::GetDatabasesInformation() {
        mongocxx::cursor DBInfoCur = Client.list_databases();
        QByteArray Result = "[";
        for (auto&& DBInfoDoc: DBInfoCur) {
            Result.append(bsoncxx::to_json(DBInfoDoc).c_str()).append(',');
        }
        Result.replace(Result.length() - 2, 1, "]");
        return Result;
    }

    QByteArray MongoDBAccessor::GetCollectionsInformation(mongocxx::database& Database) {
        mongocxx::cursor CollInfoCur = Database.list_collections();
        QByteArray Result = "[";
        for (auto&& CollInfoDoc: CollInfoCur) {
            Result.append(bsoncxx::to_json(CollInfoDoc).c_str()).append(',');
        }
        Result.replace(Result.length() - 1, 1, "]");
        return Result;
    }

    QByteArray MongoDBAccessor::GetDBsAndCollsInfo() {
        mongocxx::cursor DBInfoCur = Client.list_databases();
        QByteArray Result = "[";
        for (auto&& DBInfoDoc: DBInfoCur) {
            Result.append(bsoncxx::to_json(DBInfoDoc).c_str()).replace(Result.length() - 1, 1, R"(,"Collections":)");
            mongocxx::database Database = Client[DBInfoDoc["name"].get_utf8().value];
            Result.append(GetCollectionsInformation(Database)).append("},");
        }
        Result.replace(Result.length() - 1, 1, "]");
        return Result;
    }
}
