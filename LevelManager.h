#pragma once
#include <unordered_map>
#include <string>
#include <functional>
#include <experimental/coroutine>
#include <deque>
#include <list>
#include <DirectXMath.h>
#include <thread>
#include <mutex>
#include <algorithm>

#include "Player.h"

struct Level {
	std::function<void()> OnStart = []() {};
	std::function<void()> OnLoop = []() {};
	std::function<void()> OnEnter = []() {};
	std::function<void()> OnExit = []() {};
	std::mutex mtx = {};
	bool closing = false;
	bool ready_to_close = true;
};

struct Bullet {
	DirectX::XMFLOAT2 pos = {};
	DirectX::XMFLOAT3 velocity = {};
	float radius = 0.f;
	float angle = 0.f;
	float damage = 0.f;
	bool is_mine = true;
	std::string sprite_name = "";
	Bullet() {}
};

struct Item {
	DirectX::XMFLOAT2 pos = {};
	DirectX::XMFLOAT3 velocity = {};
	float radius = 0.f;
	std::string sprite_name = "";
	enum ItemCode {
		Heal, Levelup, Skill
	};
	ItemCode code = ItemCode::Heal;
};

struct Enemy {
	DirectX::XMFLOAT2 pos = {};
	DirectX::XMFLOAT3 velocity = {};
	float radius = 0.f;
	float hp = 0.f;
	float last_hit = -1.f;
	int reward_exp = 1;
	std::string sprite_name = "";
	bool remove = false;
	DirectX::XMINT4 diffuse = {};
	Enemy() {}
};

struct Effect {
	DirectX::XMFLOAT2 pos = {};
	DirectX::XMFLOAT3 velocity = {};
	float radius = 0.f;
	float angle = 0.f;
	DirectX::XMINT4 diffuse = {};

	bool is_frame = false;
	int frame = 0;
	int max_frame = 0;
	float frame_duration = 0.f;
	float frame_max_duration = 0.f;
	std::string sprite_name = "";

	Effect() {}
};


struct LvStartMenu : Level {
	float scroll_x = 0.f;
	LvStartMenu();
};

struct LvTutorial : Level {
	float scroll_x = 0.f;
	LvTutorial();
};

struct LvRanking : Level {
	std::list<std::pair<std::string, int>> scoreboard;
	LvRanking();
};

struct LvGameOver : Level {
	std::wstring gameover_text = L"±¦Âú´Ù. ´Ù½Ã µµÀüÇÏ¸é µÅ";
	std::wstring gameover_curr_text = L"";
	std::list<DirectX::XMFLOAT4> pieces = {};
	LvGameOver();
};

struct LevelStage : Level {
	bool is_pause = false;
	bool game_over = true;

	std::string scroll_name = "bg_tutorial";
	float scroll_x = 0.f;

	std::list<Bullet> bullets;
	std::list<Enemy> enimies;
	std::list<Effect> effects;
	std::list<Item> items;

	float fade_inout = 0.f;

	std::wstring log_text = L"";
	std::wstring log_text_buffer = L"";
	float logging_lasttime = 0.f;
	float logging_delay = 0.3f;
	float log_closing_delay = 0.3f;
	bool log_suppose_to = false;
	bool log_is = false;
	float log_progress = 0.f;

	Player& player = Player::get_instance();
	float gameover_fade = 0.f;

	bool boss_bar = false;
	int boss_bar_hp = 0;
	int boss_bar_last_hp = 0;

	void Clear();
	void Draw();
	void Loop();

	bool is_not_end();
	void show_text(std::wstring start_text, std::wstring text, float delay = 0.3f, float closing_delay = 2.f);
	void wrapped_lock();
};

struct LvStage1_1 : LevelStage {
	LvStage1_1();
};
void trd_LvStage1_1(LvStage1_1*);

struct LvStage1_2 : LevelStage {
	LvStage1_2();
};
void trd_LvStage1_2(LvStage1_2*);

struct LvStage1_3 : LevelStage {
	LvStage1_3();
};
void trd_LvStage1_3(LvStage1_3*);

struct LvStage2_1 : LevelStage {
	LvStage2_1();
};
void trd_LvStage2_1(LvStage2_1*);

struct LvStage2_2 : LevelStage {
	LvStage2_2();
};
void trd_LvStage2_2(LvStage2_2*);

struct LvStage2_3 : LevelStage {
	bool start_battle = false;
	LvStage2_3();
};
void trd_LvStage2_3(LvStage2_3*);

template<class T, class _Fn>
void remove_loop(std::list<T>& list, _Fn func)
{
	auto itr = list.begin();
	while (itr != list.end())
	{
		auto& obj = *itr;
		if (func(obj)) list.erase(itr++);
		else ++itr;
	}
}
