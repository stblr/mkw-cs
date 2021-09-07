# SPDX-License-Identifier: MIT

.section .rodata

.type patches, @object
.global patches
.align 4
patches:
    .incbin "../client/client.bin"
patches_end:

.type patches_size, @object
.global patches_size
.align 4
patches_size:
    .int patches_end - patches

