# SPDX-License-Identifier: MIT

#include "aliases.S"

.type invalidate_dcache_range, function
.global invalidate_dcache_range
invalidate_dcache_range:
    # Return early if the size is zero
    cmpwi r4, 0
    beqlr

    # Setup the block count
    clrlwi r5, r3, 27
    add r4, r4, r5
    addi r4, r4, 0x1f
    rlwinm r4, r4, 27, 5, 31
    mtctr r4

    # Invalidate the blocks in a loop
.Linvalidate_dc_loop:
    dcbi 0, r3
    addi r3, r3, 0x20
    bdnz .Linvalidate_dc_loop

    # Wait until everything has completed
    sync

    blr

.type flush_dcache_range, function
.global flush_dcache_range
flush_dcache_range:
    # Return early if the size is zero
    cmpwi r4, 0
    beqlr

    # Setup the block count
    clrlwi r5, r3, 27
    add r4, r4, r5
    addi r4, r4, 0x1f
    rlwinm r4, r4, 27, 5, 31
    mtctr r4

    # Flush the blocks in a loop
.Lflush_dc_loop:
    dcbf 0, r3
    addi r3, r3, 0x20
    bdnz .Lflush_dc_loop

    # Wait until everything has completed
    sync

    blr

.type invalidate_icache_range, function
.global invalidate_icache_range
invalidate_icache_range:
    # Return early if the size is zero
    cmpwi r4, 0
    beqlr

    # Setup the block count
    clrlwi r5, r3, 27
    add r4, r4, r5
    addi r4, r4, 0x1f
    rlwinm r4, r4, 27, 5, 31
    mtctr r4

    # Invalidate the blocks in a loop
.Linvalidate_ic_loop:
    icbi 0, r3
    addi r3, r3, 0x20
    bdnz .Linvalidate_ic_loop

    # Wait until everything has completed
    sync

    # Discard the already fetched instructions
    isync

    blr
