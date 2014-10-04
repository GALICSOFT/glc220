// GLC 2.0 Tutorial Example
//
// Implementation of Main Application Class.
//
// Copyright(C) GPH(Gamepark Holdings). All rights reserved.
// Dogma-G Game Studios In-House Library DGE
//
////////////////////////////////////////////////////////////////////////////////


#if defined(_MSC_VER) && _MSC_VER >= 1200
  #pragma warning(disable: 4996)
#endif


#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include <LcType.h>
#include <LcBase.h>
#include <LcAux.h>
#include <LcMath.h>
#include <LcStdev.h>
#include <LcxEntity.h>
#include <LcxMedia.h>

#include "GameObject.h"
#include "AppFrame.h"



static CAppGame* g_pAppFrame = NULL;



CAppGame::CAppGame()
{
	g_pAppFrame = this;

	m_hWnd		= NULL;		// Window Handle
	m_pDev		= NULL;		// Device
	m_pSpt		= NULL;		// 2D Sprite
	m_pInput	= NULL;		// Input

	m_pFontScore	= NULL;		// Font

	memset(m_pTex, 0, sizeof(PLC_TEXTURE) * GAME_MAX_TEXTURE);
	memset(m_pSnd, 0, sizeof(PLCX_SOUND ) * GAME_MAX_SOUND);


	m_bIsExit	= 0;		// Exit State

	m_PlayerStatus = PLAYER_STATUS_ALIVE;
	m_PlayerLife = PLAYERINITLIFE;
	m_bIsFilre = LC_FALSE;


	m_GhostDuration = 0;
	m_CellSelect = 0;
	m_CellDisplayTime = 0;

	m_Score = 0;
	m_ScoreH = 1000;			// High Score
	memset(m_sScore, 0, sizeof(m_Score) );

	m_Volume = 1.F;

	m_ScrollDirection = SCROLL_DIRECTION_DOWN;

	m_ShieldRatio = SHIELDRATIOmin;
	m_ShieldTimeDelta = 30;
	m_ShieldTimeBgn = LcSys_GetTickCount();
	m_ShieldExtend = 1.F;
	m_ShieldTransparency = 1.0F;
	m_ShieldEnergy = SHIELDENERGYmax;
}


INT CAppGame::App_Init()
{
	UINT	dColor = 0xFF006699;

	// Set Main procedure Function Pointer
	if(LC_FAILED(LcSys_SetMainRunFunc(App_Main)))
		return LC_EFAIL;


	// Create Window
	m_hWnd = LcSys_CreateWindow(NULL, "2D Tutorial", LC_DISPLAY_WIDTH, LC_DISPLAY_HEIGHT);
	if(NULL == m_hWnd)
		return LC_EFAIL;

	printf("LcSys_CreateWindow Success------------------------------\n");

	// Create GLC Rendering Device
	if(LC_FAILED(LcDev_CreateDevice(NULL, &m_pDev, m_hWnd)))
		return LC_EFAIL;


	printf("LcDev_CreateDevice Success------------------------------\n");

	// Setting Clear Buffer Color
	m_pDev->SetRenderState(LC_RS_CLEAR_COLOR, dColor);


	// Create 2D Rendering Sprite
	if(LC_FAILED(LcDev_CreateSprite(NULL, &m_pSpt)))
		return LC_EFAIL;

	printf("Create Sprite Success-----------------------------------\n");


	// Create Input Device
	if(LC_FAILED(LcxEnt_CreateInput(NULL, &m_pInput, m_hWnd)))
		return LC_EFAIL;


	return LC_OK;
}



