// GLC 2.0 Tutorial Example
//
// Interface for Main Application Class.
//
// Copyright(C) GPH(Gamepark Holdings). All rights reserved.
// Dogma-G Game Studios In-House Library DGE
//
////////////////////////////////////////////////////////////////////////////////


#ifndef _AppFrame_H_
#define _AppFrame_H_


#define	LC_DISPLAY_WIDTH	800
#define	LC_DISPLAY_HEIGHT	480

#define	PLAYER_WIDTH		64
#define	PLAYER_HEIGHT		64

#define	PLAYERSPEED			1
#define PLAYERINITLIFE		3

#define	MISSILE_WIDTH		7
#define	MISSILE_HEIGHT		4

#define	ENEMY_WIDTH			41
#define	ENEMY_HEIGHT		40

#define	SHIELD_WIDTH		64
#define	SHIELD_HEIGHT		64

#define	GAME_MAX_TEXTURE	8
#define	GAME_MAX_SOUND		4

#define	GHOSTDURATIONmax	180
#define	CELLNUMBER			4
#define	CELLCHANGETIME		30

/*
#define LEFTDIRECTION		-1.0f
#define RIGHTDIRECTION		+1.0f
#define UPDIRECTION			-1.0f
#define DOWNDIRECTION		+1.0f

#define	MISSILESPEED		2
#define	MISSILEMAX			3

#define ENEMYSPEEDmin		0.2f
#define ENEMYSPEEDmax		1.0f
#define	ENEMIESMAX			10

#define SCROLLSPEED			0.5f
#define SHIELDRATIOmax		1.8f
#define SHIELDRATIOmin		1.2f
#define	SHIELDENERGYmax		100
#define	ENERGYBARmax		160
*/


#define LEFTDIRECTION		-1.0f
#define RIGHTDIRECTION		+1.0f
#define UPDIRECTION			-1.0f
#define DOWNDIRECTION		+1.0f

#define	MISSILESPEED		2
#define	MISSILEMAX			3

#define ENEMYSPEEDmin		0.2f
#define ENEMYSPEEDmax		1.0f
#define	ENEMIESMAX			10

#define SCROLLSPEED			0.5f
#define SHIELDRATIOmax		1.8f
#define SHIELDRATIOmin		1.2f
#define	SHIELDENERGYmax		100
#define	ENERGYBARmax		160


enum PLAYER_STATUS
{
	PLAYER_STATUS_ALIVE,
	PLAYER_STATUS_DEAD,
	PLAYER_STATUS_GHOST,
};


enum SCROLL_DIRECTION
{
	SCROLL_DIRECTION_LEFT,
	SCROLL_DIRECTION_RIGHT,
	SCROLL_DIRECTION_UP,
	SCROLL_DIRECTION_DOWN,
};


class CAppGame
{
public:
	LC_HWND			m_hWnd		;		// Window Handle
	PLC_DEVICE		m_pDev		;		// Device
	PLC_SPRITE		m_pSpt		;		// 2D Sprite
	PLCX_INPUT		m_pInput	;		// Input

	PLC_FONT		m_pFontScore;		// Font Score

	PLC_TEXTURE		m_pTex[GAME_MAX_TEXTURE];
	PLCX_SOUND		m_pSnd[GAME_MAX_SOUND]	;

public:
	TLC_RAND	m_Random;

	INT			m_bIsExit;		// Exit State

	INT			m_PlayerStatus;
	INT			m_PlayerLife;
	INT			m_bIsFilre;
	GameObject	m_Player;


	INT			m_GhostDuration;

	LC_RECTF	m_Cells[CELLNUMBER];
	INT			m_CellSelect;
	INT			m_CellDisplayTime;

	GameObject	m_PlayerMissiles[MISSILEMAX];
	GameObject	m_EnemyMissiles[MISSILEMAX];
	GameObject	m_Enemies[ENEMIESMAX];

	INT			m_Score;
	INT			m_ScoreH;			// High Score
	char		m_sScore[256];

	FLOAT		m_Volume;

	LCXVECTOR2	m_BgPosition;				// Back ground Position
	INT			m_ScrollDirection;

	FLOAT		m_ShieldRatio;
	INT			m_ShieldTimeDelta;
	DOUBLE		m_ShieldTimeBgn;
	FLOAT		m_ShieldExtend;
	FLOAT		m_ShieldTransparency;
	INT			m_ShieldEnergy;

public:
	CAppGame();

	FLOAT				GetAppFrame();

	static INT     LC_WINAPI App_Main(LC_HANDLE);
	INT		App_Init();
	INT		App_Remove();
	INT		App_RscLoad();
	INT		App_Run();


	void SystemControl();
	void UpdatePlayer();
	void UpdatePlayerMissiles();
	void UpdateEnemies();
	void UpdateEnemyMissiles();
	void UpdateHighScore();

	void CollisionPlayerMissilesAndEnemies();
	void CollisionPlayerAndEnemies();
	void CollisionEnemyMissilesAndPlayer();

	void ShieldPulse();
	void ShieldRecharge();
	INT  ShieldProtect(INT damage);
	void ScrollBackground();

	void DrawBackground();
	void DrawMissile();
	void DrawPlayer();
	void DrawShield();
	void DrawEnergyBar();
	void DrawPlayerLife();
	void DrawScore();

	void ObjectInitialize();

	void LoadHighScore();
	void SaveHighScore();


	void PlayerRevive();
	void PlayerDead();

	void FirePlayerMissile();
	void FireEnemyMissile(LCXVECTOR2 position, LCXVECTOR2 direction, FLOAT rotation);

	void SetVolume();
};


extern CAppGame* m_pApp;



#endif

