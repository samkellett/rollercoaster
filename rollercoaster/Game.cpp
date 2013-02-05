/* 
OpenGL Template for INM376 / IN3005
City University London, School of Informatics
Source code drawn from a number of sources and examples, including contributions from
 - Drs. Greg Slabaugh, Chris Child, Dean Mohamedally
 - Ben Humphrey (gametutorials.com), Michal Bubner (mbsoftworks.sk), Christophe Riccio (glm.g-truc.net)
 - Christy Quinn, Sam Kellett
 and others

 For educational use by School of Informatics, City University London UK.

 This template contains a skybox, simple terrain, camera, lighting, shaders, texturing

 Potential ways to modify the code:  Add new geometry types, shaders, change the terrain, load new meshs, set up lighting, 
 different camera controls, etc.
 
 Template version 1.5a 31/01/2013
 Dr. Greg Slabaugh (gregory.slabaugh.1@city.ac.uk) 
*/


#include "game.h"

#include "include/gl/glew.h"
#include <gl/gl.h>

//setup includes
#include "HighResolutionTimer.h"
#include "GameWindow.h"

//game includes
#include "Camera.h"
#include "Skybox.h"
#include "Plane.h"
#include "Shaders.h"
#include "FreeTypeFont.h"
#include "ObjModel.h"
#include "Sphere.h"
#include "MatrixStack.h"


Game::Game() : 
	pSkybox(NULL), pCamera(NULL), pShaderPrograms(NULL), pPlanarTerrain(NULL), pFtFont(NULL), pObjBarrel(NULL), pObjHorse(NULL), pSphere(NULL),
	//setup objects
	rGameWindow(GameWindow::GetInstance()), pHighResolutionTimer(NULL), dDt(0.0), iFramesPerSecond(0)
{
}

Game::~Game() 
{ 
	//game objects
	delete pCamera;
	delete pSkybox;
	delete pShaderPrograms;
	delete pPlanarTerrain;
	delete pFtFont;
	delete pObjBarrel;
	delete pObjHorse;
	delete pSphere;


	//setup objects
	delete pHighResolutionTimer;
}

void Game::Init() 
{
	// Set the clear colour and depth
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClearDepth(1.0f);

	/// Create objects
	pCamera = new CCamera;
	pSkybox = new CSkybox;
	pShaderPrograms = new vector <CShaderProgram *>;
	pPlanarTerrain = new CPlane;
	pFtFont = new CFreeTypeFont;
	pObjBarrel = new CObjModel;
	pObjHorse = new CObjModel;
	pSphere = new CSphere;

	RECT dimensions = rGameWindow.GetDimensions();

	int width = dimensions.right - dimensions.left;
	int height = dimensions.bottom - dimensions.top;

	// Set the orthographic and perspective projection matrices based on the image size
	pCamera->SetOrthographicProjectionMatrix(width, height); 
	pCamera->SetPerspectiveProjectionMatrix(45.0f, (float) width / (float) height, 0.5f, 5000.0f);

	// Load shaders
	vector<CShader> shShaders;
	vector<string> sShaderFileNames;
	sShaderFileNames.push_back("perVertexLighting.vert");
	sShaderFileNames.push_back("perVertexLighting.frag");
	sShaderFileNames.push_back("ortho2D.vert");
	sShaderFileNames.push_back("font2D.frag");

	for (int i = 0; i < (int) sShaderFileNames.size(); i++) {
		string sExt = sShaderFileNames[i].substr((int) sShaderFileNames[i].size()-4, 4);
		int iShaderType = sExt == "vert" ? GL_VERTEX_SHADER : (sExt == "frag" ? GL_FRAGMENT_SHADER : GL_GEOMETRY_SHADER);
		CShader shader;
		shader.LoadShader("resources\\shaders\\"+sShaderFileNames[i], iShaderType);
		shShaders.push_back(shader);
	}

	// Create the main shader program
	CShaderProgram *pMainProgram = new CShaderProgram;
	pMainProgram->CreateProgram();
	pMainProgram->AddShaderToProgram(&shShaders[0]);
	pMainProgram->AddShaderToProgram(&shShaders[1]);
	pMainProgram->LinkProgram();
	pShaderPrograms->push_back(pMainProgram);

	// Create a shader program for fonts
	CShaderProgram *pFontProgram = new CShaderProgram;
	pFontProgram->CreateProgram();
	pFontProgram->AddShaderToProgram(&shShaders[2]);
	pFontProgram->AddShaderToProgram(&shShaders[3]);
	pFontProgram->LinkProgram();
	pShaderPrograms->push_back(pFontProgram);

	// Create the skybox
	// Skybox downloaded from http://www.akimbo.in/forum/viewtopic.php?f=10&t=9
	pSkybox->Create("resources\\skyboxes\\jajdarkland1\\", "jajdarkland1_ft.jpg", "jajdarkland1_bk.jpg", "jajdarkland1_lf.jpg", "jajdarkland1_rt.jpg", "jajdarkland1_up.jpg", "jajdarkland1_dn.jpg", 2500.0f);
	
	// Create the planar terrain
	//pPlanarTerrain->Create("resources\\textures\\", "Tile41a.jpg", 2000.0f, 2000.0f, 100.0f);
	pPlanarTerrain->Create("resources\\textures\\", "grassfloor01.jpg", 2000.0f, 2000.0f, 50.0f); // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013

	
	pFtFont->LoadSystemFont("arial.ttf", 32);
	pFtFont->SetShaderProgram(pFontProgram);

	// Load some meshes in OBJ format
	pObjBarrel->Load("resources\\models\\Barrel\\Barrel02.obj", "Barrel02.mtl");  // Downloaded from http://www.psionicgames.com/?page_id=24 on 24 Jan 2013
	pObjHorse->Load("resources\\models\\Horse\\Horse2.obj", "Horse2.mtl");  // Downloaded from http://opengameart.org/content/horse-lowpoly on 24 Jan 2013
	pSphere->Create("resources\\textures\\", "dirtpile01.jpg", 25, 25);  // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013

	// Set the texture sampler in the fragment shader
	pMainProgram->SetUniform("gSampler", 0);

}



