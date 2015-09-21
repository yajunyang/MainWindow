#include "linktrans.h"
#include <QMessageBox>

LinkTrans::LinkTrans(QObject *parent) :
    QObject(parent)
{
    dialogTra = new DialogTrans;

    // 指针指空
    imageSrc = 0;
    imageDst = 0;

    count_junaji = 0;
    count_sobel = 0;
    count_schar = 0;
    count_laplace = 0;
    count_canny = 0;
    count_houghline = 0;
    count_houghcircle = 0;

    connect(dialogTra, SIGNAL(signalTransJuanji()), this, SLOT(transistmJuanji()));
    connect(dialogTra, SIGNAL(signalTransCanny()), this, SLOT(transistmCanny()));
    connect(dialogTra, SIGNAL(signalTransLaplace()), this, SLOT(transistmLaplace()));
    connect(dialogTra, SIGNAL(signalTransHoughCircle()), this, SLOT(transistmPyrHoughCircle()));
    connect(dialogTra, SIGNAL(signalTransHoughLine()), this, SLOT(transistmHoughLine()));
    connect(dialogTra, SIGNAL(signalTransSchar()), this, SLOT(transistmSchar()));
    connect(dialogTra, SIGNAL(signalTransSobel()), this, SLOT(transistmSobel()));
}

LinkTrans::~LinkTrans()
{
    if(imageSrc)
    {
        cvReleaseImage(&imageSrc);
    }
    if(imageDst)
    {
        cvReleaseImage(&imageDst);
    }
    delete dialogTra;
}

void LinkTrans::getOpenImageFile(const QString openImageFile)
{
    file = openImageFile;
    if(imageSrc)
    {
        cvReleaseImage(&imageSrc);
        imageSrc = 0;
    }
    if(imageDst)
    {
        cvReleaseImage(&imageDst);
        imageDst = 0;
    }

    std::string mid = file.toStdString();
    const char *image = mid.c_str();
    imageSrc = cvLoadImage(image);

    imageDst = cvCreateImage(cvSize(imageSrc->width,imageSrc->height),
                             imageSrc->depth,
                             imageSrc->nChannels);
}

bool LinkTrans::generateFile(QString fileRoadandFileName, QString image, ushort &count, QList<QString> &buffer)
{
    QString rootfileName(fileRoadandFileName);
    QDir dir("D:/OpenImageTemp");
    if(dir.exists())
    {
        dir.setPath(dir.absolutePath() + "/buffer");
        if(dir.exists())
        {
            dir.setPath(dir.absolutePath() + "/" + fileRoadandFileName);
            fileRoadandFileName = dir.absolutePath().arg("/temp");
        }
        else
        {
            dir.mkdir(dir.absolutePath());
            dir.setPath(dir.absolutePath() + "/" + fileRoadandFileName);
            fileRoadandFileName = dir.absolutePath() + "/temp";

            dir.setPath(dir.absolutePath() + "/" + fileRoadandFileName);
            dir.mkdir(dir.absolutePath());
        }
    }
    else
    {
        dir.mkdir(dir.absolutePath());
        dir.setPath(dir.absolutePath() + "/buffer");
        dir.mkdir(dir.absolutePath());
        dir.setPath(dir.absolutePath() + "/" + fileRoadandFileName);
        dir.mkdir(dir.absolutePath());
        fileRoadandFileName = dir.absolutePath() + "/temp";
        dir.mkdir(dir.absolutePath());
    }

    count++;
    fileRoadandFileName += QString::number(count, 10);

    fileRoadandFileName += ".jpg";
    QFile file(fileRoadandFileName);
    file.open(QIODevice::ReadWrite);
    file.close();

    if(buffer.length() <= Max_buffer1)
    {
        buffer.push_back(fileRoadandFileName);// 存放生成的路径

        QString time = QTime::currentTime().toString("hh,mm,ss");
        QString information;
        information.clear();
        information += rootfileName;
        information += " ";
        information += time;
        buffer.push_back(information);
    }
    else
    {
        count = 0;
        buffer.pop_front(); /// buffer 里面的数据是按顺序最新的。
        buffer.pop_front();
        buffer.push_back(fileRoadandFileName);

        QString time = QTime::currentTime().toString("hh,mm,ss");
        QString information;
        information.clear();
        information += rootfileName;
        information += " ";
        information += time;
        buffer.push_back(information);
    }

    QImage imageData(image);/// Take data
    imageData.save(fileRoadandFileName);/// Save to new buffer file

    QImage transOk(fileRoadandFileName);

    ////////
    for(int i=0; i<buffer.length(); i++)
    {
        qDebug() << buffer[i];
    }

    ///////

    if(transOk.isNull())
    {
        QMessageBox::warning(dialogTra, "warning", "Buffer failed!");
        return false;
    }
    else return true;
}

