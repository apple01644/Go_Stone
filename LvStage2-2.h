LvStage2_2::LvStage2_2() {
	static thread trd;
	static float text_delay = 0;
	this->OnStart = [&]() {
		scroll_name = "bg_cave";
		mng->BindSprite("crystal", "sprites/crystal_wall13.png");
	};

	this->OnEnter = [&]() {
		Clear();
		trd = thread(trd_LvStage2_2, this);
	};

	this->OnLoop = [&]() {
		Loop();
		Draw();

	};
	this->OnExit = [&]() {
		trd.join();
	};
}

void trd_LvStage2_2(LvStage2_2* self)
{
	static Player& player = Player::get_instance();

	Bullet wall_shiled = {};
	wall_shiled.radius = mng->client_h / 30.f;
	wall_shiled.damage = 180;
	wall_shiled.sprite_name = "crystal";
	wall_shiled.is_mine = false;
	wall_shiled.velocity = {
		-130,
		2.f,
		0.f
	};

	Enemy wall = {};
	wall.radius = wall_shiled.radius;
	wall.hp = 1;
	wall.sprite_name = "crystal";
	wall.velocity = {
		-130,
		2.f,
		0.f
	};
	wall.diffuse = { -128, -128, -128, -128 };

	for (int count = 0; count < 30 && self->is_not_end(); ++count)
	{
		wall.velocity.x -= 30;
		wall_shiled.velocity.x -= 30;
		self->wrapped_lock();
		int certain = 1 + rand() % 11;
		for (int k = 0; k < 15; ++k)
		{
			wall.pos = XMFLOAT2{ mng->client_w - wall.radius,  wall.radius + (mng->client_h - wall.radius) * k / 14.f };
			wall_shiled.pos = XMFLOAT2{ mng->client_w - wall.radius - 0.1f,  wall.radius + (mng->client_h - wall.radius) * k / 14.f };
			if (k >= certain - 1 && k <= certain + 1)
			{
				wall.hp = 1;
				self->enimies.push_back(wall);
			}
			else
			{
				wall.hp = 10000;
				self->enimies.push_back(wall);
				self->bullets.push_back(wall_shiled);
			}
		}
		self->mtx.unlock();
		this_thread::sleep_for(std::chrono::duration<float>(- mng->client_w / wall_shiled.velocity.x));
	}
	if (!self->game_over)
		mng->next_level_name = "stage2-3";
}