INT CAppGame::App_Remove()
{
	INT i;

	// Release Texture
	for(i=0; i<GAME_MAX_TEXTURE; ++i)
		SAFE_RELEASE(	m_pTex[i]	);

	// Release Sound Instance
	for(i=0; i<GAME_MAX_SOUND; ++i)
		SAFE_RELEASE(	m_pSnd[i]	);


	SAFE_RELEASE(	m_pFontScore	);		// Release Font Instance

	SAFE_RELEASE(	m_pInput	);		// Release Input
	SAFE_RELEASE(	m_pSpt	);		// Release Sprite

	LcxSmd_DeviceClose();				// Release Sound Device
	i = m_pDev->Release();				// Release Device

	if(0 != i)
		printf(
				"The non-zero reference count means that\n"
				"there is something object not cleaned up.\n"
				);


	// Release Window
	LcSys_DestroyWindow(&m_hWnd);

	// Release GLC
	LcSys_Close();

	return LC_OK;
}


INT CAppGame::App_RscLoad()
{
	////////////////////////////////////////////////////////////////////////////
	// Create Texture from File: Mipmap Level = 0, Color Key = 0x0

	if(LC_FAILED(LcDev_CreateTextureFromFile(NULL, &m_pTex[0], (LC_HANDLE)"data/atx_background.bmp")))
		return LC_EFAIL;

	if(LC_FAILED(LcDev_CreateTextureFromFile(NULL, &m_pTex[1], (LC_HANDLE)"data/atx_ship.png")))
		return LC_EFAIL;

	if(LC_FAILED(LcDev_CreateTextureFromFile(NULL, &m_pTex[2], (LC_HANDLE)"data/atx_photon.png")))
		return LC_EFAIL;

	if(LC_FAILED(LcDev_CreateTextureFromFile(NULL, &m_pTex[3], (LC_HANDLE)"data/atx_villain.png")))
		return LC_EFAIL;

	if(LC_FAILED(LcDev_CreateTextureFromFile(NULL, &m_pTex[4], (LC_HANDLE)"data/atx_shield.png")))
		return LC_EFAIL;

	if(LC_FAILED(LcDev_CreateTextureFromFile(NULL, &m_pTex[5], (LC_HANDLE)"data/atx_bar.png")))
		return LC_EFAIL;


	////////////////////////////////////////////////////////////////////////////
	// Create Background, Effect Sound Instance.
	// (Default- 22050 Hz, 16 bit stereo)

	if(LC_FAILED(LcxSmd_CreateSoundFromFile(NULL, &m_pSnd[0], (LC_HANDLE)"data/asm_bgm.wav")))
		return LC_EFAIL;

	if(LC_FAILED(LcxSmd_CreateSoundFromFile(NULL, &m_pSnd[1], (LC_HANDLE)"data/asm_fire.wav")))
		return LC_EFAIL;

	if(LC_FAILED(LcxSmd_CreateSoundFromFile(NULL, &m_pSnd[2], (LC_HANDLE)"data/asm_explosion.wav")))
		return LC_EFAIL;


	////////////////////////////////////////////////////////////////////////////
	// Create Font Instance with Sprite

	TLC_FONT	tFont("data/aft_mwg_b.otf", 14);		// font name: 문화고딕, Height: 12
	if(LC_FAILED(LcDev_CreateFontFromFile(NULL, &m_pFontScore, m_pSpt, &tFont)))
		return LC_EFAIL;


	m_pSnd[0]->SetRepeat();
	m_pSnd[0]->Play();


	return LC_OK;
}



INT CAppGame::App_Run()
{
	// Update Frame
	FLOAT fFrame = GetAppFrame();

	// Update Input
	m_pInput->Update();

	SystemControl();

	if (PLAYER_STATUS_DEAD != m_PlayerStatus)
		UpdatePlayer();

	UpdatePlayerMissiles();
	UpdateEnemies();
	UpdateEnemyMissiles();

	CollisionPlayerMissilesAndEnemies();
	CollisionPlayerAndEnemies();
	CollisionEnemyMissilesAndPlayer();
	ScrollBackground();

	ShieldPulse();


	m_pDev->BeginScene();											// Prepare Rendering...
	m_pDev->Clear(LC_CLEAR_COLOR_BUFFER | LC_CLEAR_DEPTH_BUFFER);	// Clear Color and Depth of FrameBuffer

	m_pSpt->Begin();				// Sprite Begin


	// Draw Game Play

	DrawBackground();
	DrawMissile();
	DrawShield();
	DrawPlayer();
	DrawEnergyBar();
	DrawPlayerLife();

	m_pFontScore->DrawTxt();


	m_pSpt->End();

	m_pDev->EndScene();

	// Flipping
	m_pDev->Present(0,0,0,0);

	return m_bIsExit;
}



