/*
https://github.com/heptagonhust/bicubic-image-resize/issues/9
*/

#ifndef SCALENX_H_
#define SCALENX_H_

#include <stdbool.h>
#include <math.h>
#define SCALENX_VERSION "1.0"

#ifdef SCALENX_STATIC
#define SCALENXAPI static
#else
#define SCALENXAPI extern
#endif

#ifdef __cplusplus
extern "C" {
#endif
SCALENXAPI void ScaleNX (unsigned char *src, int height, int width, int channels, int scale, unsigned char *res);
#ifdef __cplusplus
}
#endif

#ifdef SCALENX_IMPLEMENTATION

typedef struct
{
    unsigned char c[4];
}
pixel;

static pixel PixelCopy(pixel p, int channels)
{
    int d;
    pixel r;

    for (d = 0; d < channels; d++)
    {
        r.c[d] = p.c[d];
    }

    return r;
}

static bool PixelEql (pixel a, pixel b, int channels)
{
    int d;

    for (d = 0; d < channels; d++)
    {
        if (a.c[d] != b.c[d])
            return false;
    }

    return true;
}

static pixel PixelNear (pixel p0, pixel p1, pixel p2, int channels)
{
    int d, d0, d1, d2;
    pixel p;

    d0 = 0;
    d1 = 0;
    d2 = 0;
    for(d = 0; d < channels; d++)
    {
        d0 += (p1.c[d] > p2.c[d]) ? (p1.c[d] - p2.c[d]) : (p2.c[d] - p1.c[d]);
        d1 += (p0.c[d] > p1.c[d]) ? (p0.c[d] - p1.c[d]) : (p1.c[d] - p0.c[d]);
        d2 += (p0.c[d] > p2.c[d]) ? (p0.c[d] - p2.c[d]) : (p2.c[d] - p0.c[d]);
    }
    if (d2 < d1)
    {
        d1 = d2;
        p1 = p2;
    }
    p = (d1 < d0) ? p0 : p1;
    return p;
}

static pixel PixelGet (unsigned char *image, int height, int width, int channels, int y, int x)
{
    int d;
    pixel p;
    size_t k;

    y = (y < 0) ? 0 : (y < height) ? y : (height - 1);
    x = (x < 0) ? 0 : (x < width) ? x : (width - 1);
    k = ((width * y) + x) * channels;

    for (d = 0; d < channels; d++)
    {
        p.c[d] = image[k + d];
    }

    return p;
}

static void PixelSet (unsigned char *image, int height, int width, int channels, int y, int x, pixel p)
{
    int d;
    size_t k;

    y = (y < 0) ? 0 : (y < height) ? y : (height - 1);
    x = (x < 0) ? 0 : (x < width) ? x : (width - 1);
    k = ((width * y) + x) * channels;

    for (d = 0; d < channels; d++)
    {
        image[k + d] = p.c[d];
    }
}

static void ScaleN2X (unsigned char *src, int height, int width, int channels, unsigned char *res)
{
    int y, x, y2, x2, h2 = height * 2, w2 = width * 2;
    pixel pB, pD, pE, pF, pH;
    pixel pBH, pDF;
    pixel rA, rB, rC, rD;

    for (y = 0; y < height; y++)
    {
        y2 = y + y;
        for (x = 0; x < width; x++)
        {
            x2 = x + x;

            pB = PixelGet(src, height, width, channels, y - 1, x);
            pD = PixelGet(src, height, width, channels, y, x - 1);
            pE = PixelGet(src, height, width, channels, y, x);
            pF = PixelGet(src, height, width, channels, y, x + 1);
            pH = PixelGet(src, height, width, channels, y + 1, x);

            rA = PixelCopy(pE, channels);
            rB = PixelCopy(pE, channels);
            rC = PixelCopy(pE, channels);
            rD = PixelCopy(pE, channels);
            pBH = PixelNear(pE, pB, pH, channels);
            pDF = PixelNear(pE, pD, pF, channels);
            if (PixelEql(pBH, pE, channels) && PixelEql(pDF, pE, channels))
            {
                rA = PixelNear(pE, pB, pD, channels);
                rB = PixelNear(pE, pB, pF, channels);
                rC = PixelNear(pE, pH, pD, channels);
                rD = PixelNear(pE, pH, pF, channels);
            }
            PixelSet(res, h2, w2, channels, y2, x2, rA);
            PixelSet(res, h2, w2, channels, y2, x2 + 1, rB);
            PixelSet(res, h2, w2, channels, y2 + 1, x2, rC);
            PixelSet(res, h2, w2, channels, y2 + 1, x2 + 1, rD);
        }
    }
}

