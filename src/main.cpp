#include <drogon/drogon.h>
#include <openssl/evp.h>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <sstream>
#include <json/json.h>
#include <mysqlx/xdevapi.h>
#include <mysqlx/xapi.h>
#include <jwt-cpp/jwt.h>

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
        [](const drogon::HttpRequestPtr& req,
            std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
                std::string url = "mysqlx://root:admin@127.0.0.1";
                mysqlx::Session sess(url);
                mysqlx::Schema sch= sess.getSchema("pairmeet");
                mysqlx::Table tab = sch.getTable("Users", true);

                // Buffer to store the hash
                uint8_t hash[EVP_MAX_MD_SIZE];
                uint32_t hash_length;

                Json::Value reqInfo;
                Json::Reader reader;
                std::string debugReq;
                {
                    debugReq = req->body().data();
                    reader.parse(req->body().data(), reqInfo);
                }
                {
                    // Create a context for the digest operation
                    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();

                    std::string password = reqInfo["Password"].asString();

                    if (mdctx == NULL) {
                        std::cerr << "Failed to create context" << std::endl;
                    }

                    // Initialize the digest operation with SHA3-512
                    if (EVP_DigestInit_ex(mdctx, EVP_sha3_512(), NULL) != 1) {
                        std::cerr << "Failed to initialize digest" << std::endl;
                        EVP_MD_CTX_free(mdctx);
                    }

                    // Update the context with the message
                    if (EVP_DigestUpdate(mdctx, password.c_str(), password.length()) != 1) {
                        std::cerr << "Failed to update digest" << std::endl;
                        EVP_MD_CTX_free(mdctx);
                    }

                    // Finalize the digest operation and obtain the hash
                    if (EVP_DigestFinal_ex(mdctx, hash, &hash_length) != 1) {
                        std::cerr << "Failed to finalize digest" << std::endl;
                        EVP_MD_CTX_free(mdctx);
                    }
                    EVP_MD_CTX_free(mdctx);
                }
                try
                {
                    mysqlx::Result res = tab.insert("Name", "Email", "Password", "VerifiedEmail", "Locale")
                    .values(reqInfo["Name"].asString(), reqInfo["Email"].asString(), mysqlx::bytes(hash, 64), false, "ja_JP")
                    .execute();

                    auto token = jwt::create()
                        .set_type("JWS")
                        .set_issuer("auth0")
                        .set_payload_claim("Email", jwt::claim(reqInfo["Email"].asString()))
                        .sign(jwt::algorithm::hs256{"secret"});

                    auto resp = drogon::HttpResponse::newHttpResponse();
                    std::string host = "http://";
                    host.append(req->getHeader("Host"));
                    host.append("/db_sample_confirm/");
                    host.append(token.data());
                    resp->setStatusCode(drogon::HttpStatusCode::k200OK);
                    resp->setBody(host);
                    callback(resp);
                }
                catch(const mysqlx::Error &err)
                {
                    auto resp = drogon::HttpResponse::newHttpResponse();
                    resp->setStatusCode(drogon::HttpStatusCode::k502BadGateway);
                    resp->setBody(err.what());
                    callback(resp);
                }
        }, { drogon::Post });

    drogon::app().registerHandler(
        "/db_sample_confirm/{token}",
        [](const drogon::HttpRequestPtr&,
            std::function<void(const drogon::HttpResponsePtr&)>&& callback, const std::string &token) {
                std::string url = "mysqlx://root:admin@127.0.0.1";
                mysqlx::Session sess(url);
                mysqlx::Schema sch= sess.getSchema("pairmeet");
                mysqlx::Table tab = sch.getTable("Users", true);

                std::string decoded = jwt::decode(token).get_payload();

                Json::Value reqInfo;
                Json::Reader reader;
                reader.parse(decoded, reqInfo);

                std::string email = reqInfo["Email"].asString();

                tab.update()
                    .set("VerifiedEmail", true)
                    .where("Email = :email")
                    .bind("email", email)
                    .execute();

                auto resp = drogon::HttpResponse::newHttpResponse();
                resp->setStatusCode(drogon::HttpStatusCode::k200OK);
                callback(resp);
        }, { drogon::Put });
    drogon::app().setLogPath("./")
         .setLogLevel(trantor::Logger::kWarn)
         //.setSSLFiles()
         .addListener("127.0.0.1", 8000)
         .setThreadNum(8)
         .run();
}
