# Loxoc Game Engine

Important: This engine has been renamed from Runespoor to Loxoc.

> A python 3D game engine written in c++ using SDL, Assimp, OpenGL(GLAD), and GLM.

To install:

```
pip install Loxoc
```

Documentation Now Available [here](https://frewtypebbles.github.io/).

Please note:

 - Loxoc is still in development and only dev versions are available right now.  Everything is subject to change, be fixed, and improve.

 - When building from source `pkg-config` must be installed.  In addition to this, `GLM` must be installed and present in your c/c++ package manager's include directory.  Successful attempts to compile have been made on windows with `vcpkg` as the package manager.

## About:

Loxoc is a flexible, straight forward, multi-paradigm game engine that is built from the ground up with developer experience in mind.  Utilizing a game loop system similar to pygame, you are able to abstract elements of your game away from the game loop as much as you'd like.

For a taste of the api check out the test file:

```py
import time
from Loxoc import (
    Vec3, Camera, Mesh, Object3D, Window, EVENT_FLAG,
    Material, Shader, ShaderType, EVENT_STATE, Quaternion,
    Texture, Sprite, Object2D, Vec2, PointLight
)
import math
from copy import copy

# The meshes used in this testfile are not provided with the library or source files.

dim = (1280, 720)
focal_length = 10000

camera = Camera(Vec3(0.0,-10,470), Vec3(0.0,0.0,0.0), *dim, focal_length, math.radians(60))
window = Window("Loxoc Engine Test Scene", camera, *dim, False, Vec3(0.2,0.2,0.2))

# Materials are equivalent to shader programs.
default_material = Material()


# # If you wanted to create a new shader program, normally you would do something like this:
#
# default_material = Material(
#   Shader.from_file("path/to/vertexshader.glsl", ShaderType.FRAGMENT),
#   Shader.from_file("path/to/fragmentshader.glsl", ShaderType.VERTEX)
# )
# 
# # Then to set a uniform:
#
# default_material.set_uniform("uniform_name", some_value, "i")
#
# # The third argument is a magic string that represents the type.  in this case the type is an integer.

car_meshes = Mesh.from_file("./meshes/vintage_racing_car/scene.gltf")

spr_doomguy = Sprite("./textures/doomguy.png")

doomguy = Object2D(spr_doomguy, scale=Vec2(0.3, 0.3))

car = Object3D(car_meshes,
    Vec3(0.0,-10,500), Vec3(0,0,0), Vec3(100,100,100), material=default_material)

car2 = Object3D(car_meshes,
    Vec3(300,30,500), Vec3(10,3.57,23.2), Vec3(100,100,100), material=default_material)

teapot = Object3D(Mesh.from_file("./meshes/teapot/scene.gltf"),
    Vec3(-100,0,200), Vec3(0,0,0), Vec3(1000,1000,1000), material=default_material)

cube = Object3D(Mesh.from_file("./meshes/basic_crate_2/scene.gltf"),
    Vec3(0,0,0), Vec3(0,0,0), Vec3(20,20,20), material=default_material)

pirate_ship = Object3D(Mesh.from_file("./meshes/pirate_ship/pirate_ship.obj"),
    Vec3(-100,0,300), Vec3(0,10,0), material=default_material)

test_light = PointLight(Vec3(0,0,0), 500.0, Vec3(1,1,1))
test_light2 = PointLight(Vec3(20,0,0), 500.0, Vec3(0,0,2))

window.add_object_list([
    car,
    car2,
    teapot,
    cube,
    pirate_ship
])

window.add_object2d_list([
    doomguy
])

window.add_point_light_list([
    test_light,
    test_light2
])

window.lock_mouse(True)

vel_yaw = 0.0
vel = 0.0
frict = 0.1
accel = 5
cam_dist = 300.0
magic_turn_dampener = 4
mouse_sensitivity = 10
counter = 0
counter_speed = 1
while not window.event.check_flag(EVENT_FLAG.QUIT) and window.event.get_flag(EVENT_FLAG.KEY_ESCAPE) != EVENT_STATE.PRESSED:
    if window.dt > 0:
        print(f"FRAMERATE: {1.0/window.dt:.1f} fps")
    else:
        print("FRAMERATE: inf fps")
    
    doomguy.position.x = math.sin(window.time/1000000000)
    doomguy.position.y = math.cos(window.time/1000000000)
    
    # Use WASD keys.
    if window.event.get_flag(EVENT_FLAG.KEY_d) == EVENT_STATE.PRESSED:
        # ROTATE RIGHT
        vel_yaw -= 0.3
    if window.event.get_flag(EVENT_FLAG.KEY_a) == EVENT_STATE.PRESSED:
        # ROTATE LEFT
        vel_yaw += 0.3
    if window.event.get_flag(EVENT_FLAG.KEY_s) == EVENT_STATE.PRESSED:
        # BACKWARDS
        vel -= accel
    if window.event.get_flag(EVENT_FLAG.KEY_w) == EVENT_STATE.PRESSED:
        # FORWARD
        vel += accel
    # apply a quaternion rotation arround the vector vec3(1,1,0)
    teapot.rotation = Quaternion.from_axis_angle(Vec3(1,1,0), math.radians(counter))

    test_light.position = car.position
    test_light.position.y += 200
    
    # Clamp and rotate, then apply friction.
    vel_yaw = min(max(vel_yaw, -100), 100) if abs(vel_yaw) > frict else 0
    car.rotation.rotate_yaw(vel_yaw/magic_turn_dampener * window.dt)
    vel_yaw -= math.copysign(frict, vel_yaw)
    
    # Clamp the velocity.
    vel = min(max(vel, -1000), 1000) if abs(vel) > frict else 0
    # Move the car forwards with its forwards vector with a magnitude of `vel` and apply friction
    # The right vector is the forward vector for this mesh because it is rotated 90 degrees by default.
    car.position += -car.rotation.forward * vel * window.dt # window.dt is deltatime
    vel -= math.copysign(frict, vel)
    
    mouse_scrolling = window.event.check_flag(EVENT_FLAG.MOUSE_WHEEL)

    cam_dist -= 100 * mouse_scrolling*window.event.mouse.wheel.y * window.dt

    
    cam_rot = copy(camera.rotation)

    mouse_moving = window.event.check_flag(EVENT_FLAG.MOUSE_MOTION)
    
    cam_rot.rotate(cam_rot.right, -mouse_moving*math.radians(window.event.mouse.rel_y * 10) * window.dt)
    if abs(cam_rot.up.y) > 0.4:
        camera.rotation = cam_rot
    camera.rotation.rotate(car.rotation.up, mouse_moving*math.radians(window.event.mouse.rel_x * 10) * window.dt)
    


    camera.position = car.position - camera.rotation.forward * cam_dist

    # Re-render the scene.
    window.update()
    # This also refreshes window.current_event.
    counter += counter_speed
```

Then when we run it, it looks something like this:

![](https://github.com/FrewtyPebbles/Runespoor-Python3D-Game-Engine/blob/main/tests/lighting_fixed_more.gif)

# How to Import 3D Assets:

For now, your 3d assets. must be structured a specific way to be picked up by the asset importer.

The 3d asset file must be accompanied by an adjacent `\textures\` folder where you must store all image assets for your 3d asset file.  This may or may not be changed in the future, the asset importing system is not finalized.

Your assets might look something like this:

```
asset_1_name_of_this_folder_doesnt_matter\
    my_3d_thing.obj
    textures\
        name_of_texture_file.png

asset_2_name_of_this_folder_doesnt_matter_either\
    my_other_3d_thing.fbx
    textures\
        name_of_other_texture_file.png
```

Then you would import yout assets like so:

```py
asset1 = Mesh.from_file("...../asset_1_name_of_this_folder_doesnt_matter/my_3d_thing.fbx")

asset2 = Mesh.from_file("...../asset_2_name_of_this_folder_doesnt_matter_either/my_other_3d_thing.fbx")
```

And voila!  Your 3D assets are now imported and ready to be used in objects and rendered to the screen.


# TODO:

 - Add `from_raw` constructor to mesh to make procedural meshes possible.

 - Add window/fullscreen scaling.

 - Add Matrix datastructures for GLM.

 - Add `Quaternion` type to `set_uniform`

 - Add `PointLight` class

# Future Plans:

 - Switch to Vulkan.
