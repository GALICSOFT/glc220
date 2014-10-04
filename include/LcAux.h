// Copyright(C) GaLic Soft. All rights reserved.
// GaLic Game Library Collection
//
// GLC SDK Auxiliary Program
//
////////////////////////////////////////////////////////////////////////////////

#if defined(_MSC_VER)
  #pragma once
#endif


#ifndef _LcAux_H_
#define _LcAux_H_

#include <LcType.h>


// Utilty Macro

#define SAFE_FREE(p)					\
{ if(p) {								\
		free(p); (p) = NULL;			\
} }

#define SAFE_DELETE(p)					\
{ if(p) {								\
		delete (p);	(p) = NULL;			\
} }

#define SAFE_DELETE_ARRAY(p)			\
{ if(p) {								\
		delete [] (p); (p) = NULL;		\
} }

#define SAFE_RELEASE(p)					\
{ if(p) {								\
		(p)->Release();	(p) = NULL;		\
} }

#define SAFE_FCLOSE(p)					\
{ if(p) {								\
		fclose(p);	(p) = NULL;			\
} }


#define SAFE_NEWCREATE1(p, CLASSTYPE, P1)				\
{ if(!(p)) {											\
		p = new CLASSTYPE;								\
		if(!(p)) {	return -1;	}						\
														\
		if(LC_FAILED(((CLASSTYPE *)p)->Create(P1)))	\
		{												\
			delete p; p=(NULL); return -1;				\
		}												\
} }

#define SAFE_NEWCREATE2(p, CLASSTYPE, P1, P2)				\
{ if(!(p)) {												\
		p = new CLASSTYPE;									\
		if(!(p)) {	return -1;	}							\
															\
		if(LC_FAILED(((CLASSTYPE *)p)->Create(P1, P2)))	\
		{													\
			delete p; p=(NULL); return -1;					\
		}													\
} }

#define SAFE_NEWCREATE3(p, CLASSTYPE, P1, P2, P3)				\
{ if(!(p)) {													\
		p = new CLASSTYPE;										\
		if(!(p)) {	return -1;	}								\
																\
		if(LC_FAILED(((CLASSTYPE *)p)->Create(P1, P2, P3)))	\
		{														\
			delete p; p=(NULL); return -1;						\
		}														\
} }

#define SAFE_NEWCREATE4(p, CLASSTYPE, P1, P2, P3, P4)			\
{ if(!(p)) {													\
		p = new CLASSTYPE;										\
		if(!(p)) {	return -1;	}								\
																\
		if(LC_FAILED(((CLASSTYPE *)p)->Create(P1, P2, P3, P4)))\
		{														\
			delete p; p=(NULL); return -1;						\
		}														\
} }


