// clazy:skip

#include "proofutils/epllabelgenerator.h"

#include "gtest/proof/test_global.h"

using namespace Proof;
using testing::TestWithParam;

using SizeTestTuple = std::tuple<QSize, QString, int, int, int>;
using BarcodeTestTuple = std::tuple<QString, QString, EplLabelGenerator::BarcodeType, int, int, int, bool, int, int, int>;
using AddTextTuple = std::tuple<QByteArray, QString, int, int, int, int, int, int, bool>;

static const QVector<QSize> singleCharSizeAt203 = {{10, 14}, {12, 18}, {14, 22}, {16, 26}, {34, 50}};
static const QVector<QSize> singleCharSizeAt300 = {{14, 22}, {18, 30}, {22, 38}, {26, 46}, {50, 82}};

class EplLabelGenerator203SizesTest : public TestWithParam<SizeTestTuple>
{};

class EplLabelGenerator300SizesTest : public TestWithParam<SizeTestTuple>
{};

class EplLabelGeneratorBarcodeTest : public TestWithParam<BarcodeTestTuple>
{};

class EplLabelGeneratorAddTextTest : public TestWithParam<AddTextTuple>
{};

template <typename... T>
struct RemoveFirstType;

template <>
struct RemoveFirstType<std::tuple<>>;

template <typename... T>
struct RemoveFirstType<std::tuple<T...>>
{
private:
    template <size_t... n>
    static auto truncate(std::index_sequence<n...>, const std::tuple<T...> &t)
    {
        return std::make_tuple(std::get<n + 1>(t)...);
    }

public:
    static auto result(const std::tuple<T...> &t) { return truncate(std::make_index_sequence<sizeof...(T) - 1>(), t); }
};

//All constants here are taken from manual https://www.zebra.com/content/dam/zebra/manuals/en-us/printer/epl2-pm-en.pdf

INSTANTIATE_TEST_CASE_P(
    TextSize203TestParameters, EplLabelGenerator203SizesTest,
    testing::Values(
        SizeTestTuple(QSize(singleCharSizeAt203[0].width(), singleCharSizeAt203[0].height()), "A", 1, 1, 1),
        SizeTestTuple(QSize(singleCharSizeAt203[0].width() * 3, singleCharSizeAt203[0].height()), "A", 1, 3, 1),
        SizeTestTuple(QSize(singleCharSizeAt203[0].width(), singleCharSizeAt203[0].height() * 3), "A", 1, 1, 3),
        SizeTestTuple(QSize(singleCharSizeAt203[0].width() * 3, singleCharSizeAt203[0].height() * 3), "A", 1, 3, 3),
        SizeTestTuple(QSize(singleCharSizeAt203[1].width(), singleCharSizeAt203[1].height()), "A", 2, 1, 1),
        SizeTestTuple(QSize(singleCharSizeAt203[1].width() * 3, singleCharSizeAt203[1].height()), "A", 2, 3, 1),
        SizeTestTuple(QSize(singleCharSizeAt203[1].width(), singleCharSizeAt203[1].height() * 3), "A", 2, 1, 3),
        SizeTestTuple(QSize(singleCharSizeAt203[1].width() * 3, singleCharSizeAt203[1].height() * 3), "A", 2, 3, 3),
        SizeTestTuple(QSize(singleCharSizeAt203[2].width(), singleCharSizeAt203[2].height()), "A", 3, 1, 1),
        SizeTestTuple(QSize(singleCharSizeAt203[2].width() * 3, singleCharSizeAt203[2].height()), "A", 3, 3, 1),
        SizeTestTuple(QSize(singleCharSizeAt203[2].width(), singleCharSizeAt203[2].height() * 3), "A", 3, 1, 3),
        SizeTestTuple(QSize(singleCharSizeAt203[2].width() * 3, singleCharSizeAt203[2].height() * 3), "A", 3, 3, 3),
        SizeTestTuple(QSize(singleCharSizeAt203[3].width(), singleCharSizeAt203[3].height()), "A", 4, 1, 1),
        SizeTestTuple(QSize(singleCharSizeAt203[3].width() * 3, singleCharSizeAt203[3].height()), "A", 4, 3, 1),
        SizeTestTuple(QSize(singleCharSizeAt203[3].width(), singleCharSizeAt203[3].height() * 3), "A", 4, 1, 3),
        SizeTestTuple(QSize(singleCharSizeAt203[3].width() * 3, singleCharSizeAt203[3].height() * 3), "A", 4, 3, 3),
        SizeTestTuple(QSize(singleCharSizeAt203[4].width(), singleCharSizeAt203[4].height()), "A", 5, 1, 1),
        SizeTestTuple(QSize(singleCharSizeAt203[4].width() * 3, singleCharSizeAt203[4].height()), "A", 5, 3, 1),
        SizeTestTuple(QSize(singleCharSizeAt203[4].width(), singleCharSizeAt203[4].height() * 3), "A", 5, 1, 3),
        SizeTestTuple(QSize(singleCharSizeAt203[4].width() * 3, singleCharSizeAt203[4].height() * 3), "A", 5, 3, 3),

        SizeTestTuple(QSize(3 * singleCharSizeAt203[0].width(), singleCharSizeAt203[0].height()), "ABC", 1, 1, 1),
        SizeTestTuple(QSize(3 * singleCharSizeAt203[0].width() * 3, singleCharSizeAt203[0].height()), "ABC", 1, 3, 1),
        SizeTestTuple(QSize(3 * singleCharSizeAt203[0].width(), singleCharSizeAt203[0].height() * 3), "ABC", 1, 1, 3),
        SizeTestTuple(QSize(3 * singleCharSizeAt203[0].width() * 3, singleCharSizeAt203[0].height() * 3), "ABC", 1, 3, 3),
        SizeTestTuple(QSize(3 * singleCharSizeAt203[1].width(), singleCharSizeAt203[1].height()), "ABC", 2, 1, 1),
        SizeTestTuple(QSize(3 * singleCharSizeAt203[1].width() * 3, singleCharSizeAt203[1].height()), "ABC", 2, 3, 1),
        SizeTestTuple(QSize(3 * singleCharSizeAt203[1].width(), singleCharSizeAt203[1].height() * 3), "ABC", 2, 1, 3),
        SizeTestTuple(QSize(3 * singleCharSizeAt203[1].width() * 3, singleCharSizeAt203[1].height() * 3), "ABC", 2, 3, 3),
        SizeTestTuple(QSize(3 * singleCharSizeAt203[2].width(), singleCharSizeAt203[2].height()), "ABC", 3, 1, 1),
        SizeTestTuple(QSize(3 * singleCharSizeAt203[2].width() * 3, singleCharSizeAt203[2].height()), "ABC", 3, 3, 1),
        SizeTestTuple(QSize(3 * singleCharSizeAt203[2].width(), singleCharSizeAt203[2].height() * 3), "ABC", 3, 1, 3),
        SizeTestTuple(QSize(3 * singleCharSizeAt203[2].width() * 3, singleCharSizeAt203[2].height() * 3), "ABC", 3, 3, 3),
        SizeTestTuple(QSize(3 * singleCharSizeAt203[3].width(), singleCharSizeAt203[3].height()), "ABC", 4, 1, 1),
        SizeTestTuple(QSize(3 * singleCharSizeAt203[3].width() * 3, singleCharSizeAt203[3].height()), "ABC", 4, 3, 1),
        SizeTestTuple(QSize(3 * singleCharSizeAt203[3].width(), singleCharSizeAt203[3].height() * 3), "ABC", 4, 1, 3),
        SizeTestTuple(QSize(3 * singleCharSizeAt203[3].width() * 3, singleCharSizeAt203[3].height() * 3), "ABC", 4, 3, 3),
        SizeTestTuple(QSize(3 * singleCharSizeAt203[4].width(), singleCharSizeAt203[4].height()), "ABC", 5, 1, 1),
        SizeTestTuple(QSize(3 * singleCharSizeAt203[4].width() * 3, singleCharSizeAt203[4].height()), "ABC", 5, 3, 1),
        SizeTestTuple(QSize(3 * singleCharSizeAt203[4].width(), singleCharSizeAt203[4].height() * 3), "ABC", 5, 1, 3),
        SizeTestTuple(QSize(3 * singleCharSizeAt203[4].width() * 3, singleCharSizeAt203[4].height() * 3), "ABC", 5, 3,
                      3)));

