#include "ste.h"

#include <string>

extern "C" {
    #include <evnets.h>
}


int ste_initScene(int win_width, int win_height, const char *url, double dpi)
{
    core_init(win_width, win_height, dpi);
    core_t* mcore = (core_t*)core_get_module("core");
    observer_t* observer = mcore->observer;

    std::string baseurl;
    if (url == NULL) {
        baseurl = "./skydata";
    }
    else {
        baseurl = url;
    }

    obj_t* stars_obj = core_get_module("core.stars");
    if (check_path_exist((baseurl + "stars").c_str())) {
        module_add_data_source(stars_obj, (baseurl + "/stars").c_str(), NULL);
    }

    if (check_path_exist((baseurl + "skycultures/western").c_str())) {
        obj_t* skycultures_obj = core_get_module("core.skycultures");
        module_add_data_source(skycultures_obj, (baseurl + "/skycultures/western").c_str(), "defualt");
    }
    ste_setMouduleProperty("core.skycultures", "current_id", "defualt");

    if (check_path_exist((baseurl + "/skycultures/western").c_str())) {
       obj_t* skycultures_obj = core_get_module("core.skycultures");
       module_add_data_source(skycultures_obj, (baseurl + "/skycultures/western").c_str(), "en-us");
    }

    if (check_path_exist((baseurl + "/dso").c_str())) {
        obj_t* dsos_obj = core_get_module("core.dsos");
        module_add_data_source(dsos_obj, (baseurl + "/dso").c_str(), NULL);
    }

    if (check_path_exist((baseurl + "/landscapes/guereins").c_str())) {
        obj_t* landscapes_obj = core_get_module("core.landscapes");
        module_add_data_source(landscapes_obj, (baseurl + "/landscapes/guereins").c_str(), "guereins");
        obj_set_attr(landscapes_obj, "visible", true);
    }

    if (check_path_exist((baseurl + "/surveys/milkyway").c_str())) {
        obj_t* milkyway_obj = core_get_module("core.milkyway");
        module_add_data_source(milkyway_obj, (baseurl + "/surveys/milkyway").c_str(), NULL);
    }

    if (check_path_exist((baseurl + "/mpcorb.dat").c_str())) {
        obj_t* minor_planets_obj = core_get_module("core.minor_planets");
        module_add_data_source(minor_planets_obj, (baseurl + "/mpcorb.dat").c_str(), "mpc_asteroids");
    }

    if (check_path_exist((baseurl + "/surveys/sso").c_str())) {
        obj_t* planets_obj = core_get_module("core.planets");
        module_add_data_source(planets_obj, (baseurl + "/surveys/sso/moon").c_str(), "moon");
        module_add_data_source(planets_obj, (baseurl + "/surveys/sso/sun").c_str(), "sun");
    }

    if (check_path_exist((baseurl + "/CometEls.txt").c_str())) {
        obj_t* comets_obj = core_get_module("core.comets");
        module_add_data_source(comets_obj, (baseurl + "/CometEls.txt").c_str(), "mpc_comets");
    }

    if (check_path_exist((baseurl + "/tle_satellite.jsonl.gz").c_str())) {
        obj_t* satellites_obj = core_get_module("core.satellites");
        module_add_data_source(satellites_obj, (baseurl + "/tle_satellite.jsonl.gz").c_str(), "jsonl/sat");
    }

    if (check_path_exist((baseurl + "/dss").c_str())) {
        obj_t* dss_obj = core_get_module("core.dss");
        module_add_data_source(dss_obj, (baseurl + "/dss").c_str(), NULL);
    }

    obj_t* atmosphere_obj = core_get_module("core.atmosphere");
    obj_set_attr(atmosphere_obj, "visible", false);

    obj_t* constellations_obj = core_get_module("core.constellations");
    obj_set_attr(constellations_obj, "lines_visible", true);
    obj_set_attr(constellations_obj, "labels_visible", true);
    obj_set_attr(constellations_obj, "images_visible", true);
    obj_set_attr(constellations_obj, "lines_animation", true);

    return 0;
}

