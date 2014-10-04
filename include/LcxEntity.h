// Interface for the 3D utilities classes.
//
////////////////////////////////////////////////////////////////////////////////

#if defined(_MSC_VER)
  #pragma once
#endif


#ifndef _ILCX_Entity_H_
#define _ILCX_Entity_H_

#include <LcType.h>



////////////////////////////////////////////////////////////////////////////////
// Grid and XYZ Axis Line

DCL_INTERFACE ILCX_Entity : public ILC_Object
{
	DCL_CLASS_DESTROYER(	ILCX_Entity	);


	virtual INT		Query(char* sCmd, LC_HANDLE pData)=0;
	virtual INT		Release()=0;


	virtual INT		FrameMove()=0;
	virtual void	Render()=0;
};


typedef	ILCX_Entity*	PLCX_ENTITY;



// Create Entity Instance
#define LCXCMD_ENTITY_AXIS	(char*)"Entity Axis"
#define LCXCMD_ENTITY_GRID	(char*)"Entity Grid"

INT LcxEnt_CreateEntity(char* sCmd
				   , PLCX_ENTITY* pOut		// Output instance
				   , LC_HANDLE p1 = NULL	// User Data(Option)
				   , LC_HANDLE p2 = NULL	// No Use
				   , LC_HANDLE p3 = NULL	// No Use
				   , LC_HANDLE p4 = NULL	// No Use
					 );




////////////////////////////////////////////////////////////////////////////////
// Interface for the ILCX_Camera class.

enum ELCX_CAMERA
{
	LCX_CAM_NONE	=0,			//
	LCX_CAM_FIRST	,			// 1st Person
	LCX_CAM_THIRD	,			// 3rd Person
	LCX_CAM_2D		= 0x00100,	// 2d camera
};

struct TLCX_CAMERA
{
	// for Projection Matrix
	FLOAT	fFov;			// Field of View
	FLOAT	fNear;			// Near value
	FLOAT	fFar;			// Far value

	// for View Matrix
	FLOAT	vEye[3];		// Camera Position X,Y,Z
	FLOAT	vLook[3];		// Camera Look At Position X,Y,Z
	FLOAT	vUp[3];			// Camera Up Vector X,Y,Z
};


// Single Camera
DCL_INTERFACE ILCX_Camera : public ILC_Object
{
	DCL_CLASS_DESTROYER(	ILCX_Camera	);


	virtual INT		Query(char* sCmd, LC_HANDLE pData)=0;
	virtual INT		Release()=0;


	virtual INT		Update()=0;				// Setup View and Projection Matrix of Camera
	virtual INT		FrameMove()=0;			// Update() + SetTransformViw() and SetTransformPrj()
	virtual INT		SetTransformViw()=0;	// Setup View Matrix of Device and 3d Pipeline.
	virtual INT		SetTransformPrj()=0;	// Setup Projection Matrix of Device and 3d Pipeline.



	// Set Parameter
	virtual void	SetupParamView(const LC_FLOAT3* Eye, const LC_FLOAT3* LookAt, const LC_FLOAT3* Up)=0;	// set the eye, look at, up vector parameter
	virtual void	SetupParamProj(FLOAT Fov, FLOAT ScnW, FLOAT ScnH, FLOAT Near, FLOAT Far)=0;				// set the screen width, height and near, far value parameter

	virtual void	SetupMatrixViw(const LC_MATRIX*)=0;		// set with the view matrix.
	virtual void	SetupMatrixPrj(const LC_MATRIX*)=0;		// set with the projection matrix.

	virtual void	SetEye (const LC_FLOAT3* v)=0;			// set the eye vector parameter.
	virtual void	SetLook(const LC_FLOAT3* v)=0;			// set the look at vector parameter.
	virtual void	SetUp  (const LC_FLOAT3* v)=0;			// set the up vector parameter.

	// Get value
	virtual void	GetParamView(LC_FLOAT3* Eye, LC_FLOAT3* Look, LC_FLOAT3* Up)=0;						// get the releated view matrix parameter.
	virtual void	GetParamProj(FLOAT* Fov, FLOAT* ScnW, FLOAT* ScnH, FLOAT* Near, FLOAT* Far)=0;		// get the releated projection matrix parameter.

	virtual const	LC_MATRIX* const GetMatrixViw()=0;		// get the view matrix.
	virtual const	LC_MATRIX* const GetMatrixPrj()=0;		// get the projection matrix.

	virtual const	LC_FLOAT3* const GetEye()  =0;			// get the eye vector.
	virtual const	LC_FLOAT3* const GetLook() =0;			// get the look at vector.
	virtual const	LC_FLOAT3* const GetUp()   =0;			// get the up vector.

	virtual const	LC_FLOAT3* const GetAxisX()=0;
	virtual const	LC_FLOAT3* const GetAxisY()=0;
	virtual const	LC_FLOAT3* const GetAxisZ()=0;

	virtual const	LC_FLOAT4* const GetFrustumPlanes()=0;
	virtual const	LC_FLOAT3* const GetFrustumPoints()=0;

	// Action
	virtual void	MoveSideward(FLOAT fSpeed)=0;
	virtual void	MoveForward	(FLOAT fSpeed,FLOAT fY=1)=0;
	virtual void	MoveUpward  (FLOAT fSpeed)=0;

	virtual void	RotateYaw	(FLOAT fDelta, FLOAT fSpeed)=0;
	virtual void	RotatePitch	(FLOAT fDelta, FLOAT fSpeed)=0;
};


