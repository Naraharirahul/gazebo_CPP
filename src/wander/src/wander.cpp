#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <geometry_msgs/Twist.h>
#include <vector>
#include <math.h>

ros::Publisher pub;

void scan_callback(const sensor_msgs::LaserScan::ConstPtr &data1){
    
    
    size_t range_size = data1->ranges.size();

    float left_sweep = 0.0;
    float left_count = 0, right_count = 0, front_count = 0;
    float right_sweep = 0.0;
    float front_sweep = 0.0;
    for(int i = 0; i < range_size ; i++){ 
        if(i > 45 && i<90){
            if(!isinf(data1->ranges[i])){
            left_sweep += data1->ranges[i];
            left_count +=1;
            }
        }

        if(i > 0 && i<45){
            if(!isinf(data1->ranges[i])){
                front_sweep = data1->ranges[i];
                front_count += 1;
            }
        }

        if(i > 270 && i<315){
            if(!isinf(data1->ranges[i])){
                right_sweep += data1->ranges[i];
                right_count += 1;
            }
        }
    }

    geometry_msgs::Twist data2;

    float avgr = right_sweep/right_count;
    float avgl = left_sweep/left_count;
    float avgf = front_sweep/front_count;
    
    if(avgf > 5 or avgf < -5){
        avgf = front_sweep;
    }
    if(avgr > 5 or avgr < -5){
        avgf = right_sweep;
    }
    if(avgl > 5 or avgl < -5){
        avgl = front_sweep;
    }
 
    if(avgl > avgr and avgf){
        data2.linear.x = 0.1;
        data2.angular.z = 0.8;
    }
    
    if(avgf > avgl and avgr){
        data2.linear.x = 0.1;
        data2.angular.z = 0;
    }

    if(avgr > avgl and avgf){
        data2.linear.x = 0.1;
        data2.angular.z = -0.8;
    }  
    
    pub.publish(data2); 
        
}
int main(int argc, char** argv){
    ros::init (argc, argv, "wall_following");
    ros::NodeHandle n; 
    
    // ros::Subscriber sub1 = n.subscribe("/", 1000, scan_callback1);
    pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
    ros::Subscriber sub = n.subscribe("/scan", 10, scan_callback);
    
    
    ros::spin();
    return 0;
}