int ste_render(int win_width, int win_height, double dpi) {
    core_update();
    core_render(win_width, win_height, dpi);
    return 0;
}

int ste_onZoom(double k, double x, double y) {
    core_on_zoom(k, x, y);
    return 0;
}

int ste_onMouse(int id, int state, double x, double y, int buttons) {
    core_on_mouse(id, state, x, y, buttons);
    return 0;
}

int ste_getMouduleProperty(const char* type, const char* pro_name, void* value) {
    int ret = 0;
    obj_t* obj = NULL;
    obj = core_get_module(type);
    if (!obj) {
        return -1;
    }

    ret = obj_get_attr(obj, pro_name, value);
    return ret;
}

int ste_setMouduleProperty(const char* type, const char* pro_name, ...) {
    int ret = 0;
    obj_t* obj = NULL;

    obj = core_get_module(type);
    if (!obj) {
        return -1;
    }

    json_value* arg, * jret;
    va_list ap;
    const attribute_t* attr;

    attr = obj_get_attr_(obj, pro_name);
    if (!attr) {
        LOG_E("Unknow attribute %s", pro_name);
        return -1;
    }

    va_start(ap, pro_name);
    arg = args_vvalue_new(attr->type, &ap);

    jret = obj_call_json(obj, pro_name, arg);
    json_builder_free(arg);
    json_builder_free(jret);
    va_end(ap);

    return ret;
}

int ste_setMoudulePropertyDouble(const char* type, const char* pro_name, double value) {
    int ret = 0;
    obj_t* obj = NULL;

    obj = core_get_module(type);
    if (!obj) {
        return -1;
    }

    ret = obj_set_attr(obj, pro_name, value);
    return ret;
}

int ste_setMoudulePropertyInt(const char* type, const char* pro_name, int value) {
    int ret = 0;
    obj_t* obj = NULL;

    obj = core_get_module(type);
    if (!obj) {
        return -1;
    }

    ret = obj_set_attr(obj, pro_name, value);
    return ret;
}

int ste_setMoudulePropertyBool(const char* type, const char* pro_name, bool value) {
    int ret = 0;
    obj_t* obj = NULL;

    obj = core_get_module(type);
    if (!obj) {
        return -1;
    }

    ret = obj_set_attr(obj, pro_name, value);
    return ret;
}

int ste_setMoudulePropertyString(const char* type, const char* pro_name, const char* value) {
    int ret = 0;
    obj_t* obj = NULL;

    obj = core_get_module(type);
    if (!obj) {
        return -1;
    }

    ret = obj_set_attr(obj, pro_name, value);
    return ret;
}

int ste_setMoudulePropertyVec2(const char* type, const char* pro_name, double values[2]) {
    int ret = 0;
    obj_t* obj = NULL;

    obj = core_get_module(type);
    if (!obj) {
        return -1;
    }

    ret = obj_set_attr(obj, pro_name, values);
    return ret;
}

int ste_setMoudulePropertyVec3(const char* type, const char* pro_name, double values[3]) {
    int ret = 0;
    obj_t* obj = NULL;

    obj = core_get_module(type);
    if (!obj) {
        return -1;
    }

    ret = obj_set_attr(obj, pro_name, values);
    return ret;
}

int ste_setMoudulePropertyVec4(const char* type, const char* pro_name, double values[4]) {
    int ret = 0;
    obj_t* obj = NULL;

    obj = core_get_module(type);
    if (!obj) {
        return -1;
    }

    ret = obj_set_attr(obj, pro_name, values);
    return ret;
}

int ste_setMoudulePropertyPtr(const char* type, const char* pro_name, void* value)
{
    int ret = 0;
    obj_t* obj = NULL;

    obj = core_get_module(type);
    if (!obj) {
        return -1;
    }

    ret = obj_set_attr(obj, pro_name, value);
    return ret;
}