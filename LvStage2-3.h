LvStage2_3::LvStage2_3() {
	static thread trd;
	static float text_delay = 0;
	this->OnStart = [&]() {
		scroll_name = "bg_cave";
		mng->BindSprite("golden_rage", "sprites/berserker_rage.png");

		mng->BindSprite("rune0", "sprites/rune_cerebov.png");
		mng->BindSprite("rune1", "sprites/rune_cocytus.png");
		mng->BindSprite("rune2", "sprites/rune_gloorx_vloq_old.png");
		mng->BindSprite("rune3", "sprites/rune_lom_lobon.png");
		mng->BindSprite("rune4", "sprites/rune_mnoleg_old.png");
	};

	this->OnEnter = [&]() {
		Clear();

		Enemy boss = {};
		boss.pos = XMFLOAT2{ mng->client_w - 64.f, mng->client_h / 2.f };
		boss.radius = 48.f;
		boss.hp = 90000.f;
		boss.sprite_name = "golden_mask";
		boss_bar = true;
		enimies.push_back(boss);
		start_battle = false;
		trd = thread(trd_LvStage2_3, this);
	};

	this->OnLoop = [&]() {
		Loop();
		Draw();

		if (boss_bar && (!is_pause) && start_battle)
		{
			size_t boss_size = 0;
			for (const Enemy& enemy : enimies)
				if (enemy.sprite_name == "golden_rage")
					++boss_size;
			if (boss_size == 0)
				boss_bar = false;


			if (boss_bar)
			{
				boss_bar_hp = (int)(roundf(enimies.begin()->hp / 90000.f * 100));
				boss_bar_last_hp = -((boss_bar_last_hp - boss_bar_hp) * mng->deltatime);
			}
		}
	};
	this->OnExit = [&]() {
		trd.join();
	};
}

