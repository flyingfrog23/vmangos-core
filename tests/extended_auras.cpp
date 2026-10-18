#include "ExtendedAuras.h"
#include <cassert>
#include <iostream>

int main()
{
    std::vector<uint16_t> used;
    std::vector<ExtendedAuras::Record> records;
    for (unsigned i = 0; i < 255; ++i)
    {
        uint16_t slot = ExtendedAuras::Allocate(false, used);
        assert(slot == 32 + i);
        used.push_back(slot);
        records.push_back({slot, 1000 + i, 8, 60, 255, 12000, 7});
    }
    assert(ExtendedAuras::Allocate(false, used) == ExtendedAuras::NoSlot);
    assert(ExtendedAuras::Allocate(true, used) == 0);
    used.erase(used.begin() + 100);
    assert(ExtendedAuras::Allocate(false, used) == 132);
    assert(ExtendedAuras::Encode(42, {}) == "BENAURA1\t42\t0\t");
    assert(ExtendedAuras::Encode(42, {{286, 123, 8, 60, 3, 12000, 7}}) ==
        "BENAURA1\t42\t1\t286,123,8,60,3,12000,7;");
    std::string message = ExtendedAuras::Encode(42, records);
    assert(message.size() < 24576);
    // Machine-readable fixture consumed by the Rust live/interop probe.
    std::cout << message << '\n';
}
