# SPDX-License-Identifier: MIT

.include "aliases.s"

.global invalidate_dcache_range
invalidate_dcache_range:
    # Return early if the size is zero
    cmpwi r4, 0
    beqlr

    # Setup the block count
    clrlwi r5, r3, 27
    add r4, r4, r5
    addi r4, r4, 31
    rlwinm r4, r4, 27, 5, 31
    mtctr r4

    # Invalidate the blocks in a loop
.Linvalidate_loop:
    dcbi 0, r3
    addi r3, r3, 0x20
    bdnz .Linvalidate_loop

    # Wait until everything has completed
    sync

    blr

.global flush_dcache_range
flush_dcache_range:
    # Return early if the size is zero
    cmplwi r4, 0
    beqlr

    # Setup the block count
    clrlwi r5, r3, 27
    add r4, r4, r5
    addi r4, r4, 31
    rlwinm r4, r4, 27, 5, 31
    mtctr r4

    # Flush the blocks in a loop
.Lflush_loop:
    dcbi 0, r3
    addi r3, r3, 0x20
    bdnz .Lflush_loop

    # Wait until everything has completed
    sync

    blr
