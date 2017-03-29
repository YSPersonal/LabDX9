#pragma once
#include "DXUTEventHandler.h"
class TestGeometryHandler :
	public DXUTEventHandler
{
	ID3DXMesh* mesh;

	HRESULT CALLBACK OnD3D9CreateDevice(IDirect3DDevice9* pd3dDevice,
		const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext) override;

	void CALLBACK OnD3D9FrameRender(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext) override;

	void CALLBACK OnD3D9DestroyDevice(void* pUserContext) override;

	HRESULT CALLBACK OnD3D9ResetDevice(IDirect3DDevice9* pd3dDevice,
		const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext) override;


public:
	TestGeometryHandler();
	~TestGeometryHandler();
};

