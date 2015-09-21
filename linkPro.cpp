#include "linkPro.h"
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QDir>

LinkPro::LinkPro(QObject *parent) :
    QObject(parent)
{
    dialogPro = new DialogPro;//(this);

    imageSrc = 0;
    imageDst = 0;
    B1 = 0;
    B2 = 0;

    count_dilate = 0; /// 1
    count_erode = 0;  /// 2
    count_floodfill = 0; /// 3
    count_pyrdown = 0; /// 4
    count_pyrup = 0; /// 5
    count_resize = 0; /// 6
    count_smooth = 0; /// 7
    count_threshold = 0; /// 8

    bufferDilate.clear();
    bufferErode.clear();
    bufferFloodFill.clear();
    bufferPyrDown.clear();
    bufferPyrUp.clear();
    bufferResize.clear();
    bufferSmooth.clear();
    bufferThreshold.clear();

    connect(dialogPro, SIGNAL(signalTransSmooth()), this, SLOT(transistmSmooth()));
    connect(dialogPro, SIGNAL(signalTransDilate()), this, SLOT(transistmDilate()));
    connect(dialogPro, SIGNAL(signalTransErode()), this, SLOT(transistmErode()));
    connect(dialogPro, SIGNAL(signalTransFloodFill()), this, SLOT(transistmFloodFill()));
    connect(dialogPro, SIGNAL(signalTransPyrDown()), this, SLOT(transistmPyrDown()));
    connect(dialogPro, SIGNAL(signalTransPyrUp()), this, SLOT(transistmPyrUp()));
    connect(dialogPro, SIGNAL(signalTransResize()), this, SLOT(transistmResize()));
    connect(dialogPro, SIGNAL(signalTransThreshold()), this, SLOT(transistmThreshold()));   
}

LinkPro::~LinkPro()
{
    if(imageSrc)
    {
        cvReleaseImage(&imageSrc);
    }
    if(imageDst)
    {
        cvReleaseImage(&imageDst);
    }
    if(B1)
    {
        cvReleaseStructuringElement(&B1);
    }
    if(B2)
    {
        cvReleaseStructuringElement(&B2);
    }
    delete dialogPro;
}

void  LinkPro::getOpenImageFile(const QString openImageFile)
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
      if(B1)
      {
          cvReleaseStructuringElement(&B1);
          B1 = 0;
      }
      if(B2)
      {
          cvReleaseStructuringElement(&B2);
          B2 = 0;
      }

      std::string mid = file.toStdString();
      const char *image = mid.c_str();
      imageSrc = cvLoadImage(image);

      imageDst = cvCreateImage(cvSize(imageSrc->width,imageSrc->height),
                               imageSrc->depth,
                               imageSrc->nChannels);
}

bool LinkPro::generateFile(QString fileRoadandFileName, QString image, ushort &count, QList<QString> &buffer)
{
    count++;
    QString rootfileName(fileRoadandFileName);
    QDir dir("D:/OpenImageTemp");
    if(dir.exists())
    {
         dir.setPath(dir.absolutePath() + "/buffer");
         if(dir.exists())
         {
             dir.setPath(dir.absolutePath() + "/" + fileRoadandFileName);
             fileRoadandFileName = dir.absolutePath() + "/temp";
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


     fileRoadandFileName += QString::number(count, 10);

     fileRoadandFileName += ".jpg";
     QFile file(fileRoadandFileName);
     file.open(QIODevice::ReadWrite);
     file.close();

     if(buffer.length() <= Max_buffer)
     {
         buffer.push_back(fileRoadandFileName);

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
         buffer.pop_front();
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
         QMessageBox::warning(dialogPro, "warning", "Buffer failed!");
         return false;
     }
     else return true;
}

bool LinkPro::imageIsEmpty()
{
    if(!imageSrc)
    {
            QMessageBox::warning(dialogPro, "Warning", "Please open a image! ! !");
            return true;
    }
    else return false;
}

QString LinkPro::generateRode(QString fileName)
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
        else
        {
            return file;
        }
    }
    return file;
}

void LinkPro::startThread(int choice)
{
//    thread.updataFilename(saveFileName);
//    thread.start();

    switch (choice)
    {
    case SMOOTH:
        thread1.updataFilename(saveFileName);
        thread1.start();
        break;
    case DILATE:
        thread2.updataFilename(saveFileName);
        thread2.start();
        break;
    case ERODE:
        thread3.updataFilename(saveFileName);
        thread3.start();
        break;
    case RSIZE:
        thread4.updataFilename(saveFileName);
        thread4.start();
        break;
    case FLOODFILL:
        thread5.updataFilename(saveFileName);
        thread5.start();
        break;
    case PYRDOWN:
        thread6.updataFilename(saveFileName);
        thread6.start();
        break;
    case PYRUP:
        thread7.updataFilename(saveFileName);
        thread7.start();
        break;
    default:
        thread8.updataFilename(saveFileName);
        thread8.start();
    }
}

