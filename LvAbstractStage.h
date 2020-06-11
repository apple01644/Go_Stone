void LevelStage::Clear() {
	bullets.clear();
	enimies.clear();
	effects.clear();
	gameover_fade = 0.f;

	log_suppose_to = false;
	log_is = false;
	log_progress = 0.f;
	log_text = L"";
	log_text_buffer = L"";

	boss_bar = false;
	boss_bar_hp = 50;
	boss_bar_last_hp = 100;

	game_over = false;
	is_pause = false;

	fade_inout = 1.f;
	ready_to_close = false;
}

void LevelStage::Draw() {
	static float backcolor[4] = { 0.f, 0.2f, 0.3f, 1.f };
	mng->device->ClearRenderTargetView(mng->rtv.Get(), backcolor);
	mng->draw_sprite(scroll_name, XMFLOAT2{ -scroll_x, 0 }, { (float)mng->client_w, (float)mng->client_h });
	mng->draw_sprite(scroll_name, XMFLOAT2{ mng->client_w - scroll_x, 0 }, { (float)mng->client_w, (float)mng->client_h });

	for (const Item& item : items)
		mng->draw_sprite(item.sprite_name, { item.pos.x - item.radius, item.pos.y - item.radius }, { item.radius * 2, item.radius * 2 });


	for (const Bullet& bullet : bullets)
		mng->draw_sprite_angle(bullet.sprite_name, { bullet.pos.x - bullet.radius, bullet.pos.y - bullet.radius }, { bullet.radius * 2, bullet.radius * 2 }, bullet.angle);

	for (const Effect& effect : effects)
		if (effect.is_frame)
			mng->draw_sprite_angle(effect.sprite_name + to_string(effect.frame), { effect.pos.x - effect.radius, effect.pos.y - effect.radius }, { effect.radius * 2, effect.radius * 2 }, effect.angle, effect.diffuse);
		else
			mng->draw_sprite_angle(effect.sprite_name, { effect.pos.x - effect.radius, effect.pos.y - effect.radius }, { effect.radius * 2, effect.radius * 2 }, effect.angle, effect.diffuse);

	for (const Enemy& enemy : enimies) {
		XMINT4 diffuse = enemy.diffuse;
		if (enemy.last_hit > 0.f)
			diffuse.x += enemy.last_hit * 255;
		mng->draw_sprite(enemy.sprite_name, { enemy.pos.x - enemy.radius, enemy.pos.y - enemy.radius }, { enemy.radius * 2, enemy.radius * 2 }, diffuse);
	}


	XMINT4 player_diffuse = {};
	if (player.hit_duration > 0)
	{
		player_diffuse.x = player.hit_duration * 255;
		if ((int)roundf(mng->totaltime * 7) % 2 == 0)
			player_diffuse.w = -128;
	}
	mng->draw_sprite("player_lv" + to_string(player.level), { player.pos.x - player.radius, player.pos.y - player.radius }, { player.radius * 2, player.radius * 2 }, player_diffuse);
	if (player.skill_cooldown[0] <= 3.f)
	{
		mng->draw_sprite("skill0", { player.pos.x - player.radius, player.pos.y - player.radius }, { player.radius * 2, player.radius * 2 }, { 0, 0, 0, -200 });
	}

	if (log_progress >= 1.f)
	{
		mng->draw_rect({ 0, 0 }, { 640, 200 }, { 0, 0, 0, 120 });
		mng->draw_text("desc", log_text, { 10, 10 }, { 255, 255, 255, 255 });
	}
	else
	{
		mng->draw_rect({ 0, 0 }, { 640, 200 * log_progress }, { 0, 0, 0, 120 });
	}

	mng->draw_rect({ 0.f,mng->client_h - 50.f }, { mng->client_w - 100.f, 25.f }, { 39, 39, 39, 255 });
	mng->draw_rect({ 0.f,mng->client_h - 25.f }, { mng->client_w - 100.f, 25.f }, { 39, 39, 39, 255 });

	if (float delta = (player.exp - player.last_exp) * mng->deltatime; delta / mng->deltatime > 0.01f * player.max_exp)
		player.last_exp += delta;
	else
		player.last_exp += (player.exp - player.last_exp) * 0.01f;
	if (float delta = (player.hp - player.last_hp) * mng->deltatime; delta / mng->deltatime > 0.01f * player.max_hp)
		player.last_hp += delta;
	else
		player.last_hp += (player.hp - player.last_hp) * 0.01f;


	if (player.last_hp > player.hp)
	{
		mng->draw_rect({ 2.f,mng->client_h - 47.f }, { (mng->client_w - 4.f - 102.f) * player.last_hp / player.max_hp, 23.f },  { 117, 232, 104, 255 });
		mng->draw_rect({ 2.f,mng->client_h - 47.f }, { (mng->client_w - 4.f - 102.f) * player.hp / player.max_hp, 23.f }, { 45, 186, 29, 255 });
	}
	else
	{
		mng->draw_rect({ 2.f,mng->client_h - 47.f }, { (mng->client_w - 4.f - 102.f) * player.hp / player.max_hp, 23.f },  { 117, 232, 104, 255 });
		mng->draw_rect({ 2.f,mng->client_h - 47.f }, { (mng->client_w - 4.f - 102.f) * player.last_hp / player.max_hp, 23.f }, { 45, 186, 29, 255 });
	}

	if (player.level < 5)
	{
		if (player.last_exp > player.exp)
		{
			mng->draw_rect({ 2.f,mng->client_h - 24.f }, { (mng->client_w - 4.f - 102.f) * player.last_exp / player.max_exp, 23.f }, { 189, 158, 239, 255 });
			mng->draw_rect({ 2.f,mng->client_h - 24.f }, { (mng->client_w - 4.f - 102.f) * player.exp / player.max_exp, 23.f }, { 91, 30, 185, 255 });
		}
		else
		{
			mng->draw_rect({ 2.f,mng->client_h - 24.f }, { (mng->client_w - 4.f - 102.f) * player.exp / player.max_exp, 23.f }, { 189, 158, 239, 255 });
			mng->draw_rect({ 2.f,mng->client_h - 24.f }, { (mng->client_w - 4.f - 102.f) * player.last_exp / player.max_exp, 23.f }, { 91, 30, 185, 255 });
		}
	}
	else
	{
		mng->draw_rect({ 2.f,mng->client_h - 24.f }, { (mng->client_w - 4.f - 102.f), 23.f }, { 91, 30, 185, 255 });
	}

	mng->draw_text("UI", "Score: " + to_string(player.score), { 10.f, mng->client_h - 23.f }, { 255, 255, 255, 255 });
	mng->draw_text("UI", "Lv: " + to_string(player.level), { 10.f, mng->client_h - 47.f }, { 255, 255, 255, 255 });


	mng->draw_rect({ mng->client_w - 100.f,mng->client_h - 50.f }, { 50.f, 50.f }, { 120, 120, 120, 255 });
	mng->draw_rect({ mng->client_w - 98.f, mng->client_h - 48.f }, { 46.f, 46.f }, { 80, 80, 80, 255 });
	if (player.skill_enable[0])
	{
		mng->draw_sprite("skill0", { mng->client_w - 98.f, mng->client_h - 48.f }, { 48, 48 });
		mng->draw_rect({ mng->client_w - 98.f, mng->client_h - 48.f }, { 46.f, 46.f - 46.f * player.skill_cooldown[0] / 60.f }, { 39, 39, 39, 200 });
		mng->draw_text("UI", "T", { mng->client_w - 98.f,mng->client_h - 48.f }, { 255, 255, 255, 255 });
	}


	mng->draw_rect({ mng->client_w - 50.f ,mng->client_h - 50.f }, { 50.f, 50.f }, { 120, 120, 120, 255 });
	mng->draw_rect({ mng->client_w - 48.f ,mng->client_h - 48.f }, { 46.f, 46.f }, { 80, 80, 80, 255 });
	if (player.skill_enable[1])
	{
		mng->draw_sprite("skill1", { mng->client_w - 48.f, mng->client_h - 48.f }, { 48, 48 });
		mng->draw_rect({ mng->client_w - 48.f ,mng->client_h - 48.f }, { 46.f, 46.f - 46.f * player.skill_cooldown[1] / 60.f }, { 39, 39, 39, 200 });
		mng->draw_text("UI", "G", { mng->client_w - 48.f,mng->client_h - 48.f }, { 255, 255, 255, 255 });
	}

	if (boss_bar && (!log_is) && (!log_suppose_to))
	{
		mng->draw_rect({ mng->client_w * 0.05f, 32 }, { mng->client_w * 0.90f, 20.f }, { 39, 39, 39, 200 });
		mng->draw_rect({ mng->client_w * 0.05f + 2.f, 32 }, { (mng->client_w * 0.90f - 4.f) * (boss_bar_last_hp / 100.f), 20.f }, { 248, 190, 175, 255 });
		mng->draw_rect({ mng->client_w * 0.05f + 2.f, 32 }, { (mng->client_w * 0.90f - 4.f) * (boss_bar_hp / 100.f), 20.f }, { 199, 54, 18, 255 });
	}

	if (mng->key_states['T'] && (!player.skill_enable[0] || player.skill_cooldown[0] < 60.f) || mng->key_states['G'] && (!player.skill_enable[1] || player.skill_cooldown[1] < 60.f))
	{
		mng->draw_text("UI", "지금 이 스킬을 이용할 수 없습니다.", { mng->client_w * 0.25f + 32.f, mng->client_h * 0.25f + 32.f }, { 255, 255, 255, 255 });
	}

	if (mng->key_states[VK_F7])
	{
		mng->draw_rect({ mng->client_w * 0.25f, mng->client_h * 0.25f }, { mng->client_w * 0.5f, mng->client_h * 0.5f }, { 39, 39, 39, 200 });
		mng->draw_text("UI", "LV. " + to_string(player.level) + " ( " + to_string(player.exp)  + " / " + to_string(player.max_exp) + " )" , { mng->client_w * 0.25f + 32.f, mng->client_h * 0.25f + 32.f }, { 255, 255, 255, 255 });
		for (int k = 1; k < 5; ++k)
		{
			mng->draw_text("UI", "LV. " + to_string(k) + " -> Lv. " + to_string(k+1) + " ( " + to_string(k * 100) + " )", { mng->client_w * 0.25f + 32.f, mng->client_h * 0.25f + (k + 1) / 7.f * mng->client_h * 0.5f + 32.f }, { 255, 255, 255, 255 });
		}
	}

	if (game_over && gameover_fade > 0)
	{
		mng->draw_rect({ 0.f, 0.f }, { (float)mng->client_w, (float)mng->client_h }, { 0, 0, 0, (int)(255 * gameover_fade) });
	}
	else if (fade_inout > 0)
	{
		mng->draw_rect({ 0.f, 0.f }, { (float)mng->client_w, (float)mng->client_h }, { 0, 0, 0, (int)(255 * fade_inout) });
	}

}

