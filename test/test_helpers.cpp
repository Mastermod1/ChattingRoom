#include <gtest/gtest.h>

#include <cstdint>

#include "src/helpers/helpers.hpp"

TEST(ConvertStringAddressToUint32Should, ReturnMaxUint32For255ValueOctets)
{
    std::string addr = "255.255.255.255";
    EXPECT_EQ(convertStringAddressToUint32(addr), UINT32_MAX);
}

TEST(ConvertStringAddressToUint32Should, Return0For0ValueOctets)
{
    std::string addr = "0.0.0.0";
    EXPECT_EQ(convertStringAddressToUint32(addr), 0);
}

TEST(ConvertStringAddressToUint32Should, Return12For12AsFirstOctetAndRest0)
{
    std::string addr = "12.0.0.0";
    EXPECT_EQ(convertStringAddressToUint32(addr), 12);
}

TEST(ConvertStringAddressToUint32Should, Return1Times2ToPow24For1AsLastOctetAndRest0)
{
    std::string addr = "0.0.0.1";
    EXPECT_EQ(convertStringAddressToUint32(addr), 1 << 24);
}
