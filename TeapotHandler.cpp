#include "DXUT.h"
#include "DXUTEventHandler.h"
#include <d3dx9.h>
class TeapotHandler : public DXUTEventHandler {

	ID3DXMesh* mesh;

	HRESULT CALLBACK OnD3D9CreateDevice(IDirect3DDevice9* pd3dDevice,
		const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext) override {

		HRESULT hr;

		D3DXCreateTeapot(pd3dDevice, &mesh, NULL);
		pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		return S_OK;
	}



	void CALLBACK OnD3D9FrameRender(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext) override {

		HRESULT hr;
		// Clear the render target and the zbuffer 
		V(pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 45, 50, 170), 1.0f, 0));

		// Render the scene
		if (SUCCEEDED(pd3dDevice->BeginScene()))
		{

			D3DXMATRIX matView;
			auto vecEye = D3DXVECTOR3(3, 2, 3);
			auto vecLook = D3DXVECTOR3(0, 0, 0);
			auto vecUp = D3DXVECTOR3(0, 1, 0);
			D3DXMatrixLookAtLH(&matView, &vecEye, &vecLook, &vecUp);

			pd3dDevice->SetTransform(D3DTS_VIEW, &matView);
			mesh->DrawSubset(0);

			V(pd3dDevice->EndScene());
		}
	}

	void CALLBACK OnD3D9DestroyDevice(void* pUserContext) override
	{
		SAFE_RELEASE(mesh);
		
	}


	HRESULT CALLBACK OnD3D9ResetDevice(IDirect3DDevice9* pd3dDevice,
		const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext) override
	{
		HRESULT hr;

		// Setup the camera's projection parameters
		float fAspectRatio = pBackBufferSurfaceDesc->Width / (FLOAT)pBackBufferSurfaceDesc->Height;
		/*camera.SetProjParams(D3DX_PI / 4, fAspectRatio, 2.0f, 4000.0f);
		camera.SetWindow(pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height);
		camera.SetButtonMasks(MOUSE_LEFT_BUTTON, MOUSE_WHEEL, MOUSE_MIDDLE_BUTTON);*/

		D3DXMATRIX matProj;
		D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 3, fAspectRatio, 0.1, 100);
		pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);

		return S_OK;
	}
};

auto p = new TeapotHandler();