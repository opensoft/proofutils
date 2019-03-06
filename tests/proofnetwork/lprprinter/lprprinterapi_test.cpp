// clazy:skip

#include "proofnetwork/lprprinter/lprprinterapi.h"

#include "gtest/proof/test_global.h"

#include <QFile>

using namespace Proof::NetworkServices;
using testing::Test;

class LprPrinterApiTest : public Test
{
public:
    LprPrinterApiTest() {}

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

TEST_F(LprPrinterApiTest, fetchStatusForDefaultPrinter)
{
    ASSERT_TRUE(serverRunner->serverIsRunning());

    QByteArray json = dataFromFile(":/data/status.json").trimmed();
    ASSERT_FALSE(json.isEmpty());
    serverRunner->setServerAnswer(json);

    auto result = lprPrinterApi->fetchStatus().result();

    EXPECT_EQ(FakeServer::Method::Get, serverRunner->lastQueryMethod());
    EXPECT_EQ(QUrl("/lpr/status"), serverRunner->lastQueryUrl());
    EXPECT_TRUE(serverRunner->lastQueryBody().isEmpty());

    EXPECT_TRUE(result.isReady);
    EXPECT_EQ("", result.reason);
}

TEST_F(LprPrinterApiTest, fetchStatus)
{
    ASSERT_TRUE(serverRunner->serverIsRunning());

    QByteArray json = dataFromFile(":/data/status.json").trimmed();
    ASSERT_FALSE(json.isEmpty());
    serverRunner->setServerAnswer(json);

    auto result = lprPrinterApi->fetchStatus("printer42").result();

    EXPECT_EQ(FakeServer::Method::Get, serverRunner->lastQueryMethod());
    EXPECT_EQ(QUrl("/lpr/status?printer=printer42"), serverRunner->lastQueryUrl());
    EXPECT_TRUE(serverRunner->lastQueryBody().isEmpty());

    EXPECT_TRUE(result.isReady);
    EXPECT_EQ("", result.reason);
}

TEST_F(LprPrinterApiTest, fetchBadStatus)
{
    ASSERT_TRUE(serverRunner->serverIsRunning());

    QByteArray json = dataFromFile(":/data/failed_status.json").trimmed();
    ASSERT_FALSE(json.isEmpty());
    serverRunner->setServerAnswer(json);

    auto result = lprPrinterApi->fetchStatus().result();

    EXPECT_EQ(FakeServer::Method::Get, serverRunner->lastQueryMethod());
    EXPECT_EQ(QUrl("/lpr/status"), serverRunner->lastQueryUrl());
    EXPECT_TRUE(serverRunner->lastQueryBody().isEmpty());

    EXPECT_FALSE(result.isReady);
    EXPECT_EQ("Some error occurred", result.reason);
}

TEST_F(LprPrinterApiTest, printLabelAtDefaultPrinter)
{
    ASSERT_TRUE(serverRunner->serverIsRunning());

    QByteArray json = dataFromFile(":/data/status.json").trimmed();
    ASSERT_FALSE(json.isEmpty());
    serverRunner->setServerAnswer(json);

    auto result = lprPrinterApi->printLabel("something");
    result.wait();

    EXPECT_EQ(FakeServer::Method::Post, serverRunner->lastQueryMethod());
    EXPECT_EQ(QUrl("/lpr/print-raw"), serverRunner->lastQueryUrl());
    auto bodyObject = QJsonDocument::fromJson(serverRunner->lastQueryBody()).object();
    EXPECT_EQ(QByteArray("something").toBase64().constData(), bodyObject["data"].toString().toLatin1());

    ASSERT_TRUE(result.isSucceeded());
    EXPECT_TRUE(result.result());
}

TEST_F(LprPrinterApiTest, printLabel)
{
    ASSERT_TRUE(serverRunner->serverIsRunning());

    QByteArray json = dataFromFile(":/data/status.json").trimmed();
    ASSERT_FALSE(json.isEmpty());
    serverRunner->setServerAnswer(json);

    auto result = lprPrinterApi->printLabel("something", "printer42");
    result.wait();

    EXPECT_EQ(FakeServer::Method::Post, serverRunner->lastQueryMethod());
    EXPECT_EQ(QUrl("/lpr/print-raw?printer=printer42"), serverRunner->lastQueryUrl());
    auto bodyObject = QJsonDocument::fromJson(serverRunner->lastQueryBody()).object();
    EXPECT_EQ(QByteArray("something").toBase64().constData(), bodyObject["data"].toString().toLatin1());

    ASSERT_TRUE(result.isSucceeded());
    EXPECT_TRUE(result.result());
}

TEST_F(LprPrinterApiTest, failedPrintLabel)
{
    ASSERT_TRUE(serverRunner->serverIsRunning());

    QByteArray json = dataFromFile(":/data/failed_status.json").trimmed();
    ASSERT_FALSE(json.isEmpty());
    serverRunner->setServerAnswer(json);

    auto result = lprPrinterApi->printLabel("something");
    result.wait();
    EXPECT_EQ(FakeServer::Method::Post, serverRunner->lastQueryMethod());
    EXPECT_EQ(QUrl("/lpr/print-raw"), serverRunner->lastQueryUrl());
    auto bodyObject = QJsonDocument::fromJson(serverRunner->lastQueryBody()).object();
    EXPECT_EQ(QByteArray("something").toBase64().constData(), bodyObject["data"].toString().toLatin1());

    ASSERT_TRUE(result.isFailed());
    EXPECT_EQ("Some error occurred", result.failureReason().message);
}

TEST_F(LprPrinterApiTest, printFileAtDefaultPrinter)
{
    ASSERT_TRUE(serverRunner->serverIsRunning());

    QByteArray json = dataFromFile(":/data/status.json").trimmed();
    ASSERT_FALSE(json.isEmpty());
    serverRunner->setServerAnswer(json);

    auto result = lprPrinterApi->printFile(":/data/status.json");
    result.wait();

    EXPECT_EQ(FakeServer::Method::Post, serverRunner->lastQueryMethod());
    EXPECT_EQ(QUrl("/lpr/print?copies=1"), serverRunner->lastQueryUrl());
    auto body = serverRunner->lastQueryBody();
    QFile f(":/data/status.json");
    ASSERT_TRUE(f.open(QFile::ReadOnly));
    EXPECT_EQ(f.readAll(), body);

    ASSERT_TRUE(result.isSucceeded());
    EXPECT_TRUE(result.result());
}

TEST_F(LprPrinterApiTest, printFile)
{
    ASSERT_TRUE(serverRunner->serverIsRunning());

    QByteArray json = dataFromFile(":/data/status.json").trimmed();
    ASSERT_FALSE(json.isEmpty());
    serverRunner->setServerAnswer(json);

    auto result = lprPrinterApi->printFile(":/data/status.json", "printer42");
    result.wait();

    EXPECT_EQ(FakeServer::Method::Post, serverRunner->lastQueryMethod());
    EXPECT_EQ(QUrl("/lpr/print?copies=1&printer=printer42"), serverRunner->lastQueryUrl());
    auto body = serverRunner->lastQueryBody();
    QFile f(":/data/status.json");
    ASSERT_TRUE(f.open(QFile::ReadOnly));
    EXPECT_EQ(f.readAll(), body);

    ASSERT_TRUE(result.isSucceeded());
    EXPECT_TRUE(result.result());
}

TEST_F(LprPrinterApiTest, failedPrintFile)
{
    ASSERT_TRUE(serverRunner->serverIsRunning());

    QByteArray json = dataFromFile(":/data/failed_status.json").trimmed();
    ASSERT_FALSE(json.isEmpty());
    serverRunner->setServerAnswer(json);

    auto result = lprPrinterApi->printFile(":/data/status.json");
    result.wait();
    EXPECT_EQ(FakeServer::Method::Post, serverRunner->lastQueryMethod());
    EXPECT_EQ(QUrl("/lpr/print?copies=1"), serverRunner->lastQueryUrl());
    auto body = serverRunner->lastQueryBody();
    QFile f(":/data/status.json");
    ASSERT_TRUE(f.open(QFile::ReadOnly));
    EXPECT_EQ(f.readAll(), body);

    ASSERT_TRUE(result.isFailed());
    EXPECT_EQ("Some error occurred", result.failureReason().message);
}

TEST_F(LprPrinterApiTest, printFileFromWrongFile)
{
    ASSERT_TRUE(serverRunner->serverIsRunning());

    QByteArray json = dataFromFile(":/data/failed_status.json").trimmed();
    ASSERT_FALSE(json.isEmpty());
    serverRunner->setServerAnswer(json);

    auto result = lprPrinterApi->printFile(":/data/this_file_doesnot_exist.json");
    result.wait();
    ASSERT_TRUE(result.isFailed());
    EXPECT_EQ("Can't open file", result.failureReason().message);
    EXPECT_EQ(Proof::NetworkErrorCode::FileError, result.failureReason().errorCode);
}

TEST_F(LprPrinterApiTest, fetchPrintersList)
{
    ASSERT_TRUE(serverRunner->serverIsRunning());

    QByteArray json = dataFromFile(":/data/printers.json").trimmed();
    ASSERT_FALSE(json.isEmpty());
    serverRunner->setServerAnswer(json);

    const QVector<Proof::NetworkServices::LprPrinterInfo> printers = lprPrinterApi->fetchPrintersList().result();
    EXPECT_EQ(FakeServer::Method::Get, serverRunner->lastQueryMethod());
    EXPECT_EQ(QUrl("/lpr/list"), serverRunner->lastQueryUrl());
    EXPECT_TRUE(serverRunner->lastQueryBody().isEmpty());

    ASSERT_EQ(2, printers.count());

    EXPECT_EQ("Zebra", printers[0].printer);
    EXPECT_TRUE(printers[0].acceptsRaw);
    EXPECT_FALSE(printers[0].acceptsFiles);

    EXPECT_EQ("HP", printers[1].printer);
    EXPECT_FALSE(printers[1].acceptsRaw);
    EXPECT_TRUE(printers[1].acceptsFiles);
}
