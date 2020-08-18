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

#ifndef POINT3D_H
#define POINT3D_H

#include "Definitions.h"

#include <stdio.h>

class Point3D
{
    public:
        Point3D();

        double x,y,z;
        int x2d_depth, y2d_depth;
        double x2d_rgb, y2d_rgb;
        int rawDepth;
        bool isCalculated;
        bool used;
        bool isHazard; //only if it is inside template area and depth is less than threshold

        void print();

        double calculateLength();
        static Point3D cross(Point3D a, Point3D b);
        static double dotProduct(Point3D a, Point3D b);
        static Point3D add(Point3D a, Point3D b);
        static Point3D subtract(Point3D a, Point3D b);
};

class cornerPoint
{
    public:

    int Cx_rgb,Cy_rgb;
    double Cx,Cy,Cz;

};

//class Keys {
//public:
//Keys(int k1, int k2) : key1(k1), key2(k2) { }
//bool operator<(const Keys &right) const {
//return (key1 < right.key1 && key2 < right.key2);
//}
//int key1;
//int key2;
//};
#endif // POINT3D_H
