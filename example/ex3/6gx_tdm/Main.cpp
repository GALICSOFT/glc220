// Implementation of the CMain class.
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


#define MEDIA_DIR	"../../media/model2d/"


CMain::CMain()
{
	m_pInput	= NULL;
	m_pCam		= NULL;
	m_pXYZ		= NULL;
	m_pGrid		= NULL;


	memset(m_pTdm, 0, sizeof(m_pTdm) );
}


INT	CMain::Init()
{
	INT	hr = LC_OK;


	// Input ����
	if(LC_FAILED(LcxEnt_CreateInput(NULL, &m_pInput, m_hWnd)))
		return LC_EFAIL;


	// Camera ����
	if(LC_FAILED(LcxEnt_CreateCamera(LCXCMD_CAMERA_3D, &m_pCam)))
		return LC_EFAIL;


	LCXVECTOR3	vcEye ( 5, -100, 0);
	LCXVECTOR3	vcLook( 5,   0,  0);
	LCXVECTOR3	vcUp  ( 0,   0,  1);
	m_pCam->SetupParamView(&vcEye, &vcLook, &vcUp);

	FLOAT	fFov = LCXToRadian(45.0F);
	FLOAT	fScnW= FLOAT(m_nScnW);
	FLOAT	fScnH= FLOAT(m_nScnH);
	FLOAT	fNear= 1.0F;
	FLOAT	fFar = 5000.0F;

	m_pCam->SetupParamProj(fFov, fScnW, fScnH, fNear, fFar);




	if(LC_FAILED(LcxEnt_CreateEntity(LCXCMD_ENTITY_AXIS, &m_pXYZ)))
		return LC_EFAIL;

	if(LC_FAILED(LcxEnt_CreateEntity(LCXCMD_ENTITY_GRID, &m_pGrid)))
		return LC_EFAIL;



	char*	sTdt[] =
	{
		(char*)MEDIA_DIR"boss01.tdt",
		(char*)MEDIA_DIR"login.tdt",
	};

	for(int i=0; i<2; ++i)
	{
		m_pTdm[i] = Setup2dModel(sTdt[i]);
		if(NULL == m_pTdm[i])
			return LC_EFAIL;
	}


	LCXVECTOR2	vcPos0(-50.0F, -50.F);
	hr = m_pTdm[0]->SetAnimation(0,"attack");
	hr = m_pTdm[0]->SetPosition(&vcPos0);


	return LC_OK;
}



INT	CMain::Destroy()
{
	ItTex _F = m_vTex.begin();
	ItTex _L = m_vTex.end();

	for(; _F != _L; ++_F )
	{
		PLC_TEXTURE pTex = (*_F).second;
		if(pTex)
			pTex->Release();
	}

	m_vTex.clear();


	for(int i=0 ;i<16; ++i)
	{
		if(m_pTdm[i])
			m_pTdm[i]->Release();
	}

	SAFE_RELEASE(	m_pXYZ		);
	SAFE_RELEASE(	m_pGrid		);
	SAFE_RELEASE(	m_pCam		);
	SAFE_RELEASE(	m_pInput	);

	return LC_OK;
}


INT	CMain::FrameMove()
{
#if defined(_WIN32)
	LcSys_Sleep(10);
#endif

	int		i;


	static INT c=0;
	++c;

	if(c>20*60*120)
	//if(c>8*60*2)
	{
		printf("Game Process Terminated\n");
		return LC_EFAIL;
	}


	static int n = 0;
	++n;

	if(n>5)
	{
		printf("FPS: %3.f\n", m_fFPS);
		n = 0;
	}

	// Input Update
	m_pInput->Update();

	const BYTE*	pKey = m_pInput->GetKeyMap();

	if(pKey[LCIK_ESCAPE])
		return LC_EFAIL;



	// Move, Rotation Speed
	FLOAT speed = (FLOAT)this->GetElapsedTime();
	speed *=0.1f;


	// Translation
	if(pKey[LCIK_W])
		m_pCam->MoveForward(+speed);

	if(pKey[LCIK_S])
		m_pCam->MoveForward(-speed);

	if(pKey[LCIK_A])
		m_pCam->MoveSideward(-speed);

	if(pKey[LCIK_D])
		m_pCam->MoveSideward(+speed);


	// Rotation
	float fAngle_degree = 2.0f;

	if(pKey[LCIK_LEFT])
		m_pCam->RotateYaw(fAngle_degree, speed);

	if(pKey[LCIK_RIGHT])
		m_pCam->RotateYaw(fAngle_degree, -speed);

	if(pKey[LCIK_UP])
		m_pCam->RotatePitch(fAngle_degree, speed);

	if(pKey[LCIK_DOWN])
		m_pCam->RotatePitch(fAngle_degree, -speed);


	m_pCam->Update();


	DOUBLE fElapsedTime = this->GetElapsedTime();
	DOUBLE fTime = fElapsedTime;






	for(i=0; i<2; ++i)
	{
		if(m_pTdm[i])
		{
			m_pTdm[i]->SetAttrib(LCXM_ADVANCE_TIME, &fTime);
		}
	}



	static float	t=0;
	t += 1.f;

	LCXVECTOR3	vcRot(40,40, LCXToRadian(t) );

	//m_pTdm[0]->SetRotation(&vcRot);


	
	LCXVECTOR2	vcPos1(40.0F, 40.F);
	LCXVECTOR2	vcScl1(-1.0F, 1.0F);
	
	m_pTdm[1]->SetPosition(&vcPos1);
	m_pTdm[1]->SetRotation(&vcRot);
	m_pTdm[1]->SetScaling(&vcScl1);


	for(i=0; i<2; ++i)
	{
		if(m_pTdm[i])
		{
			m_pTdm[i]->FrameMove();
		}
	}


	return LC_OK;
}


