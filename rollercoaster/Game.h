#pragma once

#include "Common.h"

// Classes used in game
class CCamera;
class CSkybox;
class CShader;
class CShaderProgram;
class CPlane;
class CFreeTypeFont;
class CHighResolutionTimer;
class GameWindow;
class CObjModel;
class CSphere;

class Game {
public:

private:
	void Init();
	void Update();
	void Render();

	CSkybox *pSkybox;
	CCamera *pCamera;
	vector <CShaderProgram *> *pShaderPrograms;
	CPlane *pPlanarTerrain;
	CFreeTypeFont *pFtFont;
	CObjModel *pObjBarrel;
	CObjModel *pObjHorse;
	CSphere *pSphere;



public:
	enum {
		FPS = 30
	};

	static Game& GetInstance();

	~Game();

	WPARAM Exec();

	void SetHinstance(HINSTANCE hinstance);
	LRESULT ProcessEvents(HWND window,UINT message, WPARAM w_param, LPARAM l_param);

	void DisplayFrameRate();

private:
	Game::Game();
	Game::Game(const Game&);
	void Game::operator=(const Game&);
	

	GameWindow &rGameWindow;
	HINSTANCE hHinstance;

	double dDt;
	int iFramesPerSecond;
	CHighResolutionTimer *pHighResolutionTimer;
	bool bAppActive;
};
