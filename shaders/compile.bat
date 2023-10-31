glslc.exe shader.vert -o vert.spv 
glslc.exe shader.frag -o frag.spv
glslc.exe line.vert -o line.spv
glslc.exe closesthit.rchit -o closesthit.spv --target-env=vulkan1.3
glslc.exe raygen.rgen -o raygen.spv --target-env=vulkan1.3
glslc.exe miss.rmiss -o miss.spv --target-env=vulkan1.3
glslc.exe intersection.rint -o intersection.spv --target-env=vulkan1.3
pause
