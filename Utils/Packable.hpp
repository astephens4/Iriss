#ifndef IRISS_PACKABLE_H
#define IRISS_PACKABLE_H

#include <vector>
#include <cinttypes>

namespace Utils {

class Packable {
public:
    /**
     * Convert and pack this class into a stream of bytes.
     * @param [out] bytes The packed class contents are placed in here
     */
    virtual void pack(std::vector<uint8_t>& bytes) const = 0;

    /**
     * Convert the received stream of bytes into this class' contents
     * @param [out] bytes The packed class contents are placed in here
     */
    virtual void unpack(const std::vector<uint8_t>& bytes) = 0;

protected:
    /**
     * Primitive data types packing functions
     */
    void pack_uint32(uint32_t host, uint8_t *data) const;
    void pack_int32(int32_t host, uint8_t *data) const;
    void pack_float32(float host, uint8_t *data) const;
    void pack_float64(double host, uint8_t *data) const;

    /**
     * Primitive data types unpacking functions
     */
    void unpack_uint32(const uint8_t *data, uint32_t& host) const;
    void unpack_int32(const uint8_t *data, int32_t& host) const;
    void unpack_float32(const uint8_t *data, float& host) const;
    void unpack_float64(const uint8_t *data, double& host) const;

};

} // end namespace Utils

#endif // IRISS_PACKABLE_H
