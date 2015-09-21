#ifndef INTERFACEPRO_H
#define INTERFACEPRO_H

#include "cv.h"
#include "highgui.h"

typedef struct to_McvImgProErode{
    int cols;
    int rows;
    int ancher_x;
    int ancher_y;
    int shape;
    int values;
    int iterations;
}McvImgProErode;

typedef struct to_MCvImgProDilate{
    int cols;
    int rows;
    int ancher_x;
    int ancher_y;
    int shape;
    int values;
    int iterations;
}McvImgProDilate;


typedef struct to_MCvImgProSmooth{
    int smoothType;
    int param1;
    int param2;
    double param3;
    double param4;
}MCvImgProSmooth;

typedef struct to_MCvImgProFloodFill{
    CvPoint seedPoint;
    CvScalar newVal;///
    CvScalar loDiff;///
    CvScalar upDiff;///
    CvConnectedComp *comp;
    int flags;
    CvArr *mask;
}MCvImgProFloodFill;

typedef struct to_MCvImgProThresholdHold{
    double threshold;
    double max_value;
    int threshold_type;
}MCvImgProThresHold;

typedef struct to_MCvImgProResize{
    int interpolation;///
}McvImgProResize;

typedef struct to_MCvImgProPyup{
    int filter;
}McvImgProPyup;

typedef struct to_MCvImgProPydown{
    int filter;
}McvImgProPydown;

#endif // INTERFACEPRO_H