typedef ILCX_Camera*	PLCX_CAMERA;


// Create Camera Instance
#define LCXCMD_CAMERA_2D	(char*)"2D Camera"
#define LCXCMD_CAMERA_3D	NULL

INT LcxEnt_CreateCamera(char* sCmd
					   , PLCX_CAMERA* pOut			// Output instance
					   , LC_HANDLE p1 = NULL		// User Data(Option)
					   , LC_HANDLE p2 = NULL		// No Use
					   , LC_HANDLE p3 = NULL		// No Use
					   , LC_HANDLE p4 = NULL		// No Use
						);




////////////////////////////////////////////////////////////////////////////////
// Game Model Interface

enum ELCX_MDL
{
	LCXM_TEXNAME			= 0x00000080,		// Texture Name Length 128
	LCXM_ANINAME			= 0x00000040,		// Animation Name 64
	LCXM_HEADEAR			= 0x00000040,		// Header Size 64

	LCXM_FRAME_INTERVAL		= 40,				// Default Animation Time Interval(40ms==>25Frame)


	// Resource Type: GetRscType()
	LCXM_SRC_DGT			= 0x00000201,		// 2D Text File
	LCXM_SRC_DGB			= 0x00000202,		// 2D Binary File
	LCXM_SRC_TSA			= 0x00000203,		// 2D Binary Animation List File

	LCXM_SRC_ASE			= 0x00000301,		// 3D Max ASE File
	LCXM_SRC_ACM			= 0x00000303,		// 3D LC ACM File


	// Attributes...
	LCXM_ADVANCE_TIME		= 0x00001001,		// Advanced Time(DOUBLE)
	LCXM_CURRENT_TIME		= 0x00001002,		// Current Time (DOUBLE)
	LCXM_CURRENT_FRAME		= 0x00001003,		// Current Frame(FLOAT)

	LCXM_WORLD_MATRIX		= 0x00001010,		// World Matrix(2D: 3x3, 3D: 4x4 Matrix)
	LCXM_WORLD_MATRIX_PT	= 0x00001011,		// World Matrix Address
	LCXM_LAYER_MATRIX		= 0x00001012,		// Updated Layer Matrix
	LCXM_LAYER_MATRIX_PT	= 0x00001013,		// Updated Layer Matrix Address
	LCXM_PARENT_MATRIX		= 0x00001014,		// Parent Matrix Pointer
	LCXM_GEOMETRY_MATRIX	= 0x00001015,		// Updated Geometry Matrix
	LCXM_GEOMETRY_MATRIX_PT	= 0x00001016,		// Updated Geometry Matrix Address

	LCXM_LOOP_SECTION		= 0x00001021,		// Animation Loop Section
	LCXM_FRAME_INDEX		= 0x00001022,
	LCXM_FRAME_INDEX_ABSOLUTE=0x00001023,
	LCXM_MAX_ANI_BLENDING	= 0x00001024,		// Skinning Animation Matrix Blending Number
	LCXM_FRAME_SPEED		= 0x00001025,		// Animation Frame Speed (FLOAT)
	LCXM_REPEATE_COUNT		= 0x00001026,		// Repeate Count(INT)
	LCXM_END_FREEZE_FRAME	= 0x00001027,		// End Freeze Frame

	LCXM_BOUNDING_RECT		= 0x00001041,		// Bounding rect. only 2d model. it needs LCXRECT
	LCXM_BOUNDING_BOX		= 0x00001042,		// Bounding box
	LCXM_BOUNDING_SPHERE	= 0x00001043,		// Bounding sphere
	LCXM_DIFFUSE_COLOR		= 0x00001046,		// Diffuse Color. It needs FLOAT4 RGBA color.

