// clazy:skip

#include "proofnetwork/ums/data/umstokeninfo.h"
#include "proofnetwork/ums/data/umsuser.h"
#include "proofnetwork/ums/tokensapi.h"

#include "gtest/test_global.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

using testing::Test;

class TokensApiTest : public Test
{
public:
    TokensApiTest() {}

protected:
    void SetUp() override
    {
        serverRunner = new FakeServerRunner;
        serverRunner->runServer();

        Proof::RestClientSP restClient = Proof::RestClientSP::create();
        restClient->setAuthType(Proof::RestAuthType::NoAuth);
        restClient->setHost("127.0.0.1");
        restClient->setPort(9091); //Default port for FakeServer
        restClient->setScheme("http");
        restClient->setClientName("Proof-test");
        tokensApiUT = new Proof::Ums::TokensApi("test", "test", restClient);
        tokensApiUT->setRsaKey(QCA::PublicKey::fromPEM(dataFromFile(":/data/pub_rsa.key")).toRSA());
    }

    void TearDown() override
    {
        delete tokensApiUT;
        delete serverRunner;
    }

protected:
    Proof::Ums::TokensApi *tokensApiUT;
    FakeServerRunner *serverRunner;
};

TEST_F(TokensApiTest, fetchToken)
{
    ASSERT_TRUE(serverRunner->serverIsRunning());
    QByteArray tokenFromFile = dataFromFile(":/data/token.json");
    ASSERT_FALSE(tokenFromFile.isEmpty());
    serverRunner->setServerAnswer(tokenFromFile);
    tokenFromFile = QJsonDocument::fromJson(tokenFromFile).object().value("access_token").toString().toUtf8();

    Proof::Ums::UmsTokenInfoSP tokenInfo = tokensApiUT->fetchToken()->result();
    EXPECT_EQ(FakeServer::Method::Post, serverRunner->lastQueryMethod());
    EXPECT_EQ(QUrl("/oauth2/token"), serverRunner->lastQueryUrl());
    auto query = QUrlQuery(serverRunner->lastQueryBody());
    EXPECT_TRUE(query.hasQueryItem("grant_type"));
    EXPECT_EQ("client_credentials", query.queryItemValue("grant_type"));
    EXPECT_TRUE(query.hasQueryItem("client_id"));
    EXPECT_EQ("test", query.queryItemValue("client_id"));
    EXPECT_TRUE(query.hasQueryItem("client_secret"));
    EXPECT_EQ("test", query.queryItemValue("client_secret"));

    EXPECT_TRUE(tokenInfo->isFetched());
    EXPECT_TRUE(tokenInfo->user()->isFetched());
    EXPECT_EQ("testuser@test_company.com", tokenInfo->user()->userName());
    EXPECT_EQ(QString(tokenFromFile), tokenInfo->token());
}

TEST_F(TokensApiTest, fetchTokenByBarcode)
{
    ASSERT_TRUE(serverRunner->serverIsRunning());
    QByteArray tokenFromFile = dataFromFile(":/data/token.json");
    ASSERT_FALSE(tokenFromFile.isEmpty());
    serverRunner->setServerAnswer(tokenFromFile);
    tokenFromFile = QJsonDocument::fromJson(tokenFromFile).object().value("access_token").toString().toUtf8();

    Proof::Ums::UmsTokenInfoSP tokenInfo = tokensApiUT->fetchTokenByBarcode(QString("barcode"))->result();

    EXPECT_EQ(FakeServer::Method::Post, serverRunner->lastQueryMethod());
    EXPECT_EQ(QUrl("/oauth2/token"), serverRunner->lastQueryUrl());
    auto query = QUrlQuery(serverRunner->lastQueryBody());
    EXPECT_TRUE(query.hasQueryItem("grant_type"));
    EXPECT_EQ("client_credentials", query.queryItemValue("grant_type"));
    EXPECT_TRUE(query.hasQueryItem("client_id"));
    EXPECT_EQ("test", query.queryItemValue("client_id"));
    EXPECT_TRUE(query.hasQueryItem("client_secret"));
    EXPECT_EQ("test", query.queryItemValue("client_secret"));
    EXPECT_TRUE(query.hasQueryItem("barcode"));
    EXPECT_EQ("barcode", query.queryItemValue("barcode"));

    EXPECT_TRUE(tokenInfo->isFetched());
    EXPECT_TRUE(tokenInfo->user()->isFetched());
    EXPECT_EQ("testuser@test_company.com", tokenInfo->user()->userName());
    EXPECT_EQ(QString(tokenFromFile), tokenInfo->token());
}

