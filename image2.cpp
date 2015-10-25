#include "image2.h"

void ImgDraw(IplImage *src, IplImage *u, IplImage *dst)
{
    if (!src || !dst || !u)
    {
        return;
    }

    int i,j;
    CvScalar Csrc, Cu[2], Cred;
    Cred.val[0] = Cred.val[1] = 0;
    Cred.val[2] = 255;

    for (i=0; i<src->height; i++)
    {
        for(j=0; j<src->width-1; j++)
        {
            Cu[0] = cvGet2D(u,i,j);
            Cu[1] = cvGet2D(u,i,j+1);
            Csrc = cvGet2D(src,i,j);
            if (Cu[0].val[0]*Cu[1].val[0]<=0)
            {
                if (fabs(Cu[0].val[0])<fabs(Cu[1].val[0]))
                {
                    cvSet2D(dst,i,j,Cred);
                    continue;
                }
                else
                {
                    cvSet2D(dst,i,j,Csrc);
                    cvSet2D(dst,i,j+1,Cred);
                    j++;
                    continue;
                }
            }
            cvSet2D(dst,i,j,Csrc);
        }
    }

    for (j=0; j<src->width; j++)
    {
        for(i=0; i<src->height-1; i++)
        {
            Cu[0] = cvGet2D(u,i,j);
            Cu[1] = cvGet2D(u,i+1,j);
            Csrc = cvGet2D(src,i,j);
            if (Cu[0].val[0]*Cu[1].val[0]<=0)
            {
                if (fabs(Cu[0].val[0])<fabs(Cu[1].val[0]))
                {
                    cvSet2D(dst,i,j,Cred);
                    continue;
                }
                else
                {
                    cvSet2D(dst,i,j,Csrc);
                    cvSet2D(dst,i+1,j,Cred);
                    i++;
                    continue;
                }
            }
            cvSet2D(dst,i,j,Csrc);
        }
    }
}

void ImgDraw2(IplImage* src,IplImage *u,IplImage*dst)
{
    if(!src||!dst||!u)
    {
        return;
    }
    int i,j;
    CvScalar Csrc,Cu,Cred;
    Cred.val[0]=Cred.val[1]=0;
    Cred.val[2]=255;
    for(i=0;i<src->height;i++)
    {
        for(j=0;j<src->width;j++)
        {
            Cu=cvGet2D(u,i,j);
            Csrc=cvGet2D(src,i,j);
            if(Cu.val[0]==0)
            {
                cvSet2D(dst,i,j,Cred);
            }
            else
            {
                cvSet2D(dst,i,j,Csrc);
            }
        }
    }
}

void NeumannBoundCond(IplImage *img)
{
    if (!img) return;

    int row = img->height;
    int col = img->width;
    int i,j;

    cvSet2D(img, 0, 0, cvGet2D(img, 2, 2));
    cvSet2D(img, 0, col-1, cvGet2D(img, 2, col-3));
    cvSet2D(img, row-1, 0, cvGet2D(img, row-3, 2));
    cvSet2D(img, row-1, col-1, cvGet2D(img, row-3, col-3));

    for (j=1;j<col-1;j++)
    {
        cvSet2D(img, 0, j, cvGet2D(img, 2, j));
        cvSet2D(img, row-1, j, cvGet2D(img, row-3, j));
    }

    for (i=1;i<row-1;i++)
    {
        cvSet2D(img, i, 0, cvGet2D(img, i, 2));
        cvSet2D(img, i, col-1, cvGet2D(img, i, col-3));
    }
}

void Sobel(IplImage *src, IplImage *dx, IplImage *dy)
{
    if (!src || !dx || !dy)
    {
        return;
    }

    cvSobel(src, dx, 1, 0, 1);
    cvSobel(src, dy, 0, 1, 1);

    CvScalar cur;
    for (int i=0;i<src->height;i++)
    {
        for (int j=0;j<src->width;j++)
        {
            cur = cvGet2D(dx,i,j);
            cur.val[0] /= 2.0;
            cvSet2D(dx,i,j,cur);

            cur = cvGet2D(dy,i,j);
            cur.val[0] /= 2.0;
            cvSet2D(dy,i,j,cur);
        }
    }
}

void CurvatureCentral(IplImage *nx, IplImage *ny, IplImage* dst)
{
    if (!nx || !ny || !dst)
    {
        return;
    }

    CvSize size = cvSize(nx->width, nx->height);
    IplImage* nxx = cvCreateImage(size,IPL_DEPTH_32F,1);
    IplImage* nyy = cvCreateImage(size,IPL_DEPTH_32F,1);

    Sobel(nx, nxx, nyy);

    CvScalar cx,cy,crt;
    for (int i=0; i<size.height; i++)
    {
        for (int j=0; j<size.width; j++)
        {
            cx = cvGet2D(nxx,i,j);
            cy = cvGet2D(nyy,i,j);
            crt.val[0] = cx.val[0] + cy.val[0];
            cvSet2D(dst,i,j,crt);
        }
    }

    cvReleaseImage(&nxx);
    cvReleaseImage(&nyy);
}

