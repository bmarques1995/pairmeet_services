#include <drogon/drogon.h>


int main(int argc, char** argv)
{
    drogon::app().registerHandler(
        "/",
        [](const drogon::HttpRequestPtr&,
            std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
                auto resp = drogon::HttpResponse::newHttpResponse();
                resp->setBody("Hello, World!");
                callback(resp);
        },
        { drogon::Get });
    drogon::app().setLogPath("./")
         .setLogLevel(trantor::Logger::kWarn)
         .addListener("127.0.0.1", 8000)
         .setThreadNum(8)
         .run();
}
