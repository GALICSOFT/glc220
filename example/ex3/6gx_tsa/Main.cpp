// Implementation of the CMain class.
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


#define MEDIA_DIR	"../../media/model2dspt/"


CMain::CMain()
{
	m_pInput	= NULL;
	m_pCam3D	= NULL;
	m_pXYZ		= NULL;
	m_pGrid		= NULL;

	m_pCam2D	= NULL;

	m_pTsa0		= NULL;
	m_pTsa1		= NULL;
	m_pTsa2		= NULL;
}


INT	CMain::Init()
{
	INT	hr = LC_OK;


	// Input 持失
	if(LC_FAILED(LcxEnt_CreateInput(NULL, &m_pInput, m_hWnd)))
		return LC_EFAIL;

	// 2D Create Camera
	hr = LcxEnt_CreateCamera(LCXCMD_CAMERA_2D, &m_pCam2D);

	// 3DCamera 持失
	if(LC_FAILED(LcxEnt_CreateCamera(LCXCMD_CAMERA_3D, &m_pCam3D)))
		return LC_EFAIL;


	LCXVECTOR3	vcEye ( 0, -10, 0);
	LCXVECTOR3	vcLook( 0,   0, 0);
	LCXVECTOR3	vcUp  ( 0,   0, 1);
	m_pCam3D->SetupParamView(&vcEye, &vcLook, &vcUp);

	FLOAT	fFov = LCXToRadian(45.0F);
	FLOAT	fScnW= FLOAT(m_nScnW);
	FLOAT	fScnH= FLOAT(m_nScnH);
	FLOAT	fNear= 1.0F;
	FLOAT	fFar = 5000.0F;

	m_pCam3D->SetupParamProj(fFov, fScnW, fScnH, fNear, fFar);




	if(LC_FAILED(LcxEnt_CreateEntity(LCXCMD_ENTITY_AXIS, &m_pXYZ)))
		return LC_EFAIL;

	if(LC_FAILED(LcxEnt_CreateEntity(LCXCMD_ENTITY_GRID, &m_pGrid)))
		return LC_EFAIL;


	FILE* fp = fopen((char*)MEDIA_DIR"tsa_unit01.gpsa", "rb");
	char* buf = NULL;

	int len = 0;
	
	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	buf = (char*)malloc(len);
	fread(buf, 1, len, fp);

	//hr = LcxEnt_CreateTsa(NULL, &m_pTsa0, (char*)MEDIA_DIR"unit01.gpsa");
	hr = LcxEnt_CreateTsaFromMemory(NULL, &m_pTsa0, (char*)MEDIA_DIR"unit01.gpsa", NULL, buf, &len);
	if(LC_FAILED(hr))
		return LC_EFAIL;

	free(buf);
	m_pTsa0->SetAttrib(LCXM_EXEC_TEXTURE_LOAD, NULL);


	hr = LcxEnt_CreateTsa(NULL, &m_pTsa1, NULL, m_pTsa0);
	if(LC_FAILED(hr))
		return LC_EFAIL;


	hr = LcxEnt_CreateTsa(NULL, &m_pTsa2, NULL, m_pTsa0);
	if(LC_FAILED(hr))
		return LC_EFAIL;


	LCXVECTOR2	vcPos(0.0F, 0.F);
	LCXVECTOR2	vcScl(1.5F, 1.5F);
	hr = m_pTsa1->SetAnimation("unit01_02_move");
	hr = m_pTsa1->SetPosition(&vcPos);
	hr = m_pTsa1->SetScaling(&vcScl);

	FLOAT	framespeed =  0.5F;
	hr = m_pTsa1->SetAttrib(LCXM_FRAME_SPEED, &framespeed);

	vcPos = LCXVECTOR2(-100.0F, 0.F);
	hr = m_pTsa2->SetAnimation("unit02_03_dead");
	hr = m_pTsa2->SetPosition(&vcPos);

	LCXMATRIX3X3*	pmtWld =  NULL;

	hr = m_pTsa1->GetAttrib(LCXM_WORLD_MATRIX_PT, &pmtWld);
	hr = m_pTsa2->SetAttrib(LCXM_PARENT_MATRIX, pmtWld);


	int rpt = 3;
	m_pTsa2->SetAttrib(LCXM_REPEATE_COUNT, &rpt);

	int efr = 10;
	m_pTsa2->SetAttrib(LCXM_END_FREEZE_FRAME, &efr);



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


	SAFE_RELEASE(	m_pTsa0		);
	SAFE_RELEASE(	m_pTsa1		);
	SAFE_RELEASE(	m_pTsa2		);

	SAFE_RELEASE(	m_pCam2D		);

	SAFE_RELEASE(	m_pXYZ		);
	SAFE_RELEASE(	m_pGrid		);
	SAFE_RELEASE(	m_pCam3D		);
	SAFE_RELEASE(	m_pInput	);

	return LC_OK;
}


