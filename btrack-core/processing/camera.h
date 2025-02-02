#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "wrapper.h"
#include "settings/CamSettings.h"
#include <boost/chrono/system_clocks.hpp>
#include <boost/thread/mutex.hpp>


namespace btrack::processing {


template <typename T>
struct _StampedImage
{
	// boost::chrono::nanoseconds time;
	T image;
};

typedef _StampedImage<cv::InputArray> StampedInputArray;
typedef _StampedImage<cv::OutputArray> StampedOutputArray;
typedef _StampedImage<cv::InputOutputArray> StampedInputOutputArray;

class Camera
{
public:

	/**
	 * @brief Opens the capture with the specified settings
	 */
	virtual void open() = 0;
	/**
	 * @brief Closes the capture
	 * 
	 */
	virtual void close() = 0;

	/**
	 * @brief Indicates if the capture is open
	 * 
	 * @return true if capture is open
	 */
	virtual bool isOpen() = 0;
	/**
	 * @brief Grabs a frame to be retrieved later and marks the time of capture
	 * 
	 * @return false if grabbing frame failed
	 */
	virtual bool grab() = 0;
	/**
	 * @brief Retrieves a grabbed frame
	 * 
	 * @param img Stamped image
	 * @param flag See cv::VideoCapture::Retrieve()
	 * @return false if failed or no frame grabbed
	 */
	virtual bool retrieve(StampedOutputArray img, int flag = 0) = 0;
	/**
	 * @brief Grabs and retrieves a frame
	 * 
	 * @param img Stamped image
	 * @return false if failed
	 */
	virtual bool read(StampedOutputArray img) = 0;
	
	virtual const settings::CameraSettings& settings() const = 0;
	virtual settings::CameraSettings& settings() = 0;
	virtual ~Camera() = default;
};

class CaptureCamera : public Camera
{
private:
	cv::VideoCapture mCap;
	settings::CameraSettings mSettings;
	// boost::chrono::nanoseconds mLastCapture;

public:
	CaptureCamera() = default;
	CaptureCamera(const settings::CameraSettings& settings);

	void open() override;
	void close() override;
	bool isOpen() override;

	bool grab() override;
	bool retrieve(StampedOutputArray img, int flag = 0) override;
	bool read(StampedOutputArray img) override;
	
	const settings::CameraSettings& settings() const override;
	settings::CameraSettings& settings() override;

	~CaptureCamera();
};



} // btrack::processing
#endif // __CAMERA_H__