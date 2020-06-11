LvTutorial::LvTutorial() {
	static float mob_x;
	static float textbox_height;
	static float ch_time;
	static wstring script;
	static Player& player = Player::get_instance();

	static std::deque<wstring> scripts;
	static bool clear_chat;

	this->OnStart = [&]() {
		mng->BindSprite("player_lv1", "sprites/misc_stone.png");
		mng->BindSprite("player_lv2", "sprites/helmet_2.png");
		mng->BindSprite("player_lv3", "sprites/helmet_4.png");
		mng->BindSprite("player_lv4", "sprites/helmet_5.png");
		mng->BindSprite("player_lv5", "sprites/helmet_ego_2.png");
		mng->BindSprite("francis", "sprites/francis.png");
		mng->BindSprite("green_mob", "sprites/draconic_base-green_new.png");
		mng->BindSprite("skill0", "sprites/shield_of_reflection.png");
		mng->BindSprite("skill1", "sprites/BombLitSprite.png");

		mng->BindSprite("item0", "sprites/bread_ration_new.png");
		mng->BindSprite("item1", "sprites/dark_brown_new.png");
		mng->BindSprite("item2", "sprites/dark_green_old.png");
		for (int k = 1; k <= 11; ++k)
			mng->BindSprite("explosion" + to_string(k), "sprites/explosion" + to_string(k) + ".png");
		
	};

	this->OnEnter = [&]() {
		player.Clear();
		player.pos.x = mng->client_w / 2 - 16;
		player.pos.y = mng->client_h * 2 / 3 - 16;
		scroll_x = 0.f;
		mob_x = mng->client_w + 64;
		textbox_height = 200;
		ch_time = 0.f;
		script = L"";

		scripts = {
			L"�ȳ��ϽŰ� ���, ȭ�����̿�.",
			L"����� �������� ��ġ��� ���� ���� �ʸ�\n â���Ͽ��ٳ�.",
			L"A, W, S, D�� ���� ���� ������ �� ��\n�ٳ�.",
			L"���� FŰ�� �����ϰ� Q, E Ű�� ����\n ���¸� �ٲܼ� �ִ� �ɷ��� �ο��ߴٳ�.",
			L"�� ���迡 ���� �������ڸ� ����� Ȳ��ȭ\n"
			L"�Ǿ �츮�� ������ ���ڶ�����\n"
			L" ����ġ�� �Ǿ��ٳ�",
			L"�׵��� �Ұ� �������� �츮�� ����������\n�ڳ״� ���̱� ������ ������ �ɼ�",
			L"�츮�� ���� ��\b",
			L" ���� �༮�� ������� �i�ƿ��ٴ�!\n�� �̸� ������ �ڳ�. ������ ���ڴ�.",
		};
		clear_chat = false;
	};

	this->OnLoop = [&]() {
		static float backcolor[4] = { 0.f, 0.2f, 0.3f, 1.f };
		mng->device->ClearRenderTargetView(mng->rtv.Get(), backcolor);
		mng->draw_sprite("bg_start", XMFLOAT2{ -scroll_x, 0 }, { (float)mng->client_w, (float)mng->client_h });
		mng->draw_sprite("bg_start", XMFLOAT2{ mng->client_w -scroll_x, 0 }, { (float)mng->client_w, (float)mng->client_h });

		scroll_x += mng->deltatime * 32;
		while (scroll_x > mng->client_w) scroll_x -= mng->client_w;

		
		ch_time += mng->deltatime;

		mng->draw_sprite("player_lv1", { player.pos.x - 16, player.pos.y - 16 }, { 32, 32 });
		
		mng->draw_sprite("green_mob", { mob_x - 32, mng->client_h / 2 - 32.f }, { 64, 64 });

		if (textbox_height > 0.f)
		{
			mng->draw_rect({ 0, 0 }, { 640, textbox_height }, { 0, 0, 0, 120 });
			mng->draw_sprite("francis", { 640 - 64, 0 }, { 64, 64 });
			mng->draw_text("desc", script, { 10, 10 }, { 255, 255, 255, 255 });

			if (scripts.size() <= 2)
			{
				mob_x -= mng->deltatime * 24;
				if (mob_x < mng->client_w - 64)
					mob_x = mng->client_w - 64;
			}

			if (ch_time > 0.1f)
			{
				ch_time = 0.f;
				if (clear_chat) {
					script = L"";
					clear_chat = false;
				}
				if (scripts.size())
				{
					if (wchar_t ch = scripts.at(0).at(0); ch == L'\b')
					{
						scripts.pop_front();
						clear_chat = true;
					}
					else if (ch)
						script.push_back(ch);


					if (scripts.at(0).size() > 1)
					{
						scripts.at(0) = scripts.at(0).substr(1);
					}
					else
					{
						scripts.pop_front();
						ch_time = -2.f;
						clear_chat = true;
					}
				}
				else
				{
					script = L"";
				}
			}
			if (scripts.size() == 0)
			{
				textbox_height -= mng->deltatime * 200;
			}
		}
		else
		{
			mng->next_level_name = "stage1-1";
		}
	};
}