	// Command Execution: SetAttrib()
	LCXM_EXEC_TEXTURE_LOAD	= 0x00100001,		// Texture Loading Command.
	LCXM_TEXTURE_LIST_NUMBER= 0x00100002,		// Texture List Number
	LCXM_TEXTURE_LIST_NAME	= 0x00100003,		// Texture Name List
	LCXM_TEXTURE_LIST_POINTER=0x00100004,		// Texture Pointer List
};


////////////////////////////////////////////////////////////////////////////////
// Three-dimensional Game Model Interface
DCL_INTERFACE ILCX_Mdl : public ILC_Object
{
	DCL_CLASS_DESTROYER(	ILCX_Mdl	);


	virtual INT		Query(char* sCmd, LC_HANDLE pData)=0;
	virtual INT		Release()=0;


	virtual	INT		FrameMove()=0;
	virtual void	Render()=0;

	virtual INT		SetAttrib(UINT dCmd, LC_HANDLE pData)=0;
	virtual INT		GetAttrib(UINT dCmd, LC_HANDLE pData)=0;

	virtual INT		GetRscType()=0;
};

typedef	ILCX_Mdl*	PLCX_MDL;

// Create 3D Model
INT LcxEnt_CreateMdl(char* sCmd
				 , PLCX_MDL* pOut				// Output instance
				 , LC_HANDLE sName				// Model File Name
				 , LC_HANDLE pOrg	= NULL		// Original LCX_Mdl Pointer for Clone Creating
				 , LC_HANDLE p3		= NULL		// Not Use
				 , LC_HANDLE p4		= NULL		// Not Use
				 );



////////////////////////////////////////////////////////////////////////////////
// Two-dimensional Game Model Interface
DCL_INTERFACE ILCX_Tdm : public ILC_Object
{
	DCL_CLASS_DESTROYER( ILCX_Tdm );


	virtual INT		Query(char* sCmd, LC_HANDLE pData)=0;
	virtual INT		Release()=0;


	virtual	INT		FrameMove()=0;
	virtual void	Render()=0;

	virtual INT		SetAttrib(UINT dCmd, LC_HANDLE pData)=0;	// Write Attribute Value
	virtual INT		GetAttrib(UINT dCmd, LC_HANDLE pData)=0;	// Read Attribute Value

	virtual INT		GetRscType()=0;								// Resource type: LCXM_SRC_DGT, LCXM_SRC_DGB


	// Rendering Position
	virtual INT		SetPosition(const LC_FLOAT2*)=0;	// position: float2
	virtual INT		SetRotation(const LC_FLOAT3*)=0;	// rotation: float3 rotation center(x,y) + rotation angle(z: radian)
	virtual INT		SetScaling (const LC_FLOAT2*)=0;	// scaling : float2

	// Select Sprite Animation
	virtual INT		SetAnimation(const char* Layer, const char* Animation)=0;
	virtual INT		SetAnimation(const INT Layer, const INT Animation)=0;
};


typedef	ILCX_Tdm*	PLCX_TDM;


// Create Instance
INT LcxEnt_CreateTdm(char* sCmd
				 , PLCX_TDM* pOut				// Output instance
				 , LC_HANDLE sName				// Model File Name
				 , LC_HANDLE pOrg	= NULL		// Original LCX_Tdm Pointer for Clone Creating
				 , LC_HANDLE pOption= NULL		// Texture Load Option. It needs TLC_TEXTURE pointer
				 , LC_HANDLE p4     = NULL		// No Use
				 );




