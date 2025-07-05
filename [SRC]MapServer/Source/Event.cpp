// dynamic events
#include "..\\Header\\Event.h"
#include "..\\Header\\Game.h"
#include "..\\Header\\deathmatch.h"

//HeatoN events
extern class CDeathmatch * c_dm;
extern CMapServer * c_map;

Event::Event()
{

}

Event::~Event()
{

}

void Event::Handler::Activate(EventID id)
{
	CMap * map = 0;

	int i;

	if (Is(id))	return;

	auto & p = ptr[(int)id];

	std::ofstream outfile;

	switch (id)
	{
	case EventID::Deathmatch:
	{
		c_dm->enable();
		p = new Event;
		break;
	}
	case EventID::GunGameV2:
	{
		p = new Event;
		/*const int result = remove("Csv\\dmplayers.csv");
		ofstream outputFile("Csv\\dmplayers.csv");
		outputFile.close();
		c_dm->UpdateDmFile();
		p = new Event;
		c_dm->EnableEvent();*/
		break;
	}

	default:
		p = new Event;
		break;
	}

	NotifyStatus(id);
}

void Event::Handler::NotifyStatus(EventID id)
{
	for (auto p : c_map->m_pClientList)
	{
		if (!p)
			continue;

		p->Notify(NOTIFY_EVENT_STATUS, (DWORD)id, Is(id));
	}
}


void Event::Handler::LoginNotify(int client)
{
	auto g = c_map;
	auto p = g->m_pClientList[client];
	for (int i = 0; i < (int)EventID::Max; i++)
	{
		auto is = g_ev.Is((EventID)i);
		p->Notify(NOTIFY_EVENT_STATUS2, (DWORD)i, is);
	}
}

void Event::Handler::Deactivate(EventID id)
{
	std::ofstream outfile1;
	std::ofstream outfile;

	if (!Is(id)) return;

	if (ptr[(int)id])
	{
		delete ptr[(int)id];
		ptr[(int)id] = nullptr;
	}

	NotifyStatus(id);

	switch (id)
	{

		/*case EventID::Deathmatch:
		{
			/*const int result = remove("Csv\\dmplayers.csv");
			ofstream outputFile("Csv\\dmplayers.csv");
			outputFile.close();
			c_dm->UpdateDmFile();
			c_dm->DisableEvent();*/
		/*	break;
		}*/

		case EventID::Deathmatch:
		{
			c_dm->disable();
			break;
		}

		case EventID::GunGameV2:
		{
		/*	const int result = remove("Csv\\dmplayers.csv");
			ofstream outputFile("Csv\\dmplayers.csv");
			outputFile.close();
			c_dm->UpdateDmFile();
			c_gungame->DisableEvent();*/
			break;
		}
	}
	
	
}

void Event::Handler::Toggle(EventID id)
{
	if (Is(id))
		Activate(id);
	else Deactivate(id);
}

void Event::Handler::Teleport(EventID id, CClient * p)
{
	auto g = c_map;

	if (!Is(id))
	{
		g->SendAlertMsg(p->client, "Can't teleport, event is disabled!");
		return;
	}

	if (!p->IsInsideCh())
	{
		g->SendAlertMsg(p->client, "Can't teleport on this map!");
		return;
	}

	/*p->iPartyNum = -1;
	c_cmd->SendCommand(p->client, "/clearparty", 0, 0, 0, 0);*/	

	switch (id)
	{
	case EventID::Deathmatch: /*c_dm->Join(p->handle);*/ break;
	case EventID::GunGameV2: /*c_ggv2->JoinEvent(p->handle);*/ break;
	}
}