bool LinkTrans::imageIsEmpty()
{
    if(!imageSrc)
    {
            QMessageBox::warning(dialogTra, "Warning", "Please open a image! ! !");
            return true;
    }
    else return false;
}

QString LinkTrans::generateRode(QString fileName)
{
    QString file("D:/OpenImageTemp/");
    QDir dir(file);
    if(!dir.exists())
    {
        dir.mkdir(file);
        file += fileName;
        file += ".jpg";
        if(file.isNull())
        {
            QFile createFile(file);
            createFile.open(QIODevice::ReadWrite);
            createFile.close();
        }
    }
    else
    {
        file += fileName;
        file += ".jpg";
        if(file.isNull())
        {
            QFile createFile(file);
            createFile.open(QIODevice::ReadWrite);
            createFile.close();
        }
    }
    return file;
}


void LinkTrans::startThread(int choice)
{
    switch (choice)
    {
    case JUANJI:
        thread1.updataFilename(saveFileName);
        thread1.start();
        break;
    case SOBEL:
        thread2.updataFilename(saveFileName);
        thread2.start();
        break;
    case SSCHAR:
        thread3.updataFilename(saveFileName);
        thread3.start();
        break;
    case LAPLACE:
        thread4.updataFilename(saveFileName);
        thread4.start();
        break;
    case HOUGHCIRCLE:
        thread5.updataFilename(saveFileName);
        thread5.start();
        break;
    case HOUGHLINE:
        thread6.updataFilename(saveFileName);
        thread6.start();
        break;
    default:
        thread7.updataFilename(saveFileName);
        thread7.start();
    }

}

////////////////////////////////////////////////////
//                   图片执行转换                  //
////////////////////////////////////////////////////

bool LinkTrans::transistmJuanji()
{
     volatile bool mark = false;
     saveFileName.clear();
     if(!imageIsEmpty())
     {
        cvFilter2D(imageSrc,
                   imageDst,
                   dialogTra->visitCurrentJuanji().kernel);
        mark = true;
     }
     else
     {
         return false;
     }

     saveFileName = generateRode("tempjuanji");// 公共缓存

     std::string str = saveFileName.toStdString();
     const char *ch = str.c_str();

     if(!saveFileName.isEmpty())
     {
         QString roadJuanji("juanji");
         QImage imageBuffer(saveFileName);
         if(!imageBuffer.isNull())
         {
             bool isOk = generateFile(roadJuanji, saveFileName, count_junaji, bufferJuanji);
             if(!isOk)
             {
                 QMessageBox::warning(dialogTra, "Warning", "Buffer Error! Please check your image style");
             }
         }
     }

    if(mark){
        cvSaveImage(ch, imageDst);
        startThread(JUANJI);
    }else
    {
        QMessageBox::warning(dialogTra, "warning", "Trans failure! ! !");
    }

     return true;
}

bool LinkTrans::transistmLaplace()
{
    volatile bool mark = false;
    if(!imageIsEmpty())
    {
        cvLaplace(imageSrc,
                  imageDst,
                  dialogTra->visitCurrentLaplace().apertureSize);
        mark = true;
    }
    else
    {
        return false;
    }

    saveFileName = generateRode("tempLaplace");// 公共缓存

    std::string str = saveFileName.toStdString();
    const char *ch = str.c_str();

    if(!saveFileName.isEmpty())
    {
        QString roadLaplace("laplace");
        QImage imageBuffer(saveFileName);
        if(!imageBuffer.isNull())
        {
            bool isOk = generateFile(roadLaplace, saveFileName, count_laplace, bufferLaplace);
            if(!isOk)
            {
                QMessageBox::warning(dialogTra, "Warning", "Buffer Error! Please check your image style");
            }
        }
    }

    if(mark){
        cvSaveImage(ch, imageDst);
        startThread(LAPLACE);
    }else
    {
        QMessageBox::warning(dialogTra, "warning", "Trans failure! ! !");
    }
    return true;
}