INSTANTIATE_TEST_CASE_P(
    TextSize300TestParameters, EplLabelGenerator300SizesTest,
    testing::Values(
        SizeTestTuple(QSize(singleCharSizeAt300[0].width(), singleCharSizeAt300[0].height()), "A", 1, 1, 1),
        SizeTestTuple(QSize(singleCharSizeAt300[0].width() * 3, singleCharSizeAt300[0].height()), "A", 1, 3, 1),
        SizeTestTuple(QSize(singleCharSizeAt300[0].width(), singleCharSizeAt300[0].height() * 3), "A", 1, 1, 3),
        SizeTestTuple(QSize(singleCharSizeAt300[0].width() * 3, singleCharSizeAt300[0].height() * 3), "A", 1, 3, 3),
        SizeTestTuple(QSize(singleCharSizeAt300[1].width(), singleCharSizeAt300[1].height()), "A", 2, 1, 1),
        SizeTestTuple(QSize(singleCharSizeAt300[1].width() * 3, singleCharSizeAt300[1].height()), "A", 2, 3, 1),
        SizeTestTuple(QSize(singleCharSizeAt300[1].width(), singleCharSizeAt300[1].height() * 3), "A", 2, 1, 3),
        SizeTestTuple(QSize(singleCharSizeAt300[1].width() * 3, singleCharSizeAt300[1].height() * 3), "A", 2, 3, 3),
        SizeTestTuple(QSize(singleCharSizeAt300[2].width(), singleCharSizeAt300[2].height()), "A", 3, 1, 1),
        SizeTestTuple(QSize(singleCharSizeAt300[2].width() * 3, singleCharSizeAt300[2].height()), "A", 3, 3, 1),
        SizeTestTuple(QSize(singleCharSizeAt300[2].width(), singleCharSizeAt300[2].height() * 3), "A", 3, 1, 3),
        SizeTestTuple(QSize(singleCharSizeAt300[2].width() * 3, singleCharSizeAt300[2].height() * 3), "A", 3, 3, 3),
        SizeTestTuple(QSize(singleCharSizeAt300[3].width(), singleCharSizeAt300[3].height()), "A", 4, 1, 1),
        SizeTestTuple(QSize(singleCharSizeAt300[3].width() * 3, singleCharSizeAt300[3].height()), "A", 4, 3, 1),
        SizeTestTuple(QSize(singleCharSizeAt300[3].width(), singleCharSizeAt300[3].height() * 3), "A", 4, 1, 3),
        SizeTestTuple(QSize(singleCharSizeAt300[3].width() * 3, singleCharSizeAt300[3].height() * 3), "A", 4, 3, 3),
        SizeTestTuple(QSize(singleCharSizeAt300[4].width(), singleCharSizeAt300[4].height()), "A", 5, 1, 1),
        SizeTestTuple(QSize(singleCharSizeAt300[4].width() * 3, singleCharSizeAt300[4].height()), "A", 5, 3, 1),
        SizeTestTuple(QSize(singleCharSizeAt300[4].width(), singleCharSizeAt300[4].height() * 3), "A", 5, 1, 3),
        SizeTestTuple(QSize(singleCharSizeAt300[4].width() * 3, singleCharSizeAt300[4].height() * 3), "A", 5, 3, 3),

        SizeTestTuple(QSize(3 * singleCharSizeAt300[0].width(), singleCharSizeAt300[0].height()), "ABC", 1, 1, 1),
        SizeTestTuple(QSize(3 * singleCharSizeAt300[0].width() * 3, singleCharSizeAt300[0].height()), "ABC", 1, 3, 1),
        SizeTestTuple(QSize(3 * singleCharSizeAt300[0].width(), singleCharSizeAt300[0].height() * 3), "ABC", 1, 1, 3),
        SizeTestTuple(QSize(3 * singleCharSizeAt300[0].width() * 3, singleCharSizeAt300[0].height() * 3), "ABC", 1, 3, 3),
        SizeTestTuple(QSize(3 * singleCharSizeAt300[1].width(), singleCharSizeAt300[1].height()), "ABC", 2, 1, 1),
        SizeTestTuple(QSize(3 * singleCharSizeAt300[1].width() * 3, singleCharSizeAt300[1].height()), "ABC", 2, 3, 1),
        SizeTestTuple(QSize(3 * singleCharSizeAt300[1].width(), singleCharSizeAt300[1].height() * 3), "ABC", 2, 1, 3),
        SizeTestTuple(QSize(3 * singleCharSizeAt300[1].width() * 3, singleCharSizeAt300[1].height() * 3), "ABC", 2, 3, 3),
        SizeTestTuple(QSize(3 * singleCharSizeAt300[2].width(), singleCharSizeAt300[2].height()), "ABC", 3, 1, 1),
        SizeTestTuple(QSize(3 * singleCharSizeAt300[2].width() * 3, singleCharSizeAt300[2].height()), "ABC", 3, 3, 1),
        SizeTestTuple(QSize(3 * singleCharSizeAt300[2].width(), singleCharSizeAt300[2].height() * 3), "ABC", 3, 1, 3),
        SizeTestTuple(QSize(3 * singleCharSizeAt300[2].width() * 3, singleCharSizeAt300[2].height() * 3), "ABC", 3, 3, 3),
        SizeTestTuple(QSize(3 * singleCharSizeAt300[3].width(), singleCharSizeAt300[3].height()), "ABC", 4, 1, 1),
        SizeTestTuple(QSize(3 * singleCharSizeAt300[3].width() * 3, singleCharSizeAt300[3].height()), "ABC", 4, 3, 1),
        SizeTestTuple(QSize(3 * singleCharSizeAt300[3].width(), singleCharSizeAt300[3].height() * 3), "ABC", 4, 1, 3),
        SizeTestTuple(QSize(3 * singleCharSizeAt300[3].width() * 3, singleCharSizeAt300[3].height() * 3), "ABC", 4, 3, 3),
        SizeTestTuple(QSize(3 * singleCharSizeAt300[4].width(), singleCharSizeAt300[4].height()), "ABC", 5, 1, 1),
        SizeTestTuple(QSize(3 * singleCharSizeAt300[4].width() * 3, singleCharSizeAt300[4].height()), "ABC", 5, 3, 1),
        SizeTestTuple(QSize(3 * singleCharSizeAt300[4].width(), singleCharSizeAt300[4].height() * 3), "ABC", 5, 1, 3),
        SizeTestTuple(QSize(3 * singleCharSizeAt300[4].width() * 3, singleCharSizeAt300[4].height() * 3), "ABC", 5, 3,
                      3)));

