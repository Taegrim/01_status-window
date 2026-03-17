#include <iostream>
#include <span>
#include <format>
#include <string>

using namespace std;

enum STATUS { HP, MP, ATK, DEF, STATUS_COUNT };

const string STATUS_NAMES[]{ "HP", "MP", "공격력", "방어력" };
//const int MAX_STATUS[]{ 1000000, 1000000, 500000, 500000 };
//const int MAX_POTION[]{ 100, 100 };
const int MAX_STATUS[]{ 200, 200, 100, 100 };	// 테스트용 스탯 최대값
const int MAX_POTION[]{ 7, 7 };	// 테스트용 포션 최대값
const int DEFAULT_POTION_AMOUNT = 5;
const int POTION_INCREMENTS = 20;
const int MAX_LEVEL = 99;
const int MAX_EXP = 6;
const int MIN_HP = 50;
const int MIN_ATK = 0;


void PrintStatus(span<int> status, int level, span<int> potions)
{
	cout << "--------------" << endl;

	cout << format("현재 레벨 : {}\n", level);

	for (int i = 0; i < status.size(); ++i) {
		cout << format("{} : {}\n", STATUS_NAMES[i], status[i]);
	}

	cout << format("잔여 HP 포션 : {}\n", potions[HP]);
	cout << format("잔여 MP 포션 : {}\n", potions[MP]);

	cout << "--------------" << endl << endl;
}

void IncreaseStatus(span<int> status, STATUS name, int amount)
{
	int value{};
	if (name >= 0 && name < STATUS_COUNT) {
		value = status[name] + amount;
		if (value >= MAX_STATUS[name]) {
			cout << format("{} 스탯이 한계에 도달했습니다\n", STATUS_NAMES[name]);
			value = MAX_STATUS[name];
		}
		else {
			cout << format("{} 스탯이 {} 증가했습니다\n", STATUS_NAMES[name], amount);
		}
		cout << format("현재 {} : {}\n", STATUS_NAMES[name], value);

		status[name] = value;
	}
	else {
		cout << "스탯 상승 에러" << endl;
	}
}

void SetPotion(span<int> potions, int amount)
{
	for (int& potion : potions) {
		potion = amount;
	}
}

void UsePotion(span<int> status, span<int> potions, STATUS name)
{
	if (name >= 0 && name <= MP) {
		if (potions[name] > 0) {
			if (status[name] == MAX_STATUS[name]) {
				cout << "이미 스탯이 최대치이므로 포션을 사용할 수 없습니다" << endl;
			}
			else {
				--potions[name];
				cout << format("{} 포션 사용\n", STATUS_NAMES[name]);
				IncreaseStatus(status, name, POTION_INCREMENTS);
			}
			cout << format("남은 {}포션 : {}\n", STATUS_NAMES[name], potions[name]);
		}
		else {
			cout << format("{} 포션이 없습니다\n", STATUS_NAMES[name]);
		}
	}
	else {
		cout << "포션 사용 에러" << endl;
	}
}

void LevelUp(span<int> status, int& level, span<int> potions)
{
	if (level >= MAX_LEVEL) {
		cout << "이미 최대 레벨입니다" << endl;
		return;
	}

	++level;
	cout << "레벨 업!!  HP/MP 포션이 지급됩니다" << endl;

	for (int i = 0; i < potions.size(); ++i) {
		STATUS name = static_cast<STATUS>(i);
		if (potions[i] >= MAX_POTION[name]) {
			cout << format("{} 포션은 최대치입니다\n", STATUS_NAMES[name]);
		}
		else {
			++potions[i];
		}
	}

	cout << format("남은 HP/MP 포션 수 : {}/{}\n", potions[HP], potions[MP]);
}

void ExpUp(span<int> status, int& level, int& exp, span<int> potions)
{
	exp = (exp + 1) % MAX_EXP;
	cout << "경험치 증가" << endl;

	if (0 == exp) {
		LevelUp(status, level, potions);
	}

	cout << format("현재 경험치 : {}/{}\n", exp, MAX_EXP);
}

int main()
{
	constexpr int POTION_SIZE = 2;

	int level{ 1 }, exp{ 0 };
	int potions[POTION_SIZE]{ 0, };
	int status[STATUS_COUNT]{ 0, };


	// 스탯 입력
	while (true) {
		cout << "HP와 MP를 입력하세요 : ";
		
		if (cin >> status[HP] >> status[MP]) {
			if (status[HP] > MIN_HP && status[MP] > MIN_HP) {
				break;
			}
			cout << "HP나 MP의 값이 너무 작습니다. 다시 입력하세요" << endl;
		}
		else {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "잘못된 입력값입니다. 다시 입력하세요" << endl;
		}
	}


	while (true) {
		cout << "ATK와 DEF를 입력하세요 : ";
		
		if (cin >> status[ATK] >> status[DEF]) {
			if (status[ATK] > MIN_ATK && status[DEF] > MIN_ATK) {
				break;
			}
			cout << "ATK나 DEF의 값이 너무 작습니다. 다시 입력하세요" << endl;
		}
		else {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "잘못된 입력값입니다. 다시 입력하세요" << endl;
		}
	}


	SetPotion(potions, DEFAULT_POTION_AMOUNT);
	cout << format("포션이 지급되었습니다. (HP, MP 포션 각 {}개)\n", DEFAULT_POTION_AMOUNT);
	cout << "--------------------------------------------" << endl;

	int menu{};
	bool loop{ true };
	while (loop) {
		for (int i = 0; i < STATUS_COUNT; ++i) {
			cout << format("{}. {} UP\n", (i + 1), STATUS_NAMES[i]);
		}
		cout << format("{}. 현재 능력치\n", static_cast<int>(STATUS_COUNT) + 1);
		cout << format("{}. EXP UP\n", static_cast<int>(STATUS_COUNT) + 2);
		cout << "0. 나가기" << endl;

		cout << "번호를 선택해주세요 : ";
		if (!(cin >> menu)) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "숫자만 입력해주세요." << endl;
			continue;
		}

		switch (menu) {
		case 1:
			UsePotion(status, potions, HP);
			break;
		case 2:
			UsePotion(status, potions, MP);
			break;
		case 3:
			IncreaseStatus(status, ATK, status[ATK]);
			break;
		case 4:
			IncreaseStatus(status, DEF, status[DEF]);
			break;
		case 5:
			PrintStatus(status, level, potions);
			break;
		case 6:
			ExpUp(status, level, exp, potions);
			break;
		case 0:
			loop = false;
			break;
		default:
			cout << "다시 입력해주세요" << endl;
			break;
		}
	}
	cout << "프로그램을 종료합니다" << endl;

	return 0;
}