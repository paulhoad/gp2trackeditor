#include "gp2geom.hpp"

// Cosine source: the build-time-verified formula table (gp2cos.hpp), generated
// at compile time and static_assert'd against GP2's bytes.
#include "gp2cos.hpp"

namespace gp2geom {
namespace {

// ============================== units & model ===============================
//
// ANGLES are binary radians: 65536 units per full turn, so a quarter turn is
// 0x4000. Wrapping is automatic in 16 bits and trig is a direct table lookup
// with no range reduction. The heading is integrated in Q16 fixed point — the
// whole-unit angle is the top 16 bits of a 32-bit accumulator.
constexpr int kQuarterTurn = 0x4000;   // 90 degrees
constexpr int kQ16         = 16;       // heading fractional bits

// Each segment is a FIXED-LENGTH straight chord. The engine steps 128 world
// units per segment; in its 1/8-world-unit grid that is 1024.
constexpr int kChordEighths = 0x400;   // 1024 = 128 world units, in 1/8 units
constexpr int kChordShift   = 14;      // chord = (cos * kChordEighths) >> 14

// The cc-line's per-segment "theta-prime" tilt (tseg+0x14) is the section's
// per-segment turn scaled by round(pi/2 * 2^14). Produced by the geometry pass,
// consumed later by the racing-line solver.
constexpr int kHalfPiQ14 = 0x6488;     // round( (pi/2) * 2^14 ) = 25736

// side vector = unit perpendicular to the start heading, scaled by road width;
// one component = ((trig * width) >> 17) << 6  (InitTrackSegs 0x796C3/0x79735).

// signed 16-bit wrap (binary-radian angles live in 16 bits).
constexpr int wrap16(int v) { v &= 0xFFFF; return (v & 0x8000) ? v - 0x10000 : v; }

// raw, grid-sampled cosine (amplitude 0x4000) of a binary-radian angle.
inline int rawCos(int a) {
    return scos(a);                                    // formula table (gp2cos.hpp)
}
inline int cosine(int angle) { return rawCos(angle); }
inline int sine  (int angle) { return rawCos(kQuarterTurn - angle); }  // sin(x) = cos(90-x)

// little-endian readers.
inline int     rdU16(const std::uint8_t* d, int o) { return d[o] | (d[o + 1] << 8); }
inline int     rdS16(const std::uint8_t* d, int o) { return wrap16(rdU16(d, o)); }
inline std::int32_t rdS32(const std::uint8_t* d, int o) {
    return (std::int32_t)(d[o] | (d[o+1] << 8) | (d[o+2] << 16) | (d[o+3] << 24));
}

// Track-command argument sizes (datparse). Opcode 0x45 is conditional on a c5
// bit and is handled separately; -1 means an unknown opcode (stream desync).
constexpr int kArgSize[0x66] = {
    2,2,2,0,0,4,0,0,2,2, 10,10,2,2,4,4,2,2,2,2, 2,2,0,0,2,2,4,0,0,0,
    0,0,0,0,0,0,0,0,4,4, 0,2,6,4,8,4,2,4,2,2, 2,2,4,4,2,2,26,2,4,8,
    14,4,4,4,2,2,4,4,6,14, 2,4,14,16,8,8,4,6,2,2, 4,0,2,2,0,2,0,0,0,2,
    2,0,2,4,6,6,2,0,0,0, 0,0 /* 0x64, 0x65 */
};

enum Side { Left, Right };

// ============================== the turtle ==================================
class TrackBuilder {
public:
    TrackBuilder(const std::uint8_t* dat, gp2cc_track& out) : d(dat), t(out) {}

    int run() {
        readHeader();
        if (!walkSections()) return -2;     // turn + half-step + width, per segment
        t.n = segCount - 1;                 // drop the over-produced wrap segment
        computeSideVectors();               // road-edge perpendiculars
        integratePositions();               // advance one fixed chord per segment
        distributeClosureGap();             // DDA warp so the lap closes exactly
        return 0;
    }

private:
    const std::uint8_t* d;
    gp2cc_track&        t;

    // turtle state during the section walk
    std::int32_t heading             = 0;   // d_TrkStrtAngle, Q16 binary radians
    std::int32_t sectionStartHeading = 0;   // d_StartAngle, the side-vector reference
    int          widthL = 0, widthR = 0;    // current road width (left/right halves)
    int          dWidthL = 0, dWidthR = 0;  // per-segment width ramp deltas
    int          rampL = 0, rampR = 0;      // segments left in each width transition
    bool         halfStepFull = false;      // c5 bit12: full-precision half-step
    bool         c5bit9       = false;      // c5 bit9: opcode-0x45 arg size
    int          segCount     = 0;
    int          headerEnd    = 0;

