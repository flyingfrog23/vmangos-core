# Benilla extended debuffs

Set `DebuffLimit = 255` and `Benilla.ExtendedAuras = 1` in `mangosd.conf`, then restart.
The matching Benilla fork detects support automatically; `BENILLA_EXTENDED_AURAS=0` forces
legacy negotiation. The server switch defaults off.
The gameplay limit is shared by all clients; the new display channel is negotiated per session.

The extension allocates 32 buff and 255 debuff slots independently of `MAX_AURAS`. It leaves
the fixed 48-field vanilla layout, all stock opcodes, and the Wallcraft NPC slot borrowing
unchanged. Stock clients receive no extension messages. Benilla falls back to the legacy fields
on servers that lack the extension. This code can accompany either pre-Wallcraft or Wallcraft
legacy allocation; do not raise `MAX_AURAS` or resize the update-field arrays.

Version 1 is bounded at 255 displayed debuffs. Set `DebuffLimit` no higher than 255. Existing
priority eviction, buff limits, aura mechanics, and out-of-range party records remain unchanged.
An unmodified stock client retains its existing display limit, including any client patches
already used for Wallcraft. The extension does not patch the stock executable or install files.

The client advertises the ordinary secure-addon record `Benilla_ExtendedAuras_v1`, flags 1,
CRC `0x42454131`, URL CRC 0. Only that record, client build 5875, and the server setting enable
snapshots. Older servers treat the record as a custom addon. Snapshots use existing server-only
SYSTEM addon chat, sender 0, with full replacement lists prefixed `BENAURA1` and tab separators.
The matching Benilla `docs/EXTENDED_AURAS.md` specifies the format, compatibility matrix, built-in
overflow UI, and client tests. Initial snapshots precede the batched CREATE; clients must queue
them. Changes are coalesced per unit update and sent only to negotiated viewers of that unit.

The independent limit and encoding regression test needs no database:

```sh
c++ -std=c++14 -Wall -Wextra -Werror -I src/game/Spells tests/extended_auras.cpp -o /tmp/extended-auras-test
/tmp/extended-auras-test
```

It checks 255 negative slots, exhaustion, reuse, separate positive allocation, empty replacement,
and a maximum-size fixture. A live test must separately check negotiation and aura lifecycle.
