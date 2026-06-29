#pragma once
#include <array>
#include <cstdint>

// ---------------------------------------------------------------------------
// GP2 geometry — the arctangent table expressed as its generating formula.
//
// GP2's t_ArithTab1 maps a ratio i/2048 (i = 0..2048, so the ratio runs 0..1)
// to an angle in binary-radian units (65536 per full turn). It is exactly:
//
//       ATAN1[i] = round( atan(i / 2048) * 65536 / (2*pi) )
//
// Unlike the cosine table, this one is CLEAN: every real entry matches the
// formula with no hand-tweaked exceptions (verified 2049/2049). Index 2049 is
// unused padding (0). It is also far less rounding-fragile than COS — only a
// single entry sits within 1e-3 of a .5 boundary, and it rounds correctly.
//
// So this is the poster child for replacing an opaque blob with a formula:
// one line of intent, no override.
//
// WARNING: the entries come from compile-time floating-point rounding, so a
// different compiler or standard library could round an entry that lands on a
// .5 boundary the other way. This table is far less fragile than the cosine one
// (only a single entry is anywhere near a boundary, and it rounds correctly),
// but if the compiled track or cc-line ever start to DRIFT, a rounding
// disagreement here — or in gp2cos.hpp — is the first thing to suspect:
// regenerate the table and diff it against GP2's t_ArithTab1 bytes.
// ---------------------------------------------------------------------------

namespace gp2geom {

inline constexpr int    kAtanGrid = 2050;   // 0..2048 real, 2049 = padding
inline constexpr int    kAtanReal = 2048;   // last real index
inline constexpr double kPiAtan   = 3.14159265358979323846;

// constexpr sqrt (Newton) and atan, since neither is portably constexpr before
// C++26. atan is range-reduced toward 0 via atan(x)=2*atan(x/(1+sqrt(1+x^2)))
// then a Taylor series on the small remainder; accurate to ~1e-15 on [0,1].
constexpr double atanSqrt(double v) {
    if (v <= 0.0) return 0.0;
    double g = v;
    for (int i = 0; i < 80; ++i) g = 0.5 * (g + v / g);
    return g;
}
constexpr double arctan(double x) {
    int k = 0;
    while (x > 0.05) { x = x / (1.0 + atanSqrt(1.0 + x * x)); ++k; }
    double sum = 0.0, t = x, x2 = x * x;
    for (int n = 0; n < 12; ++n) { sum += t / (2 * n + 1); t *= -x2; }
    for (int i = 0; i < k; ++i) sum *= 2.0;
    return sum;
}

constexpr short roundToShortA(double v) {
    long r = (v >= 0.0) ? static_cast<long>(v + 0.5) : static_cast<long>(v - 0.5);
    return static_cast<short>(r);
}

constexpr std::array<short, kAtanGrid> makeAtanTable() {
    std::array<short, kAtanGrid> t{};
    for (int i = 0; i <= kAtanReal; ++i)
        t[i] = roundToShortA(arctan(static_cast<double>(i) / 2048.0)
                             * 65536.0 / (2.0 * kPiAtan));
    t[2049] = 0;   // unused padding, matches GP2
    return t;
}

inline constexpr std::array<short, kAtanGrid> kArctan = makeAtanTable();

// Cheap spot-checks of the two exact endpoints (no embedded reference table).
static_assert(kArctan[0]    ==    0, "atan(0)");
static_assert(kArctan[2048] == 8192, "atan(1) = pi/4 = 0x2000");

} // namespace gp2geom
