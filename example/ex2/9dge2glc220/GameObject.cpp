// GLC 2.0 Tutorial Example
//
// Implementation of GameObejct Class.
//
// Copyright(C) GPH(Gamepark Holdings). All rights reserved.
// Dogma-G Game Studios In-House Library DGE
//
////////////////////////////////////////////////////////////////////////////////


#include <stdio.h>

#include <LcType.h>
#include <LcBase.h>
#include <LcMath.h>


#include "GameObject.h"


// ȭ�� ���
void GameObject::Draw(PLC_SPRITE pSpt, PLC_TEXTURE* pTex)
{
	// ���� ���� Ȯ��
	if(LC_TRUE != isAlive)
		return;


	// ����ִ� ���
	PLC_TEXTURE	tex = pTex[texture];
	FLOAT			fRot= FLOAT(rotation + LCX_PI);
	INT				ImgW= tex->GetImgW();
	INT				ImgH= tex->GetImgH();


	LCXVECTOR2		vTrn= position;//(100, 100);// = ;
	LCXVECTOR2		vRot = vTrn + LCXVECTOR2(ImgW * 0.5F, ImgH * 0.5F);

	pSpt->DrawEx(tex, NULL, &vTrn, NULL, &vRot, fRot);
}

