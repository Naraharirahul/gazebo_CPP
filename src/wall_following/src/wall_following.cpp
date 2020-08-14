#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <geometry_msgs/Twist.h>
#include <vector>
#include <math.h>

ros::Publisher pub;

void scan_callback(const sensor_msgs::LaserScan::ConstPtr &data1){
 
    size_t range_size = data1->ranges.size();

    float left_center = 0.0, right_center = 0.0;
    int left_count = 0, right_count = 0;

    for(int i = 0; i < range_size ; i++){
        if(i > 20 && i < 80){
            if(!isinf(data1->ranges[i])){
            left_center += data1->ranges[i];
            left_count += 1 ;
            }
        }
        if(i> 280 && i<360){
            if(!isinf(data1->ranges[i])){
            right_center += data1->ranges[i];
            right_count += 1;
            }
        }
    }
    
    float avgr = right_center/right_count, avgl = left_center/left_count;
    float error = avgr - avgl;
    float kp = 0.4;

    geometry_msgs::Twist data2;

    if(error > 0.05){
        data2.angular.z = -kp * error ;
        data2.linear.x = 0.2;  
    }        
    else if(error < -0.05){
        data2.angular.z  =  -kp * error;
        data2.linear.x = 0.2;         
    }
    else{
        data2.angular.z  =  0;
        data2.linear.x = 0.2; 
            }
    pub.publish(data2); 
        
}
int main(int argc, char** argv){
    ros::init (argc, argv, "wall_following");
    ros::NodeHandle n; 
    pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
    ros::Subscriber sub = n.subscribe("/scan", 10, scan_callback);   
    ros::spin();
    return 0;
}
