#pragma once

#include "Camera.hpp"
#include "Objects/Quad.hpp"
#include "ShaderPrograms/SimpleShaderProgram.hpp"
#include "ShaderPrograms/InstancedShaderProgram.hpp"
#include "ShaderPrograms/QuadShaderProgram.hpp"
#include "Objects/ScreenQuad.hpp"
#include "Objects/InstancedQuadManager.hpp"
#include "Objects/LowPolyLiquid.hpp"

class Rendering {
public:
    static Rendering& getInstance() {
        static Rendering instance; 
		return instance;
    }

    Rendering(Rendering const&) = delete;
	void operator=(Rendering const&) = delete;

    ~Rendering() = default;

    void reset();
	Quad* getNewQuad();
    InstancedQuadManager* getQuadManager();
	Camera* getCamera();

    
	void update(float dt);
    void draw();
    void init(unsigned int width, unsigned int height);
    void postPass();
private:
    Rendering();

    Camera m_camera;


    InstancedShaderProgram m_instancedShaderProgram;
    InstancedQuadManager m_quadManager;

    SimpleShaderProgram m_simpleShaderProgram;
    LowPolyLiquid m_lowPolyLiquid;

    QuadShaderProgram m_screenShaderProgram;
    ScreenQuad m_screenQuad;

    unsigned int m_preBuffer;
    unsigned int m_preTex;
    unsigned int m_rbo;
    unsigned int m_width, m_height;
    unsigned int quadVAO, quadVBO;
    void initGL();
};
