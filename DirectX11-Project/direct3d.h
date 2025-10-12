#pragma once

/*=====
LINKING
=====*/
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

/*======
INCLUDES
======*/
#include <d3d11.h>

class Direct3D
{
public:
	Direct3D();
	Direct3D(const Direct3D&);
	~Direct3D();

	bool Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);
	bool Resize(int screenWidth, int screenHeight);
	void Shutdown();

	void Clear(float red, float green, float blue, float alpha);
	void Render();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	float GetViewWidth();
	float GetViewHeight();

	void GetVideoCardInfo(char* cardName, int& memory);

	void SetBackBufferRenderTarget();
	void ResetViewport();

private:
    bool m_vsync_enabled;
    int m_videoCardMemory;
    char m_videoCardDescription[128];
    IDXGISwapChain* m_swapChain;
    ID3D11Device* m_device;
    ID3D11DeviceContext* m_deviceContext;
    ID3D11RenderTargetView* m_renderTargetView;
    ID3D11Texture2D* m_depthStencilBuffer;
    ID3D11DepthStencilState* m_depthStencilState;
    ID3D11DepthStencilView* m_depthStencilView;
    ID3D11RasterizerState* m_rasterState;
    D3D11_VIEWPORT m_viewport;
};