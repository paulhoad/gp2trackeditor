#include "gp2ccline.hpp"
#include "gp2cos.hpp"    // gp2geom::kCosine — the verified cosine table
#include "gp2atan.hpp"   // gp2geom::kArctan — the verified arctangent table
#include <cstring>

namespace gp2geom {
namespace {

// Short accessors for the two trig tables used throughout the math below.
inline int COS (int i)  { return kCosine[i]; }   // cosine, amplitude 0x4000
inline int ATAN(int i)  { return kArctan[i]; }    // arctangent, 0x10000 per turn

#define ONE60  (((int64_t)1) << 60)

// Converts a segment's theta-prime tilt back into a binary-radian heading
// offset: this is round(2^16 / pi), the inverse of the (pi/2)-scaled tilt that
// the geometry pass stores in f14.
constexpr int kInvPiQ16 = 20861;      // round(65536 / pi)

inline int s16(int v)   { v &= 0xFFFF; return (v & 0x8000) ? v - 0x10000 : v; }
inline int abs16(int v) { v = s16(v); return v < 0 ? -v : v; }
inline int rdw(const std::uint8_t* d, int o) { return d[o] | (d[o + 1] << 8); }

// Interpolated cosine, amplitude 0x4000 (8-unit grid lookup + linear lerp).
inline int gv(int ax) {
    ax = s16(ax);
    if (ax < 0) ax = (-ax) & 0xFFFF;
    int frac = ax & 7;
    int widx = ((ax >> 2) & 0xFFFE) >> 1;
    int base = COS(widx), nxt = COS(widx + 1);
    int d = s16(nxt - base);
    return s16(base + ((d * frac) >> 3));
}

// 32-bit integer square root: a FIXED 3-iteration 16-bit Newton step. This is
// deliberately NOT a true floor-sqrt — the fixed iteration count reproduces the
// engine's exact (slightly-off) result, which the chaotic loop below depends on.
std::uint32_t sqrt32(std::uint32_t V) {
    if (V == 0) return 0;
    int shift = 0;
    while (V < 0x40000000u) { shift++; V <<= 2; if (V == 0) return 0; }
    std::uint32_t half = V >> 1;
    std::uint32_t di = ((V >> 17) + 0x8000) & 0xFFFF;
    for (int it = 0; it < 3; it++) {
        std::uint32_t hi = half >> 16, q;
        if (hi >= di) q = half & 0xFFFF;
        else          q = half / di;
        di = ((di >> 1) + q) & 0xFFFF;
    }
    return (di >> shift) & 0xFFFF;
}

// 64-bit integer square root: a FIXED 5-iteration Newton step (same caveat as
// sqrt32 — the iteration count is part of the result, not an approximation).
std::uint32_t sqrt64(std::uint64_t V) {
    if (V == 0) return 0;
    if ((std::uint32_t)(V >> 32) == 0) return sqrt32((std::uint32_t)V);
    int shift = 0;
    while ((V >> 32) < 0x40000000ull) { shift++; V <<= 2; }
    std::uint64_t half = V >> 1;
    std::uint32_t est = (std::uint32_t)((half >> 32) + 0x80000000ull);
    for (int it = 0; it < 5; it++) {
        std::uint32_t q = (std::uint32_t)(half / est);
        est = (est >> 1) + q;
    }
    return est >> shift;
}

// cos(|angle|) in Q30 (amplitude 0x40000000) from the cosine table + interpolation.
std::int32_t cos30(int angle) {
    int a = abs16(angle);
    int widx = a >> 3, frac = a & 7;
    int base  = COS(widx);
    int delta = COS(widx + 1) - COS(widx);
    return ((std::int32_t)base << 16) + ((std::int32_t)(delta * frac) << 13);
}

// Q30 cosine and sine of an angle. To keep precision, the larger-magnitude of
// the two is recovered from the smaller via sqrt(1 - x^2) (computed in Q60),
// and the smaller-magnitude one is read straight from the table.
void q30_cossin(int angle, std::int32_t* pcos, std::int32_t* psin) {
    int a = s16(angle);
    int coarse_sin = COS(abs16(s16(0x4000 - a)) >> 3);
    int coarse_sin_abs = coarse_sin < 0 ? -coarse_sin : coarse_sin;
    std::int32_t c30, s30;
    if (coarse_sin_abs >= 0x2000) {
        c30 = cos30(a);
        std::int64_t smag = (std::int64_t)sqrt64((std::uint64_t)(ONE60 - (std::int64_t)c30 * c30));
        s30 = coarse_sin < 0 ? (std::int32_t)(-smag) : (std::int32_t)smag;
    } else {
        s30 = cos30(s16(0x4000 - a));
        std::int64_t cmag = (std::int64_t)sqrt64((std::uint64_t)(ONE60 - (std::int64_t)s30 * s30));
        int coarse_cos = COS(abs16(a) >> 3);
        c30 = coarse_cos < 0 ? (std::int32_t)(-cmag) : (std::int32_t)cmag;
    }
    *pcos = c30; *psin = s30;
}

// atan2(y, x) in binary radians (0x10000 = a full turn), 16-bit arguments.
int atan2u(int y, int x) {
    y = s16(y); x = s16(x);
    int dy = y < 0 ? -y : y;
    int dx = x < 0 ? -x : x;
    int signs_differ = (y < 0) != (x < 0);
    int a;
    if (dy >= dx) {
        if (dy == 0) return 0;
        int idx = (dx << 11) / dy;
        a = 0x4000 - ATAN(idx);
    } else {
        int idx = (dy << 11) / dx;
        a = ATAN(idx);
    }
    if (signs_differ) a = -a;
    if (x < 0) a += 0x8000;
    return s16(a);
}

// Build the world arc-centre point W for the first segment of a command.
void build_centre(const gp2cc_ccgeo* g, int idx, int P, int H,
                  std::int32_t arg2, int arg1mul4, std::int32_t* pWx, std::int32_t* pWy) {
    int seg = idx;
    int f14 = (int)g->f14[seg];
    int latP = (int)(((std::int32_t)P * f14) >> 15);
    int ang  = s16(g->segAngle[seg]);
    int cosA = gv(ang);
    int sinA = gv(s16(0x4000 - ang));
    std::int32_t RX = (std::int16_t)(((std::int32_t)P * cosA + (std::int32_t)latP * sinA) >> 14);
    std::int32_t RY = (std::int16_t)(((std::int32_t)latP * cosA - (std::int32_t)P * sinA) >> 14);
    RX += g->cx8[seg];
    RY += g->cy8[seg];
    int sinH = gv(s16(0x4000 - H));
    int cosH = gv(s16(H));
    std::int16_t a1m4 = (std::int16_t)arg1mul4;
    RX += (std::int16_t)(((std::int32_t)sinH * a1m4) >> 14);
    RY += (std::int16_t)(((std::int32_t)cosH * a1m4) >> 14);
    std::int32_t Wx = RX, Wy = RY;
    if (arg2 != 0) {
        std::int32_t mag = arg2 < 0 ? -arg2 : arg2;
        int perp = (arg2 < 0) ? s16(H - 0x4000) : s16(H + 0x4000);
        std::int32_t c32, s32; q30_cossin(perp, &c32, &s32);
        Wx += (std::int32_t)(((std::int64_t)s32 * mag) >> 30);
        Wy += (std::int32_t)(((std::int64_t)c32 * mag) >> 30);
    }
    *pWx = Wx; *pWy = Wy;
}

// Reproject the carried world point W onto segment `idx`, producing the lateral
// offset P (and, on curved commands, the updated heading H).
void reproject(const gp2cc_ccgeo* g, int idx, std::int32_t Wx, std::int32_t Wy,
               std::int32_t arg2, int* pP, int* pH) {
    int seg = idx;
    int f14 = (int)g->f14[seg];
    int ang = s16(g->segAngle[seg]);
    int thetaP = s16(ang - (int)((((std::int32_t)(f14 >> 1) * kInvPiQ16) << 1) >> 16));
    std::int32_t relx = Wx - g->cx8[seg];
    std::int32_t rely = Wy - g->cy8[seg];
    std::int32_t c32, s32; q30_cossin(thetaP, &c32, &s32);
    std::int64_t lat64 = (std::int64_t)c32 * relx - (std::int64_t)s32 * rely;
    std::int64_t lon64 = (std::int64_t)s32 * relx + (std::int64_t)c32 * rely;
    std::int32_t lat = (std::int32_t)(lat64 >> 30);
    std::int32_t lon = (std::int32_t)(lon64 >> 30);
    if (arg2 == 0) {
        int d = s16(*pH - thetaP);
        std::int32_t sn = cos30(s16(0x4000 - d));
        std::int32_t cs = cos30(d);
        std::int32_t tanterm = 0;
        if (cs != 0) tanterm = (std::int32_t)(((std::int64_t)sn * lon) / cs);
        *pP = s16(lat - tanterm);
        return;
    }
    std::int64_t a2 = arg2;
    std::int64_t disc = a2 * a2 - (std::int64_t)lon * lon;
    std::int64_t T = (std::int64_t)sqrt64((std::uint64_t)disc);
    std::int64_t Tsigned = (arg2 < 0) ? -T : T;
    *pP = s16((std::int32_t)(lat - (std::int32_t)Tsigned));
    std::int32_t Ts = (std::int32_t)Tsigned, lonv = lon;
    std::uint32_t aa = (std::uint32_t)(arg2 < 0 ? -arg2 : arg2);
    while (((aa >> 16) > 0) || ((aa >> 16) == 0 && (aa & 0xFFFF) >= 0x7F00)) {
        Ts >>= 1; lonv >>= 1; aa >>= 1;
    }
    int at = atan2u((int)(std::int16_t)(Ts & 0xFFFF), (int)(std::int16_t)(lonv & 0xFFFF));
    at = (arg2 < 0) ? s16(at + 0x4000) : s16(at - 0x4000);
    *pH = s16(at + thetaP);
}

// Per-segment nudge of the world point W along a curvature ramp (commands whose
// radius changes from segment to segment, i.e. a non-zero slope).
void nudge(std::int32_t* pWx, std::int32_t* pWy, int H, std::int32_t arg2, std::int32_t slope) {
    std::int32_t step = slope;
    int a;
    if (arg2 < 0) { step = -step; a = s16(H - 0x4000); }
    else            a = s16(H + 0x4000);
    int sinA = gv(s16(0x4000 - a));
    int cosA = gv(s16(a));
    *pWx += (std::int32_t)(((std::int64_t)sinA * step) >> 14);
    *pWy += (std::int32_t)(((std::int64_t)cosA * step) >> 14);
}

// Parse one command from the cc-line command stream.
struct cc_cmd {
    int end, word, N, a1, a2;
    std::int32_t arg2, slope;
    int arg1mul4, arg1if2is0;
};
void parse_cmd(const std::uint8_t* cc, int* po, cc_cmd* c) {
    int o = *po;
    std::memset(c, 0, sizeof *c);
    int word = rdw(cc, o); o += 2;
    c->word = word;
    c->N = word & 0x7FF;
    if (word == 0) { c->end = 1; *po = o; return; }
    if (word & 0x8000) { c->a1 = rdw(cc, o); o += 2; c->a2 = rdw(cc, o); o += 2; }
    int cx = rdw(cc, o); o += 2;
    std::int32_t arg2 = (std::int32_t)(std::int16_t)rdw(cc, o); o += 2;
    if (word & 0x4000) { arg2 = (std::int32_t)((std::uint32_t)arg2 << 16) | rdw(cc, o); o += 2; }
    if (!(word & 0x1000)) arg2 <<= 3;
    c->arg2 = arg2;
    if (word & 0x2000) {
        std::int32_t arg3 = (std::int32_t)(std::int16_t)rdw(cc, o); o += 2;
        if (word & 0x4000) { arg3 = (std::int32_t)((std::uint32_t)arg3 << 16) | rdw(cc, o); o += 2; }
        if (!(word & 0x1000)) arg3 <<= 3;
        c->slope = (arg3 - arg2) / c->N;
    }
    if (arg2 != 0) { c->arg1if2is0 = 0; c->arg1mul4 = (std::int16_t)(cx << 2); }
    else           { c->arg1mul4 = 0;   c->arg1if2is0 = cx; }
    *po = o;
}

} // anonymous namespace

// Walk the cc-line command stream: seed the lateral offset P and heading H, then
// for each segment store its racing-line offset (bestLine) and heading delta
// (angle18) before reprojecting the carried world point onto the next segment,
// nudging it along on curvature ramps.
int computeCcLine(const gp2cc_ccgeo& gref, const std::uint8_t* cc, int cc_len, int cc_off,
                  std::int16_t* bestLine, std::int16_t* angle18,
                  int* cmdStartSeg, int* pNumCmds) {
    const gp2cc_ccgeo* g = &gref;
    int n = g->n;
    if (n <= 0) return -1;
    int cur = 0, cmdIdx = 0;
    int H = s16(g->segAngle[0]);
    int P = 0;
    int w0 = rdw(cc, cc_off);
    // the initial lateral offset P is a SIGNED 16-bit word; sign-extend it.
    if (!(w0 & 0x800) && (w0 & 0x8000)) P = s16(rdw(cc, cc_off + 2));
    int o = cc_off;
    for (;;) {
        cc_cmd c;
        parse_cmd(cc, &o, &c);
        if (c.end) break;
        if (o > cc_len) return -2;
        if ((c.word & 0x800) && (c.word & 0x8000)) { P = s16(c.a1); H = s16(c.a2); }
        if (c.arg2 == 0) H = s16(H + s16(c.arg1if2is0));
        if (cmdStartSeg) cmdStartSeg[cmdIdx] = cur;
        cmdIdx++;
        std::int32_t Wx, Wy;
        build_centre(g, cur, P, H, c.arg2, c.arg1mul4, &Wx, &Wy);
        std::int32_t arg2 = c.arg2;
        int left = c.N;
        while (left > 0) {
            angle18[cur]  = (std::int16_t)s16(H - s16(g->segAngle[cur]));
            bestLine[cur] = (std::int16_t)s16(P);
            cur = (cur + 1) % n;
            reproject(g, cur, Wx, Wy, arg2, &P, &H);
            left--;
            if (left == 0) break;
            if (arg2 != 0 && c.slope != 0) {
                arg2 += c.slope;
                nudge(&Wx, &Wy, H, arg2, c.slope);
            }
        }
    }
    if (pNumCmds) *pNumCmds = cmdIdx;
    return 0;
}

} // namespace gp2geom
