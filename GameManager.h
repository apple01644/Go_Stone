#include <Windows.h>
#include <DirectXMath.h>
#include <d3d10.h>
#include <D3DX10.h>
#include <wrl/client.h>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;
using namespace DirectX;
using Microsoft::WRL::ComPtr;

extern struct Level;

struct GameManager {
	ComPtr<ID3D10Device> device;
	ComPtr<IDXGISwapChain> swapchain;

	ComPtr<ID3D10RenderTargetView> rtv;
	ComPtr<ID3D10RasterizerState> rts;
	ComPtr<ID3D10BlendState> blend;

	unordered_map<string, ComPtr<ID3D10ShaderResourceView>> sprite_map;
	unordered_map<string, ComPtr<ID3DX10Font>> font_map;
	unordered_map<string, Level*> level_map;
	Level* current_level = nullptr;
	string current_level_name = "nullptr";
	string next_level_name = "nullptr";

	ComPtr<ID3D10VertexShader> sprite_vs;
	ComPtr<ID3D10PixelShader> sprite_ps;

	ComPtr<ID3D10VertexShader> poly_vs;
	ComPtr<ID3D10PixelShader> poly_ps;
	ComPtr<ID3D10InputLayout> common_il;

	std::vector<XMFLOAT3> rect_vertices = {
			{0.f, 1.f, 0.f}, {0.f, 0.f, 0.f}, {1.f, 0.f, 0.f},
			{1.f, 1.f, 0.f}, {0.f, 1.f, 0.f}, {1.f, 0.f, 0.f}
	};
	ComPtr<ID3D10Buffer> rect_vertices_buffer;
	UINT rect_stride = sizeof(XMFLOAT3);
	UINT rect_offset = 0;

	ComPtr<ID3D10Buffer> object_buffer;

	int client_w = 0;
	int client_h = 0;
	float totaltime = 0.f;
	float deltatime = 0.f;
	
	GameManager(HWND hWnd, int client_w, int client_h);		
	~GameManager();
	void Awake();
	void Loop();
	void Peek(const MSG&);

	void BindSprite(string name, string filename);


	void draw_rect(XMFLOAT2 pos, XMFLOAT2 size, XMINT4 color);
	void draw_sprite(string name, XMFLOAT2 pos, XMFLOAT2 size, XMINT4 diffuse = { 0, 0, 0, 0 }, bool mirrored = false);
	void draw_sprite_angle(string name, XMFLOAT2 pos, XMFLOAT2 size, float angle, XMINT4 diffuse = { 0, 0, 0, 0 }, bool mirrored = false);
	void draw_text(string name, string content, XMFLOAT2 pos, XMINT4 color = { 0, 0, 0, 255 });
	void draw_text(string name, wstring content, XMFLOAT2 pos, XMINT4 color = { 0, 0, 0, 255 });

	bool key_states[256] = {};
private:
	void GotoLevel(string lv_name);
};

struct ExceptionHandler {

};
ExceptionHandler& operator << (ExceptionHandler& eh, HRESULT hr);

struct RectangleBuffer {
	XMFLOAT4X4 transform;
	XMFLOAT4 color;
};

static ExceptionHandler ex = {};
static GameManager* mng = nullptr;


float operator- (DirectX::XMFLOAT2& lhs, DirectX::XMFLOAT2& rhs);