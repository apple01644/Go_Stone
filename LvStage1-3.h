
LvStage1_3::LvStage1_3() {
	static thread trd;
	static float text_delay = 0;
	this->OnStart = [&]() {
		scroll_name = "bg_ice";
		mng->BindSprite("iceman", "sprites/ozocubus_armour.png");
		mng->BindSprite("wall", "sprites/crystal_wall_0.png");
		mng->BindSprite("ice", "sprites/ozocubus_refrigeration.png");
	};

	this->OnEnter = [&]() {
		Clear();

		Enemy boss = {};
		boss.pos = XMFLOAT2{ mng->client_w - 64.f, mng->client_h / 2.f };
		boss.radius = 64.f;
		boss.hp = 30000.f;
		boss.sprite_name = "iceman";
		boss_bar = true;
		boss.reward_exp = 300;
		enimies.push_back(boss);
		trd = thread(trd_LvStage1_3, this);
	};

	this->OnLoop = [&]() {
		Loop();
		Draw();

		if (boss_bar && (!is_pause))
		{
			size_t boss_size = 0;
			for (const Enemy& enemy : enimies)
				if (enemy.sprite_name == "iceman")
					++boss_size;
			if (boss_size == 0)
				boss_bar = false;
			

			if (boss_bar)
			{
				boss_bar_hp = (int)(roundf(enimies.begin()->hp / 30000.f * 100));
				boss_bar_last_hp = -((boss_bar_last_hp - boss_bar_hp) * mng->deltatime);
			}
		}
	};
	this->OnExit = [&]() {
		trd.join();
	};
}

void trd_LvStage1_3(LvStage1_3* self)
{
	static Player& player = Player::get_instance();
	self->show_text(L"", L"일어나보니 밤이 되어있었다.\n 아주 싸늘하다.", 0.1f, 1.f);
	self->show_text(L"???: ", L"죽어라!!! 라미아의 원수!!!", 0.1f, 1.f);
	self->show_text(L"화강암: ", L"잠깐! 그건 오해야!", 0.1f, 1.f);
	Enemy wall = {};
	wall.radius = mng->client_h / 32;
	wall.hp = 300000.f;
	wall.velocity = { -wall.radius * 20, 0.f ,0.f };
	wall.sprite_name = "wall";
	Bullet bullet = {};
	bullet.radius = 14.f;
	bullet.sprite_name = "ice";
	bullet.damage = 60;
	bullet.is_mine = false;
	while (self->is_not_end() && self->boss_bar)
	{
		self->wrapped_lock();
		wall.pos = XMFLOAT2{ mng->client_w * 1.f, mng->client_h / 2.f + mng->client_h / 2 / 2.5f * (1.5f + sinf(mng->totaltime * 2)) };
		self->enimies.push_back(wall);
		wall.pos = XMFLOAT2{ mng->client_w * 1.f, mng->client_h / 2 / 2.5f * (1.5f + sinf(mng->totaltime * 2)) };
		self->enimies.push_back(wall);

		if (rand() % 2 == 0)		
			if (rand() % 2 == 0)
				bullet.pos = { mng->client_w * 1.f, rand() % 101 * 0.01f * mng->client_h };
			else
				bullet.pos = { 0.f, rand() % 101 * 0.01f * mng->client_h };		
		else		
			if (rand() % 2 == 0)
				bullet.pos = { rand() % 101 * 0.01f * mng->client_w, mng->client_h * 1.f };
			else
				bullet.pos = { rand() % 101 * 0.01f * mng->client_w, 0.f };
		
		if (rand() % 10 == 0)
		{
			bullet.velocity = { player.pos.x - bullet.pos.x,  player.pos.y - bullet.pos.y, 2.f };
			self->bullets.push_back(bullet);
		}
		self->mtx.unlock();
		this_thread::sleep_for(0.1s);
	}
	if (!self->boss_bar)
	{
		player.score += roundf(player.hp / player.max_hp * 100);
		self->show_text(L"", L"스테이지 1 -완-", 0.1f, 1.f);
		mng->next_level_name = "stage2-1";
	}
}