////////////////////////////////////////////////////////////////////////////////
// Two-dimensional Simple Animation List Interface
DCL_INTERFACE ILCX_Tsa : public ILC_Object
{
	DCL_CLASS_DESTROYER( ILCX_Tsa );


	virtual INT		Query(char* sCmd, LC_HANDLE pData)=0;
	virtual INT		Release()=0;


	virtual	INT		FrameMove()=0;								// return the remain repeate count. infinite is LC_INFINITE
	virtual void	Render()=0;									// for 2d animation model.
	
	virtual void	DrawOneFrame(PLC_SPRITE        pSprite		// Independent rendering for time, animation frame index
								, const char*      frame_name	// frame name
								, const LC_FLOAT2* vcTrn		// Position
								, const LC_FLOAT2* vcScl=NULL	// Scaling
								, const LC_FLOAT2* vcRot=NULL	// Rotation Center
								, FLOAT            fRot = 0		// Angle(Radian)
								, const LC_COLORF* color=NULL	// color
								, const INT        bOffset=0	// draw with offset position. 0: is absolute position
								)=0;


	virtual INT		SetAttrib(UINT dCmd, LC_HANDLE pData)=0;	// Write Attribute Value
	virtual INT		GetAttrib(UINT dCmd, LC_HANDLE pData)=0;	// Read Attribute Value

	virtual INT		GetRscType()=0;								// Resource type: LCXM_SRC_DGT, LCXM_SRC_DGB

	// Rendering Position
	virtual INT		SetPosition(const LC_FLOAT2*)=0;	// position: float2
	virtual INT		SetRotation(const LC_FLOAT3*)=0;	// rotation: float3 rotation center(x,y) + rotation angle(z: radian)
	virtual INT		SetScaling (const LC_FLOAT2*)=0;	// scaling : float2

	// Select Sprite Animation
	virtual INT		SetAnimation(const char* Animation)=0;
	virtual INT		SetAnimation(const INT Animation)=0;
};


typedef	ILCX_Tsa*	PLCX_TSA;


// Create Instance
INT LcxEnt_CreateTsa(char* sCmd
				 , PLCX_TSA* pOut				// Output instance
				 , LC_HANDLE sName				// Animation List Name
				 , LC_HANDLE pOrg	= NULL		// Original LCX_Tsa Pointer for Clone Creating
				 , LC_HANDLE p3		= NULL		// No use
				 , LC_HANDLE p4		= NULL		// No use
				 );

INT LcxEnt_CreateTsaFromMemory(char* sCmd
				 , PLCX_TSA* pOut				// Output instance
				 , LC_HANDLE sName				// Animation List Name
				 , LC_HANDLE pOrg				// Original LCX_Tsa Pointer for Clone Creating
				 , LC_HANDLE Memory	= NULL		// Memory Pointer
				 , LC_HANDLE pSize	= NULL		// Memory Size(unsigned int*)
				 );






#if !defined(_LC_ORG_SRC_)

////////////////////////////////////////////////////////////////////////////////
// Graphic user interface.

// Gui Type: Dialog, Button, List, Combo box, Static, Edit Box, Check Box, Radio button, slide bar
// Dialog: Only Popup window
// Data structure: Tree Structure


enum ELCX_GUI
{
	// GUI type
	LCXGUI_TYPE			= 0x00000440,		// GuiDefault Type
	LCXGUI_DIALOG		= 0x00000441,		// Dialog
	LCXGUI_SYSBUTTON	= 0x00000442,		// System Button
	LCXGUI_BUTTON		= 0x00000443,		// Button
	LCXGUI_EDIT			= 0x00000444,		// Edit
	LCXGUI_STATIC		= 0x00000445,		// Static String

	LCXGUI_MAX_NAME		= LC_DEFAULT_NAME,	// Max Name Length

	// GUI State
	LCXGUI_DISABLE		= 0x00000000,		// Action and Event Disable
	LCXGUI_ENABLE		= 0x00000001,		// Actin and Event Enable
	LCXGUI_HIDE			= 0x00000010,		// Hide
	LCXGUI_SHOW			= 0x00000011,		// Show
	LCXGUI_ACTIVE		= 0x00000012,		// Show and Active
	LCXGUI_DRAG			= 0x00000014,		// Drag

	// Event Message or Status Value
	LCXGUI_EVNT			= 0x10000000,		// Read/Write Event Information
	LCXGUI_EVNT_DOWN	= 0x10000001,		// Mouse(touch) down
	LCXGUI_EVNT_UP		= 0x10000002,		// Mouse(touch) up
	LCXGUI_EVNT_PRESS	= 0x10000004,		// Mouse(touch) pressed
	LCXGUI_EVNT_MOVE	= 0x10000010,		// Move(drag)
	LCXGUI_EVNT_OVER	= 0x10000020,		// Mouse(touch) over

	LCXGUI_EVNT_CLOSE	= 0x70000000,		// Close Event


