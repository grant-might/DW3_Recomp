#ifndef GTE_H
#define GTE_H

/* The GTE instructions that C can't say, as the SDK's inline_c.h has them
   but with the real encodings in place of the ones DMPSX patches. The SDK's
   gtemac.h builds its larger macros (gte_MulMatrix0, gte_CompMatrix...)
   from these. */

/* the interpolation factor (IR0) */
#define gte_lddp(r0) __asm__ volatile(                                      \
    "mtc2 %0, $8"                                                           \
    :                                                                       \
    : "r"(r0))

/* an SVECTOR into IR1-IR3 */
#define gte_ldsv(r0) __asm__ volatile(                                      \
    "lhu $12, 0(%0);"                                                       \
    "lhu $13, 2(%0);"                                                       \
    "lhu $14, 4(%0);"                                                       \
    "mtc2 $12, $9;"                                                         \
    "mtc2 $13, $10;"                                                        \
    "mtc2 $14, $11"                                                         \
    :                                                                       \
    : "r"(r0)                                                               \
    : "$12", "$13", "$14")

/* IR1-IR3 = IR0 * IR1-IR3 >> 12 */
#define gte_gpf12() __asm__ volatile(                                       \
    "nop;"                                                                  \
    "nop;"                                                                  \
    ".word 0x4B98003D")

/* IR1-IR3 into an SVECTOR */
#define gte_stsv(r0) __asm__ volatile(                                      \
    "mfc2 $12, $9;"                                                         \
    "mfc2 $13, $10;"                                                        \
    "mfc2 $14, $11;"                                                        \
    "sh $12, 0(%0);"                                                        \
    "sh $13, 2(%0);"                                                        \
    "sh $14, 4(%0)"                                                         \
    :                                                                       \
    : "r"(r0)                                                               \
    : "$12", "$13", "$14", "memory")

/* a MATRIX's rotation into the GTE (RT) */
#define gte_SetRotMatrix(r0) __asm__ volatile(                              \
    "lw $12, 0(%0);"                                                        \
    "lw $13, 4(%0);"                                                        \
    "ctc2 $12, $0;"                                                         \
    "ctc2 $13, $1;"                                                         \
    "lw $12, 8(%0);"                                                        \
    "lw $13, 12(%0);"                                                       \
    "lw $14, 16(%0);"                                                       \
    "ctc2 $12, $2;"                                                         \
    "ctc2 $13, $3;"                                                         \
    "ctc2 $14, $4"                                                          \
    :                                                                       \
    : "r"(r0)                                                               \
    : "$12", "$13", "$14")

/* an SVECTOR into V0 */
#define gte_ldv0(r0) __asm__ volatile(                                      \
    "lwc2 $0, 0(%0);"                                                       \
    "lwc2 $1, 4(%0)"                                                        \
    :                                                                       \
    : "r"(r0))

/* an SVECTOR that may not be word aligned into V0 */
#define gte_ldv0_unaligned(r0) __asm__ volatile(                            \
    "lwl $12, 3(%0);"                                                       \
    "lwr $12, 0(%0);"                                                       \
    "lhu $13, 4(%0);"                                                       \
    "mtc2 $12, $0;"                                                         \
    "mtc2 $13, $1"                                                          \
    :                                                                       \
    : "r"(r0)                                                               \
    : "$12", "$13")

/* V0 through RT and TR, then the perspective (SXY2, SZ3) */
#define gte_rtps() __asm__ volatile(                                        \
    "nop;"                                                                  \
    "nop;"                                                                  \
    ".word 0x4A180001")

/* MAC1-MAC3 = RT * V0 >> 12 */
#define gte_rtv0() __asm__ volatile(                                        \
    "nop;"                                                                  \
    "nop;"                                                                  \
    ".word 0x4A486012")

/* SXY2 into a word */
#define gte_stsxy(r0) __asm__ volatile(                                     \
    "swc2 $14, 0(%0)"                                                       \
    :                                                                       \
    : "r"(r0)                                                               \
    : "memory")

/* the flags into a word */
#define gte_stflg(r0) __asm__ volatile(                                     \
    "cfc2 $12, $31;"                                                        \
    "nop;"                                                                  \
    "sw $12, 0(%0)"                                                         \
    :                                                                       \
    : "r"(r0)                                                               \
    : "$12", "memory")

/* MAC1-MAC3 into a VECTOR */
#define gte_stlvnl(r0) __asm__ volatile(                                    \
    "swc2 $25, 0(%0);"                                                      \
    "swc2 $26, 4(%0);"                                                      \
    "swc2 $27, 8(%0)"                                                       \
    :                                                                       \
    : "r"(r0)                                                               \
    : "memory")

/* SZ3 / 4 into a word, the depth in the ordering table */
#define gte_stszotz(r0) __asm__ volatile(                                   \
    "mfc2 $12, $19;"                                                        \
    "nop;"                                                                  \
    "sra $12, $12, 2;"                                                      \
    "sw $12, 0(%0)"                                                         \
    :                                                                       \
    : "r"(r0)                                                               \
    : "$12", "memory")

/* a MATRIX's translation into the GTE (TR) */
#define gte_SetTransMatrix(r0) __asm__ volatile(                            \
    "lw $12, 20(%0);"                                                       \
    "lw $13, 24(%0);"                                                       \
    "ctc2 $12, $5;"                                                         \
    "lw $14, 28(%0);"                                                       \
    "ctc2 $13, $6;"                                                         \
    "ctc2 $14, $7"                                                          \
    :                                                                       \
    : "r"(r0)                                                               \
    : "$12", "$13", "$14")

