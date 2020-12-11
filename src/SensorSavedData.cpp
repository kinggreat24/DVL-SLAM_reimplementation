//
// Created by jongsik on 20. 12. 11..
//

#include "SensorSavedData.h"


SensorSavedData::SensorSavedData(Config &config)
: Sensor(config)
, config_(config)
{
}

SensorSavedData::~SensorSavedData()
{
}

void SensorSavedData::data2Frame(Frame& frame)
{
  frame.SetOriginalImg(inputImg_);
  if (lidarFlag_)
  {
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr transformedCloud = pcl::PointCloud<pcl::PointXYZRGB>::Ptr(new pcl::PointCloud<pcl::PointXYZRGB>);
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr rectifiedCloud = pcl::PointCloud<pcl::PointXYZRGB>::Ptr(new pcl::PointCloud<pcl::PointXYZRGB>);

    Eigen::Matrix4f transform = Eigen::Matrix4f::Identity();

    transform(0, 0) = config_.r11;
    transform(0, 1) = config_.r12;
    transform(0, 2) = config_.r13;

    transform(1, 0) = config_.r21;
    transform(1, 1) = config_.r22;
    transform(1, 2) = config_.r23;

    transform(2, 0) = config_.r31;
    transform(2, 1) = config_.r32;
    transform(2, 2) = config_.r33;

    transform(0, 3) = config_.delX;
    transform(1, 3) = config_.delY;
    transform(2, 3) = config_.delZ;

    pcl::transformPointCloud(*inputCloud_, *transformedCloud, transform);

    Eigen::Matrix4f transformRect = Eigen::Matrix4f::Identity();

    transformRect(0, 0) = config_.R11;
    transformRect(0, 1) = config_.R12;
    transformRect(0, 2) = config_.R13;

    transformRect(1, 0) = config_.R21;
    transformRect(1, 1) = config_.R22;
    transformRect(1, 2) = config_.R23;

    transformRect(2, 0) = config_.R31;
    transformRect(2, 1) = config_.R32;
    transformRect(2, 2) = config_.R33;

    transformRect(0, 3) = 0;
    transformRect(1, 3) = 0;
    transformRect(2, 3) = 0;

    pcl::transformPointCloud(*transformedCloud, *rectifiedCloud, transformRect);

    pcl::PassThrough<pcl::PointXYZRGB> pass;
    pass.setInputCloud(rectifiedCloud);
    pass.setFilterFieldName("z");
    pass.setFilterLimits(minZ, maxZ);
    pass.filter(*rectifiedCloud);

    frame.SetOriginalCloud(*rectifiedCloud);
  }
}

void SensorSavedData::loadImg()
{

}

void SensorSavedData::loadPointCloud()
{

}