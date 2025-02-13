#ifndef __SETTINGSMANAGER_H__
#define __SETTINGSMANAGER_H__



#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "SceneSettings.h"
#include "paths.h"

namespace btrack::settings {

class SettingsContainer;

class _SettingsManager
{
protected:
    SceneSettings mCurrent{};
    std::string mFilename = "default";

public:
    virtual void setFilename(const std::string_view& name);
    virtual const std::string getFilenemae() const;
    virtual void load() = 0;
    virtual void save() = 0;
    virtual SceneSettings& get();
    virtual void set(const SceneSettings& settings);
};

class SettingsManager : public _SettingsManager
{
public:
    void load() override;
    void save() override;
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

} // btrack::settings
#endif // __SETTINGSMANAGER_H__