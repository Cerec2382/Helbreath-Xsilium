#pragma once
class QuestList
{
public:
	QuestList();
	~QuestList();
	void get_quests();
	void send_quest_data(int client);
	void get_quest_data(int client, char * data);
	void start_quest(int client, short id);
};

