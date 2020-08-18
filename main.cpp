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
#include <stdio.h>
#include <iostream>
#include <list>
#include "CalibrationData.h"
#include "KinectHandler.h"
#include "Point3D.h"
#include "PlaneExtractor.h"
#include "speak.cpp"
#include <string>
#include "Timer.h"
#include <math.h>
#include <map>

using namespace std;
 FILE *outPtr= fopen("System_Verbal_OutPutKeith.txt","a");
int leftHurdle=0;
int rightHurdle=0;
int noPath =0 ;
int topM = 400;
double euclideanDeistance(double x1,double x2,double y1,double y2)
{
    return sqrt(((x2-x1)*(x2-x1))+((y2-y1)*(y2-y1)));
}

double convertRawDepthToMeters(int rawDepth)
{
   //return double(1.0/(double(rawDepth) * -0.0030711016 + 3.3309495161));
   return (0.1236 * tan(double(rawDepth) / 2842.5 + 1.1863)+(-0.037));
}

vector<Point3D> getKinectData(cv::Mat Matrix )// char imgDepth[80])//char imgDepth[80])
{
    srand(time(NULL));
    Timer timer;
    CalibrationData calibrationData;
    //PlaneExtractor planeExtractor;
    int depthMatrix[HEIGHT][WIDTH];
    int count,j,k, WinCount;

        //FILE * fptr = fopen(imgDepth, "r");

        //printf("%s\n",imgDepth);
        int value;
        int x,y;
//
        for(y=0;y<HEIGHT;y++)
        {
            for(x=0;x<WIDTH;x++)
            {
             // fscanf(fptr, "%d ", &value);
                depthMatrix[y][x] = Matrix.at<uint16_t>(y,x);//value;//
            }
          //fscanf(fptr, "\n");
        }
        //fclose(fptr);
        vector<Point3D> pointList;
        int rawDepth, pixCount=0;
        CvScalar pix;
        double avegDepth=0;

        //FILE *fft = fopen("winDepth.txt","w");
        pointList.clear();
        for(x=0;x<WIDTH;x++)
        {
            for(y=0;y<HEIGHT;y++)
            {
                Point3D point3d;
                rawDepth = depthMatrix[y][x];
                if(rawDepth != 2047)                {
                    double depth = convertRawDepthToMeters(rawDepth);
                    point3d.x = double((x - cx_d) * depth / fx_d);
                    point3d.y = double((y - cy_d) * depth / fy_d);
                    point3d.z = double(depth);
                    point3d.x2d_depth = x;
                    point3d.y2d_depth = y;
                    point3d.rawDepth = rawDepth;
                    point3d.isCalculated = true;
                    pointList.push_back(point3d);
                }
                else
                {
                    point3d.x = 0;
                    point3d.y = 0;
                    point3d.z = 0;
                    point3d.x2d_depth = x;
                    point3d.y2d_depth = y;
                    point3d.rawDepth = rawDepth;
                    point3d.isCalculated = false;
                    pointList.push_back(point3d);
                }
            }
           // fflush(fft);
        }
       // fclose(fft);

        int countpix = 0;
        WinCount = 0;

        for(int i=0;i<pointList.size();i++)
        {
            if(!pointList[i].isCalculated)
            {

               if(pointList[i].y2d_depth < HEIGHT-Margin && pointList[i].x2d_depth < WIDTH -Margin &&
                  pointList[i].y2d_depth > Margin && pointList[i].x2d_depth > Margin)
                {
                double avgDepth=0;
                int counter=0;
                for(j=-Margin;j<=Margin;j++)
                    for(k=-Margin;k<=Margin;k++)
                    {
                        if(depthMatrix[pointList[i].y2d_depth+j][pointList[i].x2d_depth+k] != 2047)
                        {
                          avgDepth += depthMatrix[pointList[i].y2d_depth+j][pointList[i].x2d_depth+k];
                          counter++;
                        }
                    }
                    avgDepth = (double) (avgDepth/counter);
                    if(avgDepth == 2047)
                        pointList[i].isCalculated = false;
                    else
                    {

                    double depth = convertRawDepthToMeters(avgDepth);
                    countpix++;
                    pointList[i].x = double((pointList[i].x2d_depth - cx_d) * depth / fx_d);
                    pointList[i].y = double((pointList[i].y2d_depth - cy_d) * depth / fy_d);
                    pointList[i].z = double(depth);
                    pointList[i].isCalculated = true;
                    }
                }
            }

        }


      //  calibrationData.projectPointsWithDistortion(&pointList);
       calibrationData.projectPoints(&pointList);

  return pointList;
}