void CurvatureCentral2(IplImage *nx, IplImage *ny, IplImage* dst)
{
    if (!nx || !ny || !dst)
    {
        return;
    }

    CvSize size = cvSize(nx->width, nx->height);
    IplImage* nxx = cvCreateImage(size,IPL_DEPTH_32F,1);
    IplImage* nyy = cvCreateImage(size,IPL_DEPTH_32F,1);
    IplImage* junk = cvCreateImage(size,IPL_DEPTH_32F,1);

    Sobel(nx,nxx,junk);
    Sobel(ny,junk,nyy);

    CvScalar cx,cy,crt;
    for (int i=0; i<size.height; i++)
    {
        for (int j=0; j<size.width; j++)
        {
            cx = cvGet2D(nxx,i,j);
            cy = cvGet2D(nyy,i,j);
            crt.val[0] = cx.val[0] + cy.val[0];
            cvSet2D(dst,i,j,crt);
        }
    }

    cvReleaseImage(&nxx);
    cvReleaseImage(&nyy);
    cvReleaseImage(&junk);
}

void Dirac(IplImage *src, IplImage *dst, double sigma)
{
    if (!src || !dst)
    {
        return;
    }

    int i,j;
    CvScalar Csrc, Cdst;
    double tmp = 1.0/2.0/sigma;

    for (i=0; i<src->height; i++)
    {
        for(j=0; j<src->width; j++)
        {
            Csrc = cvGet2D(src,i,j);
            if (Csrc.val[0]<=sigma && Csrc.val[0]>=-sigma)
            {
                Cdst.val[0] = tmp*(1+cos(CV_PI*Csrc.val[0]/sigma));
            }
            else
            {
                Cdst.val[0] = 0;
            }
            cvSet2D(dst,i,j,Cdst);
        }
    }
}

void Evolution2(IplImage * u, IplImage *g, double lambda, double mu, double alf, double epsilon, double delt, int numIter)
{
    if (!u||!g)
        return;
    CvSize size = cvGetSize(u);
    IplImage* vx = cvCreateImage(size,IPL_DEPTH_32F,1);
    IplImage* vy = cvCreateImage(size,IPL_DEPTH_32F,1);
    IplImage* ux = cvCreateImage(size,IPL_DEPTH_32F,1);
    IplImage* uy = cvCreateImage(size,IPL_DEPTH_32F,1);
    IplImage* Nx = cvCreateImage(size,IPL_DEPTH_32F,1);
    IplImage* Ny = cvCreateImage(size,IPL_DEPTH_32F,1);
    IplImage* diracU = cvCreateImage(size,IPL_DEPTH_32F,1);
    IplImage* K = cvCreateImage(size,IPL_DEPTH_32F,1);
    IplImage* Laplace = cvCreateImage(size,IPL_DEPTH_32F,1);

    Sobel(g,vx,vy);
    CvScalar s1,s2,s11,s22;
    CvScalar Cdirac, Cvx, Cvy, CNx, CNy, Cg, CK, CLaplace, Cu;
    int i,j;
    for(int k=0;k<numIter;k++)
    {
        cout<<k<<"……"<<endl;
        NeumannBoundCond(u);
        Sobel(u,ux,uy);

        for(i=0;i<size.height;i++)
        {
            for(j=0;j<size.width;j++)
            {
                s1=cvGet2D(ux,i,j);
                s2=cvGet2D(uy,i,j);
                double normDu=sqrt(pow(s1.val[0],2)+pow(s2.val[0],2)+1e-10);
                s11.val[0]=s1.val[0]/normDu;
                s22.val[0]=s2.val[0]/normDu;
                cvSet2D(Nx,i,j,s11);
                cvSet2D(Ny,i,j,s22);
            }
        }

        Dirac(u,diracU,epsilon);
        CurvatureCentral2(Nx,Ny,K);
        cvLaplace(u, Laplace, 1);
        for(i=0;i<size.height;i++)
        {
            for(j=0;j<size.width;j++)
            {
                Cdirac=cvGet2D(diracU,i,j);
                Cvx=cvGet2D(vx,i,j);
                Cvy=cvGet2D(vy,i,j);
                CNx=cvGet2D(Nx,i,j);
                CNy=cvGet2D(Ny,i,j);
                Cg=cvGet2D(g,i,j);
                CK=cvGet2D(K,i,j);
                CLaplace=cvGet2D(Laplace,i,j);
                Cu=cvGet2D(u,i,j);

                double weightedLengthTerm=lambda*Cdirac.val[0]*(Cvx.val[0]*CNx.val[0]+Cvy.val[0]*CNy.val[0]+Cg.val[0]*CK.val[0]);
                double weightedAreaTerm=alf*Cdirac.val[0]*Cg.val[0];
                double penalizingTerm=mu*(CLaplace.val[0]-CK.val[0]);
                double total=weightedLengthTerm+weightedAreaTerm+penalizingTerm;
                Cu.val[0]+=delt*total;
                cvSet2D(u,i,j,Cu);
            }
        }
    }
    cvReleaseImage(&vx);
    cvReleaseImage(&vy);
    cvReleaseImage(&ux);
    cvReleaseImage(&uy);
    cvReleaseImage(&Nx);
    cvReleaseImage(&Ny);
    cvReleaseImage(&diracU);
    cvReleaseImage(&K);
    cvReleaseImage(&Laplace);
}

