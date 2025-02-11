#pragma once
#ifndef UNICODE
#define UNICODE
#endif
#include <string>
#include <thread>
#include "glad/gl.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <vector>
#include "Event.h"
#include <set>
#include <chrono>
#include "PointLight.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "Text.h"
#include "CubeMap.h"
#include "Emitter.h"
#include "Sound.h"

#define SDLBOOL(b) b ? SDL_TRUE : SDL_FALSE

using std::string;
using std::vector;
class camera;
class object3d;
class object2d;

class window {
public:
    window();
    window(string title, camera* cam, int width, int height, bool fullscreen, vec3 * ambient_light);
    ~window();
    camera* cam = nullptr;
    string title;
    int width = 0, height = 0;
    event current_event;
    bool fullscreen = false;
    bool resizeable = true;
    void update();
    double deltatime = 1.0f;
    long long time_ns = 1, time = 1;

    inline void lock_mouse(bool lock) {
        SDL_SetRelativeMouseMode(SDLBOOL(lock));
    }

    inline void set_fullscreen(bool value) {
        fullscreen = value;
        SDL_SetWindowFullscreen(app_window, fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
    }

    void add_object(object3d* obj);
    void remove_object(object3d* obj);
    void add_object_list(vector<object3d*> objs);
    void remove_object_list(vector<object3d*> objs);

    void add_object2d(object2d* obj);
    void remove_object2d(object2d* obj);
    void add_object2d_list(vector<object2d*> objs);
    void remove_object2d_list(vector<object2d*> objs);

    void add_point_light(point_light* obj);
    void remove_point_light(point_light* obj);
    void add_point_light_list(vector<point_light*> objs);
    void remove_point_light_list(vector<point_light*> objs);

    void add_directional_light(directional_light* obj);
    void remove_directional_light(directional_light* obj);
    void add_directional_light_list(vector<directional_light*> objs);
    void remove_directional_light_list(vector<directional_light*> objs);

    void add_spot_light(spot_light* obj);
    void remove_spot_light(spot_light* obj);
    void add_spot_light_list(vector<spot_light*> objs);
    void remove_spot_light_list(vector<spot_light*> objs);

    void add_text(text* obj);
    void remove_text(text* obj);
    void add_text_list(vector<text*> objs);
    void remove_text_list(vector<text*> objs);

    void add_emitter(emitter* obj);
    void remove_emitter(emitter* obj);
    void add_emitter_list(vector<emitter*> objs);
    void remove_emitter_list(vector<emitter*> objs);

    std::set<point_light*> render_list_point_lights;
    std::set<directional_light*> render_list_directional_lights;
    std::set<spot_light*> render_list_spot_lights;
    std::set<text*> render_list_text;
    std::set<emitter*> render_list_emitter;
    vec3* ambient_light = nullptr;
    skybox* sky_box = nullptr;
    audio_mixer* sound_mixer;
private:
    void create_window();
    SDL_Window* app_window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;
    SDL_GLContext gl_context = nullptr;
    std::chrono::steady_clock::time_point old_time, new_time, starttime;
    std::set<object3d*> render_list;
    std::set<object2d*> render_list2d;
};