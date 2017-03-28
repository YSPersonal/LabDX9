#include "DXUT.h"
#include "DXUTEventHandler.h"
#include "SDKmisc.h"
#include "DXUTcamera.h"
//--------------------------------------------------------------------------------------
// This function loads the mesh and ensures the mesh has normals; it also optimizes the 
// mesh for the graphics card's vertex cache, which improves performance by organizing 
// the internal triangle list for less cache misses.
//--------------------------------------------------------------------------------------
HRESULT LoadMesh(IDirect3DDevice9* pd3dDevice, WCHAR* strFileName, ID3DXMesh** ppMesh)
{
	ID3DXMesh* pMesh = NULL;
	
	WCHAR str[MAX_PATH];
	HRESULT hr;

	// Load the mesh with D3DX and get back a ID3DXMesh*.  For this
	// sample we'll ignore the X file's embedded materials since we know 
	// exactly the model we're loading.  See the mesh samples such as
	// "OptimizedMesh" for a more generic mesh loading example.
	V_RETURN(DXUTFindDXSDKMediaFileCch(str, MAX_PATH, strFileName));
	V_RETURN(D3DXLoadMeshFromX(str, D3DXMESH_MANAGED, pd3dDevice, NULL, NULL, NULL, NULL, &pMesh));

	DWORD* rgdwAdjacency = NULL;

	// Make sure there are normals which are required for lighting
	if (!(pMesh->GetFVF() & D3DFVF_NORMAL))
	{
		ID3DXMesh* pTempMesh;
		V(pMesh->CloneMeshFVF(pMesh->GetOptions(),
			pMesh->GetFVF() | D3DFVF_NORMAL,
			pd3dDevice, &pTempMesh));
		V(D3DXComputeNormals(pTempMesh, NULL));

		SAFE_RELEASE(pMesh);
		pMesh = pTempMesh;
	}

	// Optimize the mesh for this graphics card's vertex cache 
	// so when rendering the mesh's triangle list the vertices will 
	// cache hit more often so it won't have to re-execute the vertex shader 
	// on those vertices so it will improve perf.     
	rgdwAdjacency = new DWORD[pMesh->GetNumFaces() * 3];
	if (rgdwAdjacency == NULL)
		return E_OUTOFMEMORY;
	V(pMesh->GenerateAdjacency(1e-6f, rgdwAdjacency));
	V(pMesh->OptimizeInplace(D3DXMESHOPT_VERTEXCACHE, rgdwAdjacency, NULL, NULL, NULL));
	delete[]rgdwAdjacency;

	*ppMesh = pMesh;

	return S_OK;
}


class XExample : public CDXUTEventHandler {
	
	ID3DXMesh* mesh;
	IDirect3DTexture9* texture;
	CModelViewerCamera camera;

	HRESULT CALLBACK OnD3D9CreateDevice(IDirect3DDevice9* pd3dDevice,
		const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext) override {

		HRESULT hr;

		V_RETURN(LoadMesh(pd3dDevice, L"tiny\\tiny.x", &mesh));

		WCHAR str[MAX_PATH];
		V_RETURN(DXUTFindDXSDKMediaFileCch(str, MAX_PATH, L"tiny\\tiny_skin.dds"));

		V_RETURN(D3DXCreateTextureFromFileEx(pd3dDevice, str, D3DX_DEFAULT, D3DX_DEFAULT,
			D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
			D3DX_DEFAULT, D3DX_DEFAULT, 0,
			NULL, NULL, &texture));
		
		D3DXVECTOR3 vecEye(1000.0f, 0.0f, 500.0f );
		D3DXVECTOR3 vecAt(0.0f, 0.0f, -0.0f);
		camera.SetViewParams(&vecEye, &vecAt);

		return S_OK;
	}

	void CALLBACK OnD3D9FrameRender(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime,
		void* pUserContext) override {

		D3DXMATRIX matWorld = *camera.GetWorldMatrix();
		D3DXMatrixRotationX(&matWorld, -D3DX_PI/2);

		pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
		pd3dDevice->SetTransform(D3DTS_VIEW, camera.GetViewMatrix());
		pd3dDevice->SetTransform(D3DTS_PROJECTION, camera.GetProjMatrix());

		pd3dDevice->SetTexture(0, texture);
		pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		HRESULT hr;

		// Clear the render target and the zbuffer 
		V(pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DXCOLOR(0.0f, 0.25f, 0.25f, 0.55f), 1.0f,
			0));

		// Render the scene
		if (SUCCEEDED(pd3dDevice->BeginScene()))
		{
			V(mesh->DrawSubset(0));
			V(pd3dDevice->EndScene());
		}
	}

	void CALLBACK OnD3D9DestroyDevice(void* pUserContext) override
	{		
		SAFE_RELEASE(mesh);		
		SAFE_RELEASE(texture);
	}

	HRESULT CALLBACK OnD3D9ResetDevice(IDirect3DDevice9* pd3dDevice,
		const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext) override
	{
		HRESULT hr;

		// Setup the camera's projection parameters
		float fAspectRatio = pBackBufferSurfaceDesc->Width / (FLOAT)pBackBufferSurfaceDesc->Height;
		camera.SetProjParams(D3DX_PI / 4, fAspectRatio, 2.0f, 4000.0f);
		camera.SetWindow(pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height);
		camera.SetButtonMasks(MOUSE_LEFT_BUTTON, MOUSE_WHEEL, MOUSE_MIDDLE_BUTTON);

		return S_OK;
	}

	void CALLBACK OnFrameMove(double fTime, float fElapsedTime, void* pUserContext) override
	{
		// Update the camera's position based on user input 
		camera.FrameMove(fElapsedTime);
	}

	virtual LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
		bool* pbNoFurtherProcessing, void* pUserContext) override {
		camera.HandleMessages(hWnd, uMsg, wParam, lParam);

		return 0;
	}
};

auto p = new XExample();