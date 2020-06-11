#pragma once

struct Player {
	bool movable = true;
	bool unbreakable = false;
	int attack_mode = 0;

	float hit_duration = 0.f;

	int last_level = 1;
	int level = 1;
	int exp = 0;
	float last_exp = 0;
	int max_exp = 100;
	float last_hp = 600;
	float hp = 200;
	float max_hp = 600;
	int move_speed = 300;
	int score = 0;

	float damage = 90;
	float bpm = 360.f;
	float attack_delay = 0.f;
	const float radius = 16;

	bool skill_enable[2] = { true, true };
	float skill_cooldown[2] = { 60, 60 };
	
	DirectX::XMFLOAT2 pos = {};

	void Clear()
	{
		unbreakable = false;

		level = 1;
		last_level = 1;
		exp = 0;
		hp = 600;
		max_hp = 600;
		move_speed = 300;
		damage = 90;
		bpm = 360.f;
		score = 0;

		for (int k = 0; k < 2; ++k)
		{
			skill_enable[k] = false;
			skill_cooldown[k] = 60;
		}
	}

	void GrowUp()
	{
		if (level < 5)
		{
			++level;
			max_hp += max_hp * 0.20f;
			max_exp = level * 100;
			damage += damage * 0.20f;
			bpm += bpm * 0.20f;
			hp = max_hp;
			if (level == 3)
				skill_enable[0] = true;
			else if (level == 5)
				skill_enable[1] = true;			
		}
	}

	std::wstring GetLevelDescription()
	{
		if (last_level + 1 == 2) return L"Level up!\n���ݷ� +20%, ����� +20%, HP ��ü ȸ��";
		if (last_level + 1 == 3) return L"Level up!\n���ݷ� +20%, ����� +20%, HP ��ü ȸ��\n���� ��ų�� �رݵǾ����ϴ�.\n����: 3�ʰ� ��ȣ����";
		if (last_level + 1 == 4) return L"Level up!\n���ݷ� +20%, ����� +20%, HP ��ü ȸ��";
		if (last_level + 1 == 5) return L"Level up!\n���ݷ� +20%, ����� +20%, HP ��ü ȸ��\n���� ��ų�� �رݵǾ����ϴ�.\n��ź: 3�ʰ� �ǻ�ü �ı� �� ��������";
		return L"����!";
	}

	static Player& get_instance() {
		static Player instance = {};
		return instance;
	}
private:
	Player() {
		Clear();
	}
};