/// ![0]
bool LinkPro::transistmSmooth()
{
    volatile bool mark = false;
    saveFileName.clear();

    if(!imageIsEmpty())
    {
        cvSmooth(imageSrc,
                 imageDst,
                 dialogPro->visitCurrentSmooth().smoothType,
                 dialogPro->visitCurrentSmooth().param1,
                 dialogPro->visitCurrentSmooth().param2,
                 dialogPro->visitCurrentSmooth().param3,
                 dialogPro->visitCurrentSmooth().param4);
        mark = true;
    }
    else
    {
        return false;
    }

    saveFileName = generateRode("tempSmooth");

    std::string str = saveFileName.toStdString();
    const char *ch = str.c_str();

    if(!saveFileName.isEmpty())
    {
        QString roadSmooth("smooth");
        QImage imageBuffer(saveFileName);
        if(!imageBuffer.isNull())
        {
            bool isOk = generateFile(roadSmooth, saveFileName, count_smooth, bufferSmooth);
            if(!isOk)
            {
                QMessageBox::warning(dialogPro, "Warning", "Buffer Error! Please check your image style");
            }
        }
    }

    if(mark)
    {
        cvSaveImage(ch, imageDst);
        startThread(SMOOTH);
    }
    return true;
}

bool LinkPro::transistmDilate()
{
    volatile bool mark = false;

    saveFileName.clear();
    int values;
    values = dialogPro->visitCurrentDilate().values;

    if(!imageIsEmpty())
    {
        B2 = cvCreateStructuringElementEx(
                    dialogPro->visitCurrentDilate().cols,
                    dialogPro->visitCurrentDilate().rows,
                    dialogPro->visitCurrentDilate().ancher_x,
                    dialogPro->visitCurrentDilate().ancher_y,
                    dialogPro->visitCurrentDilate().shape,
                    &values);
        cvDilate(imageSrc,
                imageDst,
                B2,
                dialogPro->visitCurrentDilate().iterations);
        mark = true;
    }
    else
    {
        return false;
    }


    if(B2)
    {
        cvReleaseStructuringElement(&B2);
        B2 = 0;
    }

    saveFileName = generateRode("tempDilate");

    std::string str = saveFileName.toStdString();
    const char *ch = str.c_str();

    if(!saveFileName.isEmpty())
    {
         QString roadDilate("dilate");
         QImage imageBuffer(saveFileName);

         if(!imageBuffer.isNull())
         {
             bool isOk = generateFile(roadDilate,saveFileName, count_dilate, bufferDilate);
             if(!isOk)
                   QMessageBox::warning(dialogPro, "Warning", "Buffer Error! Please check your image style");
         }
    }

    if(true)
    {
        cvSaveImage(ch, imageDst);
        startThread(DILATE);
    }

    return true;
}

bool LinkPro::transistmErode()
{
    volatile bool mark = false;

    saveFileName.clear();
    int values;
    values = dialogPro->visitCurrentErode().values;
    if(!imageIsEmpty())
    {
        B1 = cvCreateStructuringElementEx(
                    dialogPro->visitCurrentErode().cols,
                    dialogPro->visitCurrentErode().rows,
                    dialogPro->visitCurrentErode().ancher_x,
                    dialogPro->visitCurrentErode().ancher_y,
                    dialogPro->visitCurrentErode().shape,
                    &values);
        cvErode(imageSrc,
                imageDst,
                B1,
                dialogPro->visitCurrentErode().iterations);

        mark = true;
    }
    else
    {
        return false;
    }

    if(B1)
    {
        cvReleaseStructuringElement(&B1);
        B1 = 0;
    }

    saveFileName = generateRode("tempErode");

    std::string str = saveFileName.toStdString();
    const char *ch = str.c_str();

    if(!saveFileName.isEmpty())
    {
        QString roadErode("erode");
         if(!QImage(saveFileName).isNull())
         {
             bool isOk = generateFile(roadErode, saveFileName, count_erode, bufferErode);
             if(!isOk)
                   QMessageBox::warning(dialogPro, "Warning", "Buffer Error! Please check your image style");
         }
    }

    if(mark)
    {
        cvSaveImage(ch, imageDst);
        startThread(ERODE);
    }

    return true;
}

bool LinkPro::transistmResize()
{
    volatile bool mark = false;
    saveFileName.clear();
    if(!imageIsEmpty())
    {
        cvResize(imageSrc,
                 imageDst,
                 dialogPro->visitCurrentResize().interpolation);
        mark = true;
    }
    else
    {
        return false;
    }

    saveFileName = generateRode("Resize");

    std::string str = saveFileName.toStdString();
    const char *ch = str.c_str();

    if(!saveFileName.isEmpty())
    {
        QString roadResize("resize");
        if(!QImage(saveFileName).isNull())
        {
            bool isOk = generateFile(roadResize, saveFileName, count_resize, bufferResize);
            if(!isOk)
                  QMessageBox::warning(dialogPro, "Warning", "Buffer Error! Please check your image style");
        }
    }
    if(mark)
    {
         cvSaveImage(ch, imageDst);
         startThread(RSIZE);
    }

     return true;
}

