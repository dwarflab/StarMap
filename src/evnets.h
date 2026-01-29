#ifndef EVENTS_H
#define EVENTS_H

#include "swe.h"

double compute_event(observer_t* obs,
    obj_t* obj,
    int event,
    double start_time,
    double end_time,
    double precision);

double compute_event2(observer_t* obs,
    int event,
    double ra,
    double dec,
    double radius,
    double start_time,
    double end_time,
    double precision);

double compute_sun_event(observer_t* obs,
    int event,
    double start_time,
    double end_time,
    double precision);

#endif