INT LC_CALLBACK CAppGame::App_Main(LC_HANDLE)
{
#if defined(_WIN32)
	LcSys_Sleep(10);
#endif

	if(g_pAppFrame)
		return g_pAppFrame->App_Run();

	return 0;
}



void CAppGame::ShieldPulse()
{
	m_ShieldTransparency = FLOAT(m_ShieldEnergy) / FLOAT(SHIELDENERGYmax);



	DOUBLE TimeCur = LcSys_GetTickCount();

	if(TimeCur < (m_ShieldTimeDelta+ m_ShieldTimeBgn))
		return;


	m_ShieldTimeBgn = TimeCur;


	m_ShieldRatio += m_ShieldExtend * 0.02F;

	if(m_ShieldRatio >= SHIELDRATIOmax)
	{
		m_ShieldExtend = SHIELDRATIOmax;
		m_ShieldExtend = -1.F;
	}

	if(m_ShieldRatio < SHIELDRATIOmin)
	{
		m_ShieldExtend = SHIELDRATIOmin;
		m_ShieldExtend = +1.F;
	}
}


void CAppGame::ShieldRecharge()
{
	m_ShieldEnergy = SHIELDENERGYmax;
}


INT CAppGame::ShieldProtect(INT damage)
{
	INT protect = LC_TRUE;

	m_ShieldEnergy -= damage;

	if(0 > m_ShieldEnergy)
	{
		protect = LC_FALSE;

		m_ShieldEnergy = 0;
	}

	return protect;
}



void CAppGame::DrawMissile()
{
	INT i=0;

	for(i=0; i != MISSILEMAX; ++i)
	{
		if (LC_TRUE != m_PlayerMissiles[i].isAlive)
			continue;

		m_pSpt->Draw(m_pTex[m_PlayerMissiles[i].texture], NULL, &m_PlayerMissiles[i].position);
	}

	for(i=0; i != MISSILEMAX; ++i)
	{
		if (LC_TRUE != m_EnemyMissiles[i].isAlive)
			continue;

		m_EnemyMissiles[i].Draw(m_pSpt, m_pTex);
	}

	for(i=0; i != ENEMIESMAX; ++i)
	{
		if (LC_TRUE != m_Enemies[i].isAlive)
			continue;

		m_Enemies[i].Draw(m_pSpt, m_pTex);
	}
}


void CAppGame::DrawPlayer()
{
	if (PLAYER_STATUS_ALIVE == m_PlayerStatus || PLAYER_STATUS_GHOST == m_PlayerStatus)
		m_pSpt->Draw(m_pTex[m_Player.texture], &m_Cells[m_CellSelect], &m_Player.position);
}


void CAppGame::DrawShield()
{
	LCXCOLOR	color(0,1, 1, m_ShieldTransparency);
	LCXVECTOR2	vcScl(1,1);
	LCXVECTOR2	vcTrn;

	vcScl *= (FLOAT)fabs(m_ShieldRatio);
	vcTrn = m_Player.position - vcScl * 32.f + LCXVECTOR2(PLAYER_WIDTH /2.F, PLAYER_HEIGHT /2.F);

	m_pSpt->DrawEx(m_pTex[4], NULL, &vcTrn, &vcScl, NULL, 0, &color);
}


