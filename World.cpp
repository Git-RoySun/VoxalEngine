#include "World.h"
#include "Updatable.h"

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

float interpolate(float a0, float a1, float w){
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

void World::loadVoxel(Voxel::Instance instance){
  auto o = &objects.emplace_back(std::move(Voxel(instance)));
  vc.addInstance(instance);
}

void World::loadWorld() {//load objects
	const float VOXELS = 100.f;
  const float SIZE = 1.f / 16.f;
  for (int x = -VOXELS; x < VOXELS; x++) {
    for (int z = -VOXELS ; z < VOXELS; z++) {
      float val = 0;
      float freq = 1;
      float amp = 1;
      for (int i = 0; i < 12; i++){
        val += perlin((x+VOXELS) * freq / VOXELS, (z+VOXELS) * freq / VOXELS) * amp;
        freq *= 2;
        amp /= 2;
      }
      val *= 1.2;
      glm::clamp(val,-1.f,1.f);
      for (float i = SIZE-(int)(val/SIZE)*SIZE; i > -val; i -= SIZE) {
        loadVoxel(Voxel::Instance{
          .position = { (SIZE)*x * 2 , i*2 ,(SIZE)*z * 2 },
        	.scale = glm::vec3{ SIZE }
        });
      }
    }
  }

    loadVoxel(Voxel::Instance{
      .position = {  -2,-1, 0},
      .scale = glm::vec3{0.5f},
    	.colour = { 0.f,0.f,0.f }});
}

void World::setup(){
  srand(seed);
  loadWorld();
  configureControl();
}

void World::configureControl(){
  CameraObject* camera = &cameras.emplace_back();
  camera->setPosition({ 0.f,-0.5f,0.f });
  vc.setCamera(camera->getCamera());
  camController = ic::FPMovementController(camera,camera);

  ic::InputModule::addMouseListener(&camController);
  ic::InputModule::addKeyListener(GLFW_KEY_W, &camController);
  ic::InputModule::addKeyListener(GLFW_KEY_A, &camController);
  ic::InputModule::addKeyListener(GLFW_KEY_S, &camController);
  ic::InputModule::addKeyListener(GLFW_KEY_D, &camController);

  ic::InputModule::addKeyListener(GLFW_KEY_F, &camController);
  ic::InputModule::addKeyListener(GLFW_KEY_SPACE, &camController);
  ic::InputModule::setDirection(GLFW_KEY_SPACE, UP);
  ic::InputModule::setDirection(GLFW_KEY_F, DOWN);

  ic::InputModule::setDirection(GLFW_KEY_W, FRONT);
  ic::InputModule::setDirection(GLFW_KEY_A, LEFT);
  ic::InputModule::setDirection(GLFW_KEY_S, BACK);
  ic::InputModule::setDirection(GLFW_KEY_D, RIGHT);

  //this could be done in the visual context class instead
  auto window = vc.getWindow().getGlWindow();
  glfwSetCursorPos(window, 0, 0);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetKeyCallback(window, ic::InputModule::sendKeyEvent);
  glfwSetCursorPosCallback(window, ic::InputModule::sendMouseEvent);
}


void World::run() {
  while (alive) {
    glfwPollEvents();
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<float> delta = now - last;
    last = now;
    
    Updatable::updateAll(delta.count());
    vc.renderFrame();
  }
};