INSTANTIATE_TEST_CASE_P(
    BarcodesTestParameters, EplLabelGeneratorBarcodeTest,
    testing::Values(
        BarcodeTestTuple("B25,100,0,3,2,4,100,N,\"1234\"\n", "1234", EplLabelGenerator::BarcodeType::Code39, 25, 100,
                         100, false, 2, 4, 0),
        BarcodeTestTuple("B25,100,0,3C,2,4,100,N,\"1234\"\n", "1234",
                         EplLabelGenerator::BarcodeType::Code39WithCheckDigit, 25, 100, 100, false, 2, 4, 0),
        BarcodeTestTuple("B25,100,0,9,2,4,100,N,\"1234\"\n", "1234", EplLabelGenerator::BarcodeType::Code93, 25, 100,
                         100, false, 2, 4, 0),
        BarcodeTestTuple("B25,100,0,0,2,4,100,N,\"1234\"\n", "1234", EplLabelGenerator::BarcodeType::Code128UCC, 25,
                         100, 100, false, 2, 4, 0),
        BarcodeTestTuple("B25,100,0,1,2,4,100,N,\"1234\"\n", "1234", EplLabelGenerator::BarcodeType::Code128Auto, 25,
                         100, 100, false, 2, 4, 0),
        BarcodeTestTuple("B25,100,0,1A,2,4,100,N,\"1234\"\n", "1234", EplLabelGenerator::BarcodeType::Code128A, 25, 100,
                         100, false, 2, 4, 0),
        BarcodeTestTuple("B25,100,0,1B,2,4,100,N,\"1234\"\n", "1234", EplLabelGenerator::BarcodeType::Code128B, 25, 100,
                         100, false, 2, 4, 0),
        BarcodeTestTuple("B25,100,0,1C,2,4,100,N,\"1234\"\n", "1234", EplLabelGenerator::BarcodeType::Code128C, 25, 100,
                         100, false, 2, 4, 0),
        BarcodeTestTuple("B25,100,0,1D,2,4,100,N,\"1234\"\n", "1234",
                         EplLabelGenerator::BarcodeType::Code128DeutschePost, 25, 100, 100, false, 2, 4, 0),
        BarcodeTestTuple("B25,100,0,K,2,4,100,N,\"1234\"\n", "1234", EplLabelGenerator::BarcodeType::Codabar, 25, 100,
                         100, false, 2, 4, 0),
        BarcodeTestTuple("B25,100,0,E80,2,4,100,N,\"1234\"\n", "1234", EplLabelGenerator::BarcodeType::Ean8, 25, 100,
                         100, false, 2, 4, 0),
        BarcodeTestTuple("B25,100,0,E82,2,4,100,N,\"1234\"\n", "1234", EplLabelGenerator::BarcodeType::Ean8Addon2, 25,
                         100, 100, false, 2, 4, 0),
        BarcodeTestTuple("B25,100,0,E85,2,4,100,N,\"1234\"\n", "1234", EplLabelGenerator::BarcodeType::Ean8Addon5, 25,
                         100, 100, false, 2, 4, 0),
        BarcodeTestTuple("B25,100,0,E30,2,4,100,N,\"1234\"\n", "1234", EplLabelGenerator::BarcodeType::Ean13, 25, 100,
                         100, false, 2, 4, 0),
        BarcodeTestTuple("B25,100,0,E32,2,4,100,N,\"1234\"\n", "1234", EplLabelGenerator::BarcodeType::Ean13Addon2, 25,
                         100, 100, false, 2, 4, 0),
        BarcodeTestTuple("B25,100,0,E35,2,4,100,N,\"1234\"\n", "1234", EplLabelGenerator::BarcodeType::Ean13Addon5, 25,
                         100, 100, false, 2, 4, 0),
        BarcodeTestTuple("B25,100,0,2G,2,4,100,N,\"1234\"\n", "1234", EplLabelGenerator::BarcodeType::GermanPostCode,
                         25, 100, 100, false, 2, 4, 0),
        BarcodeTestTuple("B25,100,0,2,2,4,100,N,\"1234\"\n", "1234", EplLabelGenerator::BarcodeType::Interleaved2Of5,
                         25, 100, 100, false, 2, 4, 0),
        BarcodeTestTuple("B25,100,0,2C,2,4,100,N,\"1234\"\n", "1234",
                         EplLabelGenerator::BarcodeType::Interleaved2Of5WithMod10CheckDigit, 25, 100, 100, false, 2, 4, 0),
        BarcodeTestTuple("B25,100,0,2D,2,4,100,N,\"1234\"\n", "1234",
                         EplLabelGenerator::BarcodeType::Interleaved2Of5WithHumanReadableCheckDigit, 25, 100, 100,
                         false, 2, 4, 0),
        BarcodeTestTuple("B25,100,0,P,2,4,100,N,\"1234\"\n", "1234", EplLabelGenerator::BarcodeType::Postnet, 25, 100,
                         100, false, 2, 4, 0),
        BarcodeTestTuple("B25,100,0,PL,2,4,100,N,\"1234\"\n", "1234", EplLabelGenerator::BarcodeType::Planet, 25,
                         100, 100, false, 2, 4, 0),
        BarcodeTestTuple("B25,100,0,J,2,4,100,N,\"1234\"\n", "1234", EplLabelGenerator::BarcodeType::PostnetJapanese,
                         25, 100, 100, false, 2, 4, 0),
        BarcodeTestTuple("B25,100,0,1E,2,4,100,N,\"1234\"\n", "1234", EplLabelGenerator::BarcodeType::UccEan128, 25,
                         100, 100, false, 2, 4, 0),
        BarcodeTestTuple("B25,100,0,UA0,2,4,100,N,\"1234\"\n", "1234", EplLabelGenerator::BarcodeType::UpcA, 25,
                         100, 100, false, 2, 4, 0),
        BarcodeTestTuple("B25,100,0,UA2,2,4,100,N,\"1234\"\n", "1234", EplLabelGenerator::BarcodeType::UpcAAddon2, 25,
                         100, 100, false, 2, 4, 0),
        BarcodeTestTuple("B25,100,0,UA5,2,4,100,N,\"1234\"\n", "1234", EplLabelGenerator::BarcodeType::UpcAAddon5, 25,
                         100, 100, false, 2, 4, 0),
        BarcodeTestTuple("B25,100,0,UE0,2,4,100,N,\"1234\"\n", "1234", EplLabelGenerator::BarcodeType::UpcE, 25,
                         100, 100, false, 2, 4, 0),
        BarcodeTestTuple("B25,100,0,UE2,2,4,100,N,\"1234\"\n", "1234", EplLabelGenerator::BarcodeType::UpcEAddon2, 25,
                         100, 100, false, 2, 4, 0),
        BarcodeTestTuple("B25,100,0,UE5,2,4,100,N,\"1234\"\n", "1234", EplLabelGenerator::BarcodeType::UpcEAddon5, 25,
                         100, 100, false, 2, 4, 0),
        BarcodeTestTuple("B25,100,0,2U,2,4,100,N,\"1234\"\n", "1234",
                         EplLabelGenerator::BarcodeType::UpcInterleaved2Of5, 25, 100, 100, false, 2, 4, 0),
        BarcodeTestTuple("B25,100,0,L,2,4,100,N,\"1234\"\n", "1234",
                         EplLabelGenerator::BarcodeType::Msi1WithMod10CheckDigit, 25, 100, 100, false, 2, 4, 0),
        BarcodeTestTuple("B25,100,0,M,2,4,100,N,\"1234\"\n", "1234",
                         EplLabelGenerator::BarcodeType::Msi3WithMod10CheckDigit, 25, 100, 100, false, 2, 4, 0),

        BarcodeTestTuple("B250,400,0,3,2,4,100,N,\"1234\"\n", "1234", EplLabelGenerator::BarcodeType::Code39, 250, 400,
                         100, false, 2, 4, 0),
        BarcodeTestTuple("B25,100,0,3,2,4,200,N,\"1234\"\n", "1234", EplLabelGenerator::BarcodeType::Code39, 25, 100,
                         200, false, 2, 4, 0),
        BarcodeTestTuple("B25,100,0,3,4,6,100,N,\"1234\"\n", "1234", EplLabelGenerator::BarcodeType::Code39, 25, 100,
                         100, false, 4, 6, 0),
        BarcodeTestTuple("B25,100,0,3,2,4,100,B,\"1234\"\n", "1234", EplLabelGenerator::BarcodeType::Code39, 25, 100,
                         100, true, 2, 4, 0),
        BarcodeTestTuple("B25,100,0,3,2,4,100,N,\"1234\\\"\"\n", "1234\"", EplLabelGenerator::BarcodeType::Code39, 25,
                         100, 100, false, 2, 4, 0),
        BarcodeTestTuple("B25,100,0,3,2,4,100,N,\"1234\\\\\"\n", "1234\\", EplLabelGenerator::BarcodeType::Code39, 25,
                         100, 100, false, 2, 4, 0),
        BarcodeTestTuple("B25,100,1,3,2,4,100,N,\"1234\"\n", "1234", EplLabelGenerator::BarcodeType::Code39, 25, 100,
                         100, false, 2, 4, 90),
        BarcodeTestTuple("B25,100,2,3,2,4,100,N,\"1234\"\n", "1234", EplLabelGenerator::BarcodeType::Code39, 25, 100,
                         100, false, 2, 4, 180),
        BarcodeTestTuple("B25,100,3,3,2,4,100,N,\"1234\"\n", "1234", EplLabelGenerator::BarcodeType::Code39, 25, 100,
                         100, false, 2, 4, 270)));

