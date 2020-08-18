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

#include "Point3D.h"

#include <math.h>

Point3D::Point3D()
{
    used = false;

    x = -1.0;
    y = -1.0;
    z = -1.0;

    x2d_depth = -1;
    y2d_depth = -1;

    x2d_rgb = -1;
    y2d_rgb = -1;
}

Point3D Point3D::cross(Point3D a, Point3D b)
{
    Point3D result;

    result.x = a.y*b.z - a.z*b.y;
    result.y = a.z*b.x - a.x*b.z;
    result.z = a.x*b.y - a.y*b.x;

    // normalize the result now
    double length = sqrt(result.x*result.x + result.y*result.y +result.z*result.z);
    result.x /= length;
    result.y /= length;
    result.z /= length;

    return result;
}

double Point3D::dotProduct(Point3D a, Point3D b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

Point3D Point3D::add(Point3D a, Point3D b)
{
    Point3D result;

    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;

    return result;
}

Point3D Point3D::subtract(Point3D a, Point3D b)
{
    Point3D result;

    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;

    return result;
}


void Point3D::print()
{
    printf("x: %f y: %f z: %f, depth(%g, %g) rgb(%g, %g)\n", x,y,z, x2d_depth, y2d_depth, x2d_rgb, y2d_rgb);
}

double Point3D::calculateLength()
{
    return sqrt(x*x + y*y + z*z);
}
