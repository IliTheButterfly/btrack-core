#pragma once

#include "settings/SettingsManager.h"
#include "settings/SceneSettings.h"
#include "processing/camera.h"

namespace btrack { namespace processing {

class Scene
{
private:
	settings::SettingsManager mSettingsManager;
public:
	Scene();

	virtual bool activate() = 0;
	virtual bool deactivate() = 0;
	virtual bool reload() = 0;
	virtual bool isActive() = 0;

	virtual ~Scene() = default;
};

}} // btrack::processing