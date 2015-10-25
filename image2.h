#ifndef IMAGE2_H
#define IMAGE2_H
#include<cv.h>
#include<highgui.h>
#include<iostream>
#include<cmath>
#include<ctime>
using namespace std;

extern void ImgDraw(IplImage* src,IplImage *u,IplImage*dst);
extern void ImgDraw2(IplImage* src,IplImage *u,IplImage*dst);
extern void NeumannBoundCond(IplImage * img);
extern void Sobel(IplImage *src, IplImage *dx, IplImage *dy);
extern void Evolution2(IplImage * u, IplImage *g, double lambda, double mu, double alf, double epsilon, double delt, int numIter);
extern void Evolution(IplImage *u, IplImage *g, double lambda, double mu, double alf, double epsilon, double delt, int numIter);
extern void CurvatureCentral(IplImage *nx, IplImage *ny, IplImage* dst);
extern void CurvatureCentral2(IplImage *nx, IplImage *ny, IplImage* dst);
extern void Dirac(IplImage *src, IplImage *dst, double sigma);

#endif
