// clazy:skip

#include "proofnetwork/ums/data/umstokeninfo.h"
#include "proofnetwork/ums/data/umsuser.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSignalSpy>

#include "gtest/test_global.h"

using namespace Proof::Ums;
using testing::Test;

class UmsUserTest : public Test
{
public:
    UmsUserTest() {}

protected:
    void SetUp() override
    {
        jsonDoc = QJsonDocument::fromJson(dataFromFile(":/data/umsuser.json"));
        ASSERT_FALSE(jsonDoc.isEmpty());
        jsonDoc2 = QJsonDocument::fromJson(dataFromFile(":/data/umsuser2.json"));
        ASSERT_FALSE(jsonDoc2.isEmpty());

        tokenInfoUT = UmsTokenInfo::fromJson(jsonDoc.object(), "token1");
        ASSERT_TRUE(tokenInfoUT);

        tokenInfoUT2 = UmsTokenInfo::fromJson(jsonDoc2.object(), "token2");
        ASSERT_TRUE(tokenInfoUT2);

        qmlWrapperUT = tokenInfoUT->toQmlWrapper();
    }

    void TearDown() override { delete qmlWrapperUT; }

protected:
    QJsonDocument jsonDoc;
    QJsonDocument jsonDoc2;
    UmsTokenInfoSP tokenInfoUT;
    UmsTokenInfoSP tokenInfoUT2;
    UmsTokenInfoQmlWrapper *qmlWrapperUT;
};

TEST_F(UmsUserTest, qmlWrapperProperties)
{
    QStringList invalidProperties = findWrongChangedSignalsInQmlWrapper(qmlWrapperUT);
    EXPECT_EQ(0, invalidProperties.count()) << invalidProperties.join("\n").toLatin1().constData();
}

TEST_F(UmsUserTest, fromJson)
{
    EXPECT_TRUE(tokenInfoUT->isFetched());

    EXPECT_EQ("vadim.petrunin@farheap.com", tokenInfoUT->user()->userName());
    EXPECT_EQ("vadim.petrunin@farheap.com", qmlWrapperUT->user()->userName());
    EXPECT_EQ("Vadim Petrunin", tokenInfoUT->user()->fullName());
    EXPECT_EQ("Vadim Petrunin", qmlWrapperUT->user()->fullName());
    EXPECT_EQ("vadim.petrunin@farheap.com", tokenInfoUT->user()->email());
    EXPECT_EQ("vadim.petrunin@farheap.com", qmlWrapperUT->user()->email());

    EXPECT_EQ("1.0", tokenInfoUT->version());
    EXPECT_EQ("1.0", qmlWrapperUT->version());
    EXPECT_EQ((uint)1476782390, tokenInfoUT->expiresAt().toTime_t());
    EXPECT_EQ((uint)1476782390, qmlWrapperUT->expiresAt().toTime_t());
    EXPECT_EQ((uint)1476695990, tokenInfoUT->validFrom().toTime_t());
    EXPECT_EQ((uint)1476695990, qmlWrapperUT->validFrom().toTime_t());
    auto roles = QStringList{"dev:DSB:Admin", "KY:UMS:Admin", "SH3:Admin"};
    EXPECT_EQ(roles, tokenInfoUT->user()->roles());
    EXPECT_EQ(roles, qmlWrapperUT->user()->roles());
}

TEST_F(UmsUserTest, updateFrom)
{
    QVector<QSignalSpy *> spies = spiesForObject(tokenInfoUT.data());
    QVector<QSignalSpy *> qmlspies = spiesForObject(qmlWrapperUT);

    tokenInfoUT->updateFrom(tokenInfoUT2);

    for (QSignalSpy *spy : spies)
        EXPECT_EQ(1, spy->count()) << spy->signal().constData();

    for (QSignalSpy *spy : qmlspies)
        EXPECT_EQ(1, spy->count()) << spy->signal().constData();

    qDeleteAll(spies);
    spies.clear();
    qDeleteAll(qmlspies);
    qmlspies.clear();

    EXPECT_EQ(tokenInfoUT2->user()->userName(), tokenInfoUT->user()->userName());
    EXPECT_EQ(tokenInfoUT2->user()->fullName(), tokenInfoUT->user()->fullName());
    EXPECT_EQ(tokenInfoUT2->user()->email(), tokenInfoUT->user()->email());

    EXPECT_EQ(tokenInfoUT2->version(), tokenInfoUT->version());
    EXPECT_EQ(tokenInfoUT2->expiresAt(), tokenInfoUT->expiresAt());
    EXPECT_EQ(tokenInfoUT2->validFrom(), tokenInfoUT->validFrom());
    EXPECT_EQ(tokenInfoUT2->user()->roles(), tokenInfoUT->user()->roles());
}
