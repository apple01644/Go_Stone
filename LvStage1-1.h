
LvStage1_1::LvStage1_1() {
	static thread trd;
	static float text_delay = 0;
	this->OnStart = [&]() {
		mng->BindSprite("spider", "sprites/redback_new.png");
	};

	this->OnEnter = [&]() {
		Clear();
		Enemy enemy = {};
		enemy.pos = XMFLOAT2{mng->client_w - 64.f, mng->client_h / 2 - 32.f};
		enemy.radius = 32.f;
		enemy.hp = 180.f;
		enemy.sprite_name = "green_mob";
		enemy.velocity.x = -64;

		trd = thread(trd_LvStage1_1, this);
	};

	this->OnLoop = [&]() {
		Loop();		
		Draw();
	};
	this->OnExit = [&]() {
		trd.join();
	};
}

void trd_LvStage1_1(LvStage1_1* self)
{
	static Player& player = Player::get_instance();

	Enemy green_mob = {};
	green_mob.pos = { mng->client_w + 64.f, mng->client_h / 2 - 32.f + (rand() % 3 - 1) * 100.f };
	green_mob.radius = 32.f;
	green_mob.hp = 180.f;
	green_mob.velocity.x = -64;
	green_mob.sprite_name = "green_mob";

	Enemy spider = {};
	spider.radius = 16.f;
	spider.hp = 90.f;
	spider.sprite_name = "spider";
	spider.velocity.y = 80;

	for (int k = 0; k < 10 && self->is_not_end(); ++k)
	{
		green_mob.pos = { mng->client_w + 64.f, mng->client_h / 2 - 32.f + (rand() % 3 - 1) * 100.f };
		self->wrapped_lock();
		self->enimies.push_back(green_mob);
		self->mtx.unlock();
		this_thread::sleep_for(1s);
	}
	this_thread::sleep_for(6.s);
	if (self->is_not_end())
		self->show_text(L"농부: ", L"용기사 군단이 온다!\n모두 도망쳐!!!", 0.1f, 1.f);
	for (int k = 0; k < 7 && self->is_not_end(); ++k)
	{
		self->wrapped_lock();
		for (int y = 0; y < 5; ++y)
		{
			green_mob.pos = { mng->client_w + 64.f, mng->client_h / 2 - 32.f + (y - 2) * 64.f };
			self->enimies.push_back(green_mob);
		}
		self->mtx.unlock();
		this_thread::sleep_for(1s);
	}
	this_thread::sleep_for(6.s);
	if (self->is_not_end())
		self->show_text(L"???: ", L"치이익... 치이익...", 0.1f, 1.f);
	
	for (int k = 0; k < 56 && self->is_not_end(); ++k)
	{
		self->wrapped_lock();
		int bx = rand() % 16;
		for (int x = 0; x < 24; ++x)
		{
			if (rand() % 5 < 3)
			{
				spider.pos = { bx + mng->client_w * x / 24.f + 16, - 32.f - rand() % 101 * 0.01f * 64.f };
				self->enimies.push_back(spider);
			}
		}
		self->mtx.unlock();
		if (k < 3) this_thread::sleep_for(0.5s);
		if (k < 7) this_thread::sleep_for(0.5s);
		if (k < 10) this_thread::sleep_for(0.5s);
		this_thread::sleep_for(0.15s);
	}
	if (self->is_not_end())
		this_thread::sleep_for(1s);
	if (self->is_not_end())
		self->show_text(L"", L"엄청난 거미 무리에 그만 정신을\n잃어버리고 말았다...", 0.1f, 2.f);
	if (self->is_not_end())
		mng->next_level_name = "stage1-2";
	return;
}