#include <fstream>
LvRanking::LvRanking() {
	static bool write_mode;
	static char last_key;
	static string write_buffer;
	this->OnEnter = [&]() {
		std::ifstream file{ "rank.dat" };
		std::string s;
		scoreboard.clear();
		while (file >> s)
		{
			string name = s.substr(0, s.find(':'));
			int score = stoi(s.substr(s.find(':') + 1));
			scoreboard.push_back(make_pair(name, score));
		}
		write_mode = true;
		write_buffer = "";
		return;
	};

	this->OnLoop = [&]() {
		static Player& player = Player::get_instance();
		static XMFLOAT2 center = { mng->client_w / 2.f, mng->client_h / 2.f };
		static float backcolor[4] = { 255.f, 255.f, 255.f, 1.f };
		static float keydelay = 0.f;
		mng->device->ClearRenderTargetView(mng->rtv.Get(), backcolor);
		keydelay += mng->deltatime;
		mng->draw_sprite("bg_end", { 0.f, 0.f }, { mng->client_w * 1.f, mng->client_h* 1.f });
		if (write_mode)
		{
			mng->draw_text("desc", "이니셜 3글자를 적어주세요.", { 120.f, 120.f }, { 255, 255, 255, 200 });
			mng->draw_text("desc", "당신의 점수: " + to_string(player.score), { 120.f, 200.f }, { 255, 255, 255, 200 });
			
			for (char target = 'A'; target <= 'Z'; ++target)
			{
				if (mng->key_states[target] && write_buffer.size() < 3)
				{
					if (target == last_key && keydelay <= 0.5f) continue;
					keydelay = 0.f;
					write_buffer.push_back(target);
					last_key = target;
				}
			}
			if (mng->key_states[VK_BACK])
			{
				if (VK_BACK == last_key && keydelay <= 0.5f);
				else {
					keydelay = 0.f;
					last_key = VK_BACK;
					if (write_buffer.size())
						write_buffer.pop_back();
				}
			}
			
			string buf = write_buffer;
			while (buf.size() < 3) buf.push_back('_');
			mng->draw_text("title", buf, { 120.f, 280.f }, { 255, 255, 255, 200 });
			mng->draw_text("UI", "그리고 엔터를 눌러 등록하세요.", { 120.f, 400.f }, { 255, 255, 255, 200 });

			if (mng->key_states[VK_RETURN])
			{
				if (write_buffer.size() == 3)
				{
					scoreboard.push_back(make_pair(write_buffer, player.score));
					scoreboard.sort([](std::pair<std::string, int>& lhs, std::pair<std::string, int>& rhs) {return lhs.second > rhs.second; });
					write_mode = false;
				}
			}
		}
		else
		{
			int k = 0;
			for (const auto& pair : scoreboard)
			{
				mng->draw_text("title", pair.first + "  " + to_string(pair.second), { 120.f, 120.f + 80.f * k }, { 255, 255, 255, 200 });
				mng->draw_text("desc", "명예의 전당.", { 120.f, 20.f }, { 255, 255, 255, 200 });
				mng->draw_text("UI", "F를 눌러 메뉴로 가기.", { 120.f, 70.f }, { 255, 255, 255, 200 });
				++k;
			}
			if (mng->key_states['F'])
			{
				mng->next_level_name = "start_menu";
			}
		}
	};

	this->OnExit = [&]() {
		std::ofstream file{ "rank.dat" };
		for (const auto& pair : scoreboard)
		{
			file << pair.first << ":" << to_string(pair.second) << "\n";
		}
		return;
	};
}