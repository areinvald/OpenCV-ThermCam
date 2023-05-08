#include "myvideocapture.h"
#include "mainwindow.h"
#include <string>
//#include "ui_mainwindow.h"
#include <QDebug>

MyVideoCapture::MyVideoCapture(QObject *parent)
    : QThread { parent }
    , mVideoCap { ID_CAMERA }
{
}


void MyVideoCapture::run()
{
    if (mVideoCap.isOpened())
    {
        while (true)
        {
            mVideoCap >> mFrame;
            applyColorMap(mFrame, mFrame, cv::COLORMAP_HOT);
            if (!mFrame.empty())
            {
                //cv::rectangle(mFrame, cv::Point(10, 10), cv::Point(401, 401), cv::Scalar(0, 0, 255), 1);

                mPixmap = cvMatToQPixmap(mFrame);
                emit newPixmapCaptured();
            }

        }
    }
}


QPixmap MyVideoCapture::klatka()
{
    frame2 = mFrame.clone(); // KOPIOWANIE KLATKI
    cv::cvtColor(frame2, frame4, cv::COLOR_BGR2GRAY); // KONWERTOWANIE KLATKI NA NA JEDNOKANAŁOWĄ
    cv::minMaxLoc(frame4, NULL, &max, NULL, &maxLoc ); // ODCZYTYWANIE POLOZENIA NAJJASNIEJSZEGO PIXELA
    cv::circle(frame2, maxLoc, 10, cv::Scalar(255, 0, 0), 5, 8, 0); // RYSOWANIE PUNKTU
    temp2 = (int) max; // KONWERTOWANIE WARTOSCI NAJJASNIEJSZEGO PIXELA NA ZMIENNA TYPU INT

    //----------- TABELA TEMPERATUR --------------

    if (temp2 >= 251)
    {
        temp = " 40";
    }
    else if (temp2 <= 250 && temp2 >= 241)
    {
        temp = " 39,5";
    }
    else if (temp2 < 240 && temp2 >= 231)
    {
        temp = " 39";
    }
    else if (temp2 < 230 && temp2 >= 221)
    {
        temp = " 38,5";
    }
    else if (temp2 < 220 && temp2 >= 201)
    {
        temp = " 38";
    }
    else if (temp2 < 200 && temp2 >= 181)
    {
        temp = " 37,5";
    }
    else if (temp2 < 180 && temp2 >= 161)
    {
        temp = " 37";
    }
    else if (temp2 < 160 && temp2 >= 141)
    {
        temp = " 36,5";
    }
    else if (temp2 < 140 && temp2 >= 121)
    {
        temp = " 36";
    }
    else if (temp2 < 120 && temp2 >= 101)
    {
        temp = " 35,5";
    }
    else if (temp2 < 100)
    {
        temp = " 35";
    }

    //----------- KONIEC TABELI --------------

    // ------- WYPISYWANIE TEMPERATURY KOŁO NAJJASNIEJSZEGO PUNTKU --------
    skalowanie.y = maxLoc.y;
    skalowanie.x = maxLoc.x;
    //std::string s = std::to_string(skalowanie.x);
    if (maxLoc.x >= 550)
    {
        skalowanie.x = maxLoc.x - 100;
    }
    if (maxLoc.y <= 50)
    {
        skalowanie.y = maxLoc.y + 100;
    }
    //cv::putText(frame2, s, cv::Point(500, 100), cv::FONT_HERSHEY_COMPLEX, 2.0, cv::Scalar( 0,255,0));


    cv::putText(frame2, temp, skalowanie, cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 10, 0), 2);

    // ------- WYWOLYWANIE FUNKCJI, KTORA KONWERTUJE CV::MAT NA QPIXMAP ---------------

    frame3 = cvMatToQPixmap(frame2);
    return frame3;
}



QImage MyVideoCapture::cvMatToQImage(const cv::Mat &inMat)
{
    switch (inMat.type())
    {
        // 8-bit, 4 channel
        case CV_8UC4:
        {
            QImage image(inMat.data, inMat.cols, inMat.rows, static_cast<int>(inMat.step), QImage::Format_ARGB32);
            return image;
        }
        // 8-bit, 3 channel
        case CV_8UC3:
        {
            QImage image(inMat.data, inMat.cols, inMat.rows, static_cast<int>(inMat.step), QImage::Format_RGB888);
            return image.rgbSwapped();
        }
        // 8-bit, 1 channel
        case CV_8UC1:
        {
    #if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
            QImage image(inMat.data, inMat.cols, inMat.rows, static_cast<int>(inMat.step), QImage::Format_Grayscale8);
    #else
            static QVector<QRgb>  sColorTable;

            // only create our color table the first time
            if (sColorTable.isEmpty())
            {
                sColorTable.resize(256 );

                for (int i = 0; i < 256; ++i )
                {
                    sColorTable[i] = qRgb(i, i, i );
                }
            }

            QImage image(inMat.data,
                         inMat.cols, inMat.rows,
                         static_cast<int>(inMat.step),
                         QImage::Format_Indexed8 );

            image.setColorTable(sColorTable );
    #endif
            return image;
        }
        default:
        {
            qWarning()<< "ASM::cvMatToQImage()- cv::Mat image type not handled in switch:" << inMat.type();
            break;
        }
    }
    return QImage();
}




QPixmap MyVideoCapture::cvMatToQPixmap(const cv::Mat &inMat)
{
    return QPixmap::fromImage(cvMatToQImage(inMat));
}
