
LvStage1_2::LvStage1_2() {
	static thread trd;
	static float text_delay = 0;
	this->OnStart = [&]() {
		scroll_name = "bg_desert";
		mng->BindSprite("lamia", "sprites/lamia.png");
		mng->BindSprite("golden_mask", "sprites/ensorcelled_hibernation.png");
		mng->BindSprite("fire", "sprites/orb_of_fire_new.png");
	};

	this->OnEnter = [&]() {
		Clear();
		trd = thread(trd_LvStage1_2, this);

		Enemy boss = {};
		boss.pos = XMFLOAT2{ mng->client_w - 64.f, mng->client_h / 2.f };
		boss.radius = 128.f;
		boss.hp = 300000.f;
		boss.sprite_name = "lamia";
		boss.reward_exp = 0;
		boss_bar = true;
		enimies.push_back(boss);
	};

	this->OnLoop = [&]() {
		Loop();
		Draw();

		if (boss_bar && (!is_pause))
		{
			boss_bar_hp = (int)(roundf(enimies.begin()->hp / 300000.f * 100) );
			boss_bar_last_hp = -((boss_bar_last_hp - boss_bar_hp) * mng->deltatime);
		}
	};
	this->OnExit = [&]() {
		trd.join();
	};
}

void trd_LvStage1_2(LvStage1_2* self)
{
	static Player& player = Player::get_instance();
	self->show_text(L"", L"�Ͼ����\n�Ź̿� ���з� �縷���� �����Դ�.", 0.1f, 1.f);
	self->show_text(L"???: ", L"�׳��� ������ �Ź̵��� ������\n �� �༮�̳�!", 0.1f, 1.f);
	self->show_text(L"ȭ����: ", L"�ƴմϴ�. ���� �׳� �Ź̿�\n �۾��� �°� ���Դϴ�.", 0.1f, 1.f);
	self->show_text(L"???: ", L"������ �ʿ����! �� �Ź̵��\n �Բ� ��ȭ�� ����", 0.1f, 1.f);
	if (!self->is_not_end()) return;
	

	
	{
		self->wrapped_lock();
		float grid_space = mng->client_h / 21.f;
		Enemy fire = {};
		fire.radius = grid_space * 0.7f;
		fire.hp = 300000.f;
		fire.sprite_name = "fire";
		fire.velocity = { -grid_space * 4, 0.f, 0.f };
		int map[120][10] = {};
		int cy = 5;
		for (int x = 0; x < 120; ++x)
			for (int y = 0; y < 10; ++y)
				map[x][y] = 1;
		self->mtx.unlock();

		for (int x = 0; x < 120; ++x)
		{
			for (int y = 0; y < 10; ++y)
			{
				if (cy == y)
				{
					map[x][y] = 0;
					if (x + 1 < 120)
						map[x + 1][y] = 0;
				}
			}
			cy += rand() % 3 - 1;
			if (cy < 0) cy = 0;
			if (cy > 9) cy = 9;
		}
		for (int x = 0; x < 120; ++x)
		{
			self->wrapped_lock();
			for (int y = 0; y < 10; ++y)
			{
				if (map[x][y])
				{
					fire.pos = XMFLOAT2{ mng->client_w + grid_space, grid_space * 2 * (y + 0.5f) };
					self->enimies.push_back(fire);
				}
			}
			self->mtx.unlock();
			if (!self->is_not_end()) return;
			this_thread::sleep_for(0.5s);
			self->wrapped_lock();
			remove_loop(self->enimies, [&](Enemy& enemy) {
				if (enemy.pos.x < -enemy.radius && enemy.sprite_name == "fire")
					return true;
				return false;
				});
			self->mtx.unlock();
		}
	}

	self->show_text(L"???:\n", L"�� ������ ���� �ʴ´ٴ�\n����ϱ�", 0.1f, 1.f);
	self->show_text(L"???:\n", L"�׷��� ���� ���ݵ� ������\n��ä�� �ϳ� ���� �� �Ѵ�.", 0.1f, 1.f);
	self->show_text(L"???:\n", L"�׷��ٸ� �ο��� ���� ���� ������\n�ٸ� ������� ������ �Ѵ�.", 0.1f, 1.f);
	self->show_text(L"???:\n", L"�� �縷�� ���� ��̾ƴ�\n���������� ȭ��!\n���������� ���״� ���߾�ƶ�\n", 0.1f, 1.f);
	self->show_text(L"��̾�, �縷�� ����:\n", L"�������� ���� �ִ� 4���� ���׳׿���\n�� �� �ܾ� �� �� �ִ�.\n��� �� 300���� ���׳װ� �Դ�.", 0.1f, 2.f);
	self->show_text(L"��̾�, �縷�� ����:\n", L"�� ���׳� �鿡�� ��� �� �� ��\n�� �ַ��� �ּ� ��ĥ�� �ɸ��°�?", 0.1f, 2.f);
	self->show_text(L"��̾�, �縷�� ����:\n", L"1. 65��\n2. 75��\n3. 90��", 0.1f, 2.f);
	while (self->is_not_end())
	{
		if (mng->key_states['1'])
		{
			self->show_text(L"ȭ����:", L"������........\n1���̴�.", 0.1f, 1.f);
			self->show_text(L"��̾�, �縷�� ����:\n", L".........Ʋ�ȴ�.", 0.1f, 1.f);
			self->wrapped_lock();
			self->player.hp = -1;
			self->mtx.unlock();
			this_thread::sleep_for(1.s);
			break;
		}
		else if (mng->key_states['2'])
		{
			self->show_text(L"ȭ����:", L"������........\n2���̴�.", 0.1f, 1.f);
			self->show_text(L"��̾�, �縷�� ����:\n", L".........�����̴�.", 0.1f, 1.f);
			self->enimies.begin()->hp = 200000.f;
			this_thread::sleep_for(1.s);
			break;
		}
		else if (mng->key_states['3'])
		{
			self->show_text(L"ȭ����:", L"������........\n3���̴�.", 0.1f, 1.f);
			self->show_text(L"��̾�, �縷�� ����:\n", L".........Ʋ�ȴ�.", 0.1f, 1.f);
			self->wrapped_lock();
			self->player.hp = -1;
			self->mtx.unlock();
			this_thread::sleep_for(1.s);
			break;
		}
	}
	if (!self->is_not_end()) return;
	self->show_text(L"��̾�, �縷�� ����:\n", L"�� ��° ������.\n�縷�� �� �ʳ� ���� �о����� �ִ�.\n�̸� ���� �� �ִ� ������ �����\n���� �� �����ΰ�?", 0.1f, 2.f);
	self->show_text(L"��̾�, �縷�� ����:\n", L"1.�������� ����� �ʴ´�.\n2. �縷�� �𷡸� �ۼ� �ٴٿ� ������.\n3. �縷�� ������ �ɴ´�.", 0.1f, 2.f);
	while (self->is_not_end())
	{
		if (mng->key_states['1'])
		{
			self->show_text(L"ȭ����:", L"������........\n1���̴�.", 0.1f, 1.f);
			self->show_text(L"��̾�, �縷�� ����:\n", L".........Ʋ�ȴ�.", 0.1f, 1.f);
			self->wrapped_lock();
			self->player.hp = -1;
			self->mtx.unlock();
			this_thread::sleep_for(1.s);
			break;
		}
		else if (mng->key_states['2'])
		{
			self->show_text(L"ȭ����:", L"������........\n2���̴�.", 0.1f, 1.f);
			self->show_text(L"��̾�, �縷�� ����:\n", L".........Ʋ�ȴ�.", 0.1f, 1.f);
			self->wrapped_lock();
			self->player.hp = -1;
			self->mtx.unlock();
			this_thread::sleep_for(1.s);
			break;
		}
		else if (mng->key_states['3'])
		{
			self->show_text(L"ȭ����:", L"������........\n3���̴�.", 0.1f, 1.f);
			self->show_text(L"��̾�, �縷�� ����:\n", L".........�����̴�.", 0.1f, 1.f);
			self->enimies.begin()->hp = 100000.f;
			this_thread::sleep_for(1.s);
			break;
		}
	}
	if (!self->is_not_end()) return;
	self->show_text(L"��̾�, �縷�� ����:\n", L"������ �� �̾�� ����� ����\n��� �ҿ��� ����ָ�.", 0.1f, 1.f);
	self->show_text(L"ȭ����:", L"�� ���踦 �ʰ� Ȳ��ȭ�� ������?\n�׷��ٸ� �ʰ� ���� �Ǵ°��� �ҿ��̴�.", 0.1f, 1.f);
	self->show_text(L"��̾�, �縷�� ����:\n", L"�ƴϴ�, ���� ���� �縷�� ������ �� ��.", 0.1f, 1.f);
	self->show_text(L"ȭ����:", L"�׷��ٸ� �ٸ� ����� �� ���踦\nȲ���ϰ� ������ΰ�?", 0.1f, 1.f);
	self->show_text(L"��̾�, �縷�� ����:\n", L".... �׷���", 0.1f, 1.f);
	self->show_text(L"ȭ����:", L"�� ����� ��������\n�˷��ִ°� �ҿ����� �ϰڴ�.", 0.1f, 1.f);
	self->show_text(L"��̾�, �縷�� ����:\n", L"", 0.1f, 1.f);
	self->enimies.begin()->hp = 0.1f;
	this_thread::sleep_for(1.s);
	self->wrapped_lock();
	auto& enemy = *self->enimies.begin();
	Effect explosion = {};
	explosion.is_frame = true;
	explosion.frame = 1;
	explosion.max_frame = 11;
	explosion.frame_duration = 0.7f;
	explosion.diffuse = { 0, 0, 0, -80 };
	explosion.sprite_name = "explosion";
	for (int k = 0; k < 8; ++k)
	{
		explosion.radius = 24.f + rand() % 12;
		explosion.angle = rand() % 100 * 0.01f * XM_2PI;
		explosion.pos = enemy.pos;
		explosion.pos.x += rand() % 100 - 50;
		explosion.pos.y += rand() % 100 - 50;
		explosion.velocity = { rand() % 100 - 50.f, rand() % 100 - 50.f, 0.f };
		self->effects.push_back(explosion);
	}
	self->boss_bar = false;
	self->enimies.begin()->hp = -0.1f;
	self->mtx.unlock();
	this_thread::sleep_for(2.s);
	self->wrapped_lock();
	Effect boss = {};
	boss.pos = XMFLOAT2{ mng->client_w - 64.f, mng->client_h / 2.f };
	boss.radius = 32.f;
	boss.sprite_name = "golden_mask";
	self->mtx.unlock();
	self->effects.push_back(boss);
	self->show_text(L"???: ", L"�ű� ������?", 0.1f, 1.f);
	self->show_text(L"Ȳ�� ����ũ: ", L"���� Ȳ�� ����ũ��� �Ѵ�.\n�̷� ������ ���迡 �������ڸ�\n���ϴ� ������ ����", 0.1f, 1.f);
	self->show_text(L"Ȳ�� ����ũ: ", L"���� ���̴� �� ���¸޷뤤����?", 0.1f, 1.f);
	self->show_text(L"", L"�߰ſ� ����� ���� ������ �Ҿ���ȴ�.", 0.1f, 1.f);


	mng->next_level_name = "stage1-3";
}