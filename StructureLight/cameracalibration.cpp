#include "cameracalibration.h"
#include <QFileInfo>
#include <QDebug>
#include <QDir>


cameraCalibration::cameraCalibration(QObject *parent,QString calibrationFilesPath) : QObject(parent)
{
    this->calibrationFilesPath = calibrationFilesPath;
    QFileInfo fileinfo(this->calibrationFilesPath);
    if(!fileinfo.isDir())
    {
        cout << "没有找到文件夹" << endl;
        assert(fileinfo.isDir());
    }
    QDir dir(this->calibrationFilesPath);
    QStringList nameFilters;
    nameFilters << "*.jpg" << "*.png"<< "*.bmp";
    this->files = dir.entryList(nameFilters, QDir::NoSymLinks|QDir::Files|QDir::Readable|QDir::NoDotAndDotDot , QDir::Name);
    ofstream fout(this->calibrationFilesPath.toStdString()+"/caliberation_result.txt");   //保存标定结果的文件
}


void cameraCalibration::findChessboardCorners_CameraCalibration()
{
    int image_num = 0;
    foreach (QString var, this->files)
    {
        image_num ++;
        qDebug() << "image path : " << var << endl;
        string imagePath = var.toStdString();
        cv::Mat imageInput = cv::imread(imagePath);
        if(image_num == 1)
        {
            this->image_size.width = imageInput.cols;
            this->image_size.height = imageInput.rows;
            cout << "image_size.width = " << image_size.width << endl;
            cout << "image_size.height = " << image_size.height << endl;
        }

        if (findChessboardCorners(imageInput, pattern_size, corner_points_buf) == 0)
        {
            cout << "can not find chessboard corners!\n";   //找不到角点
            exit(1);
        }
        else
        {
            cv::Mat gray;
            //cv::cvtColor(imageInput, gray,  cv::COLOR_BGR2GRAY);
            gray = imageInput;
            cv::find4QuadCornerSubpix(gray, this->corner_points_buf, cv::Size(5, 5));
            corner_points_of_all_imgs.push_back(this->corner_points_buf);
            cv::drawChessboardCorners(gray, this->pattern_size, this->corner_points_buf, true);

            QFileInfo fileinfo(var);
            //文件名
            QString image_name = fileinfo.fileName();

            //绝对路径
            QString image_path = fileinfo.absolutePath();

            cv::imwrite(image_path.toStdString()+"/to_string(image_num)"+image_name.toStdString(),gray);
            cv::imshow("camera calibration:"+to_string(image_num), gray);
            cv::waitKey(100);
        }

    }
    unsigned long long total = this->corner_points_of_all_imgs.size();
    cout << "total=" << total << endl;
    int cornerNum = pattern_size.width * pattern_size.height;//每张图片上的总的角点数
    for (int i = 0; i < total;i++)
    {
        qDebug() << "-->" << this->files[i]<< "-->"<<QString::fromStdWString(L"图片的数据 :")<< endl;
        for (int j = 0;j < cornerNum;j++)
        {
            cout << "point" << corner_points_of_all_imgs[i][j].x;
            cout << "-->" << corner_points_of_all_imgs[i][j].y;
            if ((j + 1) % 3 == 0)
            {
               cout << endl;
            }
            else
            {
                   cout.width(10);
            }
           }
           cout << endl;
       }

       cout << endl << "角点提取完成" << endl;
}