bool LinkPro::transistmFloodFill()
{
    volatile bool mark = false;
    saveFileName.clear();
    if(!imageIsEmpty())
    {
        cvFloodFill(imageSrc,
                    dialogPro->visitCurrentFloodFill().seedPoint,
                    dialogPro->visitCurrentFloodFill().newVal,
                    dialogPro->visitCurrentFloodFill().loDiff,
                    dialogPro->visitCurrentFloodFill().upDiff,
                    dialogPro->visitCurrentFloodFill().comp,
                    dialogPro->visitCurrentFloodFill().flags,
                    dialogPro->visitCurrentFloodFill().mask);
        mark = true;
    }
    else
    {
        return false;
    }

    saveFileName = generateRode("tempFloodFill");

    std::string str = saveFileName.toStdString();
    const char *ch = str.c_str();

    if(!saveFileName.isEmpty())
    {
        QString roadFloodFill("floodfill");
        if(!QImage(saveFileName).isNull())
        {
            bool isOk = generateFile(roadFloodFill, saveFileName, count_floodfill, bufferFloodFill);
            if(!isOk)
                  QMessageBox::warning(dialogPro, "Warning", "Buffer Error! Please check your image style");
        }
    }

    if(mark)
    {
        cvSaveImage(ch, imageDst);
        startThread(FLOODFILL);
    }

    return true;
}

bool LinkPro::transistmThreshold()
{
    volatile bool mark = false;

    saveFileName.clear();
    if(!imageIsEmpty())
    {
        cvThreshold(imageSrc,
                    imageDst,
                    dialogPro->visitCurrentThresHold().threshold,
                    dialogPro->visitCurrentThresHold().max_value,
                    dialogPro->visitCurrentThresHold().threshold_type);
        mark = true;
    }
    else
    {
        return false;
    }

    saveFileName = generateRode("tempThreshold");

    std::string str = saveFileName.toStdString();
    const char *ch = str.c_str();

    if(!saveFileName.isEmpty())
    {
        QString roadThreshold("threshold");
        if(!QImage(saveFileName).isNull())
        {
            bool isOk = generateFile(roadThreshold, saveFileName, count_threshold, bufferThreshold);
            if(!isOk)
                  QMessageBox::warning(dialogPro, "Warning", "Buffer Error! Please check your image style");
        }
    }

    if(mark)
    {
        cvSaveImage(ch, imageDst);
        startThread(THRESHOLD);
    }

    return true;
}

bool LinkPro::transistmPyrDown()
{
    volatile bool mark = false;

    saveFileName.clear();
    if(!imageIsEmpty())
    {
        qDebug() << "This maybe can just deal with single-channel picture, please change set in MCvImgPro.cpp "
                 << "filter is: "
                 << dialogPro->visitCurrentPydown().filter;
    }
    else
    {
        return false;
    }

    saveFileName = generateRode("tempPyrup");

    std::string str = saveFileName.toStdString();
    const char *ch = str.c_str();

    if(!saveFileName.isEmpty())
    {
         QString roadPyrDown("pyrdown");
         if(!QImage(saveFileName).isNull())
         {
             bool isOk = generateFile(roadPyrDown, saveFileName, count_pyrdown, bufferPyrDown);
             if(!isOk)
                   QMessageBox::warning(dialogPro, "Warning", "Buffer Error! Please check your image style");
         }
    }

    if(mark)
    {
        cvSaveImage(ch, imageDst);
        startThread(PYRDOWN);
    }

     return true;
}
bool LinkPro::transistmPyrUp()
{
    volatile bool mark = false;

    saveFileName.clear();
    if(!imageIsEmpty())
    {
        qDebug() << "This maybe can just deal with single-channel picture, please change set in MCvImgPro.cpp "
                 << "filter is: "
                 << dialogPro->visitCurrentPyup().filter;
    }
    else
    {
        return false;
    }

    saveFileName =  generateRode("tempPyrup");

    std::string str = saveFileName.toStdString();
    const char *ch = str.c_str();

    if(!saveFileName.isEmpty())
    {
        QString roadPyrup("floodfill");
        if(!QImage(saveFileName).isNull())
        {
            bool isOk = generateFile(roadPyrup, saveFileName, count_pyrup, bufferPyrUp);
            if(!isOk)
                  QMessageBox::warning(dialogPro, "Warning", "Buffer Error! Please check your image style");
        }
    }

    if(mark)
    {
        cvSaveImage(ch, imageDst);
        startThread(PYRUP);
    }

    return true;
}

/// ![0]
void LinkPro::showProDialog()
{
    dialogPro->show();
}