// Vector delete all in list
#define SAFE_DEL_LST(p)										\
{															\
	if(!p.empty())											\
	{														\
		int __nLst##p = p.size();							\
		for(int __n##p=0; __n##p < __nLst##p ; ++__n##p )	\
		{													\
			if( p[ __n##p ]	)								\
				delete p[ __n##p ];							\
															\
			p[ __n##p ] = NULL;								\
		}													\
		p.clear();											\
	}														\
}

#define SAFE_FRMMOV(p) { if(p) (p)->FrameMove(); }
#define SAFE_UPDATE(p) { if(p) (p)->Update();	 }
#define SAFE_RENDER(p) { if(p) (p)->Render();    }




////////////////////////////////////////////////////////////////////////////////
// LC Time and Etc...

INT    LcSys_tzset();
INT    LcSys_InitTime();														// set up the start time
DOUBLE LcSys_GetTickCount();													// return Millisecond time after program start. low resolution(10ms) timer in windows system.
DOUBLE LcSys_TimeGetTime();														// middle resolution(1ms) timer.
DOUBLE LcSys_GetTime();															// Hi resolution(1/1000ms) timer
DINT   LcSys_DateandTime(char *tmpbuf/*need 128byte*/);							// System Date, Time

INT	   LcSys_Sleep(UINT Milliseconds);											// Sleep
INT    LcSys_RandSeedSet(ULONG dSeed);											// srand( time() ) ==> LcSys_RandSeedSet(0);
INT    LcSys_Rand();															// call rand()

INT    LcSys_FindResource(char* Out,const char* sPathIn);						// Find resource from resource in iphone, android, and bada. Output needs 260bytes.
const char* LcSys_TempFolder();													// Iphone: Get Temporary folder. it needs 260bytes
const char* LcSys_AppFolder(char* apkName=NULL);								// to be able to read and write. Iphone: return the Document folder. Android: the get method App Foler when the apk Name is NULL.

const char* LcSys_IphoneRscPath();												// get the iphone resource path
const char* LcSys_AndroidApkFile(char* apkName=NULL);							// set/get android apk file. the get method Apk File when the apk Name is NULL

INT  LcSys_GetCurrentDir(char* dir, INT MaxLength);								// getcwd()
INT  LcSys_SetCurrentDir(char* dir);											// chdir()
INT  LcSys_SetCurrentDirWithCommandLine();										// Set Current directory with command line in Microsoft Windows



////////////////////////////////////////////////////////////////////////////////
// LC Memory

LC_HANDLE LcMem_Allock(INT BuffSize,INT Count,INT InitSetVal=0);				// malloc + memset(InitSetVal)
void      LcMem_Free(LC_HANDLE* v);												// free(*v) + *v = NULL;




////////////////////////////////////////////////////////////////////////////////
// LC String

LC_INLINE INT LcStr_tolower(INT c);										// Redefine tolower()
LC_INLINE INT LcStr_toupper(INT c);										// Redefine toupper()

INT   LcStr_Stricmp (const char* s1, const char* s2);					// Redefine _stricmp()
INT   LcStr_Strnicmp(const char* s1, const char* s2, UINT n);			// Redefine _strnicmp()

void  LcStr_SplitPath(							// Redefine _splitpath()
					  const char *path			// Input Path
					,	char *drive				// split drive Need 260 bytes
					,	char *dir				// split directory
					,	char *fname				// split file name
					,	char *ext				// split extension
					);


void  LcStr_Trim     (char* Buf, const char *Trim=(char*)" \t\r\n");	// Left + Right Trim
void  LcStr_TrimLeft (char* Buf, const char *Trim=(char*)" \t\r\n");	// Left Trim
void  LcStr_TrimRight(char* Buf, const char *Trim=(char*)" \t\r\n");	// Right Trim
INT   LcStr_Quot     (char* Dst, const char* Src);						// retrn string list and number with "{ }" block





////////////////////////////////////////////////////////////////////////////////
// LC File
enum
{
	LC_TYPE_FOLDER	= 0X10,
	LC_TYPE_FILE	= 0X20,
	LC_TYPE_SYSTEM	= 0X04,
};

typedef struct tagTLC_FINDDATA
{
    UINT	attrib;
    LONG	time_create;		// -1 for FAT file systems
    LONG	time_access;		// -1 for FAT file systems
    LONG	time_write;
    ULONG	size;
    char	name[LC_MAX_PATH2];
}TLC_FINDDATA;


typedef struct tagTLC_FINF
{
	INT		nType;				// Folder: 0x10, File: 0x20
	LONG	lDate;				// Make Date
	LONG	lBuf;				// Length of File Buffer
	LONG	lBufB;				// Start Buffer in Merge File
	LONG	lt1;				// temp1
	LONG	lt2;				// temp2
	LONG	lt3;				// temp3
	LONG	lt4;				// temp4
	LONG	iname;				// Length of File Name
	char*	name;				// File Name

	tagTLC_FINF*	pNext;
}TLC_FINF;


INT   LcFile_Exist(char *sFile);												// if there is the search file. true: 0 false: -1
UINT  LcFile_Attribute(const char* sPath);										// get the file attributes
L_PTR LcFile_FindFirst(TLC_FINDDATA* pFindData, const char* sSearPath);			// Redefine _findfirst()
INT   LcFile_FindNext(L_PTR handle, TLC_FINDDATA* pFindData);					// Redefine _findnext()
INT   LcFile_FindClose(L_PTR handle);											// Redefine _findClose()

void  LcFile_GatherAllFiles(TLC_FINF** pOut, const char* sStartRoot);
void  LcFile_GatherFree(TLC_FINF** pIn);

INT   LcFile_mkdir(const char* sFolder, UINT mod= 0777);						// Redefine mkdir()
INT   LcFile_rmdir(const char* sFolder);										// Redefine rmdir()

void  LcFile_SimpleStrDateTime(char* pOut, char* sYear, char* sMon, char* sDay, char* sTime, DINT lTime);
INT   LcFile_Finds(TLC_FINF** pOut, const char* sPath, const char* sFile);		// Find File List
INT   LcFile_FindSameExt(TLC_FINF** pOut, const char* sPath, const char* sExt);	// Find Same Extension File List
INT   LcFile_XcopyFileToFile(char* destfile, char* srcfile);					// File Copy src to dest. return the copyed bytes. failed -1
INT   LcFile_XcopyBufferToFile(char* destfile, BYTE* buf, INT size);			// Buffer Copy to dest file. return the copyed bytes. failed -1


DCL_INTERFACE ILC_File : public ILC_Base
{
	DCL_CLASS_DESTROYER(	ILC_File	);

	virtual INT		Query(char* sCmd, LC_HANDLE pData)=0;
	virtual INT		Release()=0;

	virtual INT		ReadByte (char* buf, INT size) =0;		// fread
	virtual INT		WriteByte(char* buf, INT size) =0;		// fwrite
	virtual INT		ReadLine(char* buf, INT size) =0;		// fgets
	virtual INT		Seek(INT offset, INT origin) =0;		// fseek

	virtual INT		GetSize() =0;							// return size of buffer or file
	virtual INT		GetMode() =0;							// read: 0x1, write: 0x2, rw: 0x3
	virtual INT		Tell() =0;								// return the current file pointer position
	virtual INT		Eof() =0;								// is End of File?
};

typedef ILC_File*	PLC_FILE;

INT LcFile_CreateFile(char*     sCmd                        //
                    , PLC_FILE* pOut                        // Output instance
                    , LC_HANDLE sFile						// file name or buffer address
                    , INT       size  = 0                   // if it'll be used to file size must be zero. buffer then size is greather than zero
                    , char*     mode  ="r"                  // read write mode. w, r, w+, r+ rt, wt, rb, wb. not support the append 'a' or 'a+'
                    );




////////////////////////////////////////////////////////////////////////////////
// zlib 1.2.5 and minizip Zip Api Interface

enum
{
	LZIP_NO_COMPRESS      =    0,
	LZIP_BEST_SPEED       =    1,
	LZIP_BEST_COMPRESS    =    9,
	LZIP_DEFAULT_COMPRESS = (-1),
};


// compressed buffer =
//    total size(unsigned long: zipped size + unsigned long*2 )
//    + orginal size(unsigned long) + zip data(unsigned byte*)


INT Lzip_CompressBufferToBuffer(BYTE**       OutZip			// output zip data with source size and zip size
							  , const BYTE*  Src			// input src data
							  , const ULONG  SrcSize		// input src size
							  , INT level = LZIP_BEST_SPEED	// zip level
							  );


INT Lzip_CompressToBufferFromFile(BYTE**     OutZip			// output zip data
							   , const char* sOrg			// source file name
							   , INT level=LZIP_BEST_SPEED	// zip level
							   );


INT Lzip_CompressFileToFile(char* sZip						// Output Compress File Name
						  , char* sOrg						// Source File Name
							);


INT Lzip_DeCompressBufferToBuffer(BYTE** OutUnzip			// output unzip data with output size
								, BYTE* InputZiped			// Input zipped data
								);

INT Lzip_DeCompressBufferToBuffer(BYTE**  OutUnzip			// output unzip data with no output size
								, ULONG* OutSize			// input src size
								, BYTE*  InputZiped			// Input zipped data
								);


INT Lzip_DeCompressToBufferFromFile(BYTE**      OutUnZip	// output unzip data
								  , char*       sOrgFile	// original file name
								  , const char* sZip		// zipped file name
									);

INT Lzip_DeCompressFileToFile(char* sOrg					// Output DeCompress File Name
						    , const char* sZip				// zipped File Name
								);


INT Lzip_UnzipbyName(							// unzip from zip file by finding name. you need to call the free()...
					   BYTE** bufOut			// DeCompress Data
					 , ULONG* lsize				// DeCompress Data Size
					 , const char* ZipFile		// Compressed zip file
					 , const char* FindingName	// to finding file Name
					 , INT bIgnorePath = 0		// Ignore the path in zip file
					 );







////////////////////////////////////////////////////////////////////////////////
// GLC Log for Debugging
enum
{
	LC_LOG_WAY_NO		= 0x0,					// can't write log.
	LC_LOG_WAY_SERIAL	= 0x1,					// default value.
	LC_LOG_WAY_FILE		= 0x2,					// It will make a glc_log.txt with 'wb' option.
	LC_LOG_WAY_MIX		= 0x3,					// log to serial and file.

	LC_LOG_DISABLE		= 0x0,					// Disable Log Writing
	LC_LOG_ENABLE		= 0x1,					// Enable Log Writing : default value.


	// Log Type
	LC_LOG_INFO			= 0x100,				// Information
	LC_LOG_WARNING		= 0x101,				// Warnning
	LC_LOG_ERROR		= 0x102,				// Error
};

void  LcLog_Out(INT Type, const char* fmt,...);	// print the log
void  LcLog_OutI(const char* fmt, ...);			// This and LcLog_Out(LC_LOG_INFO) are the same function.
void  LcLog_OutW(const char* fmt, ...);			// This and LcLog_Out(LC_LOG_WARNING) are the same function.
void  LcLog_OutE(const char* fmt, ...);			// This and LcLog_Out(LC_LOG_ERROR) are the same function.

// setup the log environment
void  LcLog_SetWay(INT iWay);
void  LcLog_SetSate(INT v);						// LC_LOG_ENABLE(LC_TRUE), LC_LOG_DISABLE(LC_FALSE)
INT   LcLog_GetState();
INT   LcLog_ErrCount();							// get the error(LC_LOG_ERROR) log count


// Log Output Definition
#if defined(__ANDROID__)
  #include <android/log.h>
  #define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO,  "G-pack", __VA_ARGS__))
  #define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN,  "G-pack", __VA_ARGS__))
  #define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "G-pack", __VA_ARGS__))

#elif defined(__BADA__)
  #include <FBase.h>	// for AppLog
  #define LOGI	AppLog
  #define LOGW	AppLog
  #define LOGE	AppLog

#else
  #define LOGI	LcLog_OutI
  #define LOGW	LcLog_OutW
  #define LOGE	LcLog_OutE

#endif



////////////////////////////////////////////////////////////////////////////////
// LC Random
class TLC_RAND
{
public:
	TLC_RAND();			// Sets the random starting point with current time.
	TLC_RAND(UINT v);	// Sets the random starting point with the argument value.

	DOUBLE	NextDouble();					// random double number in [0, 1.0 ]
	INT		Next();							// random int number in [0, 32767( RAND_MAX ) ]
	INT		Next(INT   _max);				// random int number in [0, _max)
	INT		Next(INT   _min, INT   _max);	// random int number in [_min,_max)
	FLOAT	Next(FLOAT _min, FLOAT _max);	// random float number in [_min,_max)
};





////////////////////////////////////////////////////////////////////////////////
// LC Tree(Node) Structure.
//
//	parent
//	ก่ก้         ก่             ก่             ก่
//	chld <--> sibling1 <--> sibling2 <--> sibling3

template<class T>
class TLC_Node
{
protected:
	TLC_Node<T>* P;					// parent
	TLC_Node<T>* C;					// child
	TLC_Node<T>* B;					// previous
	TLC_Node<T>* N;					// next

	T*            A;					// attribute

public:
	TLC_Node();
	TLC_Node(TLC_Node<T>*);
	virtual ~TLC_Node();


	T*	 GetAttrib()		{	return A;	}
	void SetAttrib(const T& a){	A = (T*)&a;	}
	void SetAttrib(T* a)	{	A = (T*)a;	}
	void SetAttrib(INT a)	{	A = (T*)a;	}


	////////////////////////////////////////////////////////////////////////////
	// Node Iteration

	INT HasParent()		;				// Has parent?
	INT HasNotParent()	;				// Has no parent?
	INT HasChild()		;				// Has child?
	INT HasNotChild()	;				// Has no child?
	INT IsSiblingF()	;				// Am I first child?

	INT IsSiblingL()	;				// Am I last child?
	INT IsRoot()		;				// Am I root?
	INT IsChild()		;				// Am I Child?

	TLC_Node<T>* GetSiblingF();			// find first node from sibling list
	TLC_Node<T>* GetSiblingL();			// find last node from sibling list
	TLC_Node<T>* FindRoot()   ;			// find root node

	void AttachTo(TLC_Node<T>* Parent);	// set the parent node. (I'm a child node)
	void Attach(TLC_Node<T>* Child);	// attach the child node. (I'm parent node)
	void Detach();						// detach
	INT CountNodes();					// count node
};

typedef TLC_Node<PLC_OBJECT >	VLC_OBJECT;




////////////////////////////////////////////////////////////////////////////////
//Implementation of TLC_NODE

template<class T> TLC_Node<T>::TLC_Node() : P(0), C(0), B(0), N(0){}


template<class T> TLC_Node<T>::~TLC_Node()
{
	Detach();									// detach from parent node
	if(0 == C) return;
	while(C) C->Detach();						// if has child
}


template<class T> INT TLC_Node<T>::HasParent()	 {	return (0 != P);	}		// Has parent?
template<class T> INT TLC_Node<T>::HasNotParent(){	return (0 == P);	}		// Has no parent?
template<class T> INT TLC_Node<T>::HasChild()	 {	return (0 != C);	}		// Has child?
template<class T> INT TLC_Node<T>::HasNotChild(){	return (0 == C);	}		// Has no child?
template<class T> INT TLC_Node<T>::IsSiblingF() {	return (P&&P->C==this);}	// Am I first child?

template<class T> INT TLC_Node<T>::IsSiblingL() {	return (P && 0 == N);	}	// Am I last child?
template<class T> INT TLC_Node<T>::IsRoot()		{	return (0 == P);		}	// Am I root?
template<class T> INT TLC_Node<T>::IsChild()	{	return (0 != P);		}	// Am I Child?

template<class T> TLC_Node<T>* TLC_Node<T>::GetSiblingF(){return (B)? B->GetSiblingF():this;}	// find first node from sibling list
template<class T> TLC_Node<T>* TLC_Node<T>::GetSiblingL(){return (N)? N->GetSiblingL():this;}	// find last node from sibling list
template<class T> TLC_Node<T>* TLC_Node<T>::FindRoot(){	return (P)? P->FindRoot():	 this;}	// find root node

template<class T> void TLC_Node<T>::AttachTo(TLC_Node<T>* Parent)	// set the parent node. (I'm a child node)
{
	TLC_Node<T>* pParentChild = NULL;
	TLC_Node<T>* pSiblingLast = NULL;

	if(P)
		Detach();

	P = Parent;														// set the parent node
	pParentChild =	P->C;

	if(!P->C)
	{
		P->C = this;
	}
	else
	{
		pSiblingLast = pParentChild->GetSiblingL();
		B = pSiblingLast;
		pSiblingLast->N = this;
	}
}

template<class T> void TLC_Node<T>::Attach(TLC_Node<T>* Child)	// attach the child node. (I'm parent node)
{
	TLC_Node<T>* pSiblingLast = NULL;

	if(Child->HasParent())
		Child->Detach();

	Child->P = this;
	if(C)
	{
		pSiblingLast = C->GetSiblingL();
		pSiblingLast->N = Child;
		Child->B = pSiblingLast;
	}
	else
	{
		C = Child;
	}
}

template<class T> void TLC_Node<T>::Detach()
{
	if(P && P->C == this)
	{
		if(N)
		{
			P->C = N;
			P->C->B = 0;
		}
		else
			P->C = 0;
	}

	if(B) B->N = N;
	if(N) N->B = B;

	P = 0; B = 0; N = 0;
}

template<class T> INT TLC_Node<T>::CountNodes()
{
	if(C)
		return C->CountNodes() + 1;

	return 1;
}


#endif


