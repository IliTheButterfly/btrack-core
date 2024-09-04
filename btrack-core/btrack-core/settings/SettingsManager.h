#pragma once

#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "SceneSettings.h"
#include "paths.h"

class SettingsContainer;

class _SettingsManager
{
protected:
	SceneSettings mCurrent;
	std::string mFilename = "default";

public:
	virtual void setFilename(const std::string& name);
	virtual const std::string getFilenemae() const;
	virtual void load() = 0;
	virtual void save() const = 0;
	virtual const SceneSettings& get() const;
	virtual void set(const SceneSettings& settings);
	SettingsContainer&& edit();
};

class SettingsManager : public _SettingsManager
{
public:
	void load() override;
	void save() const override;
};

class SettingsContainer
{
private:
	_SettingsManager* mManager;
	SceneSettings mSettings;
public:
	SettingsContainer(_SettingsManager* manager);
	SceneSettings& get();
	~SettingsContainer();
};
