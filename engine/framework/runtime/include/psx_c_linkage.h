#ifndef PSX_C_LINKAGE_H
#define PSX_C_LINKAGE_H
/* Globals DEFINED in C translation units and referenced from C++.
 *
 * Declared here at namespace scope with C linkage.  MSVC rejects a linkage
 * specification inside a function body (C2598), and without C linkage the C++
 * side mangles the name while the definition keeps C linkage, so the link
 * fails with unresolved externals.  Keep this list in step with the C units;
 * every entry must have a real file-scope definition in a .c file.
 */
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern int      g_audio_unmute_resync;
extern int      g_idle_skip_enabled;
extern int      g_psx_cps_mode;
extern int      g_psx_dispatch_depth;
extern int      g_call_unit_depth;
extern int      psx_in_device_service;

extern uint32_t g_overlay_region_floor;
extern uint32_t g_debug_current_func_addr;
extern uint32_t g_debug_last_store_pc;
extern uint32_t g_slice_exit_pc;
extern uint32_t g_slice_exit_reason;
extern uint32_t g_slice_exit_iter;
extern uint32_t g_slice_exit_dispatchable;
extern uint32_t g_slice_exit_dirty;
extern uint32_t g_slice_exit_in_text;
extern uint32_t g_slice_exit_want;
extern uint32_t i_stat;
extern uint32_t i_mask;

extern uint64_t g_vblank_raise_count;
extern uint64_t g_vblank_deliver_count;
extern uint64_t g_vblank_ack_count;
extern uint64_t g_guest_store_count;
extern uint64_t g_dirty_ram_blocks_run;
extern uint64_t g_dirty_ram_insns_run;
extern uint64_t g_dirty_window_dispatches;
extern uint64_t g_dirty_pump_count;
extern uint64_t g_slice_fired;
extern uint64_t g_slice_irq_taken;
extern uint64_t psx_cycle_count;
extern uint64_t s_frame_count;

#ifdef __cplusplus
}
#endif
#endif /* PSX_C_LINKAGE_H */