bool WallLikeHurdle(vector<Point3D> pointList, IplImage *templateImg)
{
    CvScalar pix;
    int pixCount=0;
    int WinCount=0;
    double avegDepth=0.0;
       // fprintf("checking avg depth\n");
     for(int i=0;i<pointList.size();i++)
     {
        pix = cvGet2D(templateImg,pointList[i].y2d_depth,pointList[i].x2d_depth);

        //======================================================
                if((pix.val[0] == 255.0) &&  pointList[i].isCalculated != false && pointList[i].y <= HeightThresh)
                {
                  avegDepth += (pointList[i].z);
                  pixCount++;
                }

                else if((pix.val[0] == 255.0) &&  pointList[i].isCalculated  == true)
                  WinCount++;
                 // printf("false pixels %d\n",WinCount);
       //======================================================
     }
                avegDepth = avegDepth/pixCount;
             //   avegDepth = convertRawDepthToMeters(avegDepth);
                fprintf(outPtr,"%g\t",avegDepth);
                fflush(outPtr);
               // cvWaitKey(1);
                if(avegDepth < DistanceThresh)
                   return 1;
                else
                   return 0;
}
double leftSafeDistance(vector<Point3D> pointsList,  vector<cornerPoint> cornersList, std::map<std::pair<int,int>, Point3D> CornerMap,  IplImage *lefttemplateImg )
{
    vector<cornerPoint>::iterator listIt;
    bool turnFlag = true;
    CvScalar pix;
    double dist;
    int count=0;
    double avgDistance= 0;


     for(listIt=cornersList.begin();listIt!= cornersList.end();listIt++)
       {
           Point3D Cpoint;
           Cpoint.x =  CornerMap[std::make_pair(listIt->Cx_rgb,listIt->Cy_rgb)].x2d_rgb;
           Cpoint.y =  CornerMap[std::make_pair(listIt->Cx_rgb,listIt->Cy_rgb)].y2d_rgb;
           Cpoint.x2d_rgb = listIt->Cx_rgb;
           Cpoint.y2d_rgb = listIt->Cy_rgb;

           pix = cvGet2D(lefttemplateImg,listIt->Cy_rgb,listIt->Cx_rgb);
           if(pix.val[0] == 255)
                Cpoint.isHazard = true;
           else
                Cpoint.isHazard = false;

           Cpoint.z =  CornerMap[std::make_pair(listIt->Cx_rgb,listIt->Cy_rgb)].z;
           Cpoint.x2d_depth = CornerMap[std::make_pair(listIt->Cx_rgb,listIt->Cy_rgb)].x2d_depth;
           Cpoint.y2d_depth = CornerMap[std::make_pair(listIt->Cx_rgb,listIt->Cy_rgb)].y2d_depth;
           Cpoint.rawDepth = CornerMap[std::make_pair(listIt->Cx_rgb,listIt->Cy_rgb)].rawDepth;

            if(Cpoint.isHazard)
            {
                if (Cpoint.z < DistanceThresh && Cpoint.z > 0 && Cpoint.y < HeightThresh)
                {
                 turnFlag = false;
                  return(Cpoint.z);
                }
                else if(Cpoint.z < DistanceThresh + safeDistance && Cpoint.z >= DistanceThresh)
                {
                    turnFlag = true;
                    avgDistance += Cpoint.z;
                    count++;
                }
                else
                {
                   turnFlag = true;
                   avgDistance += Cpoint.z;
                   count++;
                }
            }
       }
       return avgDistance/count;

}
double rightSafeDistance(vector<Point3D> pointsList,  vector<cornerPoint> cornersList, std::map<std::pair<int,int>, Point3D> CornerMap,  IplImage *righttemplateImg)
{

    vector<cornerPoint>::iterator listIt;
    bool turnFlag = true;
    CvScalar pix;
    double avgDistance = 0;
    int count =0 ;
    double dist;

              for(listIt=cornersList.begin();listIt!= cornersList.end();listIt++)
               {

                   Point3D Cpoint;

                   Cpoint.x =  CornerMap[std::make_pair(listIt->Cx_rgb,listIt->Cy_rgb)].x2d_rgb;
                   Cpoint.y =  CornerMap[std::make_pair(listIt->Cx_rgb,listIt->Cy_rgb)].y2d_rgb;
                   Cpoint.x2d_rgb = listIt->Cx_rgb;
                   Cpoint.y2d_rgb = listIt->Cy_rgb;
                   pix = cvGet2D(righttemplateImg,listIt->Cy_rgb,listIt->Cx_rgb);

                   if(pix.val[0] == 255)
                        Cpoint.isHazard = true;
                   else
                        Cpoint.isHazard = false;

                   Cpoint.z =  CornerMap[std::make_pair(listIt->Cx_rgb,listIt->Cy_rgb)].z;
                   Cpoint.x2d_depth = CornerMap[std::make_pair(listIt->Cx_rgb,listIt->Cy_rgb)].x2d_depth;
                   Cpoint.y2d_depth = CornerMap[std::make_pair(listIt->Cx_rgb,listIt->Cy_rgb)].y2d_depth;
                   Cpoint.rawDepth = CornerMap[std::make_pair(listIt->Cx_rgb,listIt->Cy_rgb)].rawDepth;

                if(Cpoint.isHazard)
                {
                    if (Cpoint.z < DistanceThresh && Cpoint.z > 0 && Cpoint.y < HeightThresh)
                    {

                       turnFlag = false;
                      return Cpoint.z;
                    }
                    else if(Cpoint.z < DistanceThresh +safeDistance && Cpoint.z >= DistanceThresh)
                    {
                        turnFlag = true;
                        avgDistance += Cpoint.z;
                        count++;
                    }
                    else
                    {
                       turnFlag = true;
                       avgDistance += Cpoint.z;
                       count++;
                    }
                }
        }
        return avgDistance/count;
}


