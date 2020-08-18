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

#include "detectDescribe.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <list>
#include "Point.h"
#include "SEA.h"
#include "Timer.h"


bool areCloseFeatures(double , double , double , double );


//using namespace std;

list<CornerDescriptor> detectDescribe::detectCorners(IplImage *frame)
{
    SEA sea;
    Timer timer;
        IplImage* gray_frame,eigImage,temp_image;
        int w,h,count,Totalcorners;
        const int MAX_CORNERS =800;
		CvPoint2D32f corners[MAX_CORNERS] = {0};
		CvPoint2D32f Framecorners[MAX_CORNERS] = {0};
		int corner_count = MAX_CORNERS;
		double quality_level =0.05;
		double min_distance = 5;
		int eig_block_size = 3;
		int use_harris = false;
		list<CornerDescriptor> frame_corners;
		w = frame->width;
        h = frame->height;

            sea.readSize(w,h);
            eig_image  = cvCreateImage(cvSize(w, h),IPL_DEPTH_32F, 1);
            temp_image = cvCreateImage(cvSize(w, h),IPL_DEPTH_32F, 1);
            gray_frame = cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U, 1);
            cvCvtColor(frame, gray_frame, CV_BGR2GRAY);
            w = gray_frame->width;
            h = gray_frame->height;

            cvGoodFeaturesToTrack(gray_frame,eig_image, temp_image,corners,&corner_count,quality_level,min_distance,NULL,eig_block_size,use_harris);
            Totalcorners = corner_count;
            cvSmooth(gray_frame,gray_frame,CV_GAUSSIAN,5,0);
            frame_corners =  sea.getDescriptor(&gray_frame,corners,Totalcorners);
}

bool areCloseFeatures(double x1, double y1, double x2, double y2)
{
    double distanceThreshold = 10.0;

    double result=0.0;
    result = sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
   // printf("distance %g\n",result);
    if(result <= distanceThreshold)
        return true;
    else
        return false;
}