bool LinkTrans::transistmCanny()
{
    volatile bool mark = false;
    IplImage *tempDst = cvCreateImage(cvSize(imageDst->width, imageDst->height),
                                      imageDst->depth,
                                      1);
    if(!imageIsEmpty())
    {
        if(imageSrc->nChannels != 1)
        {
            IplImage *tempSrc = cvCreateImage(cvSize(imageDst->width, imageSrc->height),
                                            imageSrc->depth,
                                            1);
            IplImage *resolve1 = cvCreateImage(cvSize(imageDst->width, imageDst->height),
                                               imageDst->depth,
                                               1);
            IplImage *resolve2 = cvCreateImage(cvSize(imageDst->width, imageDst->height),
                                               imageDst->depth,
                                               1);
            IplImage *resolve3 = cvCreateImage(cvSize(imageDst->width, imageDst->height),
                                               imageDst->depth,
                                               1);
            if(imageSrc->nChannels == 4)
            {
                cvSplit(imageSrc, tempSrc,resolve1, resolve2, resolve3);
                cvAddWeighted(tempSrc, 0.3, resolve1, 0.3, 0.0,  tempSrc);
                cvAddWeighted(tempSrc, 1.0, resolve2, 0.3, 0.0, tempSrc);
                cvAddWeighted(tempSrc, 1.0, resolve3, 0.3, 0.0, tempSrc);
            }
            else if(imageSrc->nChannels == 3)
            {
                cvSplit(imageSrc, tempSrc, resolve1, resolve2, NULL);
                cvAddWeighted(tempSrc, 0.3, resolve1, 0.3, 0.0, tempSrc);
                cvAddWeighted(tempSrc, 0.3, resolve2, 0.3, 0.0, tempSrc);
            }

            cvCanny(tempSrc, tempDst,
                    dialogTra->visitCurrentCanny().lowThresh,
                    dialogTra->visitCurrentCanny().highThresh,
                    dialogTra->visitCurrentCanny().apertureSize);

            cvReleaseImage(&tempSrc);
            cvReleaseImage(&resolve1);
            cvReleaseImage(&resolve2);
            cvReleaseImage(&resolve3);

            mark = true;
        }
        else if(imageSrc->nChannels == 1)
        {
            cvCanny(imageSrc, tempDst,
                    dialogTra->visitCurrentCanny().lowThresh,
                    dialogTra->visitCurrentCanny().highThresh,
                    dialogTra->visitCurrentCanny().apertureSize);

            mark = true;
        }
    }
    else
    {
        cvReleaseImage(&tempDst);
        return false;
    }

    saveFileName = generateRode("tempCanny");// 公共缓存

    std::string str = saveFileName.toStdString();
    const char *ch = str.c_str();

    if(!saveFileName.isEmpty())
    {
        QString roadCanny("canny");
        QImage imageBuffer(saveFileName);
        if(!imageBuffer.isNull())
        {
//          generateFile 将临时公共缓存的图片结果放入一个换存量更大的缓存文件中。
            bool isOk = generateFile(roadCanny, saveFileName, count_canny, bufferCanny);
            if(!isOk)
            {
                QMessageBox::warning(dialogTra, "Warning", "Buffer Error! Please check your image style");
            }
        }
    }


    if(mark){
        cvSaveImage(ch, tempDst);
        startThread(CANNY);
    } else
    {
         QMessageBox::warning(dialogTra, "warning", "Trans failure! ! !");
    }
    cvReleaseImage(&tempDst);
    return true;
}

bool LinkTrans::transistmSchar()
{
    volatile bool mark = false;
    if(!imageIsEmpty())
    {
       QMessageBox::information(dialogTra, "Inform", "This algorithm need your design! \
                                                     please go to the c++ file LinkTrans.cpp to \
                                realise your own algorithm");
    }
    else
    {
        return false;
    }

    saveFileName = generateRode("tempSchar");// 公共缓存

    std::string str = saveFileName.toStdString();
    const char *ch = str.c_str();

    if(!saveFileName.isEmpty())
    {
        QString roadSchar("schar");
        QImage imageBuffer(saveFileName);
        if(!imageBuffer.isNull())
        {
//         generateFile 将临时公共缓存的图片结果放入一个换存量更大的缓存文件中。

            bool isOk = generateFile(roadSchar, saveFileName, count_schar, bufferSchar);
            if(!isOk)
            {
                QMessageBox::warning(dialogTra, "Warning", "Buffer Error! Please check your image style");
            }
        }
    }

    if(mark){
        cvSaveImage(ch, imageDst);
        startThread(SSCHAR);
    } else
    {
        QMessageBox::warning(dialogTra, "warning", "Trans failure! ! !");
    }
    return true;
}

