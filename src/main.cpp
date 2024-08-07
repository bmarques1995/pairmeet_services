#include <drogon/drogon.h>

using namespace drogon;
int main(int argc, char** argv)
{
    app().setLogPath("./")
         .setLogLevel(trantor::Logger::kWarn)
         .addListener("127.0.0.1", 8000)
         .setThreadNum(8)
         .run();
}
