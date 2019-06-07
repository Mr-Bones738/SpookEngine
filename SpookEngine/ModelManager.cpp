#include "ModelManager.h"

Model* ModelManager::GetModel(const std::string& name) {
	// if a model is already loaded, return that pointer
	// else, load it
	for (auto it : loadedModels) {
		if (it->name == name) {
			return it;
		}
	}
	Model* m = new Model(name);
	loadedModels.push_back(m);
	return m;
}