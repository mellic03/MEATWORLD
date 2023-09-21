#include "hands_opencv.hpp"



void
Hands_OpenCV::init( idk::Engine &engine )
{
    cv::Mat image = cv::imread("lena_color.jpg");
    cv::Mat edges;
    cv::Canny(image, edges, 50, 200);
    cv::imwrite("edges.jpg", edges);
}


void
Hands_OpenCV::onAssignment( int obj_id, idk::Engine &engine )
{

}


void
Hands_OpenCV::onGameObjectCreation( int obj_id, idk::Engine &engine )
{

}


void
Hands_OpenCV::onGameObjectDeletion( int obj_id, idk::Engine &engine )
{

}


void
Hands_OpenCV::onGameObjectCopy( int src_obj_id, int dest_obj_id, idk::Engine &engine )
{

}

