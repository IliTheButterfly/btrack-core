#include "SettingsManager.h"

namespace pt = boost::property_tree;

void _SettingsManager::setFilename(const std::string& name)
{
	mFilename = name;
}

const std::string _SettingsManager::getFilenemae() const
{
	return mFilename;
}

void SettingsManager::load()
{
	pt::ptree ptree;
	pt::read_json(paths::getScenePath(mFilename).string(), ptree);
	mCurrent.from_ptree(ptree);
}

void SettingsManager::save() const
{
	pt::ptree ptree;
	mCurrent.to_ptree(ptree);
	pt::write_json(paths::getScenePath(mFilename).string(), ptree);
}

const SceneSettings& _SettingsManager::get() const
{
	return mCurrent;
}

void _SettingsManager::set(const SceneSettings& settings)
{
	mCurrent = settings;
}

SettingsContainer&& _SettingsManager::edit()
{
	return SettingsContainer(this);
}

SettingsContainer::SettingsContainer(_SettingsManager* manager) : mManager(manager)
{
	mManager->load();
	mSettings = mManager->get();
}

SceneSettings& SettingsContainer::get()
{
	return mSettings;
}

SettingsContainer::~SettingsContainer()
{
	mManager->set(mSettings);
	mManager->save();
}