INSTANTIATE_TEST_CASE_P(
    AddTextTestParameters, EplLabelGeneratorAddTextTest,
    testing::Values(AddTextTuple("A25,100,0,1,1,1,N,\"PRNT\"\n", "PRNT", 25, 100, 1, 1, 1, 0, false),
                    AddTextTuple("A25,100,0,1,1,1,N,\"PRNT\"\n", "PRNT", 25, 100, 0, 1, 1, 0, false),
                    AddTextTuple("A25,100,0,7,1,1,N,\"420\"\n", "420", 25, 100, 7, 1, 1, 0, false),
                    AddTextTuple("A25,100,0,7,1,1,N,\"420\"\n", "420", 25, 100, 8, 1, 1, 0, false),
                    AddTextTuple("A25,100,0,5,1,1,N,\"PRNT\"\n", "PRNT", 25, 100, 7, 1, 1, 0, false),
                    AddTextTuple("A25,100,0,4,1,1,N,\"PRNT\"\n", "PRNT", 25, 100, 4, 1, 1, 0, false),
                    AddTextTuple("A25,100,0,1,1,1,N,\"PRNT\"\n", "PRNT", 25, 100, 1, 0, 1, 0, false),
                    AddTextTuple("A25,100,0,1,6,1,N,\"PRNT\"\n", "PRNT", 25, 100, 1, 6, 1, 0, false),
                    AddTextTuple("A25,100,0,1,6,1,N,\"PRNT\"\n", "PRNT", 25, 100, 1, 7, 1, 0, false),
                    AddTextTuple("A25,100,0,1,8,1,N,\"PRNT\"\n", "PRNT", 25, 100, 1, 8, 1, 0, false),
                    AddTextTuple("A25,100,0,1,8,1,N,\"PRNT\"\n", "PRNT", 25, 100, 1, 9, 1, 0, false),
                    AddTextTuple("A25,100,0,1,1,1,N,\"PRNT\"\n", "PRNT", 25, 100, 1, 1, 0, 0, false),
                    AddTextTuple("A25,100,0,1,1,9,N,\"PRNT\"\n", "PRNT", 25, 100, 1, 1, 9, 0, false),
                    AddTextTuple("A25,100,0,1,1,9,N,\"PRNT\"\n", "PRNT", 25, 100, 1, 1, 10, 0, false),
                    AddTextTuple("A25,100,0,1,1,8,N,\"PRNT\"\n", "PRNT", 25, 100, 1, 1, 8, 0, false),
                    AddTextTuple("A25,100,1,1,1,1,N,\"PRNT\"\n", "PRNT", 25, 100, 1, 1, 1, 90, false),
                    AddTextTuple("A25,100,2,1,1,1,N,\"PRNT\"\n", "PRNT", 25, 100, 1, 1, 1, 180, false),
                    AddTextTuple("A25,100,3,1,1,1,N,\"PRNT\"\n", "PRNT", 25, 100, 1, 1, 1, 270, false),
                    AddTextTuple("A25,100,0,1,1,1,N,\"PRNT\"\n", "PRNT", 25, 100, 1, 1, 1, 360, false),
                    AddTextTuple("A25,100,1,1,1,1,N,\"PRNT\"\n", "PRNT", 25, 100, 1, 1, 1, 450, false),
                    AddTextTuple("A25,100,1,1,1,1,N,\"PRNT\"\n", "PRNT", 25, 100, 1, 1, 1, 100, false),
                    AddTextTuple("A25,100,3,1,1,1,N,\"PRNT\"\n", "PRNT", 25, 100, 1, 1, 1, -90, false),
                    AddTextTuple("A25,100,2,1,1,1,N,\"PRNT\"\n", "PRNT", 25, 100, 1, 1, 1, -180, false),
                    AddTextTuple("A25,100,1,1,1,1,N,\"PRNT\"\n", "PRNT", 25, 100, 1, 1, 1, -270, false),
                    AddTextTuple("A25,100,0,1,1,1,N,\"PRNT\"\n", "PRNT", 25, 100, 1, 1, 1, -360, false),
                    AddTextTuple("A25,100,3,1,1,1,N,\"PRNT\"\n", "PRNT", 25, 100, 1, 1, 1, -450, false),
                    AddTextTuple("A25,100,0,1,1,1,R,\"PRNT\"\n", "PRNT", 25, 100, 1, 1, 1, 0, true)));

