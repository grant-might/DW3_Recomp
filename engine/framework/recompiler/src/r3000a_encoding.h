// r3000a_encoding.h — normative architectural facts about the MIPS R3000A
// instruction encoding, shared by the strict translator and by function
// discovery so the two can never disagree about whether a word is translatable.
//
// WHY THIS EXISTS
// ---------------
// Function discovery sweeps data-as-code: the BIOS/game kernel pointer tables,
// embedded rodata and ASCII strings all sit in the same ROM bytes the walker
// reads as instructions. Any 32-bit data word whose top six bits happen to read
// 0x2F (every 0xBFxxxxxx ROM pointer has them) or whose low six bits read 0x3C
// used to be rejected by the strict translator, and because discovery aborts a
// whole function on a single rejected word, an entire function disappeared over
// one data word. That is how OpenBIOS lost four functions (its kernel .data
// pointer table behind 0xBFC22674 and the rodata blob behind osDbgPrintf at
// 0xBFC095F8).
//
// The R3000A implements MIPS I only. The normative PSX hardware reference
// (psx-spx, "CPU Opcode Encoding -> Illegal Opcodes") states the rule this
// header encodes: "All opcodes that are marked as N/A in the Primary and
// Secondary opcode tables are causing a Reserved Instruction Exception
// (excode=0Ah)." So the architecturally faithful translation of such a word is
// the Reserved Instruction raise the hardware performs — never a no-op (that
// would be a stub) and never a reason to drop the function.
//
// This file deliberately does NOT model the coprocessor *sub*-encodings (COP0
// TLB ops, COP2 register moves, BC0F/BC0T): those belong to instruction
// families the translator already handles explicitly, and their own fail-loud
// paths are unchanged.

#pragma once

#include <cstdint>

namespace PSXRecomp {

enum class R3000aEncoding : uint8_t {
    Valid = 0,               // a defined R3000A instruction
    ReservedInstruction,     // N/A encoding        -> RI  exception (ExcCode 0x0A)
    CoprocessorUnusable,     // absent COP1/COP3/... -> CpU exception (ExcCode 0x0B)
};

// Classify a raw instruction word against the R3000A's encoding space.
inline R3000aEncoding classify_r3000a_encoding(uint32_t raw) {
    const uint32_t op = (raw >> 26) & 0x3Fu;

    switch (op) {
        // Primary opcode table, N/A entries (psx-spx).
        case 0x14: case 0x15: case 0x16: case 0x17:  // BEQL/BNEL/BLEZL/BGTZL (MIPS II)
        case 0x18: case 0x19: case 0x1A: case 0x1B:  // DADDI/DADDIU/LDL/LDR (MIPS III)
        case 0x1C: case 0x1D: case 0x1E: case 0x1F:  // SPECIAL2 / unused / 64-bit / SPECIAL3
        case 0x27:                                   // LWU (MIPS III)
        case 0x2C: case 0x2D:                        // SDL/SDR (MIPS III)
        case 0x2F:                                   // CACHE (MIPS III)
        case 0x34: case 0x35: case 0x36: case 0x37:  // unused
        case 0x3C: case 0x3D: case 0x3E: case 0x3F:  // unused
            return R3000aEncoding::ReservedInstruction;

        // Coprocessors the PSX does not implement -> Coprocessor Unusable.
        case 0x11: case 0x13:                        // COP1 / COP3
        case 0x30: case 0x31: case 0x33:             // LWC0 / LWC1 / LWC3
        case 0x38: case 0x39: case 0x3B:             // SWC0 / SWC1 / SWC3
            return R3000aEncoding::CoprocessorUnusable;

        default:
            break;
    }

    if (op == 0x00) {
        const uint32_t funct = raw & 0x3Fu;
        switch (funct) {
            // Secondary opcode table, N/A entries (when primary opcode == 0).
            case 0x01: case 0x05:                        // unused shifts
            case 0x0A: case 0x0B:                        // MOVZ/MOVN (MIPS IV)
            case 0x0E: case 0x0F:                        // unused
            case 0x14: case 0x15: case 0x16: case 0x17:  // 64-bit (MIPS III)
            case 0x1C: case 0x1D: case 0x1E: case 0x1F:  // 64-bit (MIPS III)
            case 0x28: case 0x29:                        // unused
            case 0x2C: case 0x2D: case 0x2E: case 0x2F:  // 64-bit / unused
            case 0x30: case 0x31: case 0x32: case 0x33:  // unused
            case 0x34: case 0x35: case 0x36: case 0x37:  // TGE/TGEU/TLT/TLTU/TEQ/TNE
            case 0x38: case 0x39: case 0x3A: case 0x3B:  // DSLL/DSRL/DSRA (MIPS III)
            case 0x3C: case 0x3D: case 0x3E: case 0x3F:  // DSLL32/DSRL32/DSRA32 (MIPS III)
                return R3000aEncoding::ReservedInstruction;
            default:
                break;
        }
    }

    return R3000aEncoding::Valid;
}

// Convenience predicate: true for any encoding the R3000A cannot execute.
inline bool is_r3000a_reserved_instruction(uint32_t raw) {
    return classify_r3000a_encoding(raw) != R3000aEncoding::Valid;
}

} // namespace PSXRecomp
