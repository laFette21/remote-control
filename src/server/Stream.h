#ifndef STREAM_H
#define STREAM_H

#include <mutex>

#include <opencv2/opencv.hpp>

class Stream
{
public:
    Stream() { init(); }
    Stream(int device, int api): apiId(api), deviceId(device) { init(); }
    Stream(const Stream&) = delete;
    Stream& operator=(const Stream&) = delete;

    cv::Mat *read() { return &frame; }
    void start();
    void stop() { running = false; }
    void update();

private:
    void init();

    int apiId = cv::CAP_ANY;
    int deviceId = 0;
    bool running = true;
    cv::VideoCapture capture;
    cv::Mat frame;
    std::mutex mutex;
};

#endif // STREAM_H