void CAppGame::DrawEnergyBar()
{
	LC_RECTF	rc1(0,0,  (FLOAT)INT(1.5F * ENERGYBARmax), 16);
	LC_RECTF	rc2(0,0,  (FLOAT)INT(1.5F * ENERGYBARmax * m_ShieldEnergy / SHIELDENERGYmax), 16);
	LCXVECTOR2	vcTrn(0, LC_DISPLAY_HEIGHT - 20);
	LCXCOLOR	col2(1,0,0,0.7F);
	LCXCOLOR	col1(0,0,1,0.7F);

	m_pSpt->DrawEx(NULL, &rc1, &vcTrn, NULL, NULL, 0, &col1);
	m_pSpt->Draw(NULL, &rc2, &vcTrn, &col2);
}




void CAppGame::ObjectInitialize()
{
	INT i;
	INT	_x, _y, _w, _h;		// position x, y, width, height

	m_Player.texture = 1;

	m_Player.position.x =	(LC_DISPLAY_WIDTH - PLAYER_WIDTH) / 2;
	m_Player.position.y =	(LC_DISPLAY_HEIGHT - PLAYER_HEIGHT) / 2;

//	m_Player.position.x =	160;
//	m_Player.position.y =	120;


	m_Player.velocity.x = 0.0f;
	m_Player.velocity.y = 0.0f;

	m_PlayerStatus = PLAYER_STATUS_ALIVE;

	for(INT cellCount = 0; cellCount != CELLNUMBER; ++cellCount)
	{
		_x = (cellCount * PLAYER_WIDTH);
		_y = 0;
		_w = PLAYER_WIDTH;
		_h = PLAYER_HEIGHT;

		m_Cells[cellCount].x = FLOAT(_x);
		m_Cells[cellCount].y = FLOAT(_y);
		m_Cells[cellCount].w = FLOAT(_w);
		m_Cells[cellCount].h = FLOAT(_h);
	}

	for(i=0; i != MISSILEMAX; ++i)
	{
		m_PlayerMissiles[i].texture = 2;
		m_PlayerMissiles[i].isAlive = LC_FALSE;
		m_EnemyMissiles[i].texture = 2;
		m_EnemyMissiles[i].isAlive = LC_FALSE;
	}

	for(i=0; i != ENEMIESMAX; ++i)
	{
		m_Enemies[i].texture = 3;
		m_Enemies[i].isAlive = LC_FALSE;
	}

}




void CAppGame::PlayerRevive()
{
	if ((PLAYER_STATUS_DEAD == m_PlayerStatus) && (0 < m_PlayerLife))
	{
		--m_PlayerLife;

		m_PlayerStatus = PLAYER_STATUS_GHOST;

		m_Player.position.x = LC_DISPLAY_WIDTH / 2;
		m_Player.position.y = LC_DISPLAY_HEIGHT - 50;
	}
}

void CAppGame::PlayerDead()
{
	m_PlayerStatus = PLAYER_STATUS_DEAD;

	if(m_PlayerLife <= 0)
	{
		m_bIsExit = -1;
	}

	PlayerRevive();
}

void CAppGame::UpdatePlayerMissiles()
{
	INT i;

	for(i=0; i != MISSILEMAX; ++i)
	{
		if (LC_TRUE == m_PlayerMissiles[i].isAlive)
		{
			m_PlayerMissiles[i].position += m_PlayerMissiles[i].velocity;

			if (m_PlayerMissiles[i].position.y <= -MISSILE_HEIGHT)
			{
				m_PlayerMissiles[i].isAlive = LC_FALSE;
			}
		}
	}
}

void CAppGame::UpdateEnemyMissiles()
{
	INT i;

	for(i=0; i != MISSILEMAX; ++i)
	{
		if (LC_TRUE == m_EnemyMissiles[i].isAlive)
		{
			m_EnemyMissiles[i].position += m_EnemyMissiles[i].velocity;

			if (m_EnemyMissiles[i].position.y >= LC_DISPLAY_HEIGHT)
			{
				m_EnemyMissiles[i].isAlive = LC_FALSE;
			}
		}
	}
}

