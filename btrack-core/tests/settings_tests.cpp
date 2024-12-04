#include <gtest/gtest.h>
#include "settings/CamSettings.h"
#include "settings/SettingsManager.h"

using namespace btrack::settings;

TEST(CamSettingsIntrinsicsTests, SimpleConversionTest)
{
	CameraIntrinsics intr = {};
	CameraIntrinsics intr2 = {};
	using member = double& (CameraIntrinsics::*)();
	using constMember = const double& (CameraIntrinsics::*)() const;

	std::vector<member> members = {
		&CameraIntrinsics::k1,
		&CameraIntrinsics::k2,
		&CameraIntrinsics::k3,
		&CameraIntrinsics::p1,
		&CameraIntrinsics::p2,
		&CameraIntrinsics::cameraMat00,
		&CameraIntrinsics::cameraMat01,
		&CameraIntrinsics::cameraMat02,
		&CameraIntrinsics::cameraMat10,
		&CameraIntrinsics::cameraMat11,
		&CameraIntrinsics::cameraMat12,
		&CameraIntrinsics::cameraMat20,
		&CameraIntrinsics::cameraMat21,
		&CameraIntrinsics::cameraMat22,
	};
	
	std::vector<constMember> constMembers = {
		&CameraIntrinsics::k1,
		&CameraIntrinsics::k2,
		&CameraIntrinsics::k3,
		&CameraIntrinsics::p1,
		&CameraIntrinsics::p2,
		&CameraIntrinsics::cameraMat00,
		&CameraIntrinsics::cameraMat01,
		&CameraIntrinsics::cameraMat02,
		&CameraIntrinsics::cameraMat10,
		&CameraIntrinsics::cameraMat11,
		&CameraIntrinsics::cameraMat12,
		&CameraIntrinsics::cameraMat20,
		&CameraIntrinsics::cameraMat21,
		&CameraIntrinsics::cameraMat22,
	};


	for (auto m : members)
	{
		EXPECT_EQ(((&intr)->*m)(), ((&intr2)->*m)());
	}

	for (auto m : constMembers)
	{
		EXPECT_EQ(((&intr)->*m)(), ((&intr2)->*m)());
	}

}

bool loaded = false;
bool saved = false;

class SettingsManagerMock : public _SettingsManager
{
public:
	void load() override { loaded = true; }
	void save() override { saved = true; }
};

TEST(SettingsManagerTests, EditLoadSave)
{
	SettingsManagerMock manager{};

	loaded = false;
	saved = false;
	{
		SettingsContainer editor {&manager};
		EXPECT_TRUE(loaded);
		EXPECT_FALSE(saved);
	}
	EXPECT_TRUE(loaded);
	EXPECT_TRUE(saved);
}