    // -- header: start pose, widths, flags; cursor lands at the command stream --
    void readHeader() {
        const int base = 0x1020 + rdS32(d, 0x1010);     // MainTrackData
        t.hdr_angle = (std::int16_t)rdS16(d, base + 0);
        t.hdr_Y     = (std::int16_t)rdS16(d, base + 4);
        t.hdr_X     = (std::int16_t)rdS16(d, base + 8);
        t.hdr_width = (std::uint16_t)rdU16(d, base + 0xA);
        t.hdr_c5    = (std::uint16_t)rdU16(d, base + 0xE);
        halfStepFull = (t.hdr_c5 & 0x1000) != 0;
        c5bit9       = (t.hdr_c5 & 0x0200) != 0;
        const int unknownCount = rdU16(d, base + 0x12);

        heading = (std::int32_t)((t.hdr_angle & 0xFFFF) << kQ16);
        sectionStartHeading = heading;
        widthL = widthR = (int)t.hdr_width;

        int o = base + 0x14;                            // skip the "4 unknowns" block
        for (int i = 0; i < unknownCount; ++i) { int ax = rdU16(d, o); o += 2; if (ax != 0x1F) o += 2; }
        headerEnd = o;
    }

    // -- walk the command stream: op commands tweak width; geometry commands are
    //    sections the turtle drives through. 0xFFFF ends it (cc-line stream next). --
    bool walkSections() {
        int o = headerEnd;
        for (;;) {
            const int w = rdU16(d, o);
            if (w == 0xFFFF) { t.ccoff = o + 2; return true; }
            if (w & 0x8000) { o = applyOpCommand(o, w); if (o < 0) return false; }
            else            { o = buildSection(o, w);   if (o < 0) return false; }
        }
    }

    int applyOpCommand(int o, int w) {
        const int op = (w >> 8) & 0x7F;
        const int sz = (op == 0x45) ? (c5bit9 ? 14 : 12)
                     : (op < 0x66)  ? kArgSize[op]
                                    : -1;
        if (sz < 0) return -1;                          // unknown opcode -> desync
        if (op == 0x34 || op == 0x05) setWidthTransition(Left,  rdS16(d, o + 2), rdS16(d, o + 4));
        if (op == 0x35 || op == 0x05) setWidthTransition(Right, rdS16(d, o + 2), rdS16(d, o + 4));
        return o + 2 + sz;
    }

    // -- one geometry command = one section of `segments` fixed chords, all
    //    turning by the same per-segment amount. --
    int buildSection(int o, int segments) {
        const int          perSegTurn = rdS16(d, o + 2);          // d1word
        const std::int32_t turn       = (std::int32_t)perSegTurn << kQ16;
        // theta'-tilt the cc-line will need later (computed from this command).
        const std::int32_t f14        = ((std::int32_t)perSegTurn * kHalfPiQ14) >> 14;
        o += 10;

        // MIDPOINT INTEGRATION RULE: the heading is sampled at each segment's
        // midpoint, so a section starts and ends with a HALF turn and takes full
        // turns in between. This centres each straight chord on the underlying arc
        // (no corner-cutting) and makes curvature continuous across section joins:
        // a section's end-half plus the next section's start-half = one full turn.
        std::int32_t halfTurn = turn >> 1;
        // bit12-clear tracks zero only the LOW 16 bits of the half-step (the engine's
        // `xor ax,ax`), i.e. they floor d1/2 to a whole binary-radian unit.
        if (!halfStepFull) halfTurn &= ~0xFFFF;

        for (int k = 0; k < segments; ++k) {
            if (segCount >= GP2CC_MAXSEG) return -1;
            if (k == 0) { sectionStartHeading = heading; heading += halfTurn; } // start half-step
            else        { heading += turn; sectionStartHeading += turn; }       // full turn
            emitSegment(f14);
        }
        heading += halfTurn;        // end half-step (pairs with the next start half)
        return o;
    }

    void emitSegment(std::int32_t f14) {
        const int i = segCount++;
        t.fAngle[i]     = heading;
        t.angle[i]      = (std::int16_t)(heading >> kQ16);
        t.startAngle[i] = (std::int16_t)(sectionStartHeading >> kQ16);
        t.f14[i]        = f14;
        t.widthL[i]     = widthL;   // store current width, THEN advance the ramp
        t.widthR[i]     = widthR;
        rampWidthOneStep();
    }

