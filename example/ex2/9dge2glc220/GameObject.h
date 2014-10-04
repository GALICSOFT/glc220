// GLC 2.0 Tutorial Example
//
// Interface for Main GameObject Class.
//
//
// Copyright(C) GPH(Gamepark Holdings). All rights reserved.
// Dogma-G Game Studios In-House Library DGE
//
////////////////////////////////////////////////////////////////////////////////


#ifndef _GameObject_H_
#define _GameObject_H_



class GameObject
{
public:
	INT			texture;		// 텍스처 번호
	LCXVECTOR2	position;		// 출력 위치
	LCXVECTOR2	velocity;		// 이동 속도
	LCXVECTOR2	direction;		// 이동 방향
	FLOAT		rotation;		// 회전
	INT			isAlive;		// 생존 여부


	// 화면 출력
	void Draw(PLC_SPRITE pSpt, PLC_TEXTURE* pTex);
};



#endif

