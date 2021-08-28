# SPDX-License-Identifier: MIT

.include "aliases.s"

.global _start
_start:
    # Disable interrupts
    mfmsr r3
    rlwinm r3, r3, 0, 26, 24
    mtmsr r3

    # Initialize stack pointer
    lis r1, .Lstack_top@h
    ori r1, r1, .Lstack_top@l

    # Jump to C code
    bl main

    # We should never get there
.Linfinite_loop:
    b .Linfinite_loop

dolphin_load_wii_mode:
    # The presence of such an instruction is the criterion for Dolphin to load an ELF in Wii mode.
    mtspr HID4, r3

# TODO there must be a better way to to this
.data
.Lstack_bottom:
    .space 0x4000
.Lstack_top:
