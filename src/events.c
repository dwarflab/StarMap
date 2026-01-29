/* Stellarium Web Engine - Copyright (c) 2022 - Stellarium Labs SRL
 *
 * This program is licensed under the terms of the GNU AGPL v3, or
 * alternatively under a commercial licence.
 *
 * The terms of the AGPL v3 license can be found in the main directory of this
 * repository.
 */

/*
 * This file is only used by the javascript at the moment.  We could
 * probably try to remove it totally at some point
 */

#include "evnets.h"

// Events types
enum {
    EVENT_RISE      = 1 << 0,
    EVENT_SET       = 1 << 1,
};

// Newton algo.
#define NEWTON_MAX_STEPS 20
static double newton(double (*f)(double x, void *user),
                     double x0, double x1, double precision, void *user)
{
    int nbiter = 0;
    double f0, f1, tmp;
    double startDelta = fabs(x1 - x0);
    f0 = f(x0, user);
    f1 = f(x1, user);
    while (f1 && fabs(x1 - x0) > precision && f1 != f0) {
        tmp = x1;
        x1 = x1 + (x1 - x0) / (f0 / f1 - 1.0);
        x0 = tmp;
        // Check for diverging
        if (fabs(x1 - x0) > startDelta * 10)
            return NAN;
        f0 = f1;
        f1 = f(x1, user);

        if (nbiter++ > NEWTON_MAX_STEPS)
            return x1;
    }
    return x1;
}

static int sign(double x)
{
    return x < 0 ? -1 : 1;
}

static double find_zero(double (*f)(double x, void *user),
                        double x0, double x1, double step, double precision,
                        int rising, void *user)
{
    int last_sign = 0;
    double x, fx;
    bool found = false;
    // First find an approximate answer simply by stepping.  Not very clever.
    // Make sure the last iteration is exactly at x1.
    for (x = x0;; x += step) {
        if (x > x1) x = x1; // Clamp to x1.
        fx = f(x, user);
        if (sign(fx) * last_sign == -1 && sign(fx) == rising) {
            found = true;
            break;
        }
        last_sign = sign(fx);
        if (x == x1) break;
    }
    if (!found) return NAN;
    // Once we are near the value, use newton algorithm.
    return newton(f, x - step, x, precision, user);
}

static double rise_dist(double time, void *user)
{
    struct {
        observer_t *obs;
        obj_t *obj;
    } *data = user;
    double radius = 0, pvo[2][4], observed[4], az, alt;

    data->obs->tt = time;
    observer_update(data->obs, false);
    obj_get_pvo(data->obj, data->obs, pvo);
    convert_framev4(data->obs, FRAME_ICRF, FRAME_OBSERVED, pvo[0], observed);
    vec3_to_sphe(observed, &az, &alt);
    az = eraAnp(az);
    obj_get_info(data->obj, data->obs, INFO_RADIUS, &radius);
    //printf("radius:%d\n", radius);
    //printf("pvo:%f,%f,%f\n", pvo[0][0], pvo[0][1], pvo[0][2]);
    return alt + radius - data->obs->horizon;
}

static double rise_dist2(double time, void* user)
{
    struct {
        observer_t* obs;
        double ra;
        double dec;
        double radius;
    }  *data = user;
    double radius = data->radius, pvo[4], observed[4], az, alt;

    data->obs->tt = time;
    observer_update(data->obs,false);
    eraS2c(data->ra, data->dec, pvo);
    convert_framev4(data->obs, FRAME_ICRF, FRAME_OBSERVED, pvo, observed);
    vec3_to_sphe(observed, &az, &alt);
    az = eraAnp(az);
    //printf("radius:%d\n", radius);
    //printf("pvo:%f,%f,%f\n", pvo[0], pvo[1], pvo[2]);
    return alt + radius - data->obs->horizon;
}

static double sun_rise_dist(double time, void* user)
{
    struct {
        observer_t* obs;
    } *data = user;
    double radius = 696000000, observed[4], az, alt;

    data->obs->tt = time;
    observer_update(data->obs, false);
    double normalized_sun_pvo[3];
    vec3_normalize(data->obs->sun_pvo[0], normalized_sun_pvo);
    convert_framev4(data->obs, FRAME_ICRF, FRAME_OBSERVED, normalized_sun_pvo, observed);
    vec3_to_sphe(observed, &az, &alt);
    az = eraAnp(az);
    radius = radius * DM2AU / vec3_norm(data->obs->sun_pvo[0]);
    //printf("pvo:%f,%f,%f\n", data->obs->sun_pvo[0][0], data->obs->sun_pvo[0][1], data->obs->sun_pvo[0][2]);
    return alt + radius - data->obs->horizon;
}

EMSCRIPTEN_KEEPALIVE
double compute_event(observer_t *obs,
                     obj_t *obj,
                     int event,
                     double start_time,
                     double end_time,
                     double precision)
{
    observer_t obs2 = *obs;
    double ret;
    int rising;
    struct {
        observer_t *obs;
        obj_t *obj;
    } data = {&obs2, obj};
    rising = event == EVENT_RISE ? +1 : -1;
    ret = find_zero(rise_dist, start_time, end_time,
                    (end_time - start_time) / 24, precision,
                    rising, &data);
    return ret;
}

double compute_event2(observer_t* obs, int event, double ra, double dec, double radius, double start_time, double end_time, double precision)
{
    observer_t obs2 = *obs;
    double ret;
    int rising;
    struct {
        observer_t* obs;
        double ra;
        double dec;
        double radius;
    } data = { &obs2, ra,dec,radius };
    rising = event == EVENT_RISE ? +1 : -1;
    ret = find_zero(rise_dist2, start_time, end_time,
        (end_time - start_time) / 24, precision,
        rising, &data);
    return ret;
}

double compute_sun_event(observer_t* obs,
                    int event,
                    double start_time,
                    double end_time,
                    double precision)
{
    observer_t obs2 = *obs;
    double ret;
    int rising;
    struct {
        observer_t* obs;
        double radius;
    } data = { &obs2};
    rising = event == EVENT_RISE ? +1 : -1;
    ret = find_zero(sun_rise_dist, start_time, end_time,
        (end_time - start_time) / 24, precision,
        rising, &data);
    return ret;
}