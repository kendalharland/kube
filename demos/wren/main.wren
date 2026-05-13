// -- File: kube.wren

class Kube {
    foreign static runGameLoop()
}

class Window {
    foreign static open(width, height, title)
}

// -- File: main.wren

var title = "Rotating Kube"
var width = 1000
var height = 800

Window.open(width, height, title)

Kube.runGameLoop()

/* Code to port:
 
void runGameLoop(WrenVM* vm) {
  using namespace kube::graphics;
  
  kube::Camera camera;
  auto window = kube::Window::GetInstance();
  window->Open(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE);
  window->SetCamera(&camera);

  shader_ptr shader = Shader::SimpleColorShader("src/shaders");
  
  kube::Actor actor;
  actor.SetModel(kube::CreateCubeModel());
  actor.SetState(std::make_unique<IdleState>());

  kube::Stopwatch stopwatch;
  stopwatch.Start();

  do {
    auto dt = stopwatch.Lap();

    actor.HandleInput(window);
    actor.Update(dt);
    window->Clear();
    actor.GetModel()->Draw(&camera, shader);
    window->Update();
  } while (!window->ShouldClose());

  window->Close();
}
 **/