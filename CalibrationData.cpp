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

#include "CalibrationData.h"

CalibrationData::CalibrationData()
{
    depthCameraMatrix = cvCreateMat(3,3,CV_64F);

    cvmSet(depthCameraMatrix,0,0,fx_d); cvmSet(depthCameraMatrix,0,1,0.0);  cvmSet(depthCameraMatrix,0,2,cx_d);
    cvmSet(depthCameraMatrix,1,0,0.0);  cvmSet(depthCameraMatrix,1,1,fy_d); cvmSet(depthCameraMatrix,1,2,cy_d);
    cvmSet(depthCameraMatrix,2,0,0.0);  cvmSet(depthCameraMatrix,2,1,0.0);  cvmSet(depthCameraMatrix,2,2,1.0);

    rgbCameraMatrix = cvCreateMat(3,3,CV_64F);

    cvmSet(rgbCameraMatrix,0,0,fx_rgb); cvmSet(rgbCameraMatrix,0,1,0.0);  cvmSet(rgbCameraMatrix,0,2,cx_rgb);
    cvmSet(rgbCameraMatrix,1,0,0.0);  cvmSet(rgbCameraMatrix,1,1,fy_rgb); cvmSet(rgbCameraMatrix,1,2,cy_rgb);
    cvmSet(rgbCameraMatrix,2,0,0.0);  cvmSet(rgbCameraMatrix,2,1,0.0);  cvmSet(rgbCameraMatrix,2,2,1.0);

    depthDistCoeff = cvCreateMat(1,5,CV_64F);
    cvmSet(depthDistCoeff,0,0, k1_d);
    cvmSet(depthDistCoeff,0,1, k2_d);
    cvmSet(depthDistCoeff,0,2, p1_d);
    cvmSet(depthDistCoeff,0,3, p2_d);
    cvmSet(depthDistCoeff,0,4, k3_d);

    rgbDistCoeff = cvCreateMat(1,5,CV_64F);
    cvmSet(rgbDistCoeff,0,0, k1_rgb);
    cvmSet(rgbDistCoeff,0,1, k2_rgb);
    cvmSet(rgbDistCoeff,0,2, p1_rgb);
    cvmSet(rgbDistCoeff,0,3, p2_rgb);
    cvmSet(rgbDistCoeff,0,4, k3_rgb);

    // rotation
    rotationMatrix = cvCreateMat(3,3,CV_64F);

    cvmSet(rotationMatrix,0,0, 9.9984628826577793e-01);   cvmSet(rotationMatrix,0,1, 1.2635359098409581e-03);  cvmSet(rotationMatrix,0,2, -1.7487233004436643e-02);
    cvmSet(rotationMatrix,1,0, -1.4779096108364480e-03);   cvmSet(rotationMatrix,1,1, 9.9992385683542895e-01);  cvmSet(rotationMatrix,1,2, -1.2251380107679535e-02);
    cvmSet(rotationMatrix,2,0, 1.7470421412464927e-02);   cvmSet(rotationMatrix,2,1, 1.2275341476520762e-02);  cvmSet(rotationMatrix,2,2, 9.9977202419716948e-01);

    //cvTranspose(rotationMatrix, rotationMatrix);

    // translation
    translationMatrix = cvCreateMat(3,1,CV_64F);

//    cvmSet(translationMatrix, 0,0, -1.0 * 1.9985242312092553e-02);
//    cvmSet(translationMatrix, 1,0, -1.0 * -7.4423738761617583e-04);
//    cvmSet(translationMatrix, 2,0, -1.0 * -1.0916736334336222e-02);

    cvmSet(translationMatrix, 0,0, 1.9985242312092553e-02);
    cvmSet(translationMatrix, 1,0, -7.4423738761617583e-04);
    cvmSet(translationMatrix, 2,0, -1.0916736334336222e-02);


    // used for projection
    pointVec = cvCreateMat(3,1,CV_64F);

    mulRes = cvCreateMat(3,1,CV_64F);
    addRes = cvCreateMat(3,1,CV_64F);


    // used for distorted projection
    tVec = cvCreateMat(3,1,CV_64F);
    rVec = cvCreateMat(3,1,CV_64F);

    cvRodrigues2(rotationMatrix, rVec);
}



