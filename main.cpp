#include "stdafx.h"
#include <cv.h>
#include <highgui.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <windows.h>
#define PI 3.14159265

using namespace std;
using namespace cv;
int gx = 0, gy = 0; //initial position of the mouse
bool mouse = 0; // mouse toggled?
int x_pcs = 0, y_pcs = 0;
// Computer mouse status processing function
void mouseEvent(int evt, int x, int y, int flags, void *param)
{
    Mat *rgb = (Mat *)param;
    if (evt == CV_EVENT_LBUTTONDOWN)
    {
        mouse = 1;
        gx = x;
        gy = y;
    }
}

int main()
{
    try
    {

        VideoCapture cap(0); // Open standard camera

        // If camera is not found
        if (!cap.isOpened())
        {
            cout << "Камера не найдена!" << endl;
            Sleep(1500);
            return -1;
        }
        Mat image, grayscale;
        namedWindow("capture", 1);
        int iSliderValue1 = 80;
        //Create a slider to set the minimum length of the segment of the workpiece contour
        createTrackbar("Мин.длина", "capture", &iSliderValue1, 400);

        //Creating a slider to change the maximum distance between segments of recognition contours
        int iSliderValue2 = 1;
        createTrackbar("Max.dst", "capture", &iSliderValue2, 20);
        int dif[4] = {0, 0, 0, 0};
        int flag1 = 0;
        double angle = 0.0;
        for (;;)
        {
            int minLineLength = iSliderValue1;
            int maxlengthgap = iSliderValue2;

            cap >> image; // receiving new image from camera
            cvtColor(image, grayscale, CV_BGR2GRAY);
            GaussianBlur(grayscale, grayscale, Size(3, 3), 2, 2);
            Canny(grayscale, grayscale, 50, 200, 3);
            vector<Vec4i> lines;

            // Hough transorm initialisation
            HoughLinesP(grayscale, lines, 1, CV_PI / 180, 80, minLineLength, maxlengthgap);
            for (size_t i = 0; i < lines.size(); i++)
            {
                line(image, Point(lines[i][0], lines[i][1]), Point(lines[i][2], lines[i][3]), Scalar(0, 0, 255), 3, 8);
                if (abs(lines[i][0] - gx) < 3 && abs(lines[i][1] - gy) < 3)
                {
                    dif[0] = lines[i][0];
                    dif[1] = lines[i][1];
                    dif[2] = lines[i][2];
                    dif[3] = lines[i][3];
                    angle = atan2(abs(dif[3] - dif[1]), abs(dif[2] - dif[0])) * 180 / PI;
                    flag1 = 1;
                }
                if (abs(lines[i][2] - gx) < 3 && abs(lines[i][3] - gy) < 3)
                {

                    dif[0] = lines[i][0];
                    dif[1] = lines[i][1];
                    dif[2] = lines[i][2];
                    dif[3] = lines[i][3];

                    angle = atan2(abs(dif[3] - dif[1]), abs(dif[2] - dif[0])) * 180 / PI;
                    flag1 = 2;
                }
                // Information output about the coordinate of the reference point of the workpiece to the command line
                if (mouse)
                {
                    if (flag1 == 1)
                    {
                        printf("%d %d   %f\n", dif[0], dif[1], angle);
                        x_pcs = dif[0];
                        y_pcs = dif[1];
                    }
                    if (flag1 == 2)
                    {
                        printf("%d %d   %f\n", dif[2], dif[3], angle);
                        x_pcs = dif[2];
                        y_pcs = dif[3];
                    }

                    flag1 = 0;
                    mouse = 0;
                }
            }

            imshow("capture", image);
            setMouseCallback("capture", mouseEvent, &image);
            // Completion of the main program cycle after pressing the Enter key after 30 milliseconds
            if (waitKey(30) >= 0)
                break;
        }
        vector<int> attribute(2);
        attribute[0] = CV_IMWRITE_JPEG_QUALITY;
        attribute[1] = 90;
        // Saving an image from a webcam with recognized contours
        imwrite(string("capture.jpg"), image, attribute);
        ofstream outputfile;
        //Recording information about the coordinate of the point of the workpiece angle 
        //and the angle of rotation of the workpiece in degrees in a file data.txt
        outputfile.open("data.txt");
        outputfile << x_pcs;
        outputfile << " ";
        outputfile << y_pcs;
        outputfile << " ";
        outputfile << angle << endl;
        return 0;
    }
    // If the image cannot be generetated 
    catch (string msg)
    {
        cerr << "Cannot save the image" << endl
             << msg << endl;
        return -1;
    }
}