bool LinkTrans::transistmSobel()
{
    volatile bool mark = false;
    if(!imageIsEmpty())
    {
        if(dialogTra->visitCurrentSobel().xorder || dialogTra->visitCurrentSobel().yorder)
        {
            cvSobel(imageSrc,
                    imageDst,
                    dialogTra->visitCurrentSobel().xorder,
                    dialogTra->visitCurrentSobel().yorder,
                    dialogTra->visitCurrentSobel().aperture_size);
            mark = true;
        }
        else{
            QMessageBox::warning(dialogTra, "warning!", "xorder and yorder can't be zero both!");
        }
    }
    else
    {
        return false;
    }

    saveFileName = generateRode("tempSobel");// 公共缓存

    std::string str = saveFileName.toStdString();
    const char *ch = str.c_str();

    if(!saveFileName.isEmpty())
    {
        QString roadSobel("sobel");
        QImage imageBuffer(saveFileName);
        if(!imageBuffer.isNull())
        {
//         generateFile 将临时公共缓存的图片结果放入一个换存量更大的缓存文件中。

            bool isOk = generateFile(roadSobel, saveFileName, count_sobel, bufferSobel);
            if(!isOk)
            {
                QMessageBox::warning(dialogTra, "Warning", "Buffer Error! Please check your image style");
            }
        }
    }

    if(mark){
        cvSaveImage(ch, imageDst);
        startThread(SOBEL);
    }else
    {
        QMessageBox::warning(dialogTra, "warning", "Trans failure! ! !");
    }

    return true;
}

bool LinkTrans::transistmHoughLine()
{
    volatile bool mark = false;
    if(!imageIsEmpty())
    {
       QMessageBox::information(dialogTra, "Inform", "This algorithm need your design! \
                                                     please go to the c++ file LinkTrans.cpp to \
                                realise your own algorithm");
    }
    else
    {
        return false;
    }

    saveFileName = generateRode("tempHoughLine");// 公共缓存

    std::string str = saveFileName.toStdString();
    const char *ch = str.c_str();

    if(!saveFileName.isEmpty())
    {
        QString roadHoughLine("houghline");
        QImage imageBuffer(saveFileName);
        if(!imageBuffer.isNull())
        {
//         generateFile 将临时公共缓存的图片结果放入一个换存量更大的缓存文件中。

            bool isOk = generateFile(roadHoughLine, saveFileName, count_houghline, bufferHoughLine);
            if(!isOk)
            {
                QMessageBox::warning(dialogTra, "Warning", "Buffer Error! Please check your image style");
            }
        }
    }

    if(mark){
        cvSaveImage(ch, imageDst);
        startThread(HOUGHLINE);
    }else
    {
    QMessageBox::warning(dialogTra, "warning", "Trans failure! ! !");
    }
    return true;
}

bool LinkTrans::transistmPyrHoughCircle()
{
    volatile bool mark = false;//// 有待转换成功后的修正
    if(!imageIsEmpty())
    {
       QMessageBox::information(dialogTra, "Inform", "This algorithm need your design! \
                                                     please go to the c++ file LinkTrans.cpp to \
                                realise your own algorithm");
                                return true;
    }
    else return false;

    saveFileName = generateRode("tempHoughCircle");// 公共缓存

    std::string str = saveFileName.toStdString();
    const char *ch = str.c_str();

    if(!saveFileName.isEmpty())
    {
        QString roadHoughCircle("houghcircle");
        QImage imageBuffer(saveFileName);
        if(!imageBuffer.isNull())
        {
            bool isOk = generateFile(roadHoughCircle, saveFileName, count_houghcircle, bufferHoughCircle);
            if(!isOk)
            {
                QMessageBox::warning(dialogTra, "Warning", "Buffer Error! Please check your image style");
            }
        }
    }

    if(mark){
        cvSaveImage(ch, imageDst);
        startThread(HOUGHCIRCLE);
    }else{
        QMessageBox::warning(dialogTra, "warning", "Trans failure! ! !");
    }
    return true;
}


void LinkTrans::showTransDialog()
{
    dialogTra->show();
}
