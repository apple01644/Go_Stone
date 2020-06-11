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
			L"안녕하신가 용사, 화강암이여.",
			L"사악한 무리들을 해치우기 위해 내가 너를\n 창조하였다네.",
			L"A, W, S, D로 이제 몸을 움직일 수 있\n다네.",
			L"또한 F키로 공격하고 Q, E 키로 공격\n 형태를 바꿀수 있는 능력을 부여했다네.",
			L"이 세계에 대해 설명하자면 세계는 황폐화\n"
			L"되어서 우리는 대산맥의 끝자락으로\n"
			L" 도망치게 되었다네",
			L"그들은 불과 얼음으로 우리를 공격하지만\n자네는 돌이기 때문에 괜찮을 걸세",
			L"우리의 적은 골\b",
			L" 용기사 녀석들 여기까지 쫒아오다니!\n난 이만 가봐야 겠네. 건투를 빌겠다.",
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
