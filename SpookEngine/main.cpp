#include "Engine.h"

int main() {
	Engine& inst = Engine::Instance();
	inst.Init();
	while (inst.Update());
	return 0;
}