TEST(EplLabelGeneratorTest, sanity)
{
    {
        EplLabelGenerator generator;
        generator.startLabel();
        EXPECT_EQ(QSize(795, 1250), generator.labelSize());
    }
    {
        EplLabelGenerator generator;
        generator.startLabel(1000, 500);
        EXPECT_EQ(QSize(1000, 500), generator.labelSize());
    }
}

TEST(EplLabelGeneratorTest, emptyLabel)
{
    EplLabelGenerator generator;
    generator.startLabel();
    generator.addClearBufferCommand();
    generator.addPrintCommand();

    QByteArray reference = dataFromFile(":/data/emptylabel.epl");
    reference.replace("\n\n", "\n");
    QByteArray result = generator.labelData();
    result.replace("\n\n", "\n");
    EXPECT_EQ(reference, result);
}

TEST_P(EplLabelGenerator203SizesTest, textSize203)
{
    QSize size = std::apply(
        [](const auto &... args) {
            EplLabelGenerator generator(203);
            return generator.textSize(args...);
        },
        RemoveFirstType<SizeTestTuple>::result(GetParam()));
    QSize expected = std::get<0>(GetParam());
    EXPECT_EQ(expected, size) << expected.width() << "x" << expected.height() << " != " << size.width() << "x"
                              << size.height();
}