void CAppGame::FirePlayerMissile()
{
	INT i;

	for(i=0; i != MISSILEMAX; ++i)
	{
		if (LC_FALSE == m_PlayerMissiles[i].isAlive)
		{
			m_PlayerMissiles[i].isAlive = LC_TRUE;

			m_PlayerMissiles[i].position.x = m_Player.position.x +  (PLAYER_WIDTH - MISSILE_WIDTH) / 2;
			m_PlayerMissiles[i].position.y = m_Player.position.y +  (PLAYER_HEIGHT) / 4;

			m_PlayerMissiles[i].velocity.y = UPDIRECTION * MISSILESPEED;

			m_pSnd[1]->Play();

			return;
		}
	}
}

void CAppGame::FireEnemyMissile(LCXVECTOR2 position, LCXVECTOR2 direction, FLOAT rotation)
{
	INT i;

	for(i=0; i != MISSILEMAX; ++i)
	{
		if (LC_FALSE == m_EnemyMissiles[i].isAlive)
		{
			m_EnemyMissiles[i].isAlive = LC_TRUE;

			m_EnemyMissiles[i].position.x = position.x + (ENEMY_WIDTH + MISSILE_WIDTH) / 2;
			m_EnemyMissiles[i].position.y = position.y + (ENEMY_HEIGHT+ MISSILE_HEIGHT) / 2;

			m_EnemyMissiles[i].velocity = direction * MISSILESPEED;
			m_EnemyMissiles[i].rotation = rotation;

			m_pSnd[1]->Play();

			return;
		}
	}
}

void CAppGame::UpdatePlayer()
{
	LCXVECTOR2	playerMoveDirection(0,0);


	if (m_pInput->KeyState(LCIK_W) || m_pInput->KeyState(LCIK_Q) || m_pInput->KeyState(LCIK_E))
	{
		playerMoveDirection.y += UPDIRECTION;
	}

	if (m_pInput->KeyState(LCIK_S) || m_pInput->KeyState(LCIK_Z) || m_pInput->KeyState(LCIK_X))
	{
		playerMoveDirection.y += DOWNDIRECTION;
	}

	if (m_pInput->KeyState(LCIK_A) || m_pInput->KeyState(LCIK_Q) || m_pInput->KeyState(LCIK_Z))
	{
		playerMoveDirection.x += LEFTDIRECTION;
	}

	if (m_pInput->KeyState(LCIK_D) || m_pInput->KeyState(LCIK_E) || m_pInput->KeyState(LCIK_X))
	{
		playerMoveDirection.x += RIGHTDIRECTION;
	}

	if (m_pInput->KeyState(LCIK_UP) && m_bIsFilre == LC_FALSE)
	{
		FirePlayerMissile();
		m_bIsFilre = LC_TRUE;
	}


	// 터치 입력 확인
	LCXVECTOR3	vcTsbPos = m_pInput->GetMtsPos();
	LC_STATE	nTsbState= m_pInput->MtsState();


	if (nTsbState)
	{
		if(m_pInput->IsInRect(
			(INT)m_Player.position.x,
			(INT)m_Player.position.y,
			(INT)(m_Player.position.x + PLAYER_WIDTH),
			(INT)(m_Player.position.y + PLAYER_HEIGHT)) == 1)
		{
			if(m_bIsFilre == LC_FALSE)
			{
				FirePlayerMissile();

				m_bIsFilre = LC_TRUE;
			}
		}
		else
		{
			playerMoveDirection.x = vcTsbPos.x - (m_Player.position.x + PLAYER_WIDTH * 0.5f);
			playerMoveDirection.y = vcTsbPos.y - (m_Player.position.y + PLAYER_HEIGHT * 0.5f);
		}
	}
	else if(m_pInput->KeyState(LCIK_UP) == 0)
	{
		m_bIsFilre = LC_FALSE;
	}

	playerMoveDirection.Normalize();

	m_Player.velocity = playerMoveDirection * PLAYERSPEED;
	m_Player.position+= m_Player.velocity;

	m_Player.position.x = LCXClamp(m_Player.position.x, 0.F,	(FLOAT)(LC_DISPLAY_WIDTH - PLAYER_WIDTH  ) );
	m_Player.position.y = LCXClamp(m_Player.position.y, 0.F,	(FLOAT)(LC_DISPLAY_HEIGHT - PLAYER_HEIGHT) );

	++m_CellDisplayTime;

	if(CELLCHANGETIME <= m_CellDisplayTime)
	{
		++m_CellSelect;
		m_CellSelect = m_CellSelect % CELLNUMBER;
		m_CellDisplayTime = 0;
	}

	if(PLAYER_STATUS_GHOST == m_PlayerStatus)
	{
		++m_GhostDuration;

		if(GHOSTDURATIONmax <= m_GhostDuration)
		{
			m_PlayerStatus = PLAYER_STATUS_ALIVE;

			m_GhostDuration = 0;

			ShieldRecharge();
		}
	}
}