void Evolution(IplImage *u, IplImage *g, double lambda, double mu, double alf, double epsilon, double delt, int numIter)
{
    if (!u||!g)
        return;
    CvSize size = cvGetSize(u);
    IplImage* vx = cvCreateImage(size,IPL_DEPTH_32F,1);
    IplImage* vy = cvCreateImage(size,IPL_DEPTH_32F,1);
    IplImage* ux = cvCreateImage(size,IPL_DEPTH_32F,1);
    IplImage* uy = cvCreateImage(size,IPL_DEPTH_32F,1);
    IplImage* Nx = cvCreateImage(size,IPL_DEPTH_32F,1);
    IplImage* Ny = cvCreateImage(size,IPL_DEPTH_32F,1);
    IplImage* diracU = cvCreateImage(size,IPL_DEPTH_32F,1);
    IplImage* K = cvCreateImage(size,IPL_DEPTH_32F,1);
    IplImage* laplace = cvCreateImage(size,IPL_DEPTH_32F,1);

    Sobel(g, vx, vy);

    int i,j;
    CvScalar cur1,cur2,cur11,cur22,cur;
    CvScalar Cdirac, Cvx, CNx, Cvy, CNy, Cg, CK, Claplace, Cu;
    for (int k=0;k<numIter;k++)
    {
        cout<<k<<"……"<<endl;
        NeumannBoundCond(u);
        Sobel(u, ux, uy);

        for (i=0; i<size.height; i++)
        {
            for(j=0; j<size.width; j++)
            {
                cur1 = cvGet2D(ux,i,j);
                cur2 = cvGet2D(uy,i,j);
                cur11.val[0] = cur1.val[0]/sqrt(cur1.val[0]*cur1.val[0] + cur2.val[0]*cur2.val[0] + 1e-10);
                cur22.val[0] = cur2.val[0]/sqrt(cur1.val[0]*cur1.val[0] + cur2.val[0]*cur2.val[0] + 1e-10);
                cvSet2D(Nx,i,j,cur11);
                cvSet2D(Ny,i,j,cur22);
            }
        }

        Dirac(u,diracU,epsilon);
        CurvatureCentral2(Nx,Ny,K);
        cvLaplace(u, laplace, 1);

        for (i=0; i<size.height; i++)
        {
            for(j=0; j<size.width; j++)
            {
                Cdirac = cvGet2D(diracU,i,j);
                Cvx = cvGet2D(vx,i,j);
                CNx = cvGet2D(Nx,i,j);
                Cvy = cvGet2D(vy,i,j);
                CNy = cvGet2D(Ny,i,j);
                Cg = cvGet2D(g,i,j);
                CK = cvGet2D(K,i,j);
                Claplace = cvGet2D(laplace,i,j);
                Cu = cvGet2D(u,i,j);

                cur.val[0] = lambda*Cdirac.val[0]*(Cvx.val[0]*CNx.val[0] + Cvy.val[0]*CNy.val[0] + Cg.val[0]*CK.val[0])
                    + mu*(Claplace.val[0]-CK.val[0]) + alf*Cdirac.val[0]*Cg.val[0];
                Cu.val[0] += delt*cur.val[0];
                cvSet2D(u,i,j,Cu);
            }
        }
    }
    cvReleaseImage(&vx);
    cvReleaseImage(&vy);
    cvReleaseImage(&ux);
    cvReleaseImage(&uy);
    cvReleaseImage(&Nx);
    cvReleaseImage(&Ny);
    cvReleaseImage(&diracU);
    cvReleaseImage(&K);
    cvReleaseImage(&laplace);
}