TEST_P(EplLabelGenerator300SizesTest, textSize300)
{
    QSize size = std::apply(
        [](const auto &... args) {
            EplLabelGenerator generator(300);
            return generator.textSize(args...);
        },
        RemoveFirstType<SizeTestTuple>::result(GetParam()));
    QSize expected = std::get<0>(GetParam());
    EXPECT_EQ(expected, size) << expected.width() << "x" << expected.height() << " != " << size.width() << "x"
                              << size.height();
}

TEST_P(EplLabelGeneratorAddTextTest, addText)
{
    QByteArray result = std::apply(
        [](const auto &... args) {
            EplLabelGenerator generator;
            generator.addText(args...);
            return generator.labelData();
        },
        RemoveFirstType<AddTextTuple>::result(GetParam()));
    EXPECT_EQ(std::get<0>(GetParam()), result);
}

TEST(EplLabelGeneratorTest, straightTextBox)
{
    EplLabelGenerator generator;

    QByteArray reference = "A25,100,0,5,3,4,N,\"PRNT\"\n";
    QSize expectedSize = generator.textSize("PRNT", 5, 3, 4);

    QRect rect = generator.addText("PRNT", 25, 100, 5, 3, 4);
    QByteArray result = generator.labelData();

    EXPECT_EQ(reference, result);
    EXPECT_EQ(QRect(QPoint(25, 100), expectedSize), rect)
        << expectedSize.width() << "x" << expectedSize.height() << " != " << rect.width() << "x" << rect.height();
}