	// Member Value
	LCXGUI_INFO			= 0x00001001,		// Gui Info
	LCXGUI_ID			= 0x00001002,		// id. it needs unsigned int
	LCXGUI_NAME			= 0x00001003,		// Name
	LCXGUI_POSITION		= 0x00001004,		// Read/Write Gui Position
	LCXGUI_EXTERN		= 0x00001005,		// setup the vertex with extern value
	LCXGUI_TEXTURE		= 0x00001007,		// setup the texture
	LCXGUI_FONT			= 0x00001008,		// setup the Font. it needs TLC_FONT
	LCXGUI_TEXT			= 0x00001009,		// get the Font string. it must be need font. if there is not exist the font, then the return value is null.
	LCXGUI_CHILD_GUI	= 0x0000100A,		// get the child GUI in dialog. it needs int ID value and pointer. ex) PLCX_GUI pChild = (PLCX_GUI)0x001; pDialog->GetAttrib(LCXGUI_CHILD_GUI, &pChild);// 0x001 is exam id.
};



DCL_INTERFACE	ILCX_Gui;
typedef		ILCX_Gui*	PLCX_GUI;


struct TLCX_GUI_INF				// Gui Info
{
	UINT     nId   ;			// id
	char*    sName ;			// Gui Name
	char*    sText ;			// Gui Text
	UINT     nType ;			// Gui type

	LC_RECTF* rEvnt ;			// Gui Event(or Full Region) Rect(left, top, width, height)
	LC_RECTF* rTitle;			// Gui Menu(or Title) Rect. just only dialog
	LC_RECTF* rBrcs ;			// Gui Button, System button or Static Region.
	LC_RECTF* rRscs ;			// Residue or Remain Rect.  for All Windows

	PLCX_TDM pPrnt ;			// Parent Gui Window

	TLCX_GUI_INF();
};


struct TLCX_GUI_EVENT
{
	PLCX_GUI _o;				// event object
	UINT     _t;				// type
	UINT     _m;				// message

	union						// event value
	{
		//4 * 16 = 64byte
		FLOAT _sf[16];			// float
		BYTE  _sb[64];			// byte
		DINT  _sn;				// Dint
	};

	TLCX_GUI_EVENT(PLCX_GUI Gui=0, INT Type=0, INT Msg=0);
	TLCX_GUI_EVENT(const TLCX_GUI_EVENT& rhs);
	TLCX_GUI_EVENT& operator=(const TLCX_GUI_EVENT& rhs);

	void Set(PLCX_GUI Gui, INT Type, INT Msg);
	void SetObject(PLCX_GUI Gui);
	void SetType(UINT Type);
	void SetMessage(UINT Message);
	void SetFloat(FLOAT* v, int Number);
	void SetByte(BYTE* v, int Number);
	void SetInt(DINT v);
	void ClearValue();
	void ClearAll();
};



// Extern Gui Texture
// rEvent:
//     Dialog:  all event region
//     Button, static:  position
//
// rRects: need 32[LC_RECTF]
//     Dialog:  0~ 3: title
//              4~11: system button: 4:position 5: normal 6: over 7: pressed
//             12~31: body
//     Button: Normal, Over, Pressed.
//     Static: one Rect
//

struct TLCX_GUI_EXTERN		// Extern Gui Texture
{
	TLC_IMAGE* txInf;		// Texture Info
	LC_RECTF*  rEvnt;		// Event: position + width and height => (x, y, width, height).
	LC_RECTF*  rRcts;		// Rects

	TLCX_GUI_EXTERN();
};


DCL_INTERFACE ILCX_Gui : public ILCX_Tdm
{
	DCL_CLASS_DESTROYER( ILCX_Gui );


	virtual INT		Query(char* sCmd, LC_HANDLE pData)=0;
	virtual INT		Release()=0;


	virtual	INT		FrameMove()=0;
	virtual void	Render()=0;

	virtual INT		SetAttrib(UINT dCmd, LC_HANDLE pData)=0;	// Write Attribute Value
	virtual INT		GetAttrib(UINT dCmd, LC_HANDLE pData)=0;	// Read Attribute Value

	virtual INT		GetRscType()=0;								// Resource type: LCXGUI_DIALOG, ...


	// Rendering Position
	virtual INT		SetPosition(const LC_FLOAT2*)=0;			// position float2
	virtual INT		SetRotation(const LC_FLOAT3*)=0;			// rotation: float3 rotation center(x,y) + radian(z)
	virtual INT		SetScaling (const LC_FLOAT2*)=0;			// scaling: float2

	// Node process
	virtual INT		 Attach (PLCX_TDM pChild)=0;				// Attatch child Gui
	virtual INT		 Detatch(PLCX_TDM pChild)=0;				// Detatch child Gui
	virtual PLCX_TDM GetParent()=0;								// return Parent Gui
	virtual PLCX_TDM GetRoot()  =0;								// return Root Gui

	// values
	virtual void	SetState(INT v)=0;							// Write the gui state
	virtual INT		SetText(const char* s,LC_COLORF* c=NULL)=0; // Write the string in gui and text. col=NULL is LC_COLORF(0,0,0,1)
};




