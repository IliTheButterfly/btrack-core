#include "processing/camera.h"

namespace btrack::processing {

CaptureCamera::CaptureCamera(const settings::CameraSettings &settings)
{
	mSettings = settings;
}

void CaptureCamera::open()
{
	if (isOpen()) return;
	mCap.open(settings().deviceInfo.id);
}

void CaptureCamera::close()
{
	if (!isOpen()) return;
	mCap.release();
}

bool CaptureCamera::isOpen()
{
	return mCap.isOpened();
}

bool CaptureCamera::grab()
{
	if (!isOpen()) return false;
	if (!mCap.grab()) return false;
	// mLastCapture = boost::chrono::high_resolution_clock::now().time_since_epoch();
	return true;
}

bool CaptureCamera::retrieve(StampedOutputArray img, int flag)
{
	if (!isOpen()) return false;
	auto res = mCap.retrieve(img.image, flag);
	return res;
}

bool CaptureCamera::read(StampedOutputArray img)
{
	if (!isOpen()) return false;
	return grab() && retrieve(img);
}

const settings::CameraSettings &CaptureCamera::settings() const
{
	return mSettings;
}

settings::CameraSettings &CaptureCamera::settings()
{
	return mSettings;
}

CaptureCamera::~CaptureCamera()
{
	close();
}

} // btrack::processing