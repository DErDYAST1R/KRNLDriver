#include "../Includes.h"
#include <dwmapi.h>

class Overlay
{
private:

	HWND window_handle;
	ID3D11Device* d3d_device;
	ID3D11DeviceContext* device_context;
	IDXGISwapChain* swap_chain;
	ID3D11RenderTargetView* render_target_view;
	ImDrawList* draw_list;

	int	game_screen_width, game_screen_height;

public:

	bool InitDevice();
	void DestroyDevice();

	void InitImGui();
	void DestroyImGui();
	void InitRenderTarget();
	void DestroyRenderTarget();

	void DestroyWindow();

	// const void Render();

	bool MessageLoop(void (*drawLoop)());

	Overlay(HWND target_window);
	~Overlay();
};