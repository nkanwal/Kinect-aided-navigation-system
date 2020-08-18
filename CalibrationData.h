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

#ifndef CALIBRATIONDATA_H
#define CALIBRATIONDATA_H

#include "Definitions.h"

#include "Point3D.h"


using namespace std;

// colour parameters
#define fx_rgb 5.2921508098293293e+02
#define fy_rgb 5.2556393630057437e+02
#define cx_rgb 3.2894272028759258e+02
#define cy_rgb 2.6748068171871557e+02
#define k1_rgb 2.6451622333009589e-01
#define k2_rgb -8.3990749424620825e-01
#define p1_rgb -1.9922302173693159e-03
#define p2_rgb 1.4371995932897616e-03
#define k3_rgb 9.1192465078713847e-01


// depth parameters
#define fx_d 5.9421434211923247e+02
#define fy_d 5.9104053696870778e+02
#define cx_d 3.3930780975300314e+02
#define cy_d 2.4273913761751615e+02
#define k1_d -2.6386489753128833e-01
#define k2_d 9.9966832163729757e-01
#define p1_d -7.6275862143610667e-04
#define p2_d 5.0350940090814270e-03
#define k3_d -1.3053628089976321e+00



class CalibrationData
{
    public:
        CalibrationData();

        void projectPointsWithDistortion(vector<Point3D> * pointList);
        void projectPoints(vector<Point3D> * pointList);
        void projectPoint(Point3D * point);

        CvMat * pointVec;
        CvMat * mulRes;
        CvMat * addRes;

        CvMat * depthCameraMatrix;
        CvMat * rgbCameraMatrix;
        CvMat * depthDistCoeff;
        CvMat * rgbDistCoeff;
        CvMat * rotationMatrix;
        CvMat * translationMatrix;

        CvMat * rVec;
        CvMat * tVec;
};

#endif // CALIBRATIONDATA_H