// Create graphic user interface.
#define LCXCMD_GUI_DIALOG	(char*)"Dialog"
#define LCXCMD_GUI_SYSBTN	(char*)"System Button"
#define LCXCMD_GUI_BUTTON	(char*)"Button"
#define LCXCMD_GUI_STATIC	(char*)"Static"
#define LCXCMD_GUI_LIST	(char*)"List"

INT LcxEnt_CreateGui(char*       sCmd					// Command(sCmd).
					, PLCX_GUI*  pOut					// Output data
					, LC_HANDLE    p1					// Gui Info TLCX_GUI* Pointer
					, LC_HANDLE    p2 = NULL			// Gui extern data
					, LC_HANDLE    p3 = NULL			// No Use
					, LC_HANDLE    p4 = NULL			// No Use
					);

// Create gui list from file.
INT LcxEnt_CreateGuiListFromFile(char*	 sCmd			// No Use: just null
							, PLCX_GUI** pLst			// Output data List. Return value is the count of List
							, LC_HANDLE  sTex			// output texture file. it needs 260 byte
							, LC_HANDLE  File			// file name
							, LC_HANDLE    p3 = NULL	// No Use
							, LC_HANDLE    p4 = NULL	// No Use
							);


INT		 LcxGui_Process(TLCX_GUI_EVENT* EventInOut);	// Update Gui and return the gui count which event occurred.
void	 LcxGui_Render();								// Rendering All Gui



// Dialog functions.
void     LcxGui_AddToDialogList(PLCX_GUI pDlg);			// Add to Dialog List
void	 LcxGui_AddToRootDialogList(PLCX_GUI pDlg);		// Add to root dialog list
void	 LcxGui_RemoveDialog(PLCX_GUI pGui);			// Remove one dialog
INT		 LcxGui_DestroyAllDialog();						// Remove all dialog list

void	 LcxGui_SetActiveDialog(PLCX_GUI pDlg);			// Set active dialog. NULL is all dialogs are non-active
PLCX_GUI LcxGui_GetActiveDialog();						// get active dialog.




////////////////////////////////////////////////////////////////////////////////
// Effect interface.

enum ELCX_EFFECT
{
	// Effect  type
	LCXEFT_TYPE			= 0x00000100,		// Effect Default Type

	LCXEFT_2D_PARTICLE	= 0x00000001,		// 2D Particle
	LCXEFT_3D_PARTICLE	= 0x00000002,		// 3D Particle
	LCXEFT_TEXTURE_ANI	= 0x00000004,		// Texture Animation

	LCXEFT_REN_TIANGLE	= 0x00000110,		// triangle type
	LCXEFT_REN_QUAD		= 0x00000120,		// quad type
	LCXEFT_REN_POINT	= 0x00000140,		// Point Sprite type

	// Member Value
	LCXEFT_INFO			= 0x00001001,		// Effect Info
};


struct TLCX_PRT2d;

struct TLCX_PRT2dINF				// Particle Info
{
	// particle
	INT				nPrtNum;		// Particle Number
	INT				bPrtRen;		// Rendering type: Point sprite=0, Triangle =1 , Quad=2
	INT				nPrtSize;		// Particle Pointer size
	TLCX_PRT2d*		pPrtList;		// Particle List

	// rendering option
	FLOAT			nPntL;			// Point sprite size
	PLC_TEXTURE		pTex;			// Texture Pointer

	// boundary and Initial value
	LC_RECTF		IntlRect;		// boundary
	LC_FLOAT2		IntlPos;		// Position
	LC_FLOAT2		IntlVel;		// Velocity
	LC_FLOAT2		IntlAcc;		// Acceleration

	FLOAT			IntlRot;		// rotation Angle
	FLOAT			IntlRov;		// rotation Angle Velocity

	LC_FLOAT2		IntlScl;		// scale
	LC_FLOAT2		IntlScv;		// Scale Velocity

	LC_COLORF		IntlCol;		// Color
	LC_COLORF		IntlCov;		// Color Velocity

	FLOAT			Life;			// Life
	FLOAT			Lifv;			// Life speed. Fade Speed

	FLOAT			IntlMgW;		// Width
	FLOAT			IntlMgH;		// Height

	TLCX_PRT2dINF();
};

