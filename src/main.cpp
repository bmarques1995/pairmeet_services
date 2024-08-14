#include <drogon/drogon.h>
#include <openssl/evp.h>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <sstream>
#include <json/json.h>
#include <mysqlx/xdevapi.h>
#include <mysqlx/xapi.h>


int main(int argc, char** argv)
{
    drogon::app().registerHandler(
        "/",
        [](const drogon::HttpRequestPtr&,
            std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
                // Message to hash
                Json::Value value;
                
                //Json::Writer writer();
                std::string message = "Hello, World!";

                value["message"] = message;

                // Buffer to store the hash
                unsigned char hash[EVP_MAX_MD_SIZE];
                unsigned int hash_length;

                // Create a context for the digest operation
                EVP_MD_CTX *mdctx = EVP_MD_CTX_new();

                if (mdctx == NULL) {
                    std::cerr << "Failed to create context" << std::endl;
                }

                // Initialize the digest operation with SHA3-512
                if (EVP_DigestInit_ex(mdctx, EVP_sha3_512(), NULL) != 1) {
                    std::cerr << "Failed to initialize digest" << std::endl;
                    EVP_MD_CTX_free(mdctx);
                }

                // Update the context with the message
                if (EVP_DigestUpdate(mdctx, message.c_str(), std::strlen(message.c_str())) != 1) {
                    std::cerr << "Failed to update digest" << std::endl;
                    EVP_MD_CTX_free(mdctx);
                }

                // Finalize the digest operation and obtain the hash
                if (EVP_DigestFinal_ex(mdctx, hash, &hash_length) != 1) {
                    std::cerr << "Failed to finalize digest" << std::endl;
                    EVP_MD_CTX_free(mdctx);
                }

                // Print the hash
                std::stringstream buffer;
                for (unsigned int i = 0; i < hash_length; i++) {
                    buffer << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
                }
                // Clean up
                EVP_MD_CTX_free(mdctx);
                
                value["sha512"] = buffer.str();
                Json::FastWriter writer;

                std::string miniJson = writer.write(value);

                auto resp = drogon::HttpResponse::newHttpResponse();
                resp->setBody(miniJson);
                callback(resp);
        }, { drogon::Get });
        //38e05c33d7b067127f217d8c856e554fcff09c9320b8a5979ce2ff5d95dd27ba35d1fba50c562dfd1d6cc48bc9c5baa4390894418cc942d968f97bcb659419ed
    drogon::app().registerHandler(
        "/db_sample",
        [](const drogon::HttpRequestPtr&,
            std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
                std::string url = "mysqlx://root:admin@127.0.0.1";
                mysqlx::Session sess(url);
                mysqlx::Schema sch= sess.getSchema("pairmeet");
                mysqlx::Table tab = sch.getTable("Users", true);

                uint8_t hash[64] =
                {
                    0x38, 0xe0, 0x5c, 0x33, 0xd7, 0xb0, 0x67, 0x12,
                    0x7f, 0x21, 0x7d, 0x8c, 0x85, 0x6e, 0x55, 0x4f,
                    0xcf, 0xf0, 0x9c, 0x93, 0x20, 0xb8, 0xa5, 0x97,
                    0x9c, 0xe2, 0xff, 0x5d, 0x95, 0xdd, 0x27, 0xba,
                    0x35, 0xd1, 0xfb, 0xa5, 0x0c, 0x56, 0x2d, 0xfd,
                    0x1d, 0x6c, 0xc4, 0x8b, 0xc9, 0xc5, 0xba, 0xa4,
                    0x39, 0x08, 0x94, 0x41, 0x8c, 0xc9, 0x42, 0xd9,
                    0x68, 0xf9, 0x7b, 0xcb, 0x65, 0x94, 0x19, 0xed
                };

                std::string binaryData(reinterpret_cast<const char*>(hash), 64);

                bool verified = false;

                tab.insert("Name", "Email", "Password", "VerifiedEmail", "Locale")
                .values("John Doe", "john.doe@example.com", mysqlx::bytes(hash, 64), false, "en_US")
                .execute();

                auto resp = drogon::HttpResponse::newHttpResponse();
                resp->setStatusCode(drogon::HttpStatusCode::k200OK);
                callback(resp);
        }, { drogon::Get });

    drogon::app().registerHandler(
        "/db_sample_confirm",
        [](const drogon::HttpRequestPtr&,
            std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
                std::string url = "mysqlx://root:admin@127.0.0.1";
                mysqlx::Session sess(url);
                mysqlx::Schema sch= sess.getSchema("pairmeet");
                mysqlx::Table tab = sch.getTable("Users", true);

                tab.update()
                    .set("VerifiedEmail", true)
                    .where("Email = :email")
                    .bind("email", "john.doe@example.com")
                    .execute();

                auto resp = drogon::HttpResponse::newHttpResponse();
                resp->setStatusCode(drogon::HttpStatusCode::k200OK);
                callback(resp);
        }, { drogon::Get });
    drogon::app().setLogPath("./")
         .setLogLevel(trantor::Logger::kWarn)
         .addListener("127.0.0.1", 8000)
         .setThreadNum(8)
         .run();
}
