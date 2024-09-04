#pragma once

#include <string>
#include <vector>
#include <boost/property_tree/ptree.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/quaternion.hpp>

#include "PTreeConvertible.h"

struct CameraIntrinsics : public PTreeConvertible {
    cv::Matx<double, 1, 5> distCoeffs{};
    cv::Matx33d cameraMatrix{};

    double& k1() { return distCoeffs(0); }
    double& k2() { return distCoeffs(1); }
    double& k3() { return distCoeffs(4); }
    double& p1() { return distCoeffs(2); }
    double& p2() { return distCoeffs(3); }

    const double& k1() const { return distCoeffs(0); }
    const double& k2() const { return distCoeffs(1); }
    const double& k3() const { return distCoeffs(4); }
    const double& p1() const { return distCoeffs(2); }
    const double& p2() const { return distCoeffs(3); }

    double& cameraMat00() { return cameraMatrix(0, 0); }
    double& cameraMat01() { return cameraMatrix(0, 1); }
    double& cameraMat02() { return cameraMatrix(0, 2); }
    double& cameraMat10() { return cameraMatrix(1, 0); }
    double& cameraMat11() { return cameraMatrix(1, 1); }
    double& cameraMat12() { return cameraMatrix(1, 2); }
    double& cameraMat20() { return cameraMatrix(2, 0); }
    double& cameraMat21() { return cameraMatrix(2, 1); }
    double& cameraMat22() { return cameraMatrix(2, 2); }

    const double& cameraMat00() const { return cameraMatrix(0, 0); }
    const double& cameraMat01() const { return cameraMatrix(0, 1); }
    const double& cameraMat02() const { return cameraMatrix(0, 2); }
    const double& cameraMat10() const { return cameraMatrix(1, 0); }
    const double& cameraMat11() const { return cameraMatrix(1, 1); }
    const double& cameraMat12() const { return cameraMatrix(1, 2); }
    const double& cameraMat20() const { return cameraMatrix(2, 0); }
    const double& cameraMat21() const { return cameraMatrix(2, 1); }
    const double& cameraMat22() const { return cameraMatrix(2, 2); }

    // Inherited via PTreeConvertible
    void to_ptree(boost::property_tree::ptree& pt) const override;
    void from_ptree(const boost::property_tree::ptree& pt) override;
};

struct CameraExtrinsics : public PTreeConvertible
{
    cv::Point3d position{};
    cv::Quatd rotation{};

    // Inherited via PTreeConvertible
    void to_ptree(boost::property_tree::ptree& pt) const override;
    void from_ptree(const boost::property_tree::ptree& pt) override;
};

struct CameraDeviceInfo : public PTreeConvertible
{
    std::string displayName{};
    std::string deviceName{};
    cv::Size2i resolution{};
    int id = -1;

    // Inherited via PTreeConvertible
    void to_ptree(boost::property_tree::ptree& pt) const override;
    void from_ptree(const boost::property_tree::ptree& pt) override;
};

struct CameraSettings : public PTreeConvertible
{
    CameraDeviceInfo deviceInfo{};
    CameraIntrinsics intrinsics{};
    CameraExtrinsics extrinsics{};

    // Inherited via PTreeConvertible
    void to_ptree(boost::property_tree::ptree& pt) const override;
    void from_ptree(const boost::property_tree::ptree& pt) override;
};