void CAppGame::UpdateEnemies()
{
	INT i;

	for(i=0; i != ENEMIESMAX; ++i)
	{
		if (LC_TRUE == m_Enemies[i].isAlive)
		{
			m_Enemies[i].position += m_Enemies[i].velocity;

			if (m_Enemies[i].position.y >= LC_DISPLAY_HEIGHT)
				m_Enemies[i].isAlive = LC_FALSE;


			if(1 > m_Random.Next(0, 100))
				FireEnemyMissile(m_Enemies[i].position, m_Enemies[i].direction, m_Enemies[i].rotation);
		}
		else
		{
			m_Enemies[i].isAlive = LC_TRUE;

			m_Enemies[i].position.x = (FLOAT)(m_Random.Next(ENEMY_WIDTH, LC_DISPLAY_WIDTH - ENEMY_WIDTH));
			m_Enemies[i].position.y = -ENEMY_HEIGHT;

			LCXVECTOR2 direction = m_Player.position - m_Enemies[i].position;
			direction.Normalize();

			FLOAT speed = m_Random.Next(ENEMYSPEEDmin, ENEMYSPEEDmax);

			m_Enemies[i].direction = direction;
			m_Enemies[i].velocity = direction * speed;

			m_Enemies[i].rotation = (FLOAT)atan2(direction.y, direction.x) + FLOAT(LCX_PI/2.F);
		}
	}
}

void CAppGame::SaveHighScore()
{
	FILE* fp = fopen("data/adt_gamesave.dat", "wb");
	if(NULL == fp)
		return;


	unsigned char data[20];
	memset(data, 0, sizeof(unsigned char) * 20);

	sprintf((char*)data, "%d", m_ScoreH);

	fwrite(data, 1, 20, fp);
	fclose(fp);
}


void CAppGame::LoadHighScore()
{
	FILE* fp = fopen("data/adt_gamesave.dat", "rb");
	if(NULL == fp)
		return;


	unsigned char data[20];
	memset(data, 0, sizeof(unsigned char) * 20);
	fread(data, 1, 20, fp);
	fclose(fp);

	m_ScoreH = atoi((char*)data);
}


void CAppGame::UpdateHighScore()
{
	if (m_ScoreH < m_Score)
	{
		m_ScoreH = m_Score;
	}
}