struct TLCX_PRT2d					// 2D Particle
{
	INT				idx;			// Particle Index
	LC_FLOAT2		pos;			// Position
	LC_COLORF		col;			// Color
	LC_FLOAT2		mag;			// Width(x) and height(y)
	FLOAT			Life;			// Life time

	TLCX_PRT2d();

	virtual INT Init(TLCX_PRT2dINF* pInf);
	virtual INT Reset(TLCX_PRT2dINF* pInf);
	virtual INT Update(TLCX_PRT2dINF* pInf);
};


DCL_INTERFACE ILCX_Effect : public ILC_Object
{
	DCL_CLASS_DESTROYER( ILCX_Effect );


	virtual INT		Query(char* sCmd, LC_HANDLE pData)=0;
	virtual INT		Release()=0;


	virtual	INT		FrameMove()=0;
	virtual void	Render()=0;

	virtual INT		SetAttrib(UINT dCmd, LC_HANDLE pData)=0;	// Write Attribute Value
	virtual INT		GetAttrib(UINT dCmd, LC_HANDLE pData)=0;	// Read Attribute Value

	virtual INT		GetRscType()=0;								// Resource type: LCXEFT_2D_PARTICLE, ...
};


typedef ILCX_Effect*	PLCX_EFFECT;


// Create Effect Command
#define LCXCMD_EFT_PARTICLE2D	"Particle 2D"
#define LCXCMD_EFT_PARTICLE3D	"Particle 3D"
#define LCXCMD_EFT_TEXTURE_ANI	"Texture Animation"

INT LcxEnt_CreateEffect( char*        sCmd		// Command.
					   , PLCX_EFFECT* pOut		// Output data
					   , LC_HANDLE    p1		// TLCX_PARTICLE_INF
					   , LC_HANDLE    p2 = NULL	// Effect Info TLCX_Effect* Pointer
					   , LC_HANDLE    p3 = NULL	// No Use
					   , LC_HANDLE    p4 = NULL	// No Use
						);



////////////////////////////////////////////////////////////////////////////////
// Motion Sequence

enum ELCX_MOSQC
{
	// Motion inv
	LCXMO_SQC2D			= 0x00000001,		// 2D Motion
	LCXMO_SQC3D			= 0x00000002,		// 3D Motion

	// Motion type
	LCXMO_CART_INTP		= 0x00000010,		// Cartesian interpolation coordinate path
	LCXMO_BEZIER_INTP	= 0x00000020,		// Bezier interpolation coordinate path
	LCXMO_HERMITE_INTP	= 0x00000040,		// Hermite interpolation coordinate path

	// Member Value
	LCXMO_ADVANCE_TIME	= 0x00001001,		// Advance Time
	LCXMO_CURRENT_TIME	= 0x00001002,		// Current Time
	LCXMO_RESULT_SQC	= 0x00001004,		// Sequence

	// Motion state(get) and Execution(set)
	LCXMO_DEFAULT		= LC_DEFAULT,		// default state. do not exeqution
	LCXMO_BEGIN			= 0x00010000,		// start..
	LCXMO_PLAY			= 0x00010001,		// playing
	LCXMO_FINISH		= 0x00010002,		// end state

	// etc...
	LCXMO_NAME			= 40		,		// Motion Name length
	LCXMO_EVENT			= 4			,		// Event list number

};


// The motion sequence is action of translation, scaling,
// rotation and color variation for object with time line.
// initial conditions for motion
struct TLCX_MOSQIC2D
{
	// basic sequence
	DOUBLE		time;				// time interval
	LC_FLOAT2	pos;				// position
	LC_FLOAT2	scl;				// scaling
	LC_FLOAT2	roc;				// rotation center offset =>  rot_center =  pos + ros
	FLOAT		ror;				// rotation angle(radian).
	LC_COLORF	clr;				// color

	INT		nIdx;					// Index
	char	name[LCXMO_NAME];		// motion name or command
	INT		keys[LCXMO_EVENT];		// action key list or applied events

	// advanced opsition
	LC_FLOAT2	psa;				// acceleration => psa  = a(t)
	LC_FLOAT2	psv;				// velocity		=> psv += a * dt  => pos += v * dt
	LC_FLOAT2	scv;				// scaling speed  => scl += scv * dt
	FLOAT		rov;				// rotation speed.  ==>	ror += rov * dt
	LC_COLORF	clv;				// color speed  ==> color += clv * dt

