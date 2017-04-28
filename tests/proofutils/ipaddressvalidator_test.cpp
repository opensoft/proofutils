// clazy:skip

#include "gtest/test_global.h"

#include "proofutils/ipaddressvalidator.h"

using namespace Proof;
using testing::TestWithParam;

using TestTuple = std::tuple<QString, IpAddressValidator::State>;

class IpAddressValidationTest : public TestWithParam<TestTuple>
{
};

class SubnetMaskValidationTest : public TestWithParam<TestTuple>
{
};

INSTANTIATE_TEST_CASE_P(IpAdrressTestParameters,
                        IpAddressValidationTest,
                        testing::Values(
                            TestTuple("128.1.1.1", IpAddressValidator::Acceptable),
                            TestTuple("192.168.0.1", IpAddressValidator::Acceptable),
                            TestTuple("128. .1.1", IpAddressValidator::Intermediate),
                            TestTuple("192.168. .1", IpAddressValidator::Intermediate),

                            TestTuple("255.1.1.1", IpAddressValidator::Intermediate),
                            TestTuple("128.335.343.344", IpAddressValidator::Intermediate),

                            TestTuple("8.8.8. ", IpAddressValidator::Intermediate),
                            TestTuple("4. . . ", IpAddressValidator::Intermediate),
                            TestTuple("192.168.0.0", IpAddressValidator::Intermediate)
                            ));

INSTANTIATE_TEST_CASE_P(SubnetMaskTestParameters,
                        SubnetMaskValidationTest,
                        testing::Values(
                            TestTuple("128.0.0.0", IpAddressValidator::Acceptable),
                            TestTuple("192.0.0.0", IpAddressValidator::Acceptable),
                            TestTuple("224.0.0.0", IpAddressValidator::Acceptable),
                            TestTuple("240.0.0.0", IpAddressValidator::Acceptable),
                            TestTuple("248.0.0.0", IpAddressValidator::Acceptable),
                            TestTuple("252.0.0.0", IpAddressValidator::Acceptable),
                            TestTuple("254.0.0.0", IpAddressValidator::Acceptable),
                            TestTuple("255.0.0.0", IpAddressValidator::Acceptable),
                            TestTuple("255.128.0.0", IpAddressValidator::Acceptable),
                            TestTuple("255.192.0.0", IpAddressValidator::Acceptable),
                            TestTuple("255.224.0.0", IpAddressValidator::Acceptable),
                            TestTuple("255.240.0.0", IpAddressValidator::Acceptable),
                            TestTuple("255.248.0.0", IpAddressValidator::Acceptable),
                            TestTuple("255.252.0.0", IpAddressValidator::Acceptable),
                            TestTuple("255.254.0.0", IpAddressValidator::Acceptable),
                            TestTuple("255.255.0.0", IpAddressValidator::Acceptable),
                            TestTuple("255.255.128.0", IpAddressValidator::Acceptable),
                            TestTuple("255.255.192.0", IpAddressValidator::Acceptable),
                            TestTuple("255.255.224.0", IpAddressValidator::Acceptable),
                            TestTuple("255.255.240.0", IpAddressValidator::Acceptable),
                            TestTuple("255.255.248.0", IpAddressValidator::Acceptable),
                            TestTuple("255.255.252.0", IpAddressValidator::Acceptable),
                            TestTuple("255.255.254.0", IpAddressValidator::Acceptable),
                            TestTuple("255.255.255.0", IpAddressValidator::Acceptable),
                            TestTuple("255.255.255.128", IpAddressValidator::Acceptable),
                            TestTuple("255.255.255.192", IpAddressValidator::Acceptable),
                            TestTuple("255.255.255.224", IpAddressValidator::Acceptable),
                            TestTuple("255.255.255.240", IpAddressValidator::Acceptable),
                            TestTuple("255.255.255.248", IpAddressValidator::Acceptable),
                            TestTuple("255.255.255.252", IpAddressValidator::Acceptable),
                            TestTuple("255.255.255.254", IpAddressValidator::Acceptable),
                            TestTuple("255.255.255.255", IpAddressValidator::Acceptable),
                            TestTuple("255.255.255. ", IpAddressValidator::Intermediate),
                            TestTuple("255. . . ", IpAddressValidator::Intermediate),

                            TestTuple("255.123.255.255", IpAddressValidator::Intermediate),
                            TestTuple("255.255.123.255", IpAddressValidator::Intermediate),
                            TestTuple("255.255.255.123", IpAddressValidator::Intermediate),
                            TestTuple("0.255.255.255", IpAddressValidator::Intermediate),
                            TestTuple("123.255.255.255", IpAddressValidator::Intermediate),
                            TestTuple("255. .255.255", IpAddressValidator::Intermediate),
                            TestTuple("255.255. .255", IpAddressValidator::Intermediate),

                            TestTuple("255.25.255.0", IpAddressValidator::Intermediate),
                            TestTuple("255.255.25.0", IpAddressValidator::Intermediate)
                            ));

TEST_P(IpAddressValidationTest, ipAddress)
{
    IpAddressValidator validator;
    ASSERT_FALSE(validator.maskValidationMode());
    int pos = 0;
    QString ipAddress = std::get<0>(GetParam());
    IpAddressValidator::State state = std::get<1>(GetParam());

    EXPECT_EQ(state, validator.validate(ipAddress, pos)) << ipAddress.toLatin1().constData();
}

TEST_P(SubnetMaskValidationTest, subnetMask)
{
    IpAddressValidator validator;
    validator.setMaskValidationMode(true);
    ASSERT_TRUE(validator.maskValidationMode());
    int pos = 0;
    QString subnetMask = std::get<0>(GetParam());
    IpAddressValidator::State state = std::get<1>(GetParam());

    EXPECT_EQ(state, validator.validate(subnetMask, pos)) << subnetMask.toLatin1().constData();
}

