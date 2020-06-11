
LvStage2_1::LvStage2_1() {
	static thread trd;
	static float text_delay = 0;
	this->OnStart = [&]() {
		scroll_name = "bg_volcano";
		mng->BindSprite("flame", "sprites/flame_0.png");
	};

	this->OnEnter = [&]() {
		Clear();

		Enemy boss = {};
		boss.pos = XMFLOAT2{ mng->client_w - 64.f, mng->client_h / 2.f };
		boss.radius = 48.f;
		boss.hp = 27000.f;
		boss.sprite_name = "francis";
		boss.reward_exp = 400;
		boss.diffuse = { 127, 0,0, 0 };
		boss_bar = true;
		enimies.push_back(boss);
		trd = thread(trd_LvStage2_1, this);
	};

	this->OnLoop = [&]() {
		Loop();
		Draw();

		if (boss_bar && (!is_pause))
		{
			size_t boss_size = 0;
			for (const Enemy& enemy : enimies)
				if (enemy.sprite_name == "francis")
					++boss_size;
			if (boss_size == 0)
				boss_bar = false;


			if (boss_bar)
			{
				boss_bar_hp = (int)(roundf(enimies.begin()->hp / 27000.f * 100));
				boss_bar_last_hp = -((boss_bar_last_hp - boss_bar_hp) * mng->deltatime);
			}
		}
	};
	this->OnExit = [&]() {
		trd.join();
	};
}

void trd_LvStage2_1(LvStage2_1* self)
{
	static Player& player = Player::get_instance();
	self->show_text(L"ȭ����: ", L"�����???", 0.1f, 1.f);
	self->show_text(L"������: ", L"�׷��� ���� â���ִ�.\n���� �ӹ��� ������\n���� �Ƚ��� ��������", 0.1f, 1.f);
	self->show_text(L"ȭ����: ", L"���� �տ� ȭ���� ������ �ִµ�\n�ӹ��� �����ٴ�\n���� �Ҹ��Դϱ�?", 0.1f, 1.f);
	self->show_text(L"������: ", L"�װ��� ��ȭȰ���� �Ϻδ�\n�� �̻� �����ϸ�\n�����ϰڴ�!", 0.1f, 1.f);
	self->show_text(L"ȭ����: ", L"���ǻ����̴�!", 0.1f, 1.f);

	Bullet bullet = {};
	bullet.pos = XMFLOAT2{ mng->client_w - 64.f, mng->client_h / 2.f };
	bullet.angle = XM_PIDIV2;
	bullet.damage = 80;
	bullet.sprite_name = "flame";
	bullet.is_mine = false;
	while (self->is_not_end() && self->boss_bar)
	{
		self->wrapped_lock();

		int pk = rand() % 5;
		if (pk == 0)
		{
			for (int k = 0; k < 5; ++k)
			{
				bullet.radius = 20.f;
				bullet.pos = XMFLOAT2{ 10.f + (mng->client_w - 20.f) * k / 4.f, mng->client_h - 10.f };
				bullet.velocity = {
					(player.pos.x - bullet.pos.x),
					(player.pos.y - bullet.pos.y) / 2,
					2.f
				};
				self->bullets.push_back(bullet);
				bullet.pos = XMFLOAT2{ 10.f + (mng->client_w - 20.f) * k / 4.f, 10.f };
				bullet.velocity = {
					(player.pos.x - bullet.pos.x),
					(player.pos.y - bullet.pos.y) / 2,
					2.f
				};
				self->bullets.push_back(bullet);
			}
		}
		else if (pk == 1)
		{
			for (int k = 0; k < 3; ++k)
			{
				bullet.radius = 20.f;
				bullet.pos = XMFLOAT2{ mng->client_w - 10.f, 10.f + (mng->client_h - 20.f) * k / 2.f, };
				bullet.velocity = {
					(player.pos.x - bullet.pos.x) / 2,
					(player.pos.y - bullet.pos.y),
					2.f
				};
				self->bullets.push_back(bullet);
				bullet.pos = XMFLOAT2{ 10.f, 10.f + (mng->client_h - 20.f) * k / 2.f };
				bullet.velocity = {
					(player.pos.x - bullet.pos.x) / 2,
					(player.pos.y - bullet.pos.y),
					2.f
				};
				self->bullets.push_back(bullet);
			}
		}
		else if (pk == 2)
		{
			bullet.radius = 60.f;
			bullet.pos = XMFLOAT2{ mng->client_w - 64.f, mng->client_h / 2.f };
			bullet.velocity = {
				(player.pos.x - bullet.pos.x) * 0.5f,
				(player.pos.y - bullet.pos.y) * 0.5f,
				2.f
			};
			self->bullets.push_back(bullet);
		}
		else if (pk == 3)
		{
			int certain = 2 + rand() % 17;
			int psk = rand() % 4;
			if (psk == 0)
			{
				for (int k = 0; k < 20; ++k)
				{
					if (k >= certain - 1 && k <= certain + 1) continue;
					bullet.radius = 20.f;
					bullet.pos = XMFLOAT2{ mng->client_w - 10.f, 10.f + (mng->client_h - 20.f) * k / 21.f, };
					bullet.velocity = {
						-80.f,
						0.f,
						2.f
					};
					self->bullets.push_back(bullet);
				}
			}
			else if (psk == 1)
			{
				for (int k = 0; k < 20; ++k)
				{
					if (k >= certain - 1 && k <= certain + 1) continue;
					bullet.radius = 20.f;
					bullet.pos = XMFLOAT2{ 10.f, 10.f + (mng->client_h - 20.f) * k / 21.f, };
					bullet.velocity = {
						80.f,
						0.f,
						2.f
					};
					self->bullets.push_back(bullet);
				}
			}
			else if (psk == 2)
			{
				for (int k = 0; k < 20; ++k)
				{
					if (k >= certain - 1 && k <= certain + 1) continue;
					bullet.radius = 20.f;
					bullet.pos = XMFLOAT2{ 10.f + (mng->client_w - 20.f) * k / 21.f, 10.f };
					bullet.velocity = {
						0.f,
						80.f,
						2.f
					};
					self->bullets.push_back(bullet);
				}
			}
			else if (psk == 3)
			{
				for (int k = 0; k < 20; ++k)
				{
					if (k >= certain - 1 && k <= certain + 1) continue;
					bullet.radius = 20.f;
					bullet.pos = XMFLOAT2{ 10.f + (mng->client_w - 20.f) * k / 21.f, mng->client_h - 10.f };
					bullet.velocity = {
						0.f,
						-80.f,
						2.f
					};
					self->bullets.push_back(bullet);
				}
			}
		}
		self->mtx.unlock();
		this_thread::sleep_for(1s);
	}

	if (!self->boss_bar)
	{
		self->show_text(L"������: ", L"ũ��....\n����� ���� ����Ʈ���±���", 0.1f, 1.f);
		self->show_text(L"ȭ����: ", L"���ΰ��� �� ȭ����\n���� �� ����?", 0.1f, 1.f);
		self->show_text(L"������: ", L"������� �����൵\n�� �𸥴�...", 0.1f, 1.f);
		self->show_text(L"������: ", L"�������� �������״�\n�װ����� ����...", 0.1f, 1.f);
		self->show_text(L"", L"������� ������ ��ģ ���� ������\n ��������.", 0.1f, 1.f);
		mng->next_level_name = "stage2-2";
	}
}