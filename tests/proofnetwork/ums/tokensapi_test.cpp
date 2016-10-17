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
        tokensApiUT = new Proof::Ums::TokensApi(restClient);
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
    EXPECT_EQ("5fa11623-470d-44f4-94ba-58e7eddb0ded", umsUser->id());
}