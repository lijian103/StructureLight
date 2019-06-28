#ifndef CAMERACALIBRATION_H
#define CAMERACALIBRATION_H
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <QObject>
using namespace std;
using namespace cv;
class cameraCalibration : public QObject
{
    Q_OBJECT
public:
    explicit cameraCalibration(QObject *parent = nullptr, QString calibrationFilesPath = "../../../../Pictures/Grap_frame");
    void findChessboardCorners_CameraCalibration();
    QString calibrationFilesPath;
    QStringList files;
    cv::Size image_size;//保存图片大小
    cv::Size pattern_size = cv::Size(7, 9);//标定板上每行、每列的角点数；测试图片中的标定板上内角点数为4*6
    vector<cv::Point2f> corner_points_buf;//建一个数组缓存检测到的角点，通常采用Point2f形式
    vector<cv::Point2f>::iterator corner_points_buf_ptr;
    vector<vector<cv::Point2f>> corner_points_of_all_imgs;


signals:

public slots:
};

#endif // CAMERACALIBRATION_H
