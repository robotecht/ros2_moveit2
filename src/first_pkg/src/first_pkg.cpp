// Include memory
#include<memory>
// ROS client library C++
#include<rclcpp/rclcpp.hpp>
// Include moveit planner interface
#include<moveit/move_group_interface/move_group_interface.h>
// Library to convert the Euler angles to Quartenion
#include<tf2_geometry_msgs/tf2_geometry_msgs.hpp>
// Add library to handle quaternions
#include<tf2/LinearMath/Quaternion.h>

// Write the main function
int main(int argc, char * argv[])
{
  // Initialize ROS and create the Node
  // Initiate rclcppp
  rclcpp::init(argc,argv);
  // Create the node
  auto const node = std::make_shared<rclcpp::Node>(
    // Name the node
    "first_pkg", 
    //declare it as a node
    rclcpp::NodeOptions().automatically_declare_parameters_from_overrides(true)
  );
  // Create a ROS logger to record the messages in terminal
  auto const logger = rclcpp::get_logger("first_pkg");

  //intialize the planning group intrface
  //pass node object created as the node
  // pass panda_arm as the movegroup
  // Create the MoveIt MoveGroup Interface
  moveit::planning_interface::MoveGroupInterface MoveGroupInterface(node, "panda_arm");

  // Define goal position
  // Create a quaternion variable to define goal orientation
  tf2::Quaternion tf2_quat;
  // Set goal orinetation in raidans - Roll, Pitch, Yaw
  tf2_quat.setRPY(0,3.14/2,0);
  // convert this setted orientation as a geometry_msg of Quaternion type
  geometry_msgs::msg::Quaternion msg_quat = tf2::toMsg(tf2_quat);

  // Set the goal position using geometry_msgs and pass the orientation
  // Goal position is calculated based on the base of the robot
  geometry_msgs::msg::Pose GoalPose;
  GoalPose.orientation = msg_quat;
  GoalPose.position.x = -0.2;
  GoalPose.position.y = 0.4;
  GoalPose.position.z = 0.3;

  // set the target position for the movegroup
  MoveGroupInterface.setPoseTarget(GoalPose);

  // Now create a motion plan towards this target positon
  // Naming this plan as first plan
  moveit::planning_interface::MoveGroupInterface::Plan first_plan;
  // Check if a plan can be genrated from current position to target position
  auto const outcome = static_cast<bool>(MoveGroupInterface.plan(first_plan));

  // If the plan exists execute the plan, if not show an error message
  if(outcome)
  {
    MoveGroupInterface.execute(first_plan);
  }
  else
  {
    RCLCPP_ERROR(logger,"Plan generation failed from current position to target position");
  }

  // Task is completed shutdown ROS
  rclcpp::shutdown();
  return 0;
}