static void ScaleN3X (unsigned char *src, int height, int width, int channels, unsigned char *res)
{
    int y, x, y3, x3, h3 = height * 3, w3 = width * 3;
    pixel pA, pB, pC, pD, pE, pF, pG, pH, pI;
    pixel pBH, pDF;
    pixel pEA, pEC, pEG, pEI;
    pixel rA, rB, rC, rD, rE, rF, rG, rH, rI;

    for (y = 0; y < height; y++)
    {
        y3 = y + y + y;
        for (x = 0; x < width; x++)
        {
            x3 = x + x + x;

            pA = PixelGet(src, height, width, channels, y - 1, x - 1);
            pB = PixelGet(src, height, width, channels, y - 1, x);
            pC = PixelGet(src, height, width, channels, y - 1, x + 1);
            pD = PixelGet(src, height, width, channels, y, x - 1);
            pE = PixelGet(src, height, width, channels, y, x);
            pF = PixelGet(src, height, width, channels, y, x + 1);
            pG = PixelGet(src, height, width, channels, y + 1, x - 1);
            pH = PixelGet(src, height, width, channels, y + 1, x);
            pI = PixelGet(src, height, width, channels, y + 1, x + 1);

            rA = PixelCopy(pE, channels);
            rB = PixelCopy(pE, channels);
            rC = PixelCopy(pE, channels);
            rD = PixelCopy(pE, channels);
            rE = PixelCopy(pE, channels);
            rF = PixelCopy(pE, channels);
            rG = PixelCopy(pE, channels);
            rH = PixelCopy(pE, channels);
            rI = PixelCopy(pE, channels);
            pBH = PixelNear(pE, pB, pH, channels);
            pDF = PixelNear(pE, pD, pF, channels);
            if (PixelEql(pBH, pE, channels) && PixelEql(pDF, pE, channels))
            {
                rA = PixelNear(pE, pB, pD, channels);
                rB = PixelNear(pE, pB, pF, channels);
                rC = PixelNear(pE, pH, pD, channels);
                rD = PixelNear(pE, pH, pF, channels);
            }

            if (PixelEql(pBH, pE, channels) && PixelEql(pDF, pE, channels))
            {
                rA = PixelNear(pE, pB, pD, channels);
                rC = PixelNear(pE, pB, pF, channels);
                rG = PixelNear(pE, pD, pH, channels);
                rI = PixelNear(pE, pF, pH, channels);

                pEA = PixelNear(rA, pE, pA, channels);
                pEC = PixelNear(rC, pE, pC, channels);
                pEG = PixelNear(rG, pE, pG, channels);
                pEI = PixelNear(rI, pE, pI, channels);

                rB = PixelNear(pE, pEA, pEC, channels);
                rD = PixelNear(pE, pEA, pEG, channels);
                rF = PixelNear(pE, pEC, pEI, channels);
                rH = PixelNear(pE, pEG, pEI, channels);
            }
            PixelSet(res, h3, w3, channels, y3, x3, rA);
            PixelSet(res, h3, w3, channels, y3, x3 + 1, rB);
            PixelSet(res, h3, w3, channels, y3, x3 + 2, rC);
            PixelSet(res, h3, w3, channels, y3 + 1, x3, rD);
            PixelSet(res, h3, w3, channels, y3 + 1, x3 + 1, rE);
            PixelSet(res, h3, w3, channels, y3 + 1, x3 + 2, rF);
            PixelSet(res, h3, w3, channels, y3 + 2, x3, rG);
            PixelSet(res, h3, w3, channels, y3 + 2, x3 + 1, rH);
            PixelSet(res, h3, w3, channels, y3 + 2, x3 + 2, rI);
        }
    }
}

SCALENXAPI void ScaleNX (unsigned char *src, int height, int width, int channels, int scale, unsigned char *res)
{
    switch (scale)
    {
    case 2:
        ScaleN2X(src, height, width, channels, res);
        break;
    case 3:
        ScaleN3X(src, height, width, channels, res);
        break;
    default:
        ScaleN2X(src, height, width, channels, res);
        break;
    }
}


#endif /* SCALENX_IMPLEMENTATION */

#endif /* SCALENX_H_ */
