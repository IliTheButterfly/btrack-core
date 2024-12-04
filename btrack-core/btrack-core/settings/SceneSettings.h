#pragma once

#include "PTreeConvertible.h"
#include "CamSettings.h"

namespace btrack { namespace settings {

struct SceneSettings : public PTreeConvertible
{
	std::vector<CameraSettings> cameras{};
	
	// Inherited via PTreeConvertible
	void to_ptree(boost::property_tree::ptree& pt) override
	{
		boost::property_tree::ptree cams;
		size_t i = 0;
		cams.add("count", cameras.size());
		for (auto& cam : cameras)
		{
			boost::property_tree::ptree p;
			cam.to_ptree(p);
			cams.add_child(std::to_string(i), p);
			++i;
		}
		pt.add_child("cams", cams);
	}
	void from_ptree(boost::property_tree::ptree& pt) override
	{
		boost::property_tree::ptree cams = pt.get_child("cams");
		size_t count = cams.get<size_t>("count");
		cameras.resize(cams.size());
		for (size_t i = 0; i < count; ++i)
		{
			CameraSettings cam{};
			cam.from_ptree(cams.get_child(std::to_string(i)));
			cameras[i] = cam;
		}
	}
};


}} // btrack::settings