void CalibrationData::projectPointsWithDistortion(vector<Point3D> * pointList)
{
   // FILE *ff = fopen("projectCal.txt","w");

    int numPoints = (*pointList).size();
    CvMat * objectPoints = cvCreateMat(numPoints, 3, CV_64F);
    CvMat * imagePoints = cvCreateMat(numPoints, 2, CV_64F);
    for(int i=0;i<numPoints;i++)
    {
        cvmSet(objectPoints,i,0,(*pointList)[i].x);
        cvmSet(objectPoints,i,1,(*pointList)[i].y);
        cvmSet(objectPoints,i,2,(*pointList)[i].z);
    }

    cvProjectPoints2(objectPoints, rVec, tVec, rgbCameraMatrix, rgbDistCoeff, imagePoints);

    for(int i=0;i<numPoints;i++)
    {
        (*pointList)[i].x2d_rgb = int(cvmGet(imagePoints, i,0));
        (*pointList)[i].y2d_rgb = int(cvmGet(imagePoints, i,1));
  // fprintf(ff,"%g\t%g\n",cvmGet(imagePoints, i,0) , cvmGet(imagePoints, i,1));
    }
  //  fflush(ff);
  //  fclose(ff);

    cvReleaseMat(&objectPoints);
    cvReleaseMat(&imagePoints);
}

void CalibrationData::projectPoints(vector<Point3D> * pointList)
{
    double x,y,z;
    int size = (*pointList).size();
    for(int i=0;i<size;i++)
    {
//      printf("point %d\n", i);
        cvmSet(pointVec, 0, 0, (*pointList)[i].x);
        cvmSet(pointVec, 1, 0, (*pointList)[i].y);
        cvmSet(pointVec, 2, 0, (*pointList)[i].z);

//      printf("initial 3d:\n");
//      printf("%f %f %f\n", cvmGet(pointVec, 0,0),cvmGet(pointVec, 1,0),cvmGet(pointVec, 2,0));

        cvMatMul(rotationMatrix, pointVec, mulRes);
        cvAdd(mulRes, translationMatrix, addRes);

        x = cvmGet(addRes, 0, 0);
        y = cvmGet(addRes, 1, 0);
        z = cvmGet(addRes, 2, 0);

//      printf("final(after R and t) 3d:\n");
//      printf("%f %f %f\n", x,y,z);

//      P2D_rgb.x = (P3D'.x * fx_rgb / P3D'.z) + cx_rgb
        (*pointList)[i].x2d_rgb = (int)(x*fx_rgb/z) + cx_rgb;
        (*pointList)[i].y2d_rgb = (int)(y*fy_rgb/z) + cy_rgb;

//      printf("projection result:\n");
//      printf("%d %d\n", (*pointList)[i].x2d_rgb,(*pointList)[i].y2d_rgb);

        //getchar();
    }
}

void CalibrationData::projectPoint(Point3D * point)
{
    double x,y,z;

    cvmSet(pointVec, 0, 0, point->x);
    cvmSet(pointVec, 1, 0, point->y);
    cvmSet(pointVec, 2, 0, point->z);

//      printf("initial 3d:\n");
//      printf("%f %f %f\n", cvmGet(pointVec, 0,0),cvmGet(pointVec, 1,0),cvmGet(pointVec, 2,0));

    cvMatMul(rotationMatrix, pointVec, mulRes);
    cvAdd(mulRes, translationMatrix, addRes);

    x = cvmGet(addRes, 0, 0);
    y = cvmGet(addRes, 1, 0);
    z = cvmGet(addRes, 2, 0);

//      printf("final(after R and t) 3d:\n");
//      printf("%f %f %f\n", x,y,z);


//      P2D_rgb.x = (P3D'.x * fx_rgb / P3D'.z) + cx_rgb
    point->x2d_rgb = (int)(x*fx_rgb/z) + cx_rgb;
    point->y2d_rgb = (int)(y*fy_rgb/z) + cy_rgb;

}