INT	CMain::FrameMove()
{
	INT hr = 0;


	
#if defined(_WIN32)
	LcSys_Sleep(10);
#endif


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
	if(pKey[LCIK_DOWN])
		m_pCam2D->MoveForward(+speed);

	if(pKey[LCIK_UP])
		m_pCam2D->MoveForward(-speed);

	if(pKey[LCIK_W])
		m_pCam2D->MoveUpward(speed);

	if(pKey[LCIK_S])
		m_pCam2D->MoveUpward(-speed);

	if(pKey[LCIK_A])
		m_pCam2D->MoveSideward(-speed);

	if(pKey[LCIK_D])
		m_pCam2D->MoveSideward(+speed);


	// Rotation
	float fAngle_degree = .5f;

	if(pKey[LCIK_LEFT])
		m_pCam2D->RotateYaw(fAngle_degree, speed);

	if(pKey[LCIK_RIGHT])
		m_pCam2D->RotateYaw(fAngle_degree, -speed);


	m_pCam3D->Update();
	m_pCam2D->Update();




	DOUBLE fTime = 0;
	LcSys_ElapseTime(&fTime);




	m_pTsa1->SetAttrib(LCXM_ADVANCE_TIME, &fTime);
	m_pTsa2->SetAttrib(LCXM_ADVANCE_TIME, &fTime);



	static float	t=0;
	t += .01f;

	LCXVECTOR3	vcRot(60,60, LCXToRadian(t) );
	//m_pTsa1->SetRotation(&vcRot);



	hr = m_pTsa2->FrameMove();


	hr = m_pTsa1->FrameMove();

	// remain rpeate count is 0
	if(0==  hr)
	{
		int rpt = 3;
		m_pTsa1->SetAttrib(LCXM_REPEATE_COUNT, &rpt);
	}



	return LC_OK;
}


INT	CMain::Render()
{
	m_pDev->BeginScene();

	m_pDev->Clear(LC_CLEAR_COLOR_BUFFER | LC_CLEAR_DEPTH_BUFFER);

	m_pCam3D->SetTransformPrj();
	m_pCam3D->SetTransformViw();


	m_pDev->SetRenderState(LC_RS_CULL_FACE, LC_ST_DISABLE);
	m_pDev->SetRenderState(LC_RS_LIGHTING, LC_ST_DISABLE);


	m_pXYZ->Render();
	m_pGrid->Render();


	LCXVECTOR2	vcPos(0, -100);

	// 2D Model Rendering
	{

		m_pSpt->Begin();

		LC_MATRIX* pmtViw = (LC_MATRIX*)m_pCam2D->GetMatrixViw();

		m_pSpt->SetTransform(LC_TS_VIEW_EXT, pmtViw);

		SAFE_RENDER(	m_pTsa1	);
		SAFE_RENDER(	m_pTsa2	);
		m_pTsa2->DrawOneFrame(m_pSpt, "unit02_01_weapon_missile", &vcPos);

		m_pSpt->End();
		
	}


	m_pDev->EndScene();

	return LC_OK;
}



PLC_TEXTURE CMain::FindTex(char* sName)
{
	PLC_TEXTURE	pTex = NULL;

	ItTex it = m_vTex.find( sName );

	if( it == m_vTex.end() )
	{
		char sFile	[LC_MAX_PATH ]={0};
		char drive	[LC_MAX_DRIVE]={0};
		char dir	[LC_MAX_DIR  ]={0};
		char fname	[LC_MAX_FNAME]={0};
		char ext	[LC_MAX_EXT  ]={0};

		LcStr_SplitPath( sName, drive, dir, fname, ext );

		sprintf(sFile, MEDIA_DIR"%s%s", fname, ext);

		if(LC_FAILED(LcDev_CreateTextureFromFile(NULL, &pTex, (LC_HANDLE)sFile)))
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




