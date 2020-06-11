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

	self->show_text(L"ȭ����: ", L"��...\nȲ�ݸ���ũ?", 0.1f, 1.f);
	self->show_text(L"��縶��ũ:\n", L"������� ���ٴ�\n������ �༮�� �����ᱺ\nȲ�ݸ���ũ�� �� �����̴�.", 0.1f, 1.f);
	self->show_text(L"ȭ����: ", L"�ʰ� �� ���踦 Ȳ���ϰ�\n���� ���ΰ�?", 0.1f, 1.f);
	self->show_text(L"��縶��ũ:\n", L"�׷���\n�� �� ��ġ��� ���ϰ���?", 0.1f, 1.f);
	self->show_text(L"ȭ����: ", L"�׷���.", 0.1f, 1.f);
	self->show_text(L"��縶��ũ:\n", L"�� �� ��ġ����� ����?", 0.1f, 1.f);
	self->show_text(L"ȭ����: ", L"�ʰ� ������ Ȳ���ϰ� �����\n �����̴�..", 0.1f, 1.f);
	self->show_text(L"��縶��ũ:\n", L"Ȳ���Ե� ���� ������ ��ȹ�� �Ϻδ�.\nȫ���� �־��⿡\n����� ���ְ� �־�����", 0.1f, 1.f);
	self->show_text(L"��縶��ũ:\n", L"������ Ȳ���ϰ� ����\n���踦 ������ ���ͷ� ���� ��\n�ٽ� ������ �ɴ°Ŵ�.", 0.1f, 1.f);
	self->show_text(L"ȭ����: ", L"�� �׷��°���?\n������ ���赵 ������ �ʾҴµ�.\n���� ��ȹ ������ ���� �������\n���� �ް� �ִ�.", 0.1f, 1.f);
	self->show_text(L"��縶��ũ:\n", L"�׷��� �������.\n�� ���� ���踦 ���ؼ�\n������ ����� �Ұ��� �Ѱ�", 0.1f, 1.f);
	self->show_text(L"ȭ����: ", L"���̻� ���ټ� ���ڱ�", 0.1f, 1.f);
	self->show_text(L"��縶��ũ:\n", L"������! �ּ���", 0.1f, 1.f);
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
		self->show_text(L"��縶��ũ:\n", L"���� ����� �̷���\n������� ���±���", 0.1f, 1.f);
		self->show_text(L"��縶��ũ:\n", L"�ż���� �ʰ� ����Ʈ�ȴ�\n������ ����̰� �ʸ�\n������ �� �̴�!", 0.1f, 1.f);
		self->show_text(L"��縶��ũ:\n", L"ũ�ƾƾƾ�!!!!", 0.1f, 1.f);
		self->show_text(L"", L"�������� 2  -��-", 0.1f, 1.f);
		player.score += roundf(player.hp / player.max_hp * 100);
		mng->next_level_name = "ranking";
	}
}