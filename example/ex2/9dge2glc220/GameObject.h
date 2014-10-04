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
	INT			texture;		// �ؽ�ó ��ȣ
	LCXVECTOR2	position;		// ��� ��ġ
	LCXVECTOR2	velocity;		// �̵� �ӵ�
	LCXVECTOR2	direction;		// �̵� ����
	FLOAT		rotation;		// ȸ��
	INT			isAlive;		// ���� ����


	// ȭ�� ���
	void Draw(PLC_SPRITE pSpt, PLC_TEXTURE* pTex);
};



#endif

