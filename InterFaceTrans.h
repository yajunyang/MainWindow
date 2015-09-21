#ifndef INTERFACETRANS_H
#define INTERFACETRANS_H

#include <cv.h>
#include <highgui.h>

typedef struct To_JuanjiParam /// 1
{
    CvMat *kernel;
    CvPoint ancher;
}McvImgTransJuanji;

typedef struct To_sobel  /// 2
{
    int xorder;
    int yorder;
    int aperture_size;
}McvImgTransSobel;

typedef struct To_scharr  /// 3
{
    CvMat *kernel;
}McvImgTransSchar;

typedef struct To_laplace  /// 4
{
    int apertureSize;
}McvImgTransLaplace;

typedef struct To_canny /// 5
{
    double lowThresh;
    double highThresh;
    int apertureSize;
}McvImgTransCanny;

typedef struct To_houghLine /// 6
{
    int method;
    double rho;
    double theta;
    int threshold;
    double param1;
    double param2;
}McvImgtransHoughLine;

typedef struct To_houghCircle /// 7
{
    int method;
    double dp;
    double min_dist;
    double param1;
    double param2;
    int min_radius;
    int max_radius;
}McvImgTransHoughCircle;

#endif // INTERFACETRANS_H
