
#ifndef MYVIDEOCAPTURE_H
#define MYVIDEOCAPTURE_H

#include <QPixmap>
#include <QImage>
#include <QThread>
#include <opencv2/opencv.hpp>

#define ID_CAMERA 0

class MyVideoCapture : public QThread
{
    Q_OBJECT
public:
    MyVideoCapture(QObject *parent = nullptr);
    QPixmap klatka();

    QPixmap pixmap() const
    {
        return mPixmap;
    }
signals:
    void newPixmapCaptured(); // imagen nueva ha sido capturada
    //void nowaklatka();
protected:
    void run() override;
private:
    QPixmap mPixmap; // imagen de Qt
    cv::Mat mFrame; // imagen de OpenCV
    cv::VideoCapture mVideoCap; // capturador de video
    cv::Mat frame2;
    QPixmap frame3;
    cv::Mat frame4;

    double max; cv::Point maxLoc; std::string temp; int temp2; cv::Point skalowanie;

    QImage cvMatToQImage(const cv::Mat &inMat);
    QPixmap cvMatToQPixmap(const cv::Mat &inMat);
};

#endif // MYVIDEOCAPTURE_H