TEST(EplLabelGeneratorTest, rotatedTextBox)
{
    EplLabelGenerator generator;

    QByteArray reference = "A25,100,1,5,3,4,N,\"PRNT\"\n";
    QSize expectedSize = generator.textSize("PRNT", 5, 3, 4);

    QRect rect = generator.addText("PRNT", 25, 100, 5, 3, 4, 90);
    QByteArray result = generator.labelData();

    EXPECT_EQ(reference, result);
    EXPECT_EQ(25 - expectedSize.height(), rect.x());
    EXPECT_EQ(100, rect.y());
    EXPECT_EQ(expectedSize.height(), rect.width());
    EXPECT_EQ(expectedSize.width(), rect.height());
}

TEST(EplLabelGeneratorTest, backwardRotatedTextBox)
{
    EplLabelGenerator generator;

    QByteArray reference = "A25,100,3,5,3,4,N,\"PRNT\"\n";
    QSize expectedSize = generator.textSize("PRNT", 5, 3, 4);

    QRect rect = generator.addText("PRNT", 25, 100, 5, 3, 4, 270);
    QByteArray result = generator.labelData();

    EXPECT_EQ(reference, result);
    EXPECT_EQ(25, rect.x());
    EXPECT_EQ(100 - expectedSize.width(), rect.y());
    EXPECT_EQ(expectedSize.height(), rect.width());
    EXPECT_EQ(expectedSize.width(), rect.height());
}

TEST(EplLabelGeneratorTest, upsideDownTextBox)
{
    EplLabelGenerator generator;

    QByteArray reference = "A25,100,2,5,3,4,N,\"PRNT\"\n";
    QSize expectedSize = generator.textSize("PRNT", 5, 3, 4);

    QRect rect = generator.addText("PRNT", 25, 100, 5, 3, 4, 180);
    QByteArray result = generator.labelData();

    EXPECT_EQ(reference, result);
    EXPECT_EQ(25 - expectedSize.width(), rect.x());
    EXPECT_EQ(100 - expectedSize.height(), rect.y());
    EXPECT_EQ(expectedSize.height(), rect.height());
    EXPECT_EQ(expectedSize.width(), rect.width());
}

TEST(EplLabelGeneratorTest, inversedColorsTextBox)
{
    EplLabelGenerator generator;

    QByteArray reference = "A25,100,0,5,3,4,R,\"PRNT\"\n";
    QSize expectedSize = generator.textSize("PRNT", 5, 3, 4);

    QRect rect = generator.addText("PRNT", 25, 100, 5, 3, 4, 0, true);
    QByteArray result = generator.labelData();

    EXPECT_EQ(reference, result);
    EXPECT_EQ(QRect(QPoint(25, 100), expectedSize), rect)
        << expectedSize.width() << "x" << expectedSize.height() << " != " << rect.width() << "x" << rect.height();
}

TEST(EplLabelGeneratorTest, blackLine)
{
    EplLabelGenerator generator;

    QByteArray reference = "LO25,100,500,20\n";
    QRect expectedRect(25, 100, 500, 20);

    QRect rect = generator.addLine(expectedRect.x(), expectedRect.y(), expectedRect.width(), expectedRect.height());
    QByteArray result = generator.labelData();

    EXPECT_EQ(reference, result);
    EXPECT_EQ(expectedRect, rect) << expectedRect.x() << "," << expectedRect.y() << ";" << expectedRect.width() << "x"
                                  << expectedRect.height() << " != " << rect.x() << "," << rect.y() << ";"
                                  << rect.width() << "x" << rect.height();
}

TEST(EplLabelGeneratorTest, whiteLine)
{
    EplLabelGenerator generator;

    QByteArray reference = "LW25,100,500,20\n";
    QRect expectedRect(25, 100, 500, 20);

    QRect rect = generator.addLine(expectedRect.x(), expectedRect.y(), expectedRect.width(), expectedRect.height(),
                                   EplLabelGenerator::LineType::White);
    QByteArray result = generator.labelData();

    EXPECT_EQ(reference, result);
    EXPECT_EQ(expectedRect, rect) << expectedRect.x() << "," << expectedRect.y() << ";" << expectedRect.width() << "x"
                                  << expectedRect.height() << " != " << rect.x() << "," << rect.y() << ";"
                                  << rect.width() << "x" << rect.height();
}

TEST(EplLabelGeneratorTest, xorLine)
{
    EplLabelGenerator generator;

    QByteArray reference = "LE25,100,500,20\n";
    QRect expectedRect(25, 100, 500, 20);

    QRect rect = generator.addLine(expectedRect.x(), expectedRect.y(), expectedRect.width(), expectedRect.height(),
                                   EplLabelGenerator::LineType::Xor);
    QByteArray result = generator.labelData();

    EXPECT_EQ(reference, result);
    EXPECT_EQ(expectedRect, rect) << expectedRect.x() << "," << expectedRect.y() << ";" << expectedRect.width() << "x"
                                  << expectedRect.height() << " != " << rect.x() << "," << rect.y() << ";"
                                  << rect.width() << "x" << rect.height();
}

TEST(EplLabelGeneratorTest, diagonalLine)
{
    EplLabelGenerator generator;

    QByteArray reference = "LS25,100,20,500,300\n";
    QRect expectedRect(25, 100, 475, 220);
    int width = 20;

    QRect rect = generator.addDiagonalLine(expectedRect.x(), expectedRect.y(), expectedRect.x() + expectedRect.width(),
                                           expectedRect.y() + expectedRect.height() - width, width);
    QByteArray result = generator.labelData();

    EXPECT_EQ(reference, result);
    EXPECT_EQ(expectedRect, rect) << expectedRect.x() << "," << expectedRect.y() << ";" << expectedRect.width() << "x"
                                  << expectedRect.height() << " != " << rect.x() << "," << rect.y() << ";"
                                  << rect.width() << "x" << rect.height();
}

