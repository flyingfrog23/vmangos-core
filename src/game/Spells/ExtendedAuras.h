#ifndef VMANGOS_EXTENDED_AURAS_H
#define VMANGOS_EXTENDED_AURAS_H

#include <array>
#include <cstdint>
#include <sstream>
#include <vector>

// Independent of MAX_AURAS: never change the vanilla update-field descriptor.
namespace ExtendedAuras
{
    constexpr uint16_t PositiveSlots = 32;
    constexpr uint16_t DebuffSlots = 255;
    constexpr uint16_t Slots = PositiveSlots + DebuffSlots;
    constexpr uint16_t NoSlot = 0xffff;
    constexpr char AddonName[] = "Benilla_ExtendedAuras_v1";
    constexpr uint32_t AddonCRC = 0x42454131;

    struct Record
    {
        uint16_t slot;
        uint32_t spell;
        uint8_t flags;
        uint8_t level;
        uint8_t stacks;
        int32_t remaining;
        uint64_t caster;
    };

    inline uint16_t Allocate(bool positive, std::vector<uint16_t> const& used)
    {
        std::array<bool, Slots> occupied{};
        for (uint16_t slot : used)
            if (slot < Slots)
                occupied[slot] = true;
        for (uint16_t slot = positive ? 0 : PositiveSlots;
             slot < (positive ? PositiveSlots : Slots); ++slot)
            if (!occupied[slot])
                return slot;
        return NoSlot;
    }

    // Full replacement, including an empty list. ASCII avoids NULs in the addon chat envelope.
    inline std::string Encode(uint64_t guid, std::vector<Record> const& auras)
    {
        std::ostringstream out;
        out << "BENAURA1\t" << guid << '\t' << auras.size() << '\t';
        for (Record const& a : auras)
            out << a.slot << ',' << a.spell << ',' << unsigned(a.flags) << ','
                << unsigned(a.level) << ',' << unsigned(a.stacks) << ',' << a.remaining << ',' << a.caster << ';';
        return out.str();
    }
}

#endif