void CAppGame::DrawPlayerLife()
{
	if (0 < m_PlayerLife)
	{
		FLOAT scale = 0.5f;

		LCXVECTOR2 scl(scale, scale);

		INT marginWidth = (INT)(PLAYER_WIDTH * (1.0f - scale) * 0.5f);
		INT marginHeight = (INT)(PLAYER_HEIGHT * (1.0f - scale) * 0.5f);

		LCXVECTOR2 position;
		position.x = (FLOAT)(-marginWidth);
		position.y = (FLOAT)(LC_DISPLAY_HEIGHT - (PLAYER_HEIGHT - marginHeight));

		for (INT count = 0; count != m_PlayerLife; ++count)
		{
			m_pSpt->DrawEx(m_pTex[m_Player.texture], &m_Cells[3], &position, &scl, NULL, 0, NULL);
			position.x = position.x + PLAYER_WIDTH - (marginWidth * 2);
		}
	}
}

void CAppGame::ScrollBackground()
{
	switch (m_ScrollDirection)
	{
	case SCROLL_DIRECTION_LEFT:
		{
			m_BgPosition.x -= SCROLLSPEED;

			break;
		}
	case SCROLL_DIRECTION_RIGHT:
		{
			m_BgPosition.x += SCROLLSPEED;

			break;
		}
	case SCROLL_DIRECTION_UP:
		{
			m_BgPosition.y -= SCROLLSPEED;

			break;
		}
	case SCROLL_DIRECTION_DOWN:
		{
			m_BgPosition.y += SCROLLSPEED;

			break;
		}
	}

	while (-LC_DISPLAY_HEIGHT >= m_BgPosition.y)
	{
		m_BgPosition.y += LC_DISPLAY_HEIGHT;
	}
	while (0 < m_BgPosition.y)
	{
		m_BgPosition.y -= LC_DISPLAY_HEIGHT;
	}
	while (LC_DISPLAY_WIDTH >= m_BgPosition.x)
	{
		m_BgPosition.x += LC_DISPLAY_WIDTH;
	}
	while (0 < m_BgPosition.x)
	{
		m_BgPosition.x -= LC_DISPLAY_WIDTH;
	}
}

void CAppGame::DrawBackground()
{
	LCXVECTOR2 repeatPosition;
	repeatPosition.x = m_BgPosition.x;
	repeatPosition.y = m_BgPosition.y;

	while (LC_DISPLAY_HEIGHT > repeatPosition.y)
	{
		while(LC_DISPLAY_WIDTH > repeatPosition.x)
		{
			m_pSpt->Draw(m_pTex[0], NULL, &repeatPosition);

			repeatPosition.x += LC_DISPLAY_WIDTH;
		}

		repeatPosition.x = m_BgPosition.x;
		repeatPosition.y += LC_DISPLAY_HEIGHT;
	}
}

void CAppGame::CollisionPlayerMissilesAndEnemies()
{
	for (INT sourceIndex = 0; sourceIndex != MISSILEMAX; ++sourceIndex)
	{
		if (LC_TRUE == m_PlayerMissiles[sourceIndex].isAlive)
		{
			for (INT targetIndex = 0; targetIndex != ENEMIESMAX; ++targetIndex)
			{
				if (LC_TRUE == m_Enemies[targetIndex].isAlive)
				{
					if (LC_OK <= LCXCollisionRect2RectLTWH(
						m_PlayerMissiles[sourceIndex].position.x, m_PlayerMissiles[sourceIndex].position.y,
						(FLOAT)MISSILE_WIDTH, (FLOAT)MISSILE_HEIGHT,
						m_Enemies[targetIndex].position.x, m_Enemies[targetIndex].position.y,
						(FLOAT)ENEMY_WIDTH, (FLOAT)ENEMY_HEIGHT))
					{
						m_PlayerMissiles[sourceIndex].isAlive = LC_FALSE;
						m_Enemies[targetIndex].isAlive = LC_FALSE;

						m_Score += 100;

						UpdateHighScore();

						DrawScore();

						m_pSnd[2]->Play();

						break;
					}
				}
			}
		}
	}
}

