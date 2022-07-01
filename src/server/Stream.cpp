#include <thread>

#include "Stream.h"

void Stream::init()
{
    capture.open(deviceId, apiId);
    if (!capture.isOpened())
    {
        throw std::runtime_error("Failed to open camera!");
    }

    capture.read(frame);
    if (frame.empty())
    {
        std::cerr << "Failed to read frame!\n";
    }
}

void Stream::start()
{
    std::thread thread(&Stream::update, this);
    thread.detach();
}

void Stream::update()
{
    while (running)
    {
        std::unique_lock<std::mutex> lock(mutex);

        capture.read(frame);
        if (frame.empty())
        {
            std::cerr << "Failed to read frame!\n";
            return;
        }

        lock.unlock();
    }
}