    // -- width transitions: len==0 sets it instantly, len>0 ramps linearly. --
    void setWidthTransition(Side s, int len, int target) {
        int& cur   = (s == Left) ? widthL  : widthR;
        int& delta = (s == Left) ? dWidthL : dWidthR;
        int& ramp  = (s == Left) ? rampL   : rampR;
        if (len == 0) { cur = target; delta = 0; ramp = 0; }
        else          { ramp = len; delta = (target - cur) / len; }  // truncating step
    }
    void rampWidthOneStep() {
        if (rampL > 0) { widthL += dWidthL; --rampL; }
        if (rampR > 0) { widthR += dWidthR; --rampR; }
    }

    // -- road edges = centre +/- sideVector; sideVector is perpendicular to the
    //    start heading, scaled by width. --
    void computeSideVectors() {
        const int n = t.n;
        for (int i = 0; i < n; ++i) {
            const int sa = wrap16(t.startAngle[i]);
            const int cs = cosine(sa), sn = sine(sa);
            t.sideRX[i] = component(cs, t.widthR[i]);
            t.sideRY[i] = component(sn, t.widthR[i]);
            t.sideLX[i] = component(cs, t.widthL[i]);
            t.sideLY[i] = component(sn, t.widthL[i]);
        }
    }
    static std::int16_t component(int trig, int width) {
        // ((trig*width) >> 16) keeps the perpendicular unit*width, >>1 halves it
        // and <<6 puts it in the engine's edge units. Folded through int16 like
        // the engine's word ops.
        return (std::int16_t)(((std::int16_t)((int)trig * width >> 16) >> 1) << 6);
    }

    // -- advance one fixed 128-unit chord per segment along the heading (forward
    //    Euler), accumulating the raw, not-yet-closed position. --
    void integratePositions() {
        std::int32_t x = (std::int32_t)t.hdr_X << 3;   // start position, 1/8 world units
        std::int32_t y = (std::int32_t)t.hdr_Y << 3;
        const int n = t.n;
        for (int i = 0; i < n; ++i) {
            const int h = t.fAngle[i] >> kQ16;
            t.rawX8[i] = x;  t.rawY8[i] = y;
            x += (cosine(h) * kChordEighths) >> kChordShift;   // X step = cos(h) * 128
            y += (sine(h)   * kChordEighths) >> kChordShift;   // Y step = sin(h) * 128
        }
        t.gapX8 = x - t.rawX8[0];   // raw integration doesn't close the loop exactly
        t.gapY8 = y - t.rawY8[0];
    }

    // -- close the loop: the chord rounding leaves a gap between end and start.
    //    Spread it backward with a DDA sweep so segment i carries
    //    sign(gap)*floor(i*|gap|/M), M = n+1 (the engine counts the wrap segment
    //    we dropped). First segment ~0, last ~the whole gap -> the lap closes. --
    void distributeClosureGap() {
        const int n = t.n;
        const std::int64_t M    = (std::int64_t)n + 1;
        const std::int64_t absX = t.gapX8 < 0 ? -(std::int64_t)t.gapX8 : t.gapX8;
        const std::int64_t absY = t.gapY8 < 0 ? -(std::int64_t)t.gapY8 : t.gapY8;
        for (int i = 0; i < n; ++i) {
            const std::int32_t sx = (std::int32_t)((absX * i) / M);
            const std::int32_t sy = (std::int32_t)((absY * i) / M);
            t.X8[i] = t.rawX8[i] - (t.gapX8 < 0 ? -sx : sx);
            t.Y8[i] = t.rawY8[i] - (t.gapY8 < 0 ? -sy : sy);
        }
    }
};

} // anonymous namespace

int compileGeometry(const std::uint8_t* dat, int len, gp2cc_track& out) {
    (void)len;
    TrackBuilder builder(dat, out);
    return builder.run();
}

// GP2's GetSinusVal (0x104B9): fold the sign, sample the 8-unit cosine grid, and
// linearly interpolate over the low 3 bits. Bit-identical to the old C gp2cc_gv.
int gv(int angle) {
    int ax = wrap16(angle);
    if (ax < 0) ax = (-ax) & 0xFFFF;
    int frac = ax & 7;
    int widx = ((ax >> 2) & 0xFFFE) >> 1;            // == ax >> 3
    int base = kCosine[widx];
    int d    = wrap16(kCosine[widx + 1] - base);
    return wrap16(base + ((d * frac) >> 3));
}

} // namespace gp2geom
