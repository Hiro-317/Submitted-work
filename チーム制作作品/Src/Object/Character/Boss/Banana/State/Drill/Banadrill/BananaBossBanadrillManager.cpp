#include "BananaBossBanadrillManager.h"


BananaBossBanadrillManager::BananaBossBanadrillManager(
	const MSG_SENDER_ID operatorSenderId,
	const CharacterStats& stats
)
{
	for (int i = 0; i < NUM; i++) {
		banadrill[i] = new BananaBossBanadrill(operatorSenderId, stats);
	}
}

void BananaBossBanadrillManager::Load(void)
{
	for (int i = 0; i < NUM; i++) {
		banadrill[i]->Load();
	}
}

void BananaBossBanadrillManager::SubUpdate(void)
{
	for (int i = 0; i < NUM; i++) {
		banadrill[i]->Update();
	}
}

void BananaBossBanadrillManager::SubAlphaDraw(void)
{
	for (int i = 0; i < NUM; i++) {
		banadrill[i]->AlphaDraw();
	}
}