void displayTemplate( IplImage *rgbImage, int part, IplImage *templateImg, IplImage *lefttemplateImg, IplImage *righttemplateImg)
{
    double avgIntensity;
      CvScalar pix,sMid, sLeft, sRight, s_new;
      IplImage *newImage = cvCreateImage(cvSize(rgbImage->width,rgbImage->height),rgbImage->depth,rgbImage->nChannels);
      cvCopy(rgbImage,newImage);
     //Define and show the active area
            for(int ii=0;ii<rgbImage->width;ii++)
                for(int jj=0;jj<rgbImage->height;jj++)
                {
                    sMid  = cvGet2D(templateImg,jj,ii);
                    s_new = cvGet2D(newImage,jj,ii);
                    avgIntensity += ((s_new.val[0]+s_new.val[1]+s_new.val[2])/3);
                if(part == 0)
                {
                    if(sMid.val[0] != 255)
                    {
                        s_new.val[0] = s_new.val[0] - 20;
                        s_new.val[1] = s_new.val[1] - 20;
                        s_new.val[2] = s_new.val[2] - 20;
                        cvSet2D(newImage,jj,ii,s_new);
                    }

                }

                if(part == 1) //left
                {
                     sLeft = cvGet2D(lefttemplateImg,jj,ii);
                    if(sLeft.val[0] != 255)
                    {
                        s_new = cvGet2D(newImage,jj,ii);
                        s_new.val[0] = s_new.val[0] - 20;
                        s_new.val[1] = s_new.val[1] - 20;
                        s_new.val[2] = s_new.val[2] - 20;
                        cvSet2D(newImage,jj,ii,s_new);
                    }

                }
               if(part == 2) //right
                {
                    sRight = cvGet2D(righttemplateImg,jj,ii);
                    if(sRight.val[0] != 255)
                    {
                        s_new = cvGet2D(newImage,jj,ii);
                        s_new.val[0] = s_new.val[0] - 20;
                        s_new.val[1] = s_new.val[1] - 20;
                        s_new.val[2] = s_new.val[2] - 20;
                        cvSet2D(newImage,jj,ii,s_new);
                    }
                }


            }
          //    printf("Average Intensity %g\n", avgIntensity / (rgbImage->width*rgbImage->height));

           //   cvShowImage("Template",newImage);
           //   cvWaitKey(10);
              //cvSaveImage("CornerDepth.png",newImage);
           //   cvReleaseImage(&newImage);
}

