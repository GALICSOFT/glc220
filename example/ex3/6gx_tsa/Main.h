// Interface for the CMain class.
//
////////////////////////////////////////////////////////////////////////////////


#if defined(_MSC_VER) && _MSC_VER > 1000
	#pragma once
#endif


#ifndef _CMain_H_
#define _CMain_H_


typedef		std::map<std::string, PLC_TEXTURE >	LsTex;
typedef		LsTex::iterator							ItTex;


class CMain : public CAppGame
{
protected:
	PLCX_INPUT		m_pInput;		// Input
	PLCX_CAMERA		m_pCam3D;		// 3D Camera Instance
	PLCX_CAMERA		m_pCam2D;		// 2D Camera


	PLCX_ENTITY		m_pXYZ	;		// X, Y, Z Axis Line
	PLCX_ENTITY		m_pGrid	;		// Grid Instance

	PLCX_TSA		m_pTsa0	;		// Original 2D Animation List
	PLCX_TSA		m_pTsa1	;		// Instance 2D Animation List
	PLCX_TSA		m_pTsa2	;		// Instance 2D Animation List

	LsTex			m_vTex	;

public:
	CMain();

	virtual INT	Init();
	virtual INT	Destroy();
	virtual INT	FrameMove();
	virtual INT	Render();

protected:
	PLC_TEXTURE	FindTex(char* sTex);
};

extern	CMain*	g_pApp;
#define	GMAIN	g_pApp

#endif


