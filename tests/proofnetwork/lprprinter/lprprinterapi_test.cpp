// clazy:skip

#include "gtest/test_global.h"

#include "proofnetwork/lprprinter/lprprinterapi.h"

using namespace Proof::NetworkServices;
using testing::Test;

class LprPrinterApiTest : public Test
{
public:
    LprPrinterApiTest()
    {
    }

protected:
    void SetUp() override
    {
        auto restClient = Proof::RestClientSP::create();
        restClient->setAuthType(Proof::RestAuthType::NoAuth);
        restClient->setHost("127.0.0.1");
        restClient->setPort(9091); //Default port for FakeServer
        restClient->setScheme("http");
        restClient->setClientName("Proof-test");
        lprPrinterApi = new LprPrinterApi(restClient);

        serverRunner = new FakeServerRunner();
        serverRunner->runServer();
    }

    void TearDown() override
    {
        delete serverRunner;
        delete lprPrinterApi;
    }

protected:
    LprPrinterApi *lprPrinterApi;
    FakeServerRunner *serverRunner;
};

TEST_F(LprPrinterApiTest, fetchStatus)
{
    ASSERT_TRUE(serverRunner->serverIsRunning());

    QByteArray json = dataFromFile(":/data/status.json").trimmed();
    ASSERT_FALSE(json.isEmpty());
    serverRunner->setServerAnswer(json);

    auto result = lprPrinterApi->fetchStatus()->result();
    EXPECT_TRUE(result.isReady);
    EXPECT_EQ("", result.reason);
}

TEST_F(LprPrinterApiTest, fetchBadStatus)
{
    ASSERT_TRUE(serverRunner->serverIsRunning());

    QByteArray json = dataFromFile(":/data/failed_status.json").trimmed();
    ASSERT_FALSE(json.isEmpty());
    serverRunner->setServerAnswer(json);

    auto result = lprPrinterApi->fetchStatus()->result();
    EXPECT_FALSE(result.isReady);
    EXPECT_EQ("Some error occurred", result.reason);
}

TEST_F(LprPrinterApiTest, printLabel)
{
    ASSERT_TRUE(serverRunner->serverIsRunning());

    QByteArray json = dataFromFile(":/data/status.json").trimmed();
    ASSERT_FALSE(json.isEmpty());
    serverRunner->setServerAnswer(json);

    auto result = lprPrinterApi->printLabel("something");
    result->wait();
    ASSERT_TRUE(result->succeeded());
    EXPECT_TRUE(result->result());
}

TEST_F(LprPrinterApiTest, failedPrintLabel)
{
    ASSERT_TRUE(serverRunner->serverIsRunning());

    QByteArray json = dataFromFile(":/data/failed_status.json").trimmed();
    ASSERT_FALSE(json.isEmpty());
    serverRunner->setServerAnswer(json);

    auto result = lprPrinterApi->printLabel("something");
    result->wait();
    ASSERT_TRUE(result->failed());
    EXPECT_EQ("Some error occurred", result->failureReason().message);
}

TEST_F(LprPrinterApiTest, printFile)
{
    ASSERT_TRUE(serverRunner->serverIsRunning());

    QByteArray json = dataFromFile(":/data/status.json").trimmed();
    ASSERT_FALSE(json.isEmpty());
    serverRunner->setServerAnswer(json);

    auto result = lprPrinterApi->printFile(":/data/status.json");
    result->wait();
    ASSERT_TRUE(result->succeeded());
    EXPECT_TRUE(result->result());
}

TEST_F(LprPrinterApiTest, failedPrintFile)
{
    ASSERT_TRUE(serverRunner->serverIsRunning());

    QByteArray json = dataFromFile(":/data/failed_status.json").trimmed();
    ASSERT_FALSE(json.isEmpty());
    serverRunner->setServerAnswer(json);

    auto result = lprPrinterApi->printFile(":/data/status.json");
    result->wait();
    ASSERT_TRUE(result->failed());
    EXPECT_EQ("Some error occurred", result->failureReason().message);
}

TEST_F(LprPrinterApiTest, fetchPrintersList)
{
    ASSERT_TRUE(serverRunner->serverIsRunning());

    QByteArray json = dataFromFile(":/data/printers.json").trimmed();
    ASSERT_FALSE(json.isEmpty());
    serverRunner->setServerAnswer(json);

    const QList<Proof::NetworkServices::LprPrinterInfo> printers = lprPrinterApi->fetchPrintersList()->result();
    ASSERT_EQ(2, printers.count());

    EXPECT_EQ("Zebra", printers[0].printer);
    EXPECT_TRUE(printers[0].acceptsRaw);
    EXPECT_FALSE(printers[0].acceptsFiles);

    EXPECT_EQ("HP", printers[1].printer);
    EXPECT_FALSE(printers[1].acceptsRaw);
    EXPECT_TRUE(printers[1].acceptsFiles);
}
