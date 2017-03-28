#include "DXUT.h"
#include "DXUTgui.h"
#include "SDKmisc.h"
#include "DXUTEventHandler.h"

class UIFontExample : public DXUTEventHandler {
	
	ID3DXFont* font;
	HRESULT CALLBACK OnD3D9CreateDevice(IDirect3DDevice9* pd3dDevice,
		const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext) override {

		HRESULT hr;

		V_RETURN(D3DXCreateFont(pd3dDevice, 24, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
			L"Arial", &font));	
	}

	void CALLBACK OnD3D9FrameRender(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext) override {
		
		CDXUTTextHelper txtHelper(font, NULL, 24);
		HRESULT hr;

		// Clear the render target and the zbuffer 
		V(pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 45, 50, 170), 1.0f, 0));

		// Render the scene
		if (SUCCEEDED(pd3dDevice->BeginScene()))
		{
			// Output statistics
			txtHelper.Begin();
			txtHelper.SetInsertionPos(2, 0);

			txtHelper.SetForegroundColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));			
			txtHelper.DrawTextLine(L"Hello, world!");

			txtHelper.SetForegroundColor(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
			txtHelper.DrawTextLine(L"Hello, world!");

			txtHelper.SetForegroundColor(D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
			txtHelper.DrawTextLine(L"Hello, world!");
			txtHelper.End();

			V(pd3dDevice->EndScene());
		}

		
	}

	void CALLBACK OnD3D9DestroyDevice(void * pUserContext)
	{
		SAFE_RELEASE(font);		
	}

};

//auto p=new UIFontExample();