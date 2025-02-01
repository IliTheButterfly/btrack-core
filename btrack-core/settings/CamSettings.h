#pragma once

#include <string>
#include <vector>
#include <boost/property_tree/ptree.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/quaternion.hpp>

#include "PTreeConvertible.h"

namespace btrack { namespace settings {

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

    void k1(double v) { distCoeffs(0) = v; }
    void k2(double v) { distCoeffs(1) = v; }
    void k3(double v) { distCoeffs(4) = v; }
    void p1(double v) { distCoeffs(2) = v; }
    void p2(double v) { distCoeffs(3) = v; }

    void cameraMat00(double v) { cameraMatrix(0, 0) = v; }
    void cameraMat01(double v) { cameraMatrix(0, 1) = v; }
    void cameraMat02(double v) { cameraMatrix(0, 2) = v; }
    void cameraMat10(double v) { cameraMatrix(1, 0) = v; }
    void cameraMat11(double v) { cameraMatrix(1, 1) = v; }
    void cameraMat12(double v) { cameraMatrix(1, 2) = v; }
    void cameraMat20(double v) { cameraMatrix(2, 0) = v; }
    void cameraMat21(double v) { cameraMatrix(2, 1) = v; }
    void cameraMat22(double v) { cameraMatrix(2, 2) = v; }

    // Inherited via PTreeConvertible
    void to_ptree(boost::property_tree::ptree& pt) override 
    {
        pt.add("k1", k1());
        pt.add("k2", k2());
        pt.add("k3", k3());
        pt.add("p1", p1());
        pt.add("p2", p2());
        pt.add("cameraMat00", cameraMat00());
        pt.add("cameraMat01", cameraMat01());
        pt.add("cameraMat02", cameraMat02());
        pt.add("cameraMat10", cameraMat10());
        pt.add("cameraMat11", cameraMat11());
        pt.add("cameraMat12", cameraMat12());
        pt.add("cameraMat20", cameraMat20());
        pt.add("cameraMat21", cameraMat21());
        pt.add("cameraMat22", cameraMat22());
    }
    void from_ptree(boost::property_tree::ptree& pt) override
    {
        k1(pt.get<double>("k1"));
        k2(pt.get<double>("k2"));
        k3(pt.get<double>("k3"));
        p1(pt.get<double>("p1"));
        p2(pt.get<double>("p2"));
        cameraMat00(pt.get<double>("cameraMat00"));
        cameraMat01(pt.get<double>("cameraMat01"));
        cameraMat02(pt.get<double>("cameraMat02"));
        cameraMat10(pt.get<double>("cameraMat10"));
        cameraMat11(pt.get<double>("cameraMat11"));
        cameraMat12(pt.get<double>("cameraMat12"));
        cameraMat20(pt.get<double>("cameraMat20"));
        cameraMat21(pt.get<double>("cameraMat21"));
        cameraMat22(pt.get<double>("cameraMat22"));
    }
};

struct CameraExtrinsics : public PTreeConvertible
{
    cv::Point3d position{};
    cv::Quatd rotation{};

    // Inherited via PTreeConvertible
    void to_ptree(boost::property_tree::ptree& pt) override
    {
        boost::property_tree::ptree pos;
        boost::property_tree::ptree rot;
        convert_to_ptree(pos, position);
        convert_to_ptree(rot, rotation);
        pt.add_child("position", pos);
        pt.add_child("rotation", rot);
    }
    void from_ptree(boost::property_tree::ptree& pt) override
    {
        boost::property_tree::ptree pos = pt.get_child("position");;
        boost::property_tree::ptree rot = pt.get_child("rotation");;
        convert_from_ptree(pos, position);
        convert_from_ptree(rot, rotation);
    }
};

struct CameraDeviceInfo : public PTreeConvertible
{
    std::string displayName{};
    std::string deviceName{};
    cv::Size2i resolution{};
    int id = -1;

    // Inherited via PTreeConvertible
    void to_ptree(boost::property_tree::ptree& pt) override
    {
        pt.add("displayName", displayName);
        pt.add("deviceName", deviceName);

        boost::property_tree::ptree res;
        convert_to_ptree(res, resolution);

        pt.add_child("resolution", res);
        pt.add("id", id);
    }
    void from_ptree(boost::property_tree::ptree& pt) override
    {
        displayName = pt.get<std::string>("displayName");
        deviceName = pt.get<std::string>("deviceName");
        boost::property_tree::ptree res = pt.get_child("resolution");
        convert_from_ptree(res, resolution);
        id = pt.get<int>("id");
    }
};

struct CameraSettings : public PTreeConvertible
{
    CameraDeviceInfo deviceInfo{};
    CameraIntrinsics intrinsics{};
    CameraExtrinsics extrinsics{};

    // Inherited via PTreeConvertible
    void to_ptree(boost::property_tree::ptree& pt) override
    {
        boost::property_tree::ptree devi;
        convert_to_ptree(devi, deviceInfo);
        pt.add_child("deviceInfo", devi);
        boost::property_tree::ptree intr;
        convert_to_ptree(intr, intrinsics);
        pt.add_child("intrinsics", intr);
        boost::property_tree::ptree extr;
        convert_to_ptree(extr, extrinsics);
        pt.add_child("extrinsics", extr);
    }
    void from_ptree(boost::property_tree::ptree& pt) override
    {
        boost::property_tree::ptree devi = pt.get_child("deviceInfo");
        convert_from_ptree(devi, deviceInfo);
        boost::property_tree::ptree intr = pt.get_child("intrinsics");
        convert_from_ptree(intr, intrinsics);
        boost::property_tree::ptree extr = pt.get_child("extrinsics");
        convert_from_ptree(extr, extrinsics);
    }
};

}} // btrack::settings