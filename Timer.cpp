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

#include "Timer.h"

Timer::Timer()
{
    startTime = 0.0;
    stopTime = 0.0;
    timing = 0.0;
}

void Timer::start()
{
    startTime = (double)cvGetTickCount();
}

void Timer::stop()
{
    stopTime = (double)cvGetTickCount();
}

double Timer::getTimeMS() // returns the time in miliseconds
{
    return timing = (stopTime - startTime) / (cvGetTickFrequency() * 1000.);
}

double Timer::getTimeS() // returns the time in seconds
{
    return timing = getTimeMS()/1000;
}
