

#if defined(_MSC_VER) && _MSC_VER > 1000
  #pragma warning( disable : 4786)
  #pragma warning( disable : 4996)
#endif


#if defined(_MSC_VER)
  #pragma comment(lib, "lgpwn_b22_.lib")
  #pragma comment(lib, "lgpwn_c22_.lib")
  #pragma comment(lib, "lgpwn_x22_.lib")

  #pragma comment(lib, "png144_.lib")
  #pragma comment(lib, "ImgJpg_.lib")
  #pragma comment(lib, "zlib125_.lib")

  #pragma comment(lib, "OpenGL32.lib")
#endif


#include <vector>

using namespace std;


#if defined(_WIN32)
	#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <LcType.h>
#include <LcBase.h>
#include <LcMath.h>
#include <LcAux.h>
#include <LcxEntity.h>

#define MEDIA_DIR	"../../media/"


LC_HWND			m_hWnd;				// Window Handle
PLC_DEVICE		m_pDev	= NULL;		// Device
PLC_SPRITEMX	m_pSpMx	= NULL;		// 2D Sprite

vector<PLC_TEXTURE >	m_vTex;		// Texture List

FLOAT GetAppFrame();
LRESULT LC_WINAPI WndProc(LC_HWND, UINT, WPARAM, LPARAM);


INT LC_CALLBACK Run(LC_HANDLE)
{
#if defined(_WIN32)
	LcSys_Sleep(60);
#endif

	static INT c=0;
	++c;

//	if(c>20*60*20)
	if(c>8*60*2)
	{
		printf("Game Process Terminated\n");
		return LC_EFAIL;
	}


	FLOAT fFrame = GetAppFrame();


	static INT n=0;
	++n;

	if(n>5)
	{
		printf("%f\n", fFrame);
		n = 0;
	}



	m_pDev->BeginScene();

	// Clear Backbuffer
	LCXCOLOR clearColor(0,0,0,1);
	m_pDev->SetRenderState(LC_RS_CLEAR_COLOR, &clearColor);
	m_pDev->Clear(LC_CLEAR_COLOR_BUFFER | LC_CLEAR_DEPTH_BUFFER);


	// Render Layer
	m_pSpMx->Begin();	// It's same to ILC_Sprite::Begin(DGESPRITE_USE_DIFFUSE)

	LC_RECT	rc;
	LCXVECTOR2	s;
	LCXVECTOR2	r;
	LCXVECTOR2	t;
	LCXCOLOR	d;
	FLOAT		X=100;
	FLOAT		Y=100;

	PLC_TEXTURE pTex;


	int nTex = (int)m_vTex.size();

	for(INT i=0; i<4; ++i)
	{
		pTex = m_vTex[ rand()%nTex];

		for(INT j=0; j<4; ++j)
		{
			X = FLOAT(rand()%320)-80;
			Y = FLOAT(rand()%240)-80;
			t = LCXVECTOR2(X, Y);
			m_pSpMx->Draw(pTex, NULL, &t, NULL);
		}
	}

	m_pSpMx->DrawAndFlush();
	m_pSpMx->End();

	m_pDev->EndScene();

	return LC_OK;
}




// Main Program
int main(int argc, char* argv[])
{
	INT	hr = 0;


	char* sFile[]=
	{
		(char*)MEDIA_DIR"tex/bg256x256.bmp",
		(char*)MEDIA_DIR"tex/t01.tga",
		(char*)MEDIA_DIR"tex/t02.tga",
		(char*)MEDIA_DIR"tex/t03.tga",
		(char*)MEDIA_DIR"tex/t04.tga",
	};

	int i;

	PLC_TEXTURE	pTex;


	// Init GLC Library
	if(LC_FAILED(LcSys_Init()))
		return LC_OK;

	// Set Main procedure Function Pointer
	if(LC_FAILED(LcSys_SetMainRunFunc(Run)))
		goto End;


	// Create Window
	m_hWnd = LcSys_CreateWindow(WndProc, (char*)"LC_Tutorial", 480, 320);
	if(NULL == m_hWnd)
		goto End;

	// Create GLC Rendering Device
	if(LC_FAILED(LcDev_CreateDevice(NULL, &m_pDev, m_hWnd, NULL)))
		goto End;


	printf("LcDev_CreateDevice Success\n");


	// Create 2D Rendering Sprite
	if(LC_FAILED(LcDev_CreateSpriteMx(NULL, &m_pSpMx)))
		goto End;

	printf("LcDev_CreateSprite Success\n");


	for(i=0; i<5; ++i)
	{
		if(LC_FAILED(LcDev_CreateTextureFromFile(NULL, &pTex, sFile[i])))
			goto End;

		m_vTex.push_back(pTex);
	}

	printf("LcDev_CreateTextureFromFile Success\n");

	// Run Program
	while(1)
	{
		if(LC_FAILED(LcSys_Run()))
			break;

		// Flipping
		m_pDev->Present(0,0,0,0);
	}


End:
	// Release Texture

	for(i=0; i<(int)m_vTex.size(); ++i)
	{
		pTex = m_vTex[i];
		if(pTex)
			pTex->Release();
	}



	// Release Sprite
	SAFE_RELEASE(	m_pSpMx	);

	// Release Device
	hr = m_pDev->Release();
	if(0 != hr)
		printf(
				"The non-zero reference count means that\n"
				"there is something object not cleaned up.\n"
				);
	m_pDev = NULL;

	// Release Window
	LcSys_DestroyWindow(&m_hWnd);

	// Release GLC
	LcSys_Close();

	return LC_OK;

}



// Window Message Procedure
LRESULT LC_WINAPI WndProc(LC_HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
#if defined(_WIN32)
	switch (uMsg)
	{
		case WM_KEYDOWN:
		{
			if(VK_ESCAPE == wParam)
			{
				SendMessage(hWnd, WM_CLOSE, 0, 0);
				return LC_OK;
			}
			break;
		}

		case WM_CLOSE:
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return LC_OK;
		}
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
#else

	return LC_OK;

#endif
}




FLOAT GetAppFrame()
{
#if defined(_WIN32)
	static INT		MaxCount = 30;
#else
	static INT		MaxCount = 10;
#endif

	static DOUBLE  fFps = 0.;
	static INT     iCnt = 0;
	static DOUBLE  dBgn = ::LcSys_GetTickCount();
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
