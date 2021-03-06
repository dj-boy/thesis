/*
* Copyright (c) 2019 Kosmas Tsiakas
*
* GNU GENERAL PUBLIC LICENSE
*    Version 3, 29 June 2007
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef DRONEMOVEMENTMODEL_H
#define DRONEMOVEMENTMODEL_H

#include <libPF/MovementModel.h>

#include "particle_filter/DroneState.h"

#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/TransformStamped.h>
#include <nav_msgs/Odometry.h>

#include <tf2_ros/transform_listener.h>
#include <tf2_ros/transform_broadcaster.h>
#include <tf2/LinearMath/Transform.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <tf2_ros/message_filter.h>
#include <tf2/LinearMath/Matrix3x3.h>

#include <tf/transform_listener.h>
#include <tf/transform_datatypes.h>

// Message Filters
#include <message_filters/subscriber.h>

class RandomNumberGenerator;

/**
 * @class DroneMovementModel
 *
 * @brief Test class for ParticleFilter.
 *
 * This movement model propagates a drone state according to the translation and
 * rotation speed.
 *
 * @author Stephan Wirth
 */
class DroneMovementModel : public libPF::MovementModel<DroneState>
{
public:
  /**
   * Constructor
   */
  DroneMovementModel(ros::NodeHandle* nh, tf2_ros::Buffer* tfBuffer, const std::string& worldFrameID,
                     const std::string& baseFootprintID, const std::string& baseLinkID);

  /**
   * Destructor
   */
  ~DroneMovementModel();

  /**
   * The drift method propagates the car using its speed.
   * @param state Pointer to the state that has to be manipulated.
   */
  void drift(DroneState& state, double dt) const;

  /**
   * The diffusion consists of a very small gaussian jitter on the
   * state's variable.
   * @param state Pointer to the state that has to be manipulated.
   */
  void diffuse(DroneState& state, double dt) const;

  // param d new standard deviation for the diffusion of x
  void setXStdDev(double d);

  // return the standard deviation for the diffusion of x
  double getXStdDev() const;

  // param d new standard deviation for the diffusion of y
  void setYStdDev(double d);

  // return the standard deviation for the diffusion of y
  double getYStdDev() const;

  // param d new standard deviation for the diffusion of z
  void setZStdDev(double d);

  // return the standard deviation for the diffusion of z
  double getZStdDev() const;

  // param d new standard deviation for the diffusion of roll
  void setRollStdDev(double d);

  // return the standard deviation for the diffusion of roll
  double getRollStdDev() const;

  // param d new standard deviation for the diffusion of pitch
  void setPitchStdDev(double d);

  // return the standard deviation for the diffusion of pitch
  double getPitchStdDev() const;

  // param d new standard deviation for the diffusion of yaw
  void setYawStdDev(double d);

  // param return the standard deviation for the diffusion of yaw
  double getYawStdDev() const;

  // param odom new odometry pose
  void setLastOdomPose(geometry_msgs::PoseStamped& odomPose);

  // Reset movement model
  void reset();

  /// get the last stored odomPose
  /// returns false when there is no valid previous pose stored
  geometry_msgs::PoseStamped getLastOdomPose() const;

  // param currentPose, the current pose that the transform will be applied
  geometry_msgs::TransformStamped computeOdomTransform(geometry_msgs::PoseStamped& currentPose) const;

  // param odomTransform, the transform will be applied to statePose
  void applyOdomTransform(geometry_msgs::TransformStamped& odomTransform, geometry_msgs::Pose& statePose) const;

  /// look up the odom pose at a certain time through tf
  bool lookupOdomPose(ros::Time const& t, geometry_msgs::PoseStamped& pose) const;

  /// looks up the odometry pose at time t and then calls computeOdomTransform()
  bool lookupOdomTransform(const ros::Time& t, geometry_msgs::TransformStamped& odomTransform) const;

  //  Find the transform between base->target in TF Frame tree
  bool lookupTargetToBaseTransform(std::string const& targetFrame, ros::Time const& t,
                                   geometry_msgs::TransformStamped& localTransform) const;

protected:
private:
  /// Stores the random number generator
  libPF::RandomNumberGenerationStrategy* m_RNG;

  bool _odometryReceived;

  std::string _worldFrameID;
  std::string _baseFootprintFrameID;
  std::string _baseLinkFrameID;

  tf2_ros::Buffer* _tfBuffer;
  tf2_ros::TransformListener* _tfListener;

  geometry_msgs::PoseStamped _lastOdomPose;

  // Store the standard deviations of the model
  double _XStdDev;
  double _YStdDev;
  double _ZStdDev;
  double _RollStdDev;
  double _PitchStdDev;
  double _YawStdDev;

  // Initial position which will be used as the center of the Gaussian Distribution
  double _xMean;
  double _yMean;
  double _zMean;
  double _rollMean;
  double _pitchMean;
  double _yawMean;
};

#endif