void LevelStage::Loop() {
	if (!is_pause)
	{
		if (!boss_bar)
		{
			scroll_x += mng->deltatime * 32;
			while (scroll_x > mng->client_w) scroll_x -= mng->client_w;
		}

		player.attack_delay += mng->deltatime;
		if (mng->key_states['Q'])
			player.attack_mode = 0;
		if (mng->key_states['E'])
			player.attack_mode = 1;

		for (int k = 0; k < 2; ++k)
		{
			if (player.skill_enable[k])
			{
				player.skill_cooldown[k] += mng->deltatime;
				if (player.skill_cooldown[k] > 60) player.skill_cooldown[k] = 60;
			}
		}

		if (mng->key_states['F'])
		{
			if (player.attack_mode == 0)
			{
				if (player.attack_delay >= 60 / player.bpm)
				{
					player.attack_delay = 0.f;
					Bullet bullet = {};
					bullet.pos = player.pos;
					bullet.radius = 8.f;
					bullet.velocity = { 512.f,  0.f, 0.f };
					bullet.angle = XM_PIDIV2;
					bullet.damage = player.damage;
					bullet.sprite_name = "player_lv1";
					bullets.push_back(bullet);
				}
			}
			else
			{
				if (player.attack_delay >= 180 / player.bpm)
				{
					player.attack_delay = 0.f;
					Bullet bullet = {};
					bullet.pos = player.pos;
					bullet.radius = 8.f;
					bullet.angle = XM_PIDIV2;
					bullet.damage = player.damage;
					bullet.sprite_name = "player_lv1";

					bullet.velocity = { 443.f, -256.f, 0.f };
					bullets.push_back(bullet);
					bullet.velocity = { 512.f, 0.f , 0.f };
					bullets.push_back(bullet);
					bullet.velocity = { 443.f, 256.f, 0.f };
					bullets.push_back(bullet);
				}
			}
		}

		remove_loop(items, [&](Item& item)
			{
				item.pos.x += item.velocity.x * mng->deltatime;
				item.pos.y += item.velocity.y * mng->deltatime;

				if (item.pos - player.pos <= item.radius + player.radius)
				{
					if (item.code == Item::Heal)
					{
						player.hp += player.max_hp / 5;
						if (player.hp > player.max_hp)
							player.hp = player.max_hp;
					}
					else if (item.code == Item::Skill)
					{
						for (int k = 0; k < 2; ++k)
						{
							if (player.skill_enable[k])
								player.skill_cooldown[k] = 60;
						}
					}
					else if (item.code == Item::Levelup)
					{
						player.GrowUp();
					}
					return true;
				}
				if (item.pos.x + item.radius > mng->client_w || item.pos.y + item.radius > mng->client_h || item.pos.x - item.radius < 0 || item.pos.y - item.radius < 0)
					return true;
				return false;
			});

		remove_loop(bullets, [&](Bullet& bullet)
			{
				bullet.pos.x += bullet.velocity.x * mng->deltatime;
				bullet.pos.y += bullet.velocity.y * mng->deltatime;
				bullet.angle += bullet.velocity.z * mng->deltatime;

				bool remove = false;
				if (bullet.is_mine)
				{
					for (Enemy& enemy : enimies)
					{
						if (bullet.pos - enemy.pos <= enemy.radius + bullet.radius)
						{
							enemy.last_hit = 1.f;
							enemy.hp -= bullet.damage;
							return true;
						}
					}
				}
				else
				{
					if (bullet.pos - player.pos <= player.radius + bullet.radius)
					{
						if (player.hit_duration <= 0.f && (!player.unbreakable) && player.skill_cooldown[0] > 3.f)
						{
							player.hit_duration = 1.f;
							player.hp -= bullet.damage;
						}
						return true;
					}
				}
				if (bullet.pos.x - bullet.radius > mng->client_w || bullet.pos.y - bullet.radius > mng->client_h || bullet.pos.x + bullet.radius < 0 || bullet.pos.y + bullet.radius < 0)
					return true;
				return false;
			});

		remove_loop(enimies, [&](Enemy& enemy)
			{
				enemy.pos.x += enemy.velocity.x * mng->deltatime;
				enemy.pos.y += enemy.velocity.y * mng->deltatime;
				enemy.radius += enemy.velocity.z * mng->deltatime;

				enemy.last_hit -= mng->deltatime * 2;

				if (enemy.hp <= 0)
				{
					Effect explosion = {};
					explosion.pos = enemy.pos;
					explosion.radius = enemy.radius + 32.f;
					explosion.angle = rand() % 100 * 0.01f * XM_2PI;
					explosion.is_frame = true;
					explosion.frame = 1;
					explosion.max_frame = 11;
					explosion.frame_duration = 0.1f;
					explosion.diffuse = { 0, 0, 0, -80 };
					explosion.sprite_name = "explosion";
					effects.push_back(explosion);

					Effect its_effect = {};
					its_effect.pos = enemy.pos;
					its_effect.radius = enemy.radius;
					its_effect.velocity = {
						120.f * (1 + rand() % 101 * 0.01f),
						240.f * (1 + rand() % 101 * 0.01f),
						(rand() % 101 * 0.01f - 0.5f) * 10.f
					};
					its_effect.sprite_name = enemy.sprite_name;
					effects.push_back(its_effect);

					player.score += 10;
					player.exp += enemy.reward_exp;
					if (player.exp >= player.max_exp)
					{
						player.exp -= player.max_exp;
						player.GrowUp();
					}

					if (rand() % 100 == 0)
					{
						Item cheat_item = {};
						cheat_item.radius = 16.f;
						cheat_item.pos = enemy.pos;
						cheat_item.velocity = enemy.velocity;
						switch (rand() % ((player.level < 5) ? 3 : 2))
						{
						case 0:
							cheat_item.code = Item::Heal;
							cheat_item.sprite_name = "item0";
							break;
						case 1:
							cheat_item.code = Item::Skill;
							cheat_item.sprite_name = "item2";
							break;
						case 2:
							cheat_item.code = Item::Levelup;
							cheat_item.sprite_name = "item1";
							break;
						}
						items.push_back(cheat_item);
					}
					return true;
				}
				if (enemy.pos - player.pos <= player.radius + enemy.radius)
				{
					if (player.hit_duration <= 0.f && (!player.unbreakable))
					{
						player.hit_duration = 1.f;
						player.hp -= 30.f;
					}
				}
				return enemy.remove;
			});

		remove_loop(effects, [&](Effect& effect)
			{
				effect.pos.x += effect.velocity.x * mng->deltatime;
				effect.pos.y += effect.velocity.y * mng->deltatime;
				effect.angle += effect.velocity.z * mng->deltatime;
				if (effect.is_frame)
				{
					effect.frame_duration += mng->deltatime;
					if (effect.frame_duration >= effect.frame_max_duration)
					{
						effect.frame_duration = 0.f;
						++effect.frame;
						if (effect.frame > effect.max_frame)
							return true;
					}
				}
				return false;
			});

		if (mng->key_states['A']) player.pos.x -= mng->deltatime * player.move_speed;
		if (mng->key_states['D']) player.pos.x += mng->deltatime * player.move_speed;
		if (mng->key_states['W']) player.pos.y -= mng->deltatime * player.move_speed;
		if (mng->key_states['S']) player.pos.y += mng->deltatime * player.move_speed;
		if (mng->key_states['T'] && player.skill_enable[0] && player.skill_cooldown[0] >= 60.f)
		{
			player.skill_cooldown[0] = 0.f;
		}

		static float skill1_effect_cooldown = 0.f;
		if (mng->key_states['G'] && player.skill_enable[1] && player.skill_cooldown[1] >= 60.f)
		{
			player.skill_cooldown[1] = 0.f;
			skill1_effect_cooldown = 0.f;
		}

		static float cheat_f3_delay = 0.f;
		cheat_f3_delay += mng->deltatime;
		if (mng->key_states[VK_F3] && cheat_f3_delay > 1.f)
		{
			cheat_f3_delay = 0.f;
			Item cheat_item = {};
			float rad = rand() % 100 / 0.01f * XM_2PI;
			cheat_item.pos = { mng->client_w * 1.f - 16.f, mng->client_h / 2.f };
			cheat_item.velocity = { -64.f, 0.f, 0.f };
			cheat_item.radius = 16.f;
			float length = sqrtf(powf(cheat_item.velocity.x, 2) + powf(cheat_item.velocity.y, 2));
			cheat_item.velocity.x *= 32.f / length;
			cheat_item.velocity.y *= 32.f / length;
			switch (rand() % ((player.level < 5) ? 3 : 2))
			{
			case 0:
				cheat_item.code = Item::Heal;
				cheat_item.sprite_name = "item0";
				break;
			case 1:
				cheat_item.code = Item::Skill;
				cheat_item.sprite_name = "item2";
				break;
			case 2:
				cheat_item.code = Item::Levelup;
				cheat_item.sprite_name = "item1";
				break;
			}
			items.push_back(cheat_item);
		}

		if (player.skill_cooldown[1] <= 3)
		{
			remove_loop(bullets, [&](Bullet& bullet)
				{
					return !bullet.is_mine;
				});
			skill1_effect_cooldown += mng->deltatime;
			if (skill1_effect_cooldown > 0.1f)
			{
				for (Enemy& enemy : enimies)
				{
					enemy.hp -= rand() % 13 + 7;
					enemy.last_hit = 0.f;
				}
				skill1_effect_cooldown = 0.f;
				Effect explosion = {};
				explosion.pos = XMFLOAT2{ rand() % mng->client_w * 1.f, rand() % mng->client_h * 1.f };
				explosion.radius = rand() % 64 + 32.f;
				explosion.angle = rand() % 100 * 0.01f * XM_2PI;
				explosion.is_frame = true;
				explosion.frame = 1;
				explosion.max_frame = 11;
				explosion.frame_duration = 0.3f;
				explosion.diffuse = { 0, 0, 0, 0 };
				explosion.sprite_name = "explosion";
				effects.push_back(explosion);
			}
		}

		if (player.pos.x + player.radius > mng->client_w) player.pos.x = mng->client_w - player.radius;
		if (player.pos.y + player.radius > mng->client_h) player.pos.y = mng->client_h - player.radius;
		if (player.pos.x - player.radius < 0) player.pos.x = player.radius;
		if (player.pos.y - player.radius < 0) player.pos.y = player.radius;

		if (player.hp <= 0 && (!player.unbreakable))
		{
			game_over = true;
			is_pause = true;
		}
		player.hit_duration -= mng->deltatime;

		if ((!game_over) && (!log_suppose_to) && (!log_is))
		{
			if (player.level != player.last_level)
			{
				logging_delay = 0.0f;
				log_closing_delay = 2.f;
				is_pause = true;
				log_suppose_to = true;
				log_text = L"";
				log_text_buffer = player.GetLevelDescription();
				++player.last_level;
			}
		}

		
	}
	if (closing)
	{
		fade_inout += 1.f * mng->deltatime;
		if (fade_inout > 1.f)
			ready_to_close = true;
	}
	else
		fade_inout -= 1.f * mng->deltatime;

	if (game_over)
	{
		gameover_fade += mng->deltatime * 0.3f;
		if (gameover_fade > 1.f)
		{
			mng->next_level_name = "gameover";
		}
	}
	else
	{
		if (log_suppose_to) log_progress += 2 * mng->deltatime;
		else log_progress -= 2 * mng->deltatime;
		if (log_progress > 1.f)
		{
			log_progress = 1.f;
			if (log_is) {
				logging_lasttime += mng->deltatime;
				if (log_text_buffer.size())
				{
					if (logging_lasttime > logging_delay)
					{
						logging_lasttime = 0.f;
						log_text.push_back(log_text_buffer.at(0));
						log_text_buffer.erase(0, 1);
					}
				}
				else
				{
					if (logging_lasttime > log_closing_delay)
					{
						log_suppose_to = false;
					}
				}
			}
			else {
				log_is = true;
				logging_lasttime = 0.f;
			}
		}
		else if (log_progress < 0.f)
		{
			log_progress = 0.f;
			if (log_is)
			{
				log_is = false;
				is_pause = false;
			}
		}
	}
}

bool LevelStage::is_not_end () {
	return !closing; 
};

void LevelStage::show_text(wstring start_text, wstring text, float delay, float closing_delay)
{
	while (is_not_end())
	{
		if ((!log_suppose_to) && (!log_is) && fade_inout < 0.f)
		{
			mtx.lock();
			logging_delay = delay;
			log_closing_delay = closing_delay;
			is_pause = true;
			log_suppose_to = true;
			log_text = start_text;
			log_text_buffer = text;
			mtx.unlock();
			break;
		}
	}
	while (is_not_end())
	{
		if ((!log_suppose_to) && (!log_is))
		{
			break;
		}
	}
};

void LevelStage::wrapped_lock() {
	while (is_pause && is_not_end());
	mtx.lock();
};