//** Kinect Aided Navigation System for blind people **/
//** Author: Dr. Nadia Kanwal and Dr. Erkan Bostanci **/
//** Date: 30-12-2011                     **/
//** Contact: VASE lab, School of Computer Science  **/
//** and Electronic Engineering, University of Essex **/
//** Reference  
//** Email: nadia.tahseen@gmail.com, gebost@essex.ac.uk
Reference: 
Kanwal, N., Bostanci, E., and Clark, A. F, “ Kinect Aided Navigation System for Visually Impaired People,” Proceedings of theWorkshop on Recognition and Action for Scene Understanding (REACTS 2013), York, UK, 30- 31 August 2013.
**/

#include "KinectHandler.h"

KinectHandler::KinectHandler()
{
    device = &freenect.createDevice<MyFreenectDevice>(0);

    depthMat = cvCreateMat(480,640, CV_16UC1);
    depthf = cvCreateMat(480,640, CV_16UC1);
    rgbMat = cvCreateMat(480,640, CV_16UC3);


    rgbImage = cvCreateImage(cvSize(480,640), 8,3);
    depthImage = cvCreateImage(cvSize(480,640), 16,1);

    // for converting a cv::mat to IPlImage *
    Mat imgMat(rgbImage);
    Mat imgMat2(depthImage);
}

void KinectHandler::startDevice()
{
    device->startVideo();
	device->startDepth();
}

void KinectHandler::stopDevice()
{
    device->stopVideo();
	device->stopDepth();
}

float KinectHandler::rawDepthToMeters(uint16_t rawDepth)
{

    // distance = 0.1236 * tan(rawDisparity / 2842.5 + 1.1863) //magnenat's formula
    // distance = 1.0 / (rawDepth * -0.0030711016 + 3.3309495161); //burrus formula
    float distance = 0.1236 * tan(rawDepth / 2842.5 + 1.1863);

    return distance;
}

void KinectHandler::captureImageAndDepth()
{
    device->getVideo(rgbMat);
    device->getDepth(depthMat);

    // preivous code to show rgbMat
     //cv::imshow("rgb", rgbMat);

    // convert depth to meters
    // int a = depthMat.at<int>(320,240);
    // printf("depth=%d\n", a);

//    for(int j=0;j<640;j++)
//    {
//       for(int i=0;i<480;i++)
//       {
//         depthMatMetric = rawDepthToMeters(depthMat.at<double>(i,j));
//       }
//    }

    // convert rgbMat to iplImage
    *rgbImage = rgbMat;
    *depthImage = depthMat;

    //cvCircle(image, cvPoint(320,240),5, cvScalar(1,1,0), 5);
    //cvShowImage("rgb image", rgbImage);
    //cvShowImage("depth ipl image", depthImage);

    //CvScalar s = cvGet2D(depthImage, 240, 320);
    //printf("ipl image %d \n", (int)s.val[0]);

    depthMat.convertTo(depthf, CV_8UC1, 255.0/2048.0); // 2048
    //*depthImage = depthf;
    //cv::imshow("depth image",depthf);
   // cvWaitKey(100);
}