vector<Point3D> cornersDepth(vector<Point3D> pointsList,  vector<cornerPoint> cornersList,  IplImage *templateImg, IplImage *lefttemplateImg, IplImage *righttemplateImg)
{
    std::map<std::pair<int,int>, Point3D> myMap; //To create a lookup table

    vector<cornerPoint>::iterator listIt;
    CvScalar pix,sMid, sLeft, sRight, s_new;
    std::map<std::pair<int,int>, Point3D>::const_iterator it;
    int key=-1;
    IplImage *newImage;
    vector<Point3D> corners3dList;
    vector<cornerPoint> redCorners, blueCorners, greenCorners;
    cornerPoint corner;
       //  FILE *fid = fopen("3dPoints.txt","w");
       //  cout<<"point list size\t=="<<pointsList.size()<<endl;
       for(int i=0;i<pointsList.size();i++)
        {
             Point3D point;
             point.x = pointsList[i].x;
             point.y = pointsList[i].y;
             point.z = pointsList[i].z;
             point.x2d_depth = pointsList[i].x2d_depth;
             point.y2d_depth = pointsList[i].y2d_depth;
             point.x2d_rgb = pointsList[i].x2d_rgb;
             point.y2d_rgb = pointsList[i].y2d_rgb;
             point.rawDepth = pointsList[i].rawDepth;

           //  fprintf(fid,"%g\t%g\t%g\t%d\t%d\t%g\t%g\n",pointsList[i].x,pointsList[i].y,pointsList[i].z,
           //  pointsList[i].x2d_depth,pointsList[i].y2d_depth,(pointsList[i].x2d_rgb),(pointsList[i].y2d_rgb));
           //  fprintf(fid,"map_points\t %d\t%d\n",(int)pointsList[i].x2d_rgb,(int)pointsList[i].y2d_rgb);
             myMap[std::make_pair((int)pointsList[i].x2d_rgb,(int)pointsList[i].y2d_rgb)] = point;
        }
      // cout<<"map size\t===="<<myMap.size()<<endl;

       int i,j, noMatch=0;
       double avgIntensity = 0;

       // newImage = cvCreateImage(cvSize(rgbImage->width,rgbImage->height),rgbImage->depth,rgbImage->nChannels);
        //cvCopy(rgbImage,newImage);

//*******************************************************
//     cvDrawCircle(newImage,cvPoint(rgbImage->width/2,rgbImage->height/2),4, cvScalar(0,255,255),2);
//     cvDrawLine(newImage,cvPoint(rgbImage->width/2,0),cvPoint(rgbImage->width/6,rgbImage->height),cvScalar(0,255,255),1,8,0);
//     cvDrawLine(newImage,cvPoint(rgbImage->width/2,0),cvPoint(rgbImage->width/2+(rgbImage->width/2-rgbImage->width/6),rgbImage->height),cvScalar(0,255,255),1,8,0);
//     cvDrawLine(newImage,cvPoint(0,430),cvPoint(rgbImage->width,430),cvScalar(0,255,255),1,8,0);
       int noDepthCorners , nonHazard;
       noDepthCorners = nonHazard = 0;

      // printf("processing ----------%d -----corners\n",cornersList.size());

       redCorners.clear();
       blueCorners.clear();
       greenCorners.clear();
       int dd;

       for(listIt=cornersList.begin();listIt!= cornersList.end();listIt++)
        {
            dd=0;
           Point3D Cpoint, tempPoint;
           double height;

          // projectionX = cvGet2D()

           Cpoint.x =  myMap[std::make_pair(listIt->Cx_rgb,listIt->Cy_rgb)].x2d_rgb; // Projected x loc
           Cpoint.y =  myMap[std::make_pair(listIt->Cx_rgb,listIt->Cy_rgb)].y2d_rgb; // projected y loc

           Cpoint.x2d_rgb = listIt->Cx_rgb; //Actual corner x loc
           Cpoint.y2d_rgb = listIt->Cy_rgb; //Actual corner y loc

           pix = cvGet2D(templateImg,listIt->Cy_rgb,listIt->Cx_rgb);

           if(pix.val[0] == 255)
                Cpoint.isHazard = true;
           else
                Cpoint.isHazard = false;

           Cpoint.z =  myMap[std::make_pair(listIt->Cx_rgb,listIt->Cy_rgb)].z;
           Cpoint.x2d_depth = myMap[std::make_pair(listIt->Cx_rgb,listIt->Cy_rgb)].x2d_depth;
           Cpoint.y2d_depth = myMap[std::make_pair(listIt->Cx_rgb,listIt->Cy_rgb)].y2d_depth;
           Cpoint.rawDepth = myMap[std::make_pair(listIt->Cx_rgb,listIt->Cy_rgb)].rawDepth;
           dd = myMap[std::make_pair(listIt->Cx_rgb,listIt->Cy_rgb)].rawDepth;

            if(Cpoint.x ==-1 || Cpoint.y == -1)
           {
               for(i =-10;i<10; i++)
                for(j=-10;j<10;j++)
                {
                   Cpoint.x =  myMap[std::make_pair(listIt->Cx_rgb+i,listIt->Cy_rgb+j)].x2d_rgb;
                   Cpoint.y =  myMap[std::make_pair(listIt->Cx_rgb+i,listIt->Cy_rgb+j)].y2d_rgb;
                   Cpoint.z =  myMap[std::make_pair(listIt->Cx_rgb+i,listIt->Cy_rgb+j)].z;
                   Cpoint.x2d_depth = myMap[std::make_pair(listIt->Cx_rgb+i,listIt->Cy_rgb+j)].x2d_depth;
                   Cpoint.y2d_depth = myMap[std::make_pair(listIt->Cx_rgb+i,listIt->Cy_rgb+j)].y2d_depth;
                   Cpoint.rawDepth = myMap[std::make_pair(listIt->Cx_rgb+i,listIt->Cy_rgb+j)].rawDepth;

                   dd = myMap[std::make_pair(listIt->Cx_rgb+i,listIt->Cy_rgb+j)].rawDepth;

                   corners3dList.push_back(Cpoint);

                   if(Cpoint.x != -1 && Cpoint.y != -1)
                    break;
                }
           }
          //  fprintf(fid,"%d\t",dd);

            corners3dList.push_back(Cpoint);
//          double dist;
//          dist = euclideanDeistance(Cpoint.x,rgbImage->width/2.0,Cpoint.y,rgbImage->height/2.0);
         // fprintf(fid,"%g\t%g\t%g\t%g\t%g\t%d\n",Cpoint.x2d_rgb ,Cpoint.y2d_rgb ,Cpoint.x ,Cpoint.y ,Cpoint.z ,Cpoint.rawDepth);

          // cvDrawLine(newImage,cvPoint(0,HeightThresh),cvPoint(rgbImage->width,HeightThresh),cvScalar(0,255,255),1,8,0);
         //  cvDrawCircle(newImage,cvPoint( Cpoint.x2d_rgb,Cpoint.y2d_rgb),2, cvScalar(100,100,100),2);

            if(Cpoint.isHazard)
            {

                if (Cpoint.z <= DistanceThresh && Cpoint.z > 0 && Cpoint.y < HeightThresh)
                {
                    fprintf(outPtr,"%g\n",Cpoint.z );
                    fflush(outPtr);
                corner.Cx = Cpoint.x;
                corner.Cy = Cpoint.y;
                redCorners.push_back(corner);

               // noPath++;
 //              cvDrawCircle(newImage,cvPoint(Cpoint.x,Cpoint.y),2, cvScalar(100,0,255),2); //Red
               //displayTemplate(newImage, 0, templateImg, lefttemplateImg, righttemplateImg);
              // cvWaitKey(1);
               double leftDist = leftSafeDistance(pointsList, cornersList, myMap,  lefttemplateImg);
               //cvWaitKey(1);
               double rightDist = rightSafeDistance(pointsList, cornersList, myMap, righttemplateImg);

               if(leftDist < rightDist)
               {
                  // fprintf(outPtr,"%s\tslight Left is better\n",newImage);
                  // displayTemplate(newImage, 1, templateImg, lefttemplateImg, righttemplateImg);
                   leftHurdle++;
                  // speech(2);
               }
               else
               {
                  // fprintf(outPtr,"%s\tslight Left is better\n",newImage);
                 //  displayTemplate(newImage, 2, templateImg, lefttemplateImg, righttemplateImg);
                   rightHurdle++;
                //   speech(3);
               }

               goto next;

            }
            else if(Cpoint.z < DistanceThresh +safeDistance&& Cpoint.z > DistanceThresh && Cpoint.z >0)
            {
                corner.Cx = Cpoint.x;
                corner.Cy = Cpoint.y;
                blueCorners.push_back(corner);
 //        cvDrawCircle(newImage,cvPoint(Cpoint.x,Cpoint.y),2, cvScalar(255,0,0),2); //Blue
            }
            else if(Cpoint.z >0)
            {
                corner.Cx = Cpoint.x;
                corner.Cy = Cpoint.y;
                greenCorners.push_back(corner);
//          cvDrawCircle(newImage,cvPoint(Cpoint.x,Cpoint.y),2, cvScalar(0,255,0),2); //Green
            }
            else
                noDepthCorners++;
        }
        else
        {
            nonHazard++;
           // cvDrawCircle(newImage,cvPoint(Cpoint.x2d_rgb,Cpoint.y2d_rgb),2, cvScalar(100,100,255),2);
        }

      }
      //  fprintf(fid,"next frame \n");

       // fflush(fid);
       // fclose(fid);


//       printf("green corners %d\n",greenCorners.size());
//      for(listIt=greenCorners.begin();listIt!= greenCorners.end();listIt++)
//      {
//          cvDrawCircle(newImage,cvPoint(listIt->Cx,listIt->Cy),2, cvScalar(0,255,0),2); //Green
//      }
//       printf("blue corners %d\n",blueCorners.size());
//      for(listIt=blueCorners.begin();listIt!= blueCorners.end();listIt++)
//      {
//          cvDrawCircle(newImage,cvPoint(listIt->Cx,listIt->Cy),2, cvScalar(255,0,0),2); //Blue
//      }
//        printf("red corners %d\n",redCorners.size());
//      for(listIt=redCorners.begin();listIt!= redCorners.end();listIt++)
//      {
//          cvDrawCircle(newImage,cvPoint(listIt->Cx,listIt->Cy),2, cvScalar(0,0,255),2); //Red
//      }
//      printf("no Depth corners %d\n non Hazard corners %d\n",noDepthCorners, nonHazard);
//      cvShowImage("CornersWithDepth",newImage);
//      cvWaitKey(5);
////      cvSaveImage("CornerDepth.png",newImage);
//      cvReleaseImage(&newImage);

next:

       return corners3dList;
}
//=========================================================================================

