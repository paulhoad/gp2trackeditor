/*
 * gp2cc.h — the shared compiled-track data format.
 *
 * These two structs are the bit-exact output of GP2's track-geometry +
 * cc-line (racing-line) compilation: gp2cc_track is filled by
 * gp2geom::compileGeometry, and gp2cc_ccgeo feeds gp2ccline::computeCcLine.
 * The editor's overlay just draws the arrays — it never touches the fixed
 * point. Field offsets/units reference the original engine layout (tseg+N).
 */
#ifndef GP2CC_H
#define GP2CC_H

#include <stdint.h>

#define GP2CC_MAXSEG 4096

typedef struct {
    int     n;                  /* number of track segments */
    int32_t fAngle[GP2CC_MAXSEG];   /* d_TrkStrtAngle per seg, Q16 (full precision) */
    int16_t angle[GP2CC_MAXSEG];    /* heading = fAngle>>16 (tseg+0)   */
    int16_t startAngle[GP2CC_MAXSEG]; /* d_StartAngle>>16 (tseg+0xA, side-vec) */
    int32_t f14[GP2CC_MAXSEG];      /* tseg+0x14 = (cmd d1 * 0x6488)>>14 (θ' tilt) */
    int32_t widthL[GP2CC_MAXSEG];   /* tseg+0x60 left road width (w_TrkBeginWidth)  */
    int32_t widthR[GP2CC_MAXSEG];   /* tseg+0x62 right road width (w_TrkBeginWidth2) */
    /* side vectors (tseg+0x4C/0x4E left, +0x0C/0x0E right): edge = centre ± side/64.
     * side = ((scos(startAngle)*width)>>17)<<6 components. (sideRX/RY validated == dump xSide/ySide.) */
    int16_t sideLX[GP2CC_MAXSEG], sideLY[GP2CC_MAXSEG];
    int16_t sideRX[GP2CC_MAXSEG], sideRY[GP2CC_MAXSEG];
    /* exact 1/8-world-unit position (the game's w_TrkStrtX/Y, warped) */
    int32_t X8[GP2CC_MAXSEG];   /* world X (= dump col4) * 8, warped */
    int32_t Y8[GP2CC_MAXSEG];   /* world Y (= dump col3) * 8, warped */
    int32_t rawX8[GP2CC_MAXSEG]; /* world X * 8, BEFORE warp (raw integration) */
    int32_t rawY8[GP2CC_MAXSEG]; /* world Y * 8, BEFORE warp */
    int32_t gapX8, gapY8;        /* raw closure gap (rawEnd - start), 1/8 units */
    int     ccoff;              /* file offset of the cc-line command stream */
    /* header */
    int16_t hdr_angle, hdr_X, hdr_Y;
    uint16_t hdr_width, hdr_c5;
} gp2cc_track;

/* Per-segment geometry the cc-line compiler consumes. The cc-line internal
 * convention pairs C94BC with tseg+4 (=dump col3 =world Y) and C94C0 with tseg+8
 * (=dump col4 =world X), so cx8/cy8 follow that pairing (NOT the X8/Y8 naming). */
typedef struct {
    int     n;
    int16_t segAngle[GP2CC_MAXSEG];  /* tseg+0    heading                       */
    int32_t cx8[GP2CC_MAXSEG];       /* tseg+4 full 1/8-unit (C94BC-paired)      */
    int32_t cy8[GP2CC_MAXSEG];       /* tseg+8 full 1/8-unit (C94C0-paired)      */
    int32_t f14[GP2CC_MAXSEG];       /* tseg+0x14 (width-derived, drives θ' tilt) */
} gp2cc_ccgeo;

#endif
