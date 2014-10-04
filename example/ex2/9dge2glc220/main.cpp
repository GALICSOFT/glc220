// GLC 2.0 Tutorial Example
//
// Start program.
//
// Copyright(C) GPH(Gamepark Holdings). All rights reserved.
// Dogma-G Game Studios In-House Library DGE
//
////////////////////////////////////////////////////////////////////////////////


#if defined(_MSC_VER) && _MSC_VER > 1000
  #pragma warning( disable : 4786)
  #pragma warning( disable : 4996)
#endif


#if defined(_MSC_VER)
  #pragma comment(linker, "/subsystem:console")
  #pragma comment(linker, "/NODEFAULTLIB[:library]")

  #pragma comment(lib, "lgpwn_b22_.lib")
  #pragma comment(lib, "lgpwn_c22_.lib")
  #pragma comment(lib, "lgpwn_x22_.lib")
  #pragma comment(lib, "lgpwn_t22_.lib")

  #pragma comment(lib, "ftf243m_.lib")
  #pragma comment(lib, "ImgJpg_.lib")
  #pragma comment(lib, "png144_.lib")
  #pragma comment(lib, "zlib125_.lib")
  #pragma comment(lib, "al11s_.lib")

  #pragma comment(lib, "opengl32.lib")
  #pragma comment(lib, "winmm.lib")
  #pragma comment(lib, "dxguid.lib")
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


CAppGame*	g_pApp = NULL;


int main(int argc, char* argv[])
{
	// Init GLC Library
	if(LC_FAILED(LcSys_Init()))
		return LC_EFAIL;


	CAppGame	mainapp;

	g_pApp = &mainapp;

	if(LC_FAILED(mainapp.App_Init()))
		goto End;

	if(LC_FAILED(mainapp.App_RscLoad()))
		goto End;

	mainapp.LoadHighScore();

	mainapp.ObjectInitialize();

	mainapp.DrawScore();

	while(1)
	{
		// 프로그램 로직 실행
		if(LC_FAILED(LcSys_Run()))
			break;
	}


	mainapp.SaveHighScore();


End:
	mainapp.App_Remove();

	return LC_OK;
}



