#include "DXUT.h"
#include "TestGeometryHandler.h"


HRESULT CALLBACK TestGeometryHandler::OnD3D9CreateDevice(IDirect3DDevice9* pd3dDevice,
	const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext) {

	HRESULT hr;

	D3DXCreateTeapot(pd3dDevice, &mesh, NULL);	
	return S_OK;
}



void CALLBACK TestGeometryHandler::OnD3D9FrameRender(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext) {

	HRESULT hr;
	// Clear the render target and the zbuffer 
	V(pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 45, 50, 170), 1.0f, 0));

	pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// Render the scene
	if (SUCCEEDED(pd3dDevice->BeginScene()))
	{		
		mesh->DrawSubset(0);

		V(pd3dDevice->EndScene());
	}
}

void CALLBACK TestGeometryHandler::OnD3D9DestroyDevice(void* pUserContext)
{
	SAFE_RELEASE(mesh);
}


TestGeometryHandler::TestGeometryHandler()
{
}


TestGeometryHandler::~TestGeometryHandler()
{
}

//#include "DXUTApplication.h"
//#include "SurroundCameraHandler.h"
//
//int main() {
//	
//	DXUTApplication::Instance()->AddHandler(new SurroundCameraHandler);
//	DXUTApplication::Instance()->AddHandler(new TestGeometryHandler);
//	DXUTApplication::Run();
//
//	return 0;
//}