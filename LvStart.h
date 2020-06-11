LvStartMenu::LvStartMenu() {

	this->OnStart = [&]() {
		mng->BindSprite("bg_start", "sprites/BG_mountains_noon2_1024.png");
		mng->BindSprite("bg_desert", "sprites/BG_desert_noon_1024.png");
		mng->BindSprite("bg_ice", "sprites/BG_desert_night_1024.png");
		mng->BindSprite("bg_volcano", "sprites/BG_volcano_1024.png"); 
		mng->BindSprite("bg_cave", "sprites/BG_cave2_1024.png");
		mng->BindSprite("bg_end", "sprites/BG_mountains_night2_1024.png");
		ex << D3DX10CreateFontA(mng->device.Get(), 100, 0, 0, 1, FALSE, HANGUL_CHARSET, OUT_DEFAULT_PRECIS, CLEARTYPE_QUALITY, FF_MODERN | DEFAULT_PITCH,
			"Bauhaus 93", mng->font_map["title"].GetAddressOf());
		ex << D3DX10CreateFontA(mng->device.Get(), 40, 0, 0, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLEARTYPE_QUALITY, FF_MODERN | DEFAULT_PITCH,
			"Consolas", mng->font_map["desc"].GetAddressOf());
		ex << D3DX10CreateFontA(mng->device.Get(), 20, 0, 0, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLEARTYPE_QUALITY, FF_MODERN | DEFAULT_PITCH,
			"Consolas", mng->font_map["UI"].GetAddressOf());
	};
	this->OnLoop = [&]() {
		static float backcolor[4] = { 0.f, 0.2f, 0.3f, 1.f };
		mng->device->ClearRenderTargetView(mng->rtv.Get(), backcolor);

		mng->draw_sprite("bg_start", XMFLOAT2{ -scroll_x, 0 }, { (float)mng->client_w, (float)mng->client_h });
		mng->draw_sprite("bg_start", XMFLOAT2{ mng->client_w - scroll_x, 0 }, { (float)mng->client_w, (float)mng->client_h });

		scroll_x += mng->deltatime * 32;
		while (scroll_x > mng->client_w) scroll_x -= mng->client_w;

		mng->draw_text("title", "Go Stone!", { 96, 96 }, { 0, 0, 0, 255 });
		mng->draw_text("desc", "Press A to Start", { 158, 238 }, { 0, 0, 0, 255 });
		mng->draw_text("desc", "Press E to Exit", { 158, 298 }, { 0, 0, 0, 255 });
		mng->draw_text("title", "Go Stone!", { 100, 100 }, { 255, 255, 255, 255 });
		mng->draw_text("desc", "Press A to Start", { 160, 240 }, { 255, 255, 255, 255 });
		mng->draw_text("desc", "Press E to Exit", { 160, 300 }, { 255, 255, 255, 255 });

		if (mng->key_states['A'])
		{
			mng->next_level_name = "tutorial";
		}
		else if (mng->key_states['E'])
		{
			PostQuitMessage(0);
		}
	};
}