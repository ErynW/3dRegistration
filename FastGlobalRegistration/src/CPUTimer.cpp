#include "CPUTimer.h"
#include <cassert>
#include <stdio.h>
#include <iomanip> 
#include <iostream>
#include <sstream>

using namespace std;

CPUTimer::CPUTimer() : startWasSet(false), stopWasSet(true)  {
}

CPUTimer::~CPUTimer() {
}

void CPUTimer::getTime(struct timespec *ts){
#ifdef __MACH__ 
  //https://gist.github.com/jbenet/1087739
  // OS X does not have clock_gettime, use clock_get_time
  clock_serv_t cclock;
  mach_timespec_t mts;
  host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
  clock_get_time(cclock, &mts);
  mach_port_deallocate(mach_task_self(), cclock);
  ts->tv_sec = mts.tv_sec;
  ts->tv_nsec = mts.tv_nsec;
#else
#ifdef _WIN32
  clock_gettime(ts);
#else
  clock_gettime(CLOCK_REALTIME, ts);
#endif
#endif

}

void CPUTimer::tic() {
  assert(stopWasSet && !startWasSet);

  getTime(&startTime);
  startWasSet = true;
  stopWasSet = false;
}

timespec CPUTimer::toc() {
  assert(startWasSet && !stopWasSet);
  getTime(&endTime);

  timespec temp;

  if ((endTime.tv_nsec - startTime.tv_nsec)<0) {
    temp.tv_sec = endTime.tv_sec - startTime.tv_sec-1;
    temp.tv_nsec = 1000000000 + endTime.tv_nsec - startTime.tv_nsec;
  } else {
    temp.tv_sec = endTime.tv_sec - startTime.tv_sec;
    temp.tv_nsec = endTime.tv_nsec - startTime.tv_nsec;
  }

  stopWasSet = true;
  startWasSet = false;

  return temp;
}

void CPUTimer::toc(std::string name){
  timespec elapsedTimeCPUTimer = this->toc();

  //cout<<"=====  TIMING ["<<name<<"] is ";
  //cout<< elapsedTimeCPUTimer.tv_sec << "." << elapsedTimeCPUTimer.tv_nsec << " s" << endl;
  
  float seconds=elapsedTimeCPUTimer.tv_sec + elapsedTimeCPUTimer.tv_nsec / 1e9f;
  timingsMap[name]=seconds;
}

vector<TimingInfo> CPUTimer::getMeasurements(){
  //typedef std::map<std::string,float>::iterator it_type;
  vector<TimingInfo> ti;

  for(it_type it = timingsMap.begin(); it != timingsMap.end(); ++it) 
    ti.push_back(make_pair(it->first, it->second));

  return ti;
}

string CPUTimer::allTimings(){
  //typedef std::map<std::string,float>::iterator it_type;
  stringstream ss;

  for(it_type it = timingsMap.begin(); it != timingsMap.end(); ++it)
    ss << left << setw(20) << it->first << ":\t" << it->second << " sec" <<endl;
  ss << "TOT:\t" << totalTiming() << " sec";
  return ss.str();
}

float CPUTimer::totalTiming(){
  //typedef std::map<std::string,float>::iterator it_type;
  float tot = 0;
  for(it_type it = timingsMap.begin(); it != timingsMap.end(); ++it)
    tot+=it->second;
  return tot;
}