	TLCX_MOSQIC2D();
	TLCX_MOSQIC2D(const TLCX_MOSQIC2D& v);
	TLCX_MOSQIC2D(char* sName, int TimeOut=0, int* KeyList=NULL);
	TLCX_MOSQIC2D& operator =(const TLCX_MOSQIC2D& v);

	void Reset();
};


struct TLCX_MOSQ2D					// result Motion sequence
{
	DOUBLE		time;				// current time
	LC_FLOAT2	pos;				// position
	LC_FLOAT2	scl;				// scaling
	LC_FLOAT2	roc;				// rotation center
	FLOAT		ror;				// rotation angle(radian).
	LC_COLORF	clr;				// color

	TLCX_MOSQ2D();
};


DCL_INTERFACE ILCX_MoSqc : public ILC_Object
{
	DCL_CLASS_DESTROYER( ILCX_MoSqc );

	virtual INT		Query(char* sCmd, LC_HANDLE pData)=0;
	virtual INT		Release()=0;

	virtual	INT		FrameMove()=0;

	virtual INT		SetAttrib(UINT dCmd, LC_HANDLE pData)=0;	// Write Attribute Value
	virtual INT		GetAttrib(UINT dCmd, LC_HANDLE pData)=0;	// Read Attribute Value

	virtual INT		GetRscType()=0;								// Resource type: LCXMO_2D_PARTICLE, ...
};

typedef ILCX_MoSqc*	PLCX_MOSQC;



// Create Motion Sequence
#define LCXCMD_MOSQC_2D	(char*)"MoSqc 2D"
#define LCXCMD_MOSQC_3D	(char*)"MoSqc 3D"

INT LcxEnt_CreateMoSqc( char*     sCmd			// Command.
					, PLCX_MOSQC* pOut			// Output data
					, LC_HANDLE    p1			// Motion Sequence List. => TLCX_MOSQC pointer
					, LC_HANDLE    p2			// List Counter. Int type pointer
					, LC_HANDLE    p3 = NULL	// No Use
					, LC_HANDLE    p4 = NULL	// No Use
					);



#endif// _LC_ORG_SRC_





////////////////////////////////////////////////////////////////////////////////
// Two-dimensional Game Field Map
enum ELCX_TDF
{
	// Type
	LCXTDF_TOPVIEW			= 0x00000011,		// rectangle tile of top view method field.
	LCXTDF_ISOMETRY			= 0x00000012,		// Isometry tile.
};



DCL_INTERFACE ILCX_Tdf : public ILC_Object
{
	DCL_CLASS_DESTROYER(	ILCX_Tdf	);

	virtual INT		Query(char* sCmd, LC_HANDLE pData)=0;
	virtual INT		Release()=0;

	virtual INT		FrameMove()=0;
	virtual void	Render()=0;
};

typedef ILCX_Tdf*	PLCX_TDF;



// make 2d vertex list from the drawing image
void LcxEnt_VtxFromDrawingImage(
				 LC3D::Vx2UV*		OutVtx		// Output Vertex List(Position + Texture Coord)
			   , const FLOAT		ImgW		// Input Image width
			   , const FLOAT		ImgH		// Input Image height
			   , const LCXRECT*		pImgRc		// Input Image Rect
			   , const LCXVECTOR2*	OffsetPos	// StartPosition
			   , INT		bCCW = LC_TRUE		// Texture coordinate orientation
			  );

void LcxEnt_VtxFromDrawingImage(
				 LCXVECTOR2*		OutPos		// Output Position
			   , LCXVECTOR2*		OutTex		// Output Texture Coordinate
			   , const FLOAT		ImgW		// Input Image width
			   , const FLOAT		ImgH		// Input Image height
			   , const LCXRECT*		pImgRc		// Input Image Rect
			   , const LCXVECTOR2*	OffsetPos	// StartPosition
			   , INT		bCCW = LC_TRUE		// Texture coordinate orientation
			   );

void LcxEnt_VtxFromDrawingImage(
				  LCXVECTOR2*		OutPos		// Output position List. need 4 LCXVECTOR2
				, LCXVECTOR2*		OutTex		// Output Texture List. need 4 LCXVECTOR2
				, const FLOAT		ImgW		// Image Width
				, const FLOAT		ImgH		// Image Height
				, const LCXRECT*	pImgRc		// Image rect
				, const LCXVECTOR2* OffsetPos	// Position
				, const LCXVECTOR2*	pvcScl		// Scaling
				, const LCXVECTOR2*	pvcRot		// Rotation Center
				, FLOAT				fRot		// Angle(Radian)
				, INT		bCCW = LC_TRUE		// Texture coordinate orientation
				);


#endif

