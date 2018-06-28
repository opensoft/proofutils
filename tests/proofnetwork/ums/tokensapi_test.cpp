// clazy:skip

#include "proofnetwork/ums/data/umstokeninfo.h"
#include "proofnetwork/ums/data/umsuser.h"
#include "proofnetwork/ums/tokensapi.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSignalSpy>

#include "gtest/test_global.h"

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
#ifndef QCA_DISABLED
        tokensApiUT->setRsaKey(QCA::PublicKey::fromPEM(dataFromFile(":/data/pub_rsa.key")).toRSA());
#endif
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

    Proof::Ums::UmsTokenInfoSP tokenInfo = tokensApiUT->fetchTokenByBarcode(QString())->result();
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

    Proof::Ums::UmsTokenInfoSP tokenInfo = tokensApiUT->fetchTokenByLogin("", "")->result();
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

    Proof::Ums::UmsTokenInfoSP tokenInfo = tokensApiUT->refreshToken(QString())->result();
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
    EXPECT_EQ("12345", answer);
}

TEST_F(TokensApiTest, fetchCertificate)
{
    ASSERT_TRUE(serverRunner->serverIsRunning());
    serverRunner->setServerAnswer("12345");

    QString answer = tokensApiUT->fetchCertificate()->result();
    EXPECT_EQ("12345", answer);
}
