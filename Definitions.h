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

#ifndef DEFINITIONS_H_INCLUDED
#define DEFINITIONS_H_INCLUDED

#include "time.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"

#include <vector>
#include <map>


#include "cv.h"
#include "cvaux.h"
#include "cxcore.h"
#include "highgui.h"

#include <string.h>
#include <malloc.h>
#include <speak_lib.h>


#define WIDTH 640//640  // 20
#define HEIGHT 480//480 //15
#define POINT_THRESHOLD 500
#define NUMBER_OF_PLANES 3
#define DistanceThresh 1.85
#define safeDistance 3.0
#define HeightThresh 400
#define Margin  13

#define eps 0.000001


#endif // DEFINITIONS_H_INCLUDED
