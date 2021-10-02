#pragma once

static unsigned int SCR_WIDTH = 800;
static unsigned int SCR_HEIGHT = 800;

class Game;
class Window {
public:
    Window() = default;
    ~Window() = default;

    bool setup();
    bool open();
	bool run();
	bool menuLoop(Game& game);
	bool gameLoop(Game& game);

	
private:

	void renderImgui();

	double currentTime;
	double previousTime;
	double dt = 0.0;

	float fps = 0.0f;
	float tempFps = 0.0f;
	int counter = 0;
	float fpsUpdate = 1.0f;
	float fpsUpdateTimer = 0.0f;

    double minUpdateRate = 1.0f / 60.0f;
	double updateTimer = 0.0f;
	int updatesSinceRender = 0;
};