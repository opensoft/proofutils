// clazy:skip

#include "gtest/test_global.h"

#include "proofnetwork/ums/tokensapi.h"
#include "proofnetwork/ums/data/umsuser.h"

#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QSignalSpy>

using testing::Test;

class TokensApiTest: public Test
{
public:
    TokensApiTest()
    {
    }

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

    QSignalSpy spy(tokensApiUT, &Proof::Ums::TokensApi::tokenFetched);
    qulonglong opId = tokensApiUT->fetchToken();
    ASSERT_TRUE(spy.wait());
    EXPECT_EQ(1, spy.count());
    QList<QVariant> arguments = spy.takeFirst();
    ASSERT_EQ(3, arguments.count());

    qulonglong retOpId = arguments.at(0).toULongLong();
    QString token = qvariant_cast<QString>(arguments.at(1));
    EXPECT_EQ(token, QString(tokenFromFile));

    Proof::Ums::UmsUserSP umsUser = qvariant_cast<Proof::Ums::UmsUserSP>(arguments.at(2));
    EXPECT_EQ(opId, retOpId);
    EXPECT_TRUE(umsUser->isFetched());
    EXPECT_EQ("0e513a63-035b-49e6-94ee-b452e20da844", umsUser->id());
}

TEST_F(TokensApiTest, fetchTokenByBarcode)
{
    ASSERT_TRUE(serverRunner->serverIsRunning());
    QByteArray tokenFromFile = dataFromFile(":/data/token.json");
    ASSERT_FALSE(tokenFromFile.isEmpty());
    serverRunner->setServerAnswer(tokenFromFile);
    tokenFromFile = QJsonDocument::fromJson(tokenFromFile).object().value("access_token").toString().toUtf8();

    QSignalSpy spy(tokensApiUT, &Proof::Ums::TokensApi::tokenFetched);
    qulonglong opId = tokensApiUT->fetchTokenByBarcode("");
    ASSERT_TRUE(spy.wait());
    EXPECT_EQ(1, spy.count());
    QList<QVariant> arguments = spy.takeFirst();
    ASSERT_EQ(3, arguments.count());

    qulonglong retOpId = arguments.at(0).toULongLong();
    QString token = qvariant_cast<QString>(arguments.at(1));
    EXPECT_EQ(token, QString(tokenFromFile));

    Proof::Ums::UmsUserSP umsUser = qvariant_cast<Proof::Ums::UmsUserSP>(arguments.at(2));
    EXPECT_EQ(opId, retOpId);
    EXPECT_TRUE(umsUser->isFetched());
    EXPECT_EQ("0e513a63-035b-49e6-94ee-b452e20da844", umsUser->id());
}

TEST_F(TokensApiTest, fetchTokenByLogin)
{
    ASSERT_TRUE(serverRunner->serverIsRunning());
    QByteArray tokenFromFile = dataFromFile(":/data/token.json");
    ASSERT_FALSE(tokenFromFile.isEmpty());
    serverRunner->setServerAnswer(tokenFromFile);
    tokenFromFile = QJsonDocument::fromJson(tokenFromFile).object().value("access_token").toString().toUtf8();

    QSignalSpy spy(tokensApiUT, &Proof::Ums::TokensApi::tokenFetched);
    qulonglong opId = tokensApiUT->fetchTokenByLogin("", "");
    ASSERT_TRUE(spy.wait());
    EXPECT_EQ(1, spy.count());
    QList<QVariant> arguments = spy.takeFirst();
    ASSERT_EQ(3, arguments.count());

    qulonglong retOpId = arguments.at(0).toULongLong();
    QString token = qvariant_cast<QString>(arguments.at(1));
    EXPECT_EQ(token, QString(tokenFromFile));

    Proof::Ums::UmsUserSP umsUser = qvariant_cast<Proof::Ums::UmsUserSP>(arguments.at(2));
    EXPECT_EQ(opId, retOpId);
    EXPECT_TRUE(umsUser->isFetched());
    EXPECT_EQ("0e513a63-035b-49e6-94ee-b452e20da844", umsUser->id());
}

TEST_F(TokensApiTest, refreshToken)
{
    ASSERT_TRUE(serverRunner->serverIsRunning());
    QByteArray tokenFromFile = dataFromFile(":/data/token.json");
    ASSERT_FALSE(tokenFromFile.isEmpty());
    serverRunner->setServerAnswer(tokenFromFile);
    tokenFromFile = QJsonDocument::fromJson(tokenFromFile).object().value("access_token").toString().toUtf8();

    QSignalSpy spy(tokensApiUT, &Proof::Ums::TokensApi::tokenFetched);
    qulonglong opId = tokensApiUT->refreshToken("");
    ASSERT_TRUE(spy.wait());
    EXPECT_EQ(1, spy.count());
    QList<QVariant> arguments = spy.takeFirst();
    ASSERT_EQ(3, arguments.count());

    qulonglong retOpId = arguments.at(0).toULongLong();
    QString token = qvariant_cast<QString>(arguments.at(1));
    EXPECT_EQ(token, QString(tokenFromFile));

    Proof::Ums::UmsUserSP umsUser = qvariant_cast<Proof::Ums::UmsUserSP>(arguments.at(2));
    EXPECT_EQ(opId, retOpId);
    EXPECT_TRUE(umsUser->isFetched());
    EXPECT_EQ("0e513a63-035b-49e6-94ee-b452e20da844", umsUser->id());
}

TEST_F(TokensApiTest, fetchPublicKey)
{
    ASSERT_TRUE(serverRunner->serverIsRunning());
    serverRunner->setServerAnswer("12345");

    QSignalSpy spy(tokensApiUT, &Proof::Ums::TokensApi::publicKeyFetched);
    qulonglong opId = tokensApiUT->fetchPublicKey();
    ASSERT_TRUE(spy.wait());
    EXPECT_EQ(1, spy.count());
    QList<QVariant> arguments = spy.takeFirst();
    ASSERT_EQ(2, arguments.count());

    qulonglong retOpId = arguments.at(0).toULongLong();
    EXPECT_EQ(opId, retOpId);
    QString answer = qvariant_cast<QString>(arguments.at(1));
    EXPECT_EQ("12345", answer);
}

TEST_F(TokensApiTest, fetchCertificate)
{
    ASSERT_TRUE(serverRunner->serverIsRunning());
    serverRunner->setServerAnswer("12345");

    QSignalSpy spy(tokensApiUT, &Proof::Ums::TokensApi::certificateFetched);
    qulonglong opId = tokensApiUT->fetchCertificate();
    ASSERT_TRUE(spy.wait());
    EXPECT_EQ(1, spy.count());
    QList<QVariant> arguments = spy.takeFirst();
    ASSERT_EQ(2, arguments.count());

    qulonglong retOpId = arguments.at(0).toULongLong();
    EXPECT_EQ(opId, retOpId);
    QString answer = qvariant_cast<QString>(arguments.at(1));
    EXPECT_EQ("12345", answer);
}