void Game::Render() 
{

	
	// Clear the buffers and enable depth testing (z-buffering)
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	// Set up a matrix stack
	glutil::MatrixStack modelViewMatrixStack;
	modelViewMatrixStack.SetIdentity();

	// Use the main shader program 
	CShaderProgram *pMainProgram = (*pShaderPrograms)[0];
	pMainProgram->UseProgram();
	pMainProgram->SetUniform("bUseTexture", true);

	// Set the projection and modelview matrix based on the current camera location	
	pMainProgram->SetUniform("matrices.projMatrix", pCamera->GetPerspectiveProjectionMatrix());
	modelViewMatrixStack.LookAt(pCamera->GetPosition(), pCamera->GetView(), pCamera->GetUpVector());


	
	// Set light and materials in main shader program
	glm::vec4 vPosition(-100, 100, -100, 1);
	glm::mat3 normalMatrix = pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top());
	// Convert light position to eye coordinates, since lighting is done in eye coordinates
	glm::vec4 vLightEye = modelViewMatrixStack.Top()*vPosition;
	pMainProgram->SetUniform("light1.position", vLightEye);		// Position of light source in eye coordinates
	pMainProgram->SetUniform("light1.La", glm::vec3(1.0f));		// Ambient colour of light
	pMainProgram->SetUniform("light1.Ld", glm::vec3(1.0f));		// Diffuse colour of light
	pMainProgram->SetUniform("light1.Ls", glm::vec3(1.0f));		// Specular colour of light
	pMainProgram->SetUniform("material1.Ma", glm::vec3(1.0f));	// Ambient material reflectance
	pMainProgram->SetUniform("material1.Md", glm::vec3(0.0f));	// Diffuse material reflectance
	pMainProgram->SetUniform("material1.Ms", glm::vec3(0.0f));	// Specular material reflectance
	pMainProgram->SetUniform("material1.shininess", 15.0f);		// Shininess material property


	// Render the skybox and terrain with full ambient light 
	modelViewMatrixStack.Push();
		// Translate the modelview matrix to the camera eye point so skybox stays centred around camera
		glm::vec3 vEye = pCamera->GetPosition();
		modelViewMatrixStack.Translate(vEye);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		pSkybox->Render();
	modelViewMatrixStack.Pop();
	

	// Render the planar terrain
	modelViewMatrixStack.Push();
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		pPlanarTerrain->Render();
	modelViewMatrixStack.Pop();


	// Turn on diffuse + specular materials
	pMainProgram->SetUniform("material1.Ma", glm::vec3(0.5f));	// Ambient material reflectance
	pMainProgram->SetUniform("material1.Md", glm::vec3(0.5f));	// Diffuse material reflectance
	pMainProgram->SetUniform("material1.Ms", glm::vec3(1.0f));	// Specular material reflectance	


	// Render the horse 
	modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(glm::vec3(0.0f, 0.0f, 0.0f));
		modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 180.0f);
		modelViewMatrixStack.Scale(2.5f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		pObjHorse->Render();
	modelViewMatrixStack.Pop();

	
	// Render the barrel 
	modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(glm::vec3(100.0f, 0.0f, 0.0f));
		modelViewMatrixStack.Scale(5.0f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		pObjBarrel->Render();
	modelViewMatrixStack.Pop();


	// Render the sphere
	modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(glm::vec3(0.0f, 2.5f, 150.0f));
		modelViewMatrixStack.Scale(2.0f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		// To turn off texture mapping and use the sphere colour only (currently white material), uncomment the next line
		//pMainProgram->SetUniform("bUseTexture", false);
		pSphere->Render();
	modelViewMatrixStack.Pop();

	DisplayFrameRate();

	// Swap buffers to show the rendered image
	SwapBuffers(rGameWindow.Hdc());		
}



void Game::Update() 
{
	// Update the camera using the amount of time that has elapsed to avoid framerate dependent motion
	pCamera->Update(dDt);
}



void Game::DisplayFrameRate()
{
	static int frameCount = 0;
	static double elapsedTime = 0.0f;

	CShaderProgram *fontProgram = (*pShaderPrograms)[1];

	RECT dimensions = rGameWindow.GetDimensions();
	int height = dimensions.bottom - dimensions.top;

	// Increase the elapsed time and frame counter
	elapsedTime += dDt;
    frameCount++;

	// Now we want to subtract the current time by the last time that was stored
	// to see if the time elapsed has been over a second, which means we found our FPS.
    if(elapsedTime > 1000 )
    {
		elapsedTime = 0;
		iFramesPerSecond = frameCount;

		// Reset the frames per second
        frameCount = 0;
    }

	if (iFramesPerSecond > 0) {
		// Use the font shader program and render the text
		fontProgram->UseProgram();
		glDisable(GL_DEPTH_TEST);
		fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1));
		fontProgram->SetUniform("matrices.projMatrix", pCamera->GetOrthographicProjectionMatrix());
		fontProgram->SetUniform("vColour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		pFtFont->PrintFormatted(20, height - 20, 20, "FPS: %d", iFramesPerSecond);
	}


}
		

WPARAM Game::Exec() 
{
	pHighResolutionTimer = new CHighResolutionTimer;
	rGameWindow.Init(hHinstance);

	if(!rGameWindow.Hdc()) {
		return 1;
	}

	Init();

	pHighResolutionTimer->Start();

	double frame_duration = 1000.0 / (double) Game::FPS;

	MSG msg;

	while(1) {													
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { 
			if(msg.message == WM_QUIT) {
				break;
			}

			TranslateMessage(&msg);	
			DispatchMessage(&msg);
		} else if (bAppActive) {
			/*
			// Fixed timer
			dt_ = high_resolution_timer_->elapsed();

			if (dt_ > frame_duration) {
				high_resolution_timer_->start();

				update();
				render();
			}
			*/

			// Variable timer
				pHighResolutionTimer->Start();

				Update();
				Render();
				dDt = pHighResolutionTimer->Elapsed();

				

		} 
		else Sleep(200); // Do not consume processor power if application isn't active
	}

	rGameWindow.Deinit();

	return(msg.wParam);
}

LRESULT Game::ProcessEvents(HWND window,UINT message, WPARAM w_param, LPARAM l_param) 
{
	LRESULT result = 0;

	switch (message) {


	case WM_ACTIVATE:
	{
		switch(LOWORD(w_param))
		{
			case WA_ACTIVE:
			case WA_CLICKACTIVE:
				bAppActive = true;
				pHighResolutionTimer->Start();
				break;
			case WA_INACTIVE:
				bAppActive = false;
				break;
		}
		break;
		}

	case WM_SIZE:
			RECT dimensions;
			GetClientRect(window, &dimensions);
			rGameWindow.SetDimensions(dimensions);
		break;

	case WM_PAINT:
		PAINTSTRUCT ps;
		BeginPaint(window, &ps);
		EndPaint(window, &ps);
		break;

	case WM_KEYDOWN:
		switch(w_param) {
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		result = DefWindowProc(window, message, w_param, l_param);
		break;
	}

	return result;
}

Game& Game::GetInstance() 
{
	static Game instance;

	return instance;
}

void Game::SetHinstance(HINSTANCE hinstance) 
{
	hHinstance = hinstance;
}