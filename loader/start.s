# SPDX-License-Identifier: MIT

.include "aliases.s"

.global _start
_start:
    # Disable interrupts
    mfmsr r3
    rlwinm r3, r3, 0, 17, 15
    mtmsr r3

    # Initialize the stack pointer
    .set stack_top, 0x8080fff0
    lis r1, stack_top@h
    ori r1, r1, stack_top@l

    # Jump to C code
    bl main

    # We should never get there
.Linfinite_loop:
    b .Linfinite_loop

dolphin_load_wii_mode:
    # The presence of such an instruction is the criterion for Dolphin to load an ELF in Wii mode.
    mtspr HID4, r3
