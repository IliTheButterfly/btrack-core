#pragma once

#include "PTreeConvertible.h"
#include "CamSettings.h"

struct SceneSettings : public PTreeConvertible
{
	std::vector<CameraSettings> cameras{};
	// Inherited via PTreeConvertible
	void to_ptree(boost::property_tree::ptree& pt) const override;
	void from_ptree(const boost::property_tree::ptree& pt) override;
};