TEST_F(TokensApiTest, fetchTokenByLogin)
{
    ASSERT_TRUE(serverRunner->serverIsRunning());
    QByteArray tokenFromFile = dataFromFile(":/data/token.json");
    ASSERT_FALSE(tokenFromFile.isEmpty());
    serverRunner->setServerAnswer(tokenFromFile);
    tokenFromFile = QJsonDocument::fromJson(tokenFromFile).object().value("access_token").toString().toUtf8();

    Proof::Ums::UmsTokenInfoSP tokenInfo = tokensApiUT->fetchTokenByLogin("login", "password")->result();

    EXPECT_EQ(FakeServer::Method::Post, serverRunner->lastQueryMethod());
    EXPECT_EQ(QUrl("/oauth2/token"), serverRunner->lastQueryUrl());
    auto query = QUrlQuery(serverRunner->lastQueryBody());
    EXPECT_TRUE(query.hasQueryItem("grant_type"));
    EXPECT_EQ("password", query.queryItemValue("grant_type"));
    EXPECT_TRUE(query.hasQueryItem("client_id"));
    EXPECT_EQ("test", query.queryItemValue("client_id"));
    EXPECT_TRUE(query.hasQueryItem("client_secret"));
    EXPECT_EQ("test", query.queryItemValue("client_secret"));
    EXPECT_TRUE(query.hasQueryItem("username"));
    EXPECT_EQ("login", query.queryItemValue("username"));
    EXPECT_TRUE(query.hasQueryItem("password"));
    EXPECT_EQ("password", query.queryItemValue("password"));

    EXPECT_TRUE(tokenInfo->isFetched());
    EXPECT_TRUE(tokenInfo->user()->isFetched());
    EXPECT_EQ("testuser@test_company.com", tokenInfo->user()->userName());
    EXPECT_EQ(QString(tokenFromFile), tokenInfo->token());
}

TEST_F(TokensApiTest, refreshToken)
{
    ASSERT_TRUE(serverRunner->serverIsRunning());
    QByteArray tokenFromFile = dataFromFile(":/data/token.json");
    ASSERT_FALSE(tokenFromFile.isEmpty());
    serverRunner->setServerAnswer(tokenFromFile);
    tokenFromFile = QJsonDocument::fromJson(tokenFromFile).object().value("access_token").toString().toUtf8();

    Proof::Ums::UmsTokenInfoSP tokenInfo = tokensApiUT->refreshToken(QString("oldtoken"))->result();
    EXPECT_EQ(FakeServer::Method::Post, serverRunner->lastQueryMethod());
    EXPECT_EQ(QUrl("/oauth2/token"), serverRunner->lastQueryUrl());
    auto query = QUrlQuery(serverRunner->lastQueryBody());
    EXPECT_TRUE(query.hasQueryItem("grant_type"));
    EXPECT_EQ("refresh_token", query.queryItemValue("grant_type"));
    EXPECT_TRUE(query.hasQueryItem("client_id"));
    EXPECT_EQ("test", query.queryItemValue("client_id"));
    EXPECT_TRUE(query.hasQueryItem("client_secret"));
    EXPECT_EQ("test", query.queryItemValue("client_secret"));
    EXPECT_TRUE(query.hasQueryItem("refresh_token"));
    EXPECT_EQ("oldtoken", query.queryItemValue("refresh_token"));

    EXPECT_TRUE(tokenInfo->isFetched());
    EXPECT_TRUE(tokenInfo->user()->isFetched());
    EXPECT_EQ("testuser@test_company.com", tokenInfo->user()->userName());
    EXPECT_EQ(QString(tokenFromFile), tokenInfo->token());
}

TEST_F(TokensApiTest, fetchPublicKey)
{
    ASSERT_TRUE(serverRunner->serverIsRunning());
    serverRunner->setServerAnswer("12345");

    QString answer = tokensApiUT->fetchPublicKey()->result();
    EXPECT_EQ(FakeServer::Method::Get, serverRunner->lastQueryMethod());
    EXPECT_EQ(QUrl("/Token/GetPublicKey"), serverRunner->lastQueryUrl());
    EXPECT_TRUE(serverRunner->lastQueryBody().isEmpty());

    EXPECT_EQ("12345", answer);
}

TEST_F(TokensApiTest, fetchCertificate)
{
    ASSERT_TRUE(serverRunner->serverIsRunning());
    serverRunner->setServerAnswer("12345");

    QString answer = tokensApiUT->fetchCertificate()->result();
    EXPECT_EQ(FakeServer::Method::Get, serverRunner->lastQueryMethod());
    EXPECT_EQ(QUrl("/Token/GetCertificate"), serverRunner->lastQueryUrl());
    EXPECT_TRUE(serverRunner->lastQueryBody().isEmpty());

    EXPECT_EQ("12345", answer);
}
