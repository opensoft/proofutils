#include "gtest/test_global.h"

#include "proofnetwork/lprprinter/lprprinterapi.h"

#include <QSignalSpy>
#include <QJsonDocument>
#include <QImage>

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

    QSignalSpy spy(lprPrinterApi, SIGNAL(statusFetched(qulonglong,Proof::NetworkServices::LprPrinterStatus)));
    const qulonglong operationId = lprPrinterApi->fetchStatus();

    ASSERT_TRUE(spy.wait());
    EXPECT_EQ(1, spy.count());

    QList<QVariant> arguments = spy.takeFirst();
    ASSERT_EQ(2, arguments.count());

    EXPECT_EQ(operationId, arguments[0].toULongLong());

    auto result = qvariant_cast<Proof::NetworkServices::LprPrinterStatus>(arguments[1]);
    EXPECT_TRUE(result.isReady);
    EXPECT_EQ("", result.reason);
}

TEST_F(LprPrinterApiTest, fetchBadStatus)
{
    ASSERT_TRUE(serverRunner->serverIsRunning());

    QByteArray json = dataFromFile(":/data/failed_status.json").trimmed();
    ASSERT_FALSE(json.isEmpty());
    serverRunner->setServerAnswer(json);

    QSignalSpy spy(lprPrinterApi, SIGNAL(statusFetched(qulonglong,Proof::NetworkServices::LprPrinterStatus)));
    const qulonglong operationId = lprPrinterApi->fetchStatus();

    ASSERT_TRUE(spy.wait());
    EXPECT_EQ(1, spy.count());

    QList<QVariant> arguments = spy.takeFirst();
    ASSERT_EQ(2, arguments.count());

    EXPECT_EQ(operationId, arguments[0].toULongLong());

    auto result = qvariant_cast<Proof::NetworkServices::LprPrinterStatus>(arguments[1]);
    EXPECT_FALSE(result.isReady);
    EXPECT_EQ("Some error occurred", result.reason);
}

TEST_F(LprPrinterApiTest, printLabel)
{
    ASSERT_TRUE(serverRunner->serverIsRunning());

    QByteArray json = dataFromFile(":/data/status.json").trimmed();
    ASSERT_FALSE(json.isEmpty());
    serverRunner->setServerAnswer(json);

    QSignalSpy spy(lprPrinterApi, SIGNAL(labelPrinted(qulonglong)));
    const qulonglong operationId = lprPrinterApi->printLabel("something");

    ASSERT_TRUE(spy.wait());
    EXPECT_EQ(1, spy.count());

    QList<QVariant> arguments = spy.takeFirst();
    ASSERT_EQ(1, arguments.count());

    EXPECT_EQ(operationId, arguments[0].toULongLong());
}

TEST_F(LprPrinterApiTest, failedPrintLabel)
{
    ASSERT_TRUE(serverRunner->serverIsRunning());

    QByteArray json = dataFromFile(":/data/failed_status.json").trimmed();
    ASSERT_FALSE(json.isEmpty());
    serverRunner->setServerAnswer(json);

    QSignalSpy spy(lprPrinterApi, SIGNAL(labelPrinted(qulonglong)));
    QSignalSpy errorSpy(lprPrinterApi, SIGNAL(errorOccurred(qulonglong,Proof::RestApiError)));
    const qulonglong operationId = lprPrinterApi->printLabel("something");

    ASSERT_TRUE(errorSpy.wait());
    ASSERT_FALSE(spy.wait(100));
    EXPECT_EQ(1, errorSpy.count());
    EXPECT_EQ(0, spy.count());

    QList<QVariant> arguments = errorSpy.takeFirst();
    ASSERT_EQ(2, arguments.count());

    EXPECT_EQ(operationId, arguments[0].toULongLong());

    auto result = qvariant_cast<Proof::RestApiError>(arguments[1]);
    EXPECT_EQ(Proof::RestApiError::Level::ServerError, result.level);
    EXPECT_EQ("Some error occurred", result.message);
}

TEST_F(LprPrinterApiTest, printFile)
{
    ASSERT_TRUE(serverRunner->serverIsRunning());

    QByteArray json = dataFromFile(":/data/status.json").trimmed();
    ASSERT_FALSE(json.isEmpty());
    serverRunner->setServerAnswer(json);

    QSignalSpy spy(lprPrinterApi, SIGNAL(filePrinted(qulonglong)));
    const qulonglong operationId = lprPrinterApi->printFile(":/data/status.json");

    ASSERT_TRUE(spy.wait());
    EXPECT_EQ(1, spy.count());

    QList<QVariant> arguments = spy.takeFirst();
    ASSERT_EQ(1, arguments.count());

    EXPECT_EQ(operationId, arguments[0].toULongLong());
}

TEST_F(LprPrinterApiTest, failedPrintFile)
{
    ASSERT_TRUE(serverRunner->serverIsRunning());

    QByteArray json = dataFromFile(":/data/failed_status.json").trimmed();
    ASSERT_FALSE(json.isEmpty());
    serverRunner->setServerAnswer(json);

    QSignalSpy spy(lprPrinterApi, SIGNAL(filePrinted(qulonglong)));
    QSignalSpy errorSpy(lprPrinterApi, SIGNAL(errorOccurred(qulonglong,Proof::RestApiError)));
    const qulonglong operationId = lprPrinterApi->printFile(":/data/status.json");

    ASSERT_TRUE(errorSpy.wait());
    ASSERT_FALSE(spy.wait(100));
    EXPECT_EQ(1, errorSpy.count());
    EXPECT_EQ(0, spy.count());

    QList<QVariant> arguments = errorSpy.takeFirst();
    ASSERT_EQ(2, arguments.count());

    EXPECT_EQ(operationId, arguments[0].toULongLong());

    auto result = qvariant_cast<Proof::RestApiError>(arguments[1]);
    EXPECT_EQ(Proof::RestApiError::Level::ServerError, result.level);
    EXPECT_EQ("Some error occurred", result.message);
}

TEST_F(LprPrinterApiTest, fetchPrintersList)
{
    ASSERT_TRUE(serverRunner->serverIsRunning());

    QByteArray json = dataFromFile(":/data/printers.json").trimmed();
    ASSERT_FALSE(json.isEmpty());
    serverRunner->setServerAnswer(json);

    QSignalSpy spy(lprPrinterApi, SIGNAL(printersListFetched(qulonglong,QList<Proof::NetworkServices::LprPrinterInfo>)));
    const qulonglong operationId = lprPrinterApi->fetchPrintersList();

    ASSERT_TRUE(spy.wait());
    EXPECT_EQ(1, spy.count());

    QList<QVariant> arguments = spy.takeFirst();
    ASSERT_EQ(2, arguments.count());

    EXPECT_EQ(operationId, arguments[0].toULongLong());

    const QList<Proof::NetworkServices::LprPrinterInfo> printers = qvariant_cast<QList<Proof::NetworkServices::LprPrinterInfo>>(arguments[1]);
    ASSERT_EQ(2, printers.count());

    EXPECT_EQ("Zebra", printers[0].printer);
    EXPECT_TRUE(printers[0].acceptsRaw);
    EXPECT_FALSE(printers[0].acceptsFiles);

    EXPECT_EQ("HP", printers[1].printer);
    EXPECT_FALSE(printers[1].acceptsRaw);
    EXPECT_TRUE(printers[1].acceptsFiles);
}
