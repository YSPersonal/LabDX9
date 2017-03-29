#pragma once

#include "DXUTEventHandler.h"

class GroundHandler:
	public DXUTEventHandler
{
	int halfWidth = 5;
	int halfHeight = 5;

	IDirect3DVertexBuffer9* pVB;
	
	ID3DXMesh* objMesh;
	static void CreateGridBuffer(int halfWidth, int halfHeight, IDirect3DDevice9 * pd3dDevice, 
		IDirect3DVertexBuffer9** out);

public:
	HRESULT CALLBACK OnD3D9CreateDevice(IDirect3DDevice9* pd3dDevice,
		const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext) override;

	void CALLBACK OnD3D9FrameRender(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime,
		void* pUserContext) override;

	void CALLBACK OnD3D9DestroyDevice(void* pUserContext) override;

	

	GroundHandler();
	~GroundHandler();
};

