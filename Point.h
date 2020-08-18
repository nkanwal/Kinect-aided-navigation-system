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

#ifndef POINT_H
#define POINT_H
#include "Radius_Info.h"
#include <cv.h>
#include <list>
#include "eigen.h"
#define sum_hist_bin 50

//using namespace std;

class Point
{
    public:
        Point();

  	int x;							/**< x coord */
	int y;							/**< y coord */
	int mt_x;						/**< x coord */
	int mt_y;						/**< y coord */
	int des;
	int edge_index[10];
	int direction;              /** 1 for clockwise, 2 for anticlockwise, 3 for whole circle **/
	Radius_Info radius3[16];
	Radius_Info radius5[28];
	Radius_Info radius7[40];
	Radius_Info radius9[52];
	Radius_Info radius11[64];
	double SEA[25];
	double sum_des[sum_hist_bin];	/** Sum of inside pixels histogram descriptor*/
	double A_sum_des[sum_hist_bin];
    eigen eigenValsVes[23][23];
    int clusterID;
    bool visited;
    bool isNoise;
    bool isMember;

    protected:
    private:
};

class CornerDescriptor
{
public:
    int x;
    int y;
    int has_des;
    double SEA[25];
    int mt_x;
    int mt_y;
    bool inlier;
    double mt_SEA[25];
    int similarity[4];
};
//class similarCorners
//{
//public:
//
//    int x1;
//    int y1;
//    int x2;
//    int y2;
//    double SEA1[25];
//    double SEA2[25];
//    double descriptorDistance[3];
//    int flag;
//
//
//};


#endif // POINT_H