TEST(EplLabelGeneratorTest, rotatedDiagonalLine)
{
    EplLabelGenerator generator;

    QByteArray reference = "LS25,300,20,500,100\n";
    QRect expectedRect(25, 100, 475, 220);
    int width = 20;

    QRect rect = generator.addDiagonalLine(expectedRect.x(), expectedRect.y() + expectedRect.height() - width,
                                           expectedRect.x() + expectedRect.width(), expectedRect.y(), width);
    QByteArray result = generator.labelData();

    EXPECT_EQ(reference, result);
    EXPECT_EQ(expectedRect, rect) << expectedRect.x() << "," << expectedRect.y() << ";" << expectedRect.width() << "x"
                                  << expectedRect.height() << " != " << rect.x() << "," << rect.y() << ";"
                                  << rect.width() << "x" << rect.height();
}

TEST_P(EplLabelGeneratorBarcodeTest, barcodes)
{
    QByteArray result = std::apply(
        [](const auto &... args) {
            EplLabelGenerator generator;
            generator.addBarcode(args...);
            return generator.labelData();
        },
        RemoveFirstType<BarcodeTestTuple>::result(GetParam()));
    QByteArray expected = std::get<0>(GetParam()).toLatin1();
    EXPECT_EQ(expected, result);
}

TEST(EplLabelGeneratorTest, straightBarcodeSize)
{
    EplLabelGenerator generator;
    QRect expectedRect(250, 300, 795 - 250, 150);
    QRect rect = generator.addBarcode("1234", EplLabelGenerator::BarcodeType::Code39, 250, 300, 150, false);
    EXPECT_EQ(expectedRect, rect) << expectedRect.x() << "," << expectedRect.y() << ";" << expectedRect.width() << "x"
                                  << expectedRect.height() << " != " << rect.x() << "," << rect.y() << ";"
                                  << rect.width() << "x" << rect.height();
}

TEST(EplLabelGeneratorTest, straightBarcodeWithTextSize)
{
    EplLabelGenerator generator;
    QRect expectedRect(250, 300, 795 - 250, 150 + generator.textSize("1234").height());
    QRect rect = generator.addBarcode("1234", EplLabelGenerator::BarcodeType::Code39, 250, 300, 150);
    EXPECT_EQ(expectedRect, rect) << expectedRect.x() << "," << expectedRect.y() << ";" << expectedRect.width() << "x"
                                  << expectedRect.height() << " != " << rect.x() << "," << rect.y() << ";"
                                  << rect.width() << "x" << rect.height();
}

TEST(EplLabelGeneratorTest, straightTallBarcodeSize)
{
    EplLabelGenerator generator;
    QRect expectedRect(250, 300, 795 - 250, 450);
    QRect rect = generator.addBarcode("1234", EplLabelGenerator::BarcodeType::Code39, 250, 300, 450, false);
    EXPECT_EQ(expectedRect, rect) << expectedRect.x() << "," << expectedRect.y() << ";" << expectedRect.width() << "x"
                                  << expectedRect.height() << " != " << rect.x() << "," << rect.y() << ";"
                                  << rect.width() << "x" << rect.height();
}

TEST(EplLabelGeneratorTest, rotatedBarcodeSize)
{
    EplLabelGenerator generator;
    QRect expectedRect(250 - 150, 300, 150, 1250 - 300);
    QRect rect = generator.addBarcode("1234", EplLabelGenerator::BarcodeType::Code39, 250, 300, 150, false, 2, 4, 90);
    EXPECT_EQ(expectedRect, rect) << expectedRect.x() << "," << expectedRect.y() << ";" << expectedRect.width() << "x"
                                  << expectedRect.height() << " != " << rect.x() << "," << rect.y() << ";"
                                  << rect.width() << "x" << rect.height();
}

TEST(EplLabelGeneratorTest, backwardRotatedBarcodeSize)
{
    EplLabelGenerator generator;
    QRect expectedRect(250, 0, 150, 300);
    QRect rect = generator.addBarcode("1234", EplLabelGenerator::BarcodeType::Code39, 250, 300, 150, false, 2, 4, 270);
    EXPECT_EQ(expectedRect, rect) << expectedRect.x() << "," << expectedRect.y() << ";" << expectedRect.width() << "x"
                                  << expectedRect.height() << " != " << rect.x() << "," << rect.y() << ";"
                                  << rect.width() << "x" << rect.height();
}

TEST(EplLabelGeneratorTest, upsideDownBarcodeSize)
{
    EplLabelGenerator generator;
    QRect expectedRect(0, 300 - 150, 250, 150);
    QRect rect = generator.addBarcode("1234", EplLabelGenerator::BarcodeType::Code39, 250, 300, 150, false, 2, 4, 180);
    EXPECT_EQ(expectedRect, rect) << expectedRect.x() << "," << expectedRect.y() << ";" << expectedRect.width() << "x"
                                  << expectedRect.height() << " != " << rect.x() << "," << rect.y() << ";"
                                  << rect.width() << "x" << rect.height();
}