/* a VECTOR's low halves into V0 */
#define gte_ldlv0(r0) __asm__ volatile(                                     \
    "lhu $13, 4(%0);"                                                       \
    "lhu $12, 0(%0);"                                                       \
    "sll $13, $13, 16;"                                                     \
    "or $12, $12, $13;"                                                     \
    "mtc2 $12, $0;"                                                         \
    "lwc2 $1, 8(%0)"                                                        \
    :                                                                       \
    : "r"(r0)                                                               \
    : "$12", "$13")

/* a MATRIX's column into IR1-IR3 */
#define gte_ldclmv(r0) __asm__ volatile(                                    \
    "lhu $12, 0(%0);"                                                       \
    "lhu $13, 6(%0);"                                                       \
    "lhu $14, 12(%0);"                                                      \
    "mtc2 $12, $9;"                                                         \
    "mtc2 $13, $10;"                                                        \
    "mtc2 $14, $11"                                                         \
    :                                                                       \
    : "r"(r0)                                                               \
    : "$12", "$13", "$14")

/* MAC1-MAC3 = RT * V0 + TR */
#define gte_rt() __asm__ volatile(                                          \
    "nop;"                                                                  \
    "nop;"                                                                  \
    ".word 0x4A480012")

/* MAC1-MAC3 = RT * IR1-IR3 >> 12 */
#define gte_rtir() __asm__ volatile(                                        \
    "nop;"                                                                  \
    "nop;"                                                                  \
    ".word 0x4A49E012")

/* IR1-IR3 into a MATRIX's column */
#define gte_stclmv(r0) __asm__ volatile(                                    \
    "mfc2 $12, $9;"                                                         \
    "mfc2 $13, $10;"                                                        \
    "mfc2 $14, $11;"                                                        \
    "sh $12, 0(%0);"                                                        \
    "sh $13, 6(%0);"                                                        \
    "sh $14, 12(%0)"                                                        \
    :                                                                       \
    : "r"(r0)                                                               \
    : "$12", "$13", "$14", "memory")

/* a MATRIX's rotation into the GTE's light matrix (L) */
#define gte_SetLightMatrix(r0) __asm__ volatile(                            \
    "lw $12, 0(%0);"                                                        \
    "lw $13, 4(%0);"                                                        \
    "ctc2 $12, $8;"                                                         \
    "ctc2 $13, $9;"                                                         \
    "lw $12, 8(%0);"                                                        \
    "lw $13, 12(%0);"                                                       \
    "lw $14, 16(%0);"                                                       \
    "ctc2 $12, $10;"                                                        \
    "ctc2 $13, $11;"                                                        \
    "ctc2 $14, $12"                                                         \
    :                                                                       \
    : "r"(r0)                                                               \
    : "$12", "$13", "$14")

/* the color of V0 as a normal under the lights (RGB2) */
#define gte_ncs() __asm__ volatile(                                         \
    "nop;"                                                                  \
    "nop;"                                                                  \
    ".word 0x4AC8041E")

/* RGB2 into a word */
#define gte_strgb(r0) __asm__ volatile(                                     \
    "swc2 $22, 0(%0)"                                                       \
    :                                                                       \
    : "r"(r0)                                                               \
    : "memory")

/* three screen points (SXY0-SXY2) */
#define gte_ldsxy3(r0, r1, r2) __asm__ volatile(                            \
    "mtc2 %0, $12;"                                                         \
    "mtc2 %2, $14;"                                                         \
    "mtc2 %1, $13"                                                          \
    :                                                                       \
    : "r"(r0), "r"(r1), "r"(r2))

/* MAC0 = the winding of SXY0-SXY2, which side of a polygon faces us */
#define gte_nclip() __asm__ volatile(                                       \
    "nop;"                                                                  \
    "nop;"                                                                  \
    ".word 0x4B400006")

/* MAC0 into a word */
#define gte_stopz(r0) __asm__ volatile(                                     \
    "swc2 $24, 0(%0)"                                                       \
    :                                                                       \
    : "r"(r0)                                                               \
    : "memory")

/* three depths (SZ1-SZ3) */
#define gte_ldsz3(r0, r1, r2) __asm__ volatile(                             \
    "mtc2 %0, $17;"                                                         \
    "mtc2 %1, $18;"                                                         \
    "mtc2 %2, $19"                                                          \
    :                                                                       \
    : "r"(r0), "r"(r1), "r"(r2))

/* four depths (SZ0-SZ3) */
#define gte_ldsz4(r0, r1, r2, r3) __asm__ volatile(                         \
    "mtc2 %0, $16;"                                                         \
    "mtc2 %1, $17;"                                                         \
    "mtc2 %2, $18;"                                                         \
    "mtc2 %3, $19"                                                          \
    :                                                                       \
    : "r"(r0), "r"(r1), "r"(r2), "r"(r3))

/* OTZ = the average of SZ1-SZ3 */
#define gte_avsz3() __asm__ volatile(                                       \
    "nop;"                                                                  \
    "nop;"                                                                  \
    ".word 0x4B58002D")

/* OTZ = the average of SZ0-SZ3 */
#define gte_avsz4() __asm__ volatile(                                       \
    "nop;"                                                                  \
    "nop;"                                                                  \
    ".word 0x4B68002E")

/* OTZ into a word */
#define gte_stotz(r0) __asm__ volatile(                                     \
    "swc2 $7, 0(%0)"                                                        \
    :                                                                       \
    : "r"(r0)                                                               \
    : "memory")

#include <gtemac.h>

#endif /* GTE_H */
