// clazy:skip

#include "proofutils/labelprinter.h"

#include "gtest/proof/test_global.h"

using namespace Proof;
using testing::Test;

class LabelPrinterTest : public Test
{
public:
    LabelPrinterTest() {}

protected:
    void SetUp() override
    {
        serverRunner = new FakeServerRunner;
        serverRunner->runServer();
    }

    void TearDown() override { delete serverRunner; }

protected:
    FakeServerRunner *serverRunner;
};

TEST_F(LabelPrinterTest, sanity)
{
    {
        LabelPrinterParams params;
    }
    {
        LabelPrinterParams params("someTitle", "192.168.1.15", "shortNameHere", 1250, true, false);
        EXPECT_EQ("someTitle", params.printerTitle);
        EXPECT_EQ("192.168.1.15", params.printerHost);
        EXPECT_EQ(1250, params.printerPort);
        EXPECT_EQ("shortNameHere", params.printerName);
        EXPECT_TRUE(params.forceServiceUsage);
        EXPECT_FALSE(params.strictHardwareCheck);
        LabelPrinter printer(params);
        EXPECT_EQ("someTitle", printer.title());
    }
}

TEST_F(LabelPrinterTest, printerIsReady)
{
    ASSERT_TRUE(serverRunner->serverIsRunning());
    QByteArray serverResponse = R"({"is_ready": true})";
    serverRunner->setServerAnswer(serverResponse);
    LabelPrinter printer(LabelPrinterParams("someTitle", "127.0.0.1", "shortNameHere", 9091, true, false));
    auto f = printer.printerIsReady();
    f.wait(1000);
    ASSERT_TRUE(f.isCompleted());
    EXPECT_TRUE(f.isSucceeded());
    EXPECT_TRUE(f.result());
}

TEST_F(LabelPrinterTest, printerIsReadyNegative)
{
    ASSERT_TRUE(serverRunner->serverIsRunning());
    QByteArray serverResponse = R"({"is_ready": false, "reason": "some reason"})";
    serverRunner->setServerAnswer(serverResponse);
    LabelPrinter printer(LabelPrinterParams("someTitle", "127.0.0.1", "shortNameHere", 9091, true, false));
    auto f = printer.printerIsReady();
    f.wait(1000);
    ASSERT_TRUE(f.isCompleted());
    EXPECT_TRUE(f.isFailed());
    EXPECT_EQ("some reason", f.failureReason().message);
    EXPECT_EQ(UTILS_MODULE_CODE, f.failureReason().moduleCode);
    EXPECT_EQ(UtilsErrorCode::LabelPrinterError, f.failureReason().errorCode);
}

TEST_F(LabelPrinterTest, print)
{
    ASSERT_TRUE(serverRunner->serverIsRunning());
    QByteArray serverResponse = R"({"is_ready": true})";
    serverRunner->setServerAnswer(serverResponse);
    LabelPrinter printer(LabelPrinterParams("someTitle", "127.0.0.1", "shortNameHere", 9091, true, false));
    auto f = printer.printLabel("some label");
    f.wait(1000);
    ASSERT_TRUE(f.isCompleted());
    EXPECT_TRUE(f.isSucceeded());
    EXPECT_TRUE(f.result());
}

TEST_F(LabelPrinterTest, printNegative)
{
    ASSERT_TRUE(serverRunner->serverIsRunning());
    QByteArray serverResponse = R"({"is_ready": false, "reason": "some reason"})";
    serverRunner->setServerAnswer(serverResponse);
    LabelPrinter printer(LabelPrinterParams("someTitle", "127.0.0.1", "shortNameHere", 9091, true, false));
    auto f = printer.printLabel("some label");
    f.wait(1000);
    ASSERT_TRUE(f.isCompleted());
    EXPECT_TRUE(f.isFailed());
    EXPECT_EQ("some reason", f.failureReason().message);
}
