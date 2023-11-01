#include "ChunkLoader.h"

#include "Material.h"
const float ChunkLoader::CHUNKSIZE = 8;
const float ChunkLoader::VOXELSIZE = 1.f/16.f;
static const int HEIGHT = 3;
glm::vec2 randomGradient(int ix, int iy) {
  // No precomputed gradients mean this works for any number of grid coordinates
  const unsigned w = 8 * sizeof(unsigned);
  const unsigned s = w / 2;
  unsigned a = ix, b = iy;
  a *= 3284157443;

  b ^= a << s | a >> w - s;
  b *= 1911520717;

  a ^= b << s | b >> w - s;
  a *= 2048419325;
  float random = a * (glm::pi<float>() / ~(~0u >> 1)); // in [0, 2*Pi]

  return glm::vec2{sin(random), cos(random)};
}

// Computes the dot product of the distance and gradient vectors.
float dotGridGradient(int ix, int iy, float x, float y) {
  // Get gradient from integer coordinates
  glm::vec2 gradient = randomGradient(ix, iy);

  // Compute the distance vector
  float dx = x - (float)ix;
  float dy = y - (float)iy;

  // Compute the dot-product
  return (dx * gradient.x + dy * gradient.y);
}

float interpolate(float a0, float a1, float w) {
  return (a1 - a0) * (3.0 - w * 2.0) * w * w + a0;
}

// Sample Perlin noise at coordinates x, y
float perlin(float x, float y) {

  // Determine grid cell corner coordinates
  int x0 = (int)x;
  int y0 = (int)y;
  int x1 = x0 + 1;
  int y1 = y0 + 1;

  // Compute Interpolation weights
  float sx = x - (float)x0;
  float sy = y - (float)y0;

  // Compute and interpolate top two corners
  float n0 = dotGridGradient(x0, y0, x, y);
  float n1 = dotGridGradient(x1, y0, x, y);
  float ix0 = interpolate(n0, n1, sx);

  // Compute and interpolate bottom two corners
  n0 = dotGridGradient(x0, y1, x, y);
  n1 = dotGridGradient(x1, y1, x, y);
  float ix1 = interpolate(n0, n1, sx);

  // Final step: interpolate between the two previously interpolated values, now in y
  float value = interpolate(ix0, ix1, sy);

  return value;
}

bool ChunkLoader::loadChunk(int cx, int cz){
	if(!chunks.contains(std::make_pair(cx,cz))){
    Chunk chunk{};
    for (int x = 0; x < CHUNKSIZE;x++) {
      for (int z = 0; z < CHUNKSIZE; z++) {
        float val = 0;
        float freq = 1;
        float amp = HEIGHT;
        float xPos = (cx*CHUNKSIZE+x)*VOXELSIZE;
        float zPos = (cz*CHUNKSIZE+z)*VOXELSIZE;
        for (int i = 0; i < 12; i++) {
          val += perlin((xPos+1000000)/2 * freq / CHUNKSIZE, (zPos+ 1000000)/2 * freq / CHUNKSIZE) * amp;
          freq *= 2;
          amp /= 2;
        }
        val *= 1.2;
        for (float i = (1 * VOXELSIZE) - (int)(val / VOXELSIZE) * VOXELSIZE; i > -val; i -= VOXELSIZE) {
	        obj::Voxel::Instance instance{
            .position = glm::vec3{ xPos,i,zPos },
            .scale = glm::vec3{ VOXELSIZE },
            .materialID = (i - VOXELSIZE > -val) ? vc::Material::RED.getId() : vc::Material::GREEN.getId()
          };
          chunk.voxels.emplace_back(instance);
          if (!vc.addInstance(instance)) return false;
        }
      }
    }
    chunks.insert(std::make_pair(std::make_pair(cx,cz),std::move(chunk)));
    return true;
	}
}
 
void ChunkLoader::loadAround(float x, float z){
  int cx = floor(x / (CHUNKSIZE*VOXELSIZE));
  int cz = floor(z / (CHUNKSIZE*VOXELSIZE));

  for (int x = -loadDistance/2; x <= loadDistance/2;x++) {
    for (int z = -loadDistance / 2; z <= loadDistance/2; z++) {
    	if(!loadChunk(cx+x, cz+z)){
        vc.clearInstances();
        chunks = std::map<std::pair<int, int>, Chunk>{};
        x = -loadDistance / 2;
        z = (- loadDistance / 2)+1;
    	}
    }
  }
  obj::Voxel::Instance instance{
  .position = glm::vec3{ 0,0,0,},
  .scale = glm::vec3{ 2 },
  .materialID = vc::Material::RED.getId()};
  vc.addInstance(instance);
}
