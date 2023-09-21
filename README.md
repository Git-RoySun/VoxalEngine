# Voxal Engine
Voxal Engine is a voxel game engine made with Vulkan and GLFW with a focus on procedural generation.

![image](https://github.com/Git-RoySun/Engine2/assets/25252952/d10aad84-218e-48dd-81e3-1516461b7aeb)


<details>
<summary>More Pictures</summary>
  
![gallery](https://github.com/Git-RoySun/VoxalEngine/assets/25252952/214c9555-fb11-496a-82ed-9dd6e674eb99)
</details>

## Plan:
- Add collision detection and gravity/physics (through physics controller class)
- Add biomes (with better chunk class)
- Add shading/ray-tracing
- Make View projection far plane round (cuttoff as a spehere not a plane, so when you spin the cutoff looks identical)
- Chunk management should be compute shaders (will need hash map in glsl)

## (Optional) Todo:
- Make chunk loading a queue so it doesnt freeze when many are added
- Remove outlines for all blocks except target
- Add sea level/ water
- Add jumping
- Serializable input configurations
- File loading for input configuration

## Done:
- Voxel material class
- Chunk UnLoading
- Add multisampling
- Integrate imgui
- Extend world generation to be on the fly as player moves.
- Reorganize World.cpp and VisualModule.cpp
- Rearange the buffer creation functions in model.cpp
- Fix Y-axis movement