int main()
{
    //printf("starting \n");
    vector<Point3D> Points3d_depths, corners3d_depths;
    int count, direction, chkWall;
    vector<cornerPoint> cornerList, PreFrameCorners;
    vector<Point3D>::iterator listC;
    std::map<std::pair<double,double>, Point3D> Pre_depthMap;
    int leftM ,rightM,  w,h, RedFrequency;
    IplImage  *rgb_img, * newImage;

    //IplImage *rgb_img3 = cvCreateImage(cvSize(WIDTH,HEIGHT),IPL_DEPTH_8U,3);
    IplImage *rgb_img1 = cvCreateImage(cvSize(WIDTH,HEIGHT),IPL_DEPTH_8U,1);
    IplImage *MidTemplateDepthImage = cvCreateImage(cvSize(WIDTH,HEIGHT),IPL_DEPTH_8U,1);
    IplImage *leftTemplateDepthImage = cvCreateImage(cvSize(WIDTH,HEIGHT),IPL_DEPTH_8U,1);
    IplImage *rightTemplateDepthImage = cvCreateImage(cvSize(WIDTH,HEIGHT),IPL_DEPTH_8U,1);

    leftM =  WIDTH/2;
    rightM = WIDTH-leftM;

    double s,s1;
    s = 255.0;
    s1 = 0;
    int step =1;

        for(h=0; h<HEIGHT-10; h++)
        {
             step++;
             for(w=0; w<WIDTH; w++)
            {
                if(h > HEIGHT-topM && w > leftM && w < rightM)
                    ((uchar *)(MidTemplateDepthImage->imageData +h*MidTemplateDepthImage->widthStep))[w] = s;
                    //cvSet2D(MidTemplateDepthImage,h,w,s);
                else
                 ((uchar *)(MidTemplateDepthImage->imageData +h*MidTemplateDepthImage->widthStep))[w] = s1;
                   // cvsu rootSet2D(MidTemplateDepthImage,h,w,s1);
            }
            if(leftM > 5 && rightM < WIDTH-5 && step>2)
          {
                step=1;
                leftM--;
                rightM++;
          }
        }
   // printf("1\n");
    leftM =  10;
    rightM = WIDTH/6;


        for(h=0; h<HEIGHT-10; h++)
        {
             step++;
             for(w=0; w<WIDTH; w++)
            {
                if(h > HEIGHT-topM && w > leftM && w < rightM)
                 ((uchar *)(leftTemplateDepthImage->imageData +h*leftTemplateDepthImage->widthStep))[w] = s;
                   // cvSet2D(leftTemplateDepthImage,h,w,s);
                else
                ((uchar *)(leftTemplateDepthImage->imageData +h*leftTemplateDepthImage->widthStep))[w] = s1;
                    //cvSet2D(leftTemplateDepthImage,h,w,s1);
            }
            if( step>3)
          {
                step=1;
                rightM++;
          }
        }

    leftM =  WIDTH - WIDTH/6;
    rightM = WIDTH-10;
        for(h=0; h<HEIGHT-10; h++)
        {
             step++;
             for(w=0; w<WIDTH; w++)
            {
                if(h > HEIGHT-topM && w > leftM && w < rightM)
                 ((uchar *)(rightTemplateDepthImage->imageData +h*rightTemplateDepthImage->widthStep))[w] = s;
                    //cvSet2D(rightTemplateDepthImage,h,w,s);printf("processing");
                else
                ((uchar *)(rightTemplateDepthImage->imageData +h*rightTemplateDepthImage->widthStep))[w] = s1;
                  //  cvSet2D(rightTemplateDepthImage,h,w,s1);
            }
            if( step>3)
          {
                step=1;
                leftM--;
          }
        }
        //cvSaveImage("template.png", MidTemplateDepthImage);
        //cvSaveImage("templateRight.png",rightTemplateDepthImage );
       // cvSaveImage("templateLeft.png", leftTemplateDepthImage);

    //cvShowImage("template",rightTemplateDepthImage);
    //cvWaitKey(0);
    //cvReleaseImage(&rightTemplateDepthImage);

     cv::Mat rgbImg;
     cv::Mat rgbMatrix;

    KinectHandler kinectHandler;
    kinectHandler.startDevice();


    char imageNameRgb[180]={'\0'};
    char imageNameDepth[180]={'\0'};
    IplImage *img;
    count =1;

    speech(0); //[0] Welcome to load libraries

    while(1)
    {
        kinectHandler.captureImageAndDepth();

        if(count%2 == 0)
        {


        //sprintf(imageNameRgb, "image_rgb_%04d.png", count);
        //sprintf(imageNameDepth, "image_depth_%04d.txt", count);

        Points3d_depths.clear();
        corners3d_depths.clear();
        cornerList.clear();

       // cvSaveImage(imageNameRgb, kinectHandler.rgbImage);
        Points3d_depths= getKinectData(kinectHandler.depthMat);

        rgbMatrix = kinectHandler.rgbMat.clone();
        //cout<<"Frame No: \n"<<count<<endl;
       // cvClone(kinectHandler.rgbMat);
        //imshow("original",rgbMatrix);
        //cvWaitKey(10);
        cv::cvtColor(rgbMatrix,rgbImg,CV_BGR2GRAY);
        // cv::Mat rgbImg(rgb_img1);
        //rgb_img = cvLoadImage(imageNameRgb,1);
        cv::FeatureDetector * detector = new cv::GFTTDetector();
        std::vector<cv::KeyPoint> imgKeyPoints;

        //printf("processing\n");
        cornerPoint corner;
        //rgbImg = imread(imageNameRgb, cv::IMREAD_GRAYSCALE);
        detector->detect(rgbImg, imgKeyPoints);

        for(int i=0;i<imgKeyPoints.size();i++)
            {
                corner.Cx_rgb = imgKeyPoints[i].pt.x;
                corner.Cy_rgb = imgKeyPoints[i].pt.y;
                cornerList.push_back(corner);
            }

        //printf("number of corners %d\n",cornerList.size());
        if(WallLikeHurdle( Points3d_depths, MidTemplateDepthImage))
         {
        // printf("wall like \t");
           noPath++;
          }

       //    fprintf(outPtr,"%s\tstop please\n",imageNameRgb);
         //  speech(1);

      corners3d_depths = cornersDepth(Points3d_depths, cornerList,  MidTemplateDepthImage, leftTemplateDepthImage, rightTemplateDepthImage);
   // if(count%2 == 0)
      {
           if(noPath == 0 ){
                speech(4); //[4] go ahead
                fprintf(outPtr,"%d\t1\tGo ahead\n",count);
                fflush(outPtr);
           }
            else if(noPath > 0)
            {
                 speech(1);// [1]please stop cant see any path
                 fprintf(outPtr,"%d\t0\t Please Stop Cant See any Path\n",count);
                 fflush(outPtr);

                 if(leftHurdle == 0 && rightHurdle == 0){
                    speech(5); //[5] Move either left or right
                    fprintf(outPtr,"%d\t5\tMove either left or right\n",count);
                    fflush(outPtr);
                 }

                 if(rightHurdle > 0 )
                 {
                   speech(2); //[2] slight left is better
                   fprintf(outPtr,"%d\t2\t Slight left is better\n",count);
                   fflush(outPtr);
                   leftHurdle = 0;
                 }
                 else if(leftHurdle > 0 )
                 {
                   speech(3); //[3] slight right is better
                   fprintf(outPtr,"%d\t3\t Slight right is better\n",count);
                   fflush(outPtr);
                   rightHurdle = 0;
                 }
                   noPath = 0;
            }
        }

        }  count++;
 }
    fclose(outPtr);

    kinectHandler.stopDevice();

    return 0;
}
