#include "gtest/test_global.h"

#include "proofnetwork/ums/data/umsuser.h"
#include "proofnetwork/ums/data/role.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QSignalSpy>
#include <QFile>

using namespace Proof::Ums;
using testing::Test;

class UmsUserTest: public Test
{
public:
    UmsUserTest()
    {
    }

protected:
    void SetUp() override
    {
        jsonDoc = QJsonDocument::fromJson(dataFromFile(":/data/umsuser.json"));
        ASSERT_FALSE(jsonDoc.isEmpty());
        jsonDoc2 = QJsonDocument::fromJson(dataFromFile(":/data/umsuser2.json"));
        ASSERT_FALSE(jsonDoc2.isEmpty());

        umsUserUT = UmsUser::fromJson(jsonDoc.object());
        ASSERT_TRUE(umsUserUT);

        umsUserUT2 = UmsUser::fromJson(jsonDoc2.object());
        ASSERT_TRUE(umsUserUT2);

        qmlWrapperUT = umsUserUT->toQmlWrapper();
    }

    void TearDown() override
    {
        delete qmlWrapperUT;
    }

protected:
    QJsonDocument jsonDoc;
    QJsonDocument jsonDoc2;
    UmsUserSP umsUserUT;
    UmsUserSP umsUserUT2;
    UmsUserQmlWrapper *qmlWrapperUT;
};

TEST_F(UmsUserTest, fromJson)
{
    EXPECT_TRUE(umsUserUT->isFetched());

    EXPECT_EQ("User User", umsUserUT->userName());
    EXPECT_EQ("User User", umsUserUT->fullName());
    EXPECT_EQ("user.user@opensoftdev.ru", umsUserUT->email());

    EXPECT_EQ("5a38ba5c-88d2-4cb2-9449-f25d47bbe577", umsUserUT->id());
    EXPECT_EQ("1.0", umsUserUT->version());
    EXPECT_EQ((uint)1474717647, umsUserUT->expiresAt().toTime_t());
    EXPECT_EQ((uint)1474631247, umsUserUT->validFrom().toTime_t());
    EXPECT_EQ(1, umsUserUT->roles().count());

    RoleSP role = umsUserUT->roles().first();
    ASSERT_TRUE(role);
    EXPECT_EQ("KY", role->locale());
    EXPECT_EQ("UMS", role->service());
    EXPECT_EQ("Manager", role->name());
}

TEST_F(UmsUserTest, updateFrom)
{
    QList<QSignalSpy *> spies = spiesForObject(umsUserUT.data());
    QList<QSignalSpy *> qmlspies = spiesForObject(qmlWrapperUT);

    umsUserUT->updateFrom(umsUserUT2);

    for (QSignalSpy *spy: spies)
        EXPECT_EQ(1, spy->count()) << spy->signal().constData();

    for (QSignalSpy *spy: qmlspies)
        EXPECT_EQ(1, spy->count()) << spy->signal().constData();

    qDeleteAll(spies);
    spies.clear();
    qDeleteAll(qmlspies);
    qmlspies.clear();

    EXPECT_EQ(umsUserUT2->userName(), umsUserUT->userName());
    EXPECT_EQ(umsUserUT2->fullName(), umsUserUT->fullName());
    EXPECT_EQ(umsUserUT2->email(), umsUserUT->email());

    EXPECT_EQ(umsUserUT2->id(), umsUserUT->id());
    EXPECT_EQ(umsUserUT2->version(), umsUserUT->version());
    EXPECT_EQ(umsUserUT2->expiresAt(), umsUserUT->expiresAt());
    EXPECT_EQ(umsUserUT2->validFrom(), umsUserUT->validFrom());
    EXPECT_EQ(umsUserUT2->roles().count(), umsUserUT->roles().count());

    RoleSP role = umsUserUT->roles().first();
    ASSERT_TRUE(role);
    RoleSP role2 = umsUserUT2->roles().first();
    ASSERT_TRUE(role2);
    EXPECT_EQ(role2->locale(), role->locale());
    EXPECT_EQ(role2->service(), role->service());
    EXPECT_EQ(role2->name(), role->name());
}

