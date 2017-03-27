#pragma once
#include"DXUT.h"
#include<memory>
class CUserdata
{
public:
	ID3DXFont* font;
	ID3DXSprite* sprite;

	CUserdata();
	~CUserdata();
};

typedef std::shared_ptr<CUserdata> Userdata;