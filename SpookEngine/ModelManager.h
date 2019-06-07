#ifndef _H_MODMAN_
#define _H_MODMAN_
#include "Model.h"
#include <string>
#include <vector>
class ModelManager {
public:
	Model* GetModel(const std::string& name);
private:
	std::vector<Model*> loadedModels;
};
#endif