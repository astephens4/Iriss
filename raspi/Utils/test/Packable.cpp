#include "Utils/Packable.hpp"
#include "test/Testing.hpp"

class PackableImpl : public Utils::Packable {
public:
    PackableImpl(void) :
        testInt(-42),
        testUint(87252),
        testFloat(-0.3245f)
    {
    }

    virtual void pack(std::vector<uint8_t>& bytes) const
    {
        bytes.clear();
        bytes.resize(12);
        pack_int32(testInt, &(bytes[0]));
        pack_uint32(testUint, &(bytes[4]));
        pack_float32(testFloat, &(bytes[8]));
    }

    virtual void unpack(const std::vector<uint8_t>& bytes)
    {
        int32_t unpackedInt;
        uint32_t unpackedUint;
        float unpackedFloat;

        unpack_int32(&(bytes[0]), unpackedInt);
        unpack_uint32(&(bytes[4]), unpackedUint);
        unpack_float32(&(bytes[8]), unpackedFloat);

        AssertEquals(testInt, unpackedInt);
        AssertEquals(testUint, unpackedUint);
        AssertEquals(testFloat, unpackedFloat);
    }

private:
    int32_t testInt;
    uint32_t testUint;
    float testFloat;
};

int main(void)
{
    PackableImpl packer;

    std::vector<uint8_t> bytes;
    packer.pack(bytes);
    packer.unpack(bytes);
    return 0;
}