INT	CMain::Render()
{
	m_pDev->BeginScene();

	m_pDev->Clear(LC_CLEAR_COLOR_BUFFER | LC_CLEAR_DEPTH_BUFFER);

	m_pCam->SetTransformPrj();
	m_pCam->SetTransformViw();


	m_pDev->SetRenderState(LC_RS_CULL_FACE, LC_ST_DISABLE);
	m_pDev->SetRenderState(LC_RS_LIGHTING, LC_ST_DISABLE);


	m_pXYZ->Render();
	m_pGrid->Render();



	// 2D Model Rendering
	{
		m_pSpt->Begin();

		for(int i=0; i<2; ++i)
		{
			SAFE_RENDER(	m_pTdm[i]	);
		}

		m_pSpt->End();
	}


	m_pDev->EndScene();

	return LC_OK;
}



PLCX_TDM CMain::Setup2dModel(char* sModel)
{
	INT				hr;
	INT				nTexList = 0;
	INT				i = 0;

	char**			s = NULL;
	PLC_TEXTURE*	t = NULL;
	PLCX_TDM		pTdm = NULL;

	hr = LcxEnt_CreateTdm(NULL, &pTdm, (void*)sModel);

	if(LC_FAILED(hr))
		return NULL;


	nTexList = 0;
	pTdm->GetAttrib(LCXM_TEXTURE_LIST_NUMBER, &nTexList);


	s = (char**      )malloc( (nTexList+1) * sizeof(char*       ) );
	t = (PLC_TEXTURE*)malloc( (nTexList+1) * sizeof(PLC_TEXTURE*) );

	memset(s, 0, (nTexList+1) * sizeof(char*       ));
	memset(t, 0, (nTexList+1) * sizeof(PLC_TEXTURE*));


	pTdm->GetAttrib(LCXM_TEXTURE_LIST_NAME, &s);
	for(i=0; i<nTexList; ++i)
	{
		//Find Texture
		char*       sName = s[i];
		PLC_TEXTURE pTex = this->FindTex(sName);

		if(NULL == pTex)
			return NULL;

		//
		t[i] = pTex;
	}

	// set textures
	pTdm->SetAttrib(LCXM_TEXTURE_LIST_POINTER, t);


	free(s);
	free(t);

	return pTdm;
}




PLC_TEXTURE CMain::FindTex(char* sName)
{
	PLC_TEXTURE	pTex = NULL;

	ItTex it = m_vTex.find( sName );

	if( it == m_vTex.end() )
	{
		UINT colorkey = 0x0;			// 4444
		UINT b16 = LC_FMT_INDEX16;

		char sFile	[LC_MAX_PATH ]={0};
		char drive	[LC_MAX_DRIVE]={0};
		char dir	[LC_MAX_DIR  ]={0};
		char fname	[LC_MAX_FNAME]={0};
		char ext	[LC_MAX_EXT  ]={0};


		LcStr_SplitPath( sName, drive, dir, fname, ext );
		sprintf(sFile, MEDIA_DIR"%s%s", fname, ext);

		if(LC_FAILED(LcDev_CreateTextureFromFile(NULL, &pTex, sFile, &colorkey, NULL, &b16)))
		{
			printf("Cannot Read Model Texture: %s\n", sFile);
			return NULL;
		}

		if(pTex)
			m_vTex.insert(ItTex::value_type(sName, pTex) );
	}

	else
	{
		pTex = (*it).second;
	}


	return pTex;
}

