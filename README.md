# Voxal Engine
Voxal Engine is a voxel game engine made with Vulkan and GLFW.
![image](https://github.com/Git-RoySun/Engine2/assets/25252952/d10aad84-218e-48dd-81e3-1516461b7aeb)

## Plan:
- Voxel material class
- Add collision detection and gravity/physics (through physics controller class)
- Add sea level/ water
- Add biomes (with better chunk class)
- Add shading/ray-tracing
- Remove outlines for all blocks except target
- Make View projection far plane round (cuttoff as a spehere not a plane, so when you spin the cutoff looks identical)
- Chunk management should be compute shaders (will need hash map in glsl)

## (Optional) Todo:
- Add jumping
- Serializable input configurations
- File loading for input configuration

## Done:
- Chunk UnLoading
- Add multisampling
- Integrate imgui
- Extend world generation to be on the fly as player moves.
- Reorganize World.cpp and VisualModule.cpp
- Rearange the buffer creation functions in model.cpp
- Fix Y-axis movement
