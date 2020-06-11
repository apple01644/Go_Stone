
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
	self->show_text(L"", L"일어나보니\n거미에 떠밀려 사막까지 내려왔다.", 0.1f, 1.f);
	self->show_text(L"???: ", L"네놈이 더러운 거미들을 데리고\n 온 녀석이냐!", 0.1f, 1.f);
	self->show_text(L"화강암: ", L"아닙니다. 저는 그냥 거미에\n 휩쓸려 온것 뿐입니다.", 0.1f, 1.f);
	self->show_text(L"???: ", L"변명은 필요없다! 저 거미들과\n 함께 정화해 주지", 0.1f, 1.f);
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

	self->show_text(L"???:\n", L"이 정도로 죽지 않는다니\n대단하군", 0.1f, 1.f);
	self->show_text(L"???:\n", L"그러나 너의 공격도 나에게\n생채기 하나 내지 못 한다.", 0.1f, 1.f);
	self->show_text(L"???:\n", L"그렇다면 싸움의 끝이 나지 않으니\n다른 방식으로 결투를 한다.", 0.1f, 1.f);
	self->show_text(L"???:\n", L"나 사막의 여제 라미아는\n수수께끼의 화신!\n수수께끼를 낼테니 맞추어보아라\n", 0.1f, 1.f);
	self->show_text(L"라미아, 사막의 여제:\n", L"선술집은 매일 최대 4명의 나그네에게\n물 한 잔씩 줄 수 있다.\n어느 날 300명의 나그네가 왔다.", 0.1f, 2.f);
	self->show_text(L"라미아, 사막의 여제:\n", L"이 나그네 들에게 모두 물 한 잔\n씩 주려면 최소 며칠이 걸리는가?", 0.1f, 2.f);
	self->show_text(L"라미아, 사막의 여제:\n", L"1. 65일\n2. 75일\n3. 90일", 0.1f, 2.f);
	while (self->is_not_end())
	{
		if (mng->key_states['1'])
		{
			self->show_text(L"화강암:", L"정답은........\n1번이다.", 0.1f, 1.f);
			self->show_text(L"라미아, 사막의 여제:\n", L".........틀렸다.", 0.1f, 1.f);
			self->wrapped_lock();
			self->player.hp = -1;
			self->mtx.unlock();
			this_thread::sleep_for(1.s);
			break;
		}
		else if (mng->key_states['2'])
		{
			self->show_text(L"화강암:", L"정답은........\n2번이다.", 0.1f, 1.f);
			self->show_text(L"라미아, 사막의 여제:\n", L".........정답이다.", 0.1f, 1.f);
			self->enimies.begin()->hp = 200000.f;
			this_thread::sleep_for(1.s);
			break;
		}
		else if (mng->key_states['3'])
		{
			self->show_text(L"화강암:", L"정답은........\n3번이다.", 0.1f, 1.f);
			self->show_text(L"라미아, 사막의 여제:\n", L".........틀렸다.", 0.1f, 1.f);
			self->wrapped_lock();
			self->player.hp = -1;
			self->mtx.unlock();
			this_thread::sleep_for(1.s);
			break;
		}
	}
	if (!self->is_not_end()) return;
	self->show_text(L"라미아, 사막의 여제:\n", L"두 번째 문제다.\n사막은 수 십년 동안 넓어지고 있다.\n이를 막을 수 있는 최적의 방법은\n보기 중 무엇인가?", 0.1f, 2.f);
	self->show_text(L"라미아, 사막의 여제:\n", L"1.수돗물을 잠그지 않는다.\n2. 사막의 모래를 퍼서 바다에 버린다.\n3. 사막에 나무를 심는다.", 0.1f, 2.f);
	while (self->is_not_end())
	{
		if (mng->key_states['1'])
		{
			self->show_text(L"화강암:", L"정답은........\n1번이다.", 0.1f, 1.f);
			self->show_text(L"라미아, 사막의 여제:\n", L".........틀렸다.", 0.1f, 1.f);
			self->wrapped_lock();
			self->player.hp = -1;
			self->mtx.unlock();
			this_thread::sleep_for(1.s);
			break;
		}
		else if (mng->key_states['2'])
		{
			self->show_text(L"화강암:", L"정답은........\n2번이다.", 0.1f, 1.f);
			self->show_text(L"라미아, 사막의 여제:\n", L".........틀렸다.", 0.1f, 1.f);
			self->wrapped_lock();
			self->player.hp = -1;
			self->mtx.unlock();
			this_thread::sleep_for(1.s);
			break;
		}
		else if (mng->key_states['3'])
		{
			self->show_text(L"화강암:", L"정답은........\n3번이다.", 0.1f, 1.f);
			self->show_text(L"라미아, 사막의 여제:\n", L".........정답이다.", 0.1f, 1.f);
			self->enimies.begin()->hp = 100000.f;
			this_thread::sleep_for(1.s);
			break;
		}
	}
	if (!self->is_not_end()) return;
	self->show_text(L"라미아, 사막의 여제:\n", L"결투를 더 이어가긴 힘들것 같군\n대신 소원을 들어주마.", 0.1f, 1.f);
	self->show_text(L"화강암:", L"이 세계를 너가 황폐화한 것이지?\n그렇다면 너가 봉인 되는것이 소원이다.", 0.1f, 1.f);
	self->show_text(L"라미아, 사막의 여제:\n", L"아니다, 나는 그저 사막의 정원사 일 뿐.", 0.1f, 1.f);
	self->show_text(L"화강암:", L"그렇다면 다른 사람이 이 세계를\n황폐하게 만든것인가?", 0.1f, 1.f);
	self->show_text(L"라미아, 사막의 여제:\n", L".... 그렇다", 0.1f, 1.f);
	self->show_text(L"화강암:", L"그 사람이 누구인지\n알려주는걸 소원으로 하겠다.", 0.1f, 1.f);
	self->show_text(L"라미아, 사막의 여제:\n", L"", 0.1f, 1.f);
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
	self->show_text(L"???: ", L"거기 괜찮니?", 0.1f, 1.f);
	self->show_text(L"황금 마스크: ", L"나는 황금 마스크라고 한다.\n이런 곳에서 위험에 떨어진자를\n구하는 역할을 하지", 0.1f, 1.f);
	self->show_text(L"황금 마스크: ", L"지쳐 보이는 것 같읓메룸ㄴㅇㅁ?", 0.1f, 1.f);
	self->show_text(L"", L"뜨거운 열기로 인해 정신을 잃어버렸다.", 0.1f, 1.f);


	mng->next_level_name = "stage1-3";
}