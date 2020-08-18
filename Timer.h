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


#ifndef TIMER_H
#define	TIMER_H

#include "cv.h"



class Timer
{
public:
    Timer();
    void start(); // starts the timer
    void stop(); // stops timer
    double getTimeMS(); // returns the time in miliseconds
    double getTimeS(); // returns the time in seconds
private:
    double startTime;
    double stopTime;
    double timing; // difference between start and stop in miliseconds
};

#endif	/* TIMER_H */

