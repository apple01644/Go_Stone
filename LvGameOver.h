LvGameOver::LvGameOver() {
	static Player& player = Player::get_instance();
	static float text_delay = 0.f;
	static float bg_fade = 1.f;
	static float pieces_appera_delay = 0.f;

	this->OnEnter = [&]() {
		text_delay = 0.f;
		bg_fade = 1.f;
		pieces_appera_delay = 0.f;
	};

	this->OnLoop = [&]() {
		static XMFLOAT2 center = { mng->client_w / 2.f, mng->client_h / 2.f };
		static float backcolor[4] = { 0.f, 0.0f, 0.0f, 1.f };
		mng->device->ClearRenderTargetView(mng->rtv.Get(), backcolor);
		mng->draw_text("title", "Game Over", { 100.f, 40.f }, { 255, 255, 255, 255 });
		pieces_appera_delay += mng->deltatime;

		if (pieces.size() == 0)
		{
			if (pieces_appera_delay > 3)
			{
				pieces.push_back({ center.x, center.y, -0.3f, -0.9f });
				pieces.push_back({ center.x, center.y, 0.2f, -0.7f });
				pieces.push_back({ center.x, center.y, 0.1f, -0.5f });
			}
			mng->draw_sprite("player_lv" + to_string(player.level), { center.x - player.radius * 3, center.y - player.radius * 3 }, { player.radius * 6, player.radius * 6 });
		}
		else
		{
			float r = 0;
			for (XMFLOAT4& piece : pieces)
			{
				piece.w += mng->deltatime;
				piece.x += piece.z * mng->deltatime * 100;
				piece.y += piece.w * mng->deltatime * 100;
				mng->draw_sprite_angle("player_lv1", { piece.x - 16.f, piece.y - 16.f }, { 16.f * 2, 16.f * 2 }, r);
				r += 1.f;
			}
			text_delay += mng->deltatime;
			if (text_delay > 0.2f)
			{
				text_delay = 0.f;
				if (gameover_curr_text.size() < gameover_text.size())
					gameover_curr_text.push_back(gameover_text.at(gameover_curr_text.size()));
			}
			mng->draw_text("desc", gameover_curr_text, { 160.f, 280.f }, { 255, 255, 255, 255 });
			mng->draw_text("desc", "Press B to go menu", { 160.f, 340.f }, { 255, 255, 255, 255 });
			if (mng->key_states['B'])
				mng->next_level_name = "start_menu";
		}

		if (bg_fade > 0)
		{
			bg_fade -= 0.5f * mng->deltatime;
			mng->draw_rect({ 0.f, 0.f }, { (float)mng->client_w, (float)mng->client_h }, { 0, 0, 0, (int)(255 * bg_fade) });
		}
	};
}