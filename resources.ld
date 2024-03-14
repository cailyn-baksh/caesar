/*
 * Linker script for generating resource files.
 *
 * This script generates object files for BLOB resources, presenting read-only
 * data along with an easy-to-use interface for using the resources in C.
 */

/* Read input files as BLOBs */
TARGET("binary")

/*
 * TODO: make this platform/target independant
 * consider using `INSERT AFTER` to augment the default linker script
 */
OUTPUT_FORMAT("elf64-x86-64", "elf64-x86-64", "elf64-x86-64")

SECTIONS
{
    .data : { *(.data) }
}
