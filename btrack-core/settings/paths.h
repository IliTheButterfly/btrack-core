#pragma once

#include <boost/filesystem.hpp>


namespace btrack { namespace settings {

namespace paths
{
	namespace fs = boost::filesystem;
	const fs::path base = "";
	const fs::path settings = base / "settings";
	const fs::path cameraSettings = settings / "cameras";
	const fs::path sceneSettings = settings / "scenes";
	const std::string sceneExtension = ".bts";
	
	inline const fs::path getScenePath(const std::string& filename)
	{
		return sceneSettings / (filename + sceneExtension);
	}

	inline void ensureExists()
	{
		std::vector<fs::path> paths = {
			base,
			settings,
			cameraSettings,
			sceneSettings,
		};
		for (auto p : paths)
		{
			if (!fs::exists(p)) fs::create_directory(p);
		}
	}
};

}} // btrack::settings