#pragma once
#include <cstdint>
#include "gp2cc.h"   // gp2cc_ccgeo input struct

// ---------------------------------------------------------------------------
// gp2ccline — GP2's racing-line (cc-line) solver, ported to C++ and fed by the
// formula-generated cosine/atan tables (gp2cos.hpp / gp2atan.hpp).
//
// This is the CHAOTIC kernel: a reproject/nudge feedback loop whose output
// diverges on any rounding difference. It therefore reproduces the engine's
// fixed-point math exactly, rather than re-deriving it — kept legible with
// comments and meaningful names, but every integer operation is load-bearing
// and must not be "simplified". Validated bit-exact against the GP2Lap dumps.
// ---------------------------------------------------------------------------
namespace gp2geom {

// Run the cc-line pass. Writes the per-segment racing-line offset (bestLine) and
// heading delta (angle18); cmdStartSeg/pNumCmds (may be null) record each
// cc-command's first segment. Returns 0 on success.
int computeCcLine(const gp2cc_ccgeo& g, const std::uint8_t* cc, int cc_len, int cc_off,
                  std::int16_t* bestLine, std::int16_t* angle18,
                  int* cmdStartSeg, int* pNumCmds);

} // namespace gp2geom