void trd_LvStage2_3(LvStage2_3* self)
{
	static Player& player = Player::get_instance();

	self->show_text(L"화강암: ", L"넌...\n황금마스크?", 0.1f, 1.f);
	self->show_text(L"골든마스크:\n", L"여기까지 오다니\n마법사 녀석이 보내줬군\n황금마스크는 내 가명이다.", 0.1f, 1.f);
	self->show_text(L"화강암: ", L"너가 이 세계를 황폐하게\n만든 놈인가?", 0.1f, 1.f);
	self->show_text(L"골든마스크:\n", L"그렇다\n넌 날 해치우기 원하겠지?", 0.1f, 1.f);
	self->show_text(L"화강암: ", L"그렇다.", 0.1f, 1.f);
	self->show_text(L"골든마스크:\n", L"왜 날 해치우려는 거지?", 0.1f, 1.f);
	self->show_text(L"화강암: ", L"너가 세상을 황폐하게 만들기\n 때문이다..", 0.1f, 1.f);
	self->show_text(L"골든마스크:\n", L"황폐함도 나의 원대한 계획의 일부다.\n홍수가 있었기에\n노아의 방주가 있었듯이", 0.1f, 1.f);
	self->show_text(L"골든마스크:\n", L"세상을 황폐하게 만들어서\n세계를 깨끗한 공터로 만든 뒤\n다시 생명을 심는거다.", 0.1f, 1.f);
	self->show_text(L"화강암: ", L"왜 그러는거지?\n기존의 세계도 나쁘지 않았는데.\n너의 계획 때문에 많은 사람들이\n고통 받고 있다.", 0.1f, 1.f);
	self->show_text(L"골든마스크:\n", L"그런건 상관없다.\n더 나은 세계를 위해서\n그정도 희생은 불가피 한것", 0.1f, 1.f);
	self->show_text(L"화강암: ", L"더이상 봐줄수 없겠군", 0.1f, 1.f);
	self->show_text(L"골든마스크:\n", L"덤벼라! 애송이", 0.1f, 1.f);
	self->wrapped_lock();
	self->enimies.begin()->sprite_name = "golden_rage";
	self->start_battle = true;
	self->boss_bar = true;
	self->mtx.unlock();

	Bullet rune = {};
	rune.radius = 15.f;
	rune.damage = 180;
	rune.sprite_name = "rune0";
	rune.is_mine = false;

	while (self->is_not_end() && self->boss_bar)
	{
		int pk = rand() % 4;
		if (pk == 0) {
			int certain = 5 + rand() % 26;
			self->wrapped_lock();
			for (int k = 0; k < 36 && self->is_not_end(); ++k)
			{
				if (k >= certain - 5 && k <= certain + 5) continue;
				rune.sprite_name = "rune" + to_string(rand() % 5);
				rune.pos = { 320.f + 225.f * cosf(k / 35.f * XM_2PI), 240.f + 225.f * sinf(k / 35.f * XM_2PI) };
				rune.velocity = { (320.f - rune.pos.x) * 0.5f, (240.f - rune.pos.y) * 0.5f, 0.f };
				self->bullets.push_back(rune);
			}
			self->mtx.unlock();
			this_thread::sleep_for(3.s);
		}
		else if (pk == 1) {
			int certain_x = 5 + rand() % 26;
			int certain_y = 5 + rand() % 26;
			self->wrapped_lock();
			for (int x = 0; x < 36 && self->is_not_end(); ++x)
			{
				if (x >= certain_x - 5 && x <= certain_x + 5) continue;
				rune.sprite_name = "rune" + to_string(rand() % 5);
				rune.pos = { 15.f , 15.f + (mng->client_h - 30.f) * x / 35.f };
				rune.velocity = { 100.f , 0.f, 0.f };
				self->bullets.push_back(rune);
				rune.sprite_name = "rune" + to_string(rand() % 5);
				rune.pos = { mng->client_w - 15.f , 15.f + (mng->client_h - 30.f) * x / 35.f };
				rune.velocity = { -100.f , 0.f, 0.f };
				self->bullets.push_back(rune);
			}
			for (int y = 0; y < 36 && self->is_not_end(); ++y)
			{
				if (y >= certain_y - 5 && y <= certain_y + 5) continue;
				rune.sprite_name = "rune" + to_string(rand() % 5);
				rune.pos = {15.f + (mng->client_w - 30.f) * y / 35.f, 15.f };
				rune.velocity = { 0.f, 100.f ,  0.f };
				self->bullets.push_back(rune);
				rune.sprite_name = "rune" + to_string(rand() % 5);
				rune.pos = { 15.f + (mng->client_w - 30.f) * y / 35.f, mng->client_h - 15.f };
				rune.velocity = { 0.f, -100.f , 0.f };
				self->bullets.push_back(rune);
			}
			self->mtx.unlock();
			this_thread::sleep_for(6.s);
		}
		else if (pk == 2) {
			float centain_rad = 0.f;
			for (int k = 0; k < 640 && self->is_not_end(); ++k)
			{
				centain_rad += 0.008f;
				self->wrapped_lock();
				for (int s = 0; s < 4; ++s)
				{
					rune.sprite_name = "rune" + to_string(rand() % 5);
						rune.pos = { 320.f + 225.f * cosf(centain_rad * XM_2PI + 90 * s), 240.f + 225.f * sinf(centain_rad * XM_2PI + 90 * s) };
					rune.velocity = { (320.f - rune.pos.x) * 0.5f, (240.f - rune.pos.y) * 0.5f, 0.f };
					rune.velocity.x *= 20.f;
					rune.velocity.y *= 20.f;
					self->bullets.push_back(rune);
				}
				
				self->mtx.unlock();
				this_thread::sleep_for(0.03s);
			}
			this_thread::sleep_for(2.s);
		}
		else if (pk == 3) {
			for (int k = 0; k < 360 && self->is_not_end(); ++k)
			{
				self->wrapped_lock();
				rune.sprite_name = "rune" + to_string(rand() % 5);
				rune.pos = { mng->client_w - 15.f, (mng->client_h - 128.f) * 0.5f * (1 + sinf(k * 0.4f / XM_2PI)) };
				rune.velocity = { -400.f, 0.f, 0.f };
				self->bullets.push_back(rune);
				rune.pos.y += 128;
				self->bullets.push_back(rune);
				self->mtx.unlock();
				this_thread::sleep_for(0.05s);
			}
			this_thread::sleep_for(1.s);
		}

		this_thread::sleep_for(1.s);
	}

	if (!self->game_over)
	{
		self->show_text(L"골든마스크:\n", L"나의 사명이 이렇게\n산산조각 나는구나", 0.1f, 1.f);
		self->show_text(L"골든마스크:\n", L"신세계는 너가 무너트렸다\n세간의 모든이가 너를\n저주할 것 이다!", 0.1f, 1.f);
		self->show_text(L"골든마스크:\n", L"크아아아악!!!!", 0.1f, 1.f);
		self->show_text(L"", L"스테이지 2  -완-", 0.1f, 1.f);
		player.score += roundf(player.hp / player.max_hp * 100);
		mng->next_level_name = "ranking";
	}
}