void CAppGame::CollisionPlayerAndEnemies()
{
	INT i;

	if (PLAYER_STATUS_ALIVE == m_PlayerStatus)
	{
		for(i=0; i != ENEMIESMAX; ++i)
		{
			if (LC_TRUE == m_Enemies[i].isAlive)
			{
				if (LC_OK <= LCXCollisionRect2RectLTWH(
					m_Enemies[i].position.x, m_Enemies[i].position.y,
					(FLOAT)ENEMY_WIDTH, (FLOAT)ENEMY_HEIGHT,
					m_Player.position.x, m_Player.position.y,
					(FLOAT)PLAYER_WIDTH,(FLOAT)PLAYER_HEIGHT))
				{
					m_Enemies[i].isAlive = LC_FALSE;

					if(LC_FALSE == ShieldProtect(30))
					{
						PlayerDead();
					}

					m_pSnd[2]->Play();

					break;
				}
			}
		}
	}
}

void CAppGame::CollisionEnemyMissilesAndPlayer()
{
	INT i;

	if (PLAYER_STATUS_ALIVE != m_PlayerStatus)
		return;


	for(i=0; i != MISSILEMAX; ++i)
	{

		if (LC_TRUE == m_EnemyMissiles[i].isAlive)
		{
			if (LC_OK <= LCXCollisionRect2RectLTWH(
				m_EnemyMissiles[i].position.x, m_EnemyMissiles[i].position.y,
				(FLOAT)MISSILE_WIDTH, (FLOAT)MISSILE_HEIGHT,
				m_Player.position.x, m_Player.position.y,
				(FLOAT)PLAYER_WIDTH, (FLOAT)PLAYER_HEIGHT))
			{
				m_EnemyMissiles[i].isAlive = LC_FALSE;

				if(LC_FALSE == ShieldProtect(10))
				{
					PlayerDead();
				}

				m_pSnd[2]->Play();

				break;
			}
		}
	}
}



void CAppGame::SetVolume()
{
	if(m_pInput->KeyDown(LCIK_F8))
	{
		m_Volume +=0.1f;
		m_Volume = LCXClamp(m_Volume, 0.F, 2.F);

		m_pSnd[0]->SetVolume(m_Volume);
		m_pSnd[1]->SetVolume(m_Volume);
		m_pSnd[2]->SetVolume(m_Volume);
	}
	else if (m_pInput->KeyPress(LCIK_F7))
	{
		m_Volume -=0.1f;
		m_Volume = LCXClamp(m_Volume, 0.F, 2.F);

		m_pSnd[0]->SetVolume(m_Volume);
		m_pSnd[1]->SetVolume(m_Volume);
		m_pSnd[2]->SetVolume(m_Volume);
	}
}


void CAppGame::SystemControl()
{
	if(m_pInput->KeyDown(LCIK_ESCAPE))
	{
		m_bIsExit = -1;
	}

	SetVolume();
}



void CAppGame::DrawScore()
{
	memset(m_sScore, 0, 256);
	sprintf(m_sScore, "Score : %d / HighScore : %d", m_Score, m_ScoreH);

	LCXCOLOR	color(1,0,0,1);
	LCXVECTOR2	vcPos(5,5);

	m_pFontScore->SetColor(&color);
	m_pFontScore->SetPosition(&vcPos);
	m_pFontScore->SetString(m_sScore);
}



FLOAT CAppGame::GetAppFrame()
{
#if defined(_WIN32)
	static INT		MaxCount = 10;
#else
	static INT		MaxCount = 10;
#endif

	static DOUBLE  fFps = 0.;
	static INT     iCnt = 0;
	static DOUBLE   dBgn = ::LcSys_GetTickCount();
	DOUBLE   dCur = ::LcSys_GetTickCount();  // GetTickCount()

	++iCnt;

	if(iCnt>=MaxCount)
	{
		fFps = DOUBLE(dCur-dBgn);
		fFps = iCnt*1000./fFps;
		iCnt = 0;
		dBgn = dCur;
	}

	return (FLOAT)fFps;
}


