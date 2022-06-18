#include <iostream>

#include <opencv2/opencv.hpp>

#include "Server.h"

int main(int argc, char *argv[])
{
    try
    {
        boost::asio::io_service service;  
        Server server(service);

        cv::namedWindow("recv", cv::WINDOW_AUTOSIZE);

        size_t receivedMsgSize;
        auto lastTick = std::chrono::high_resolution_clock::now();

        while (true)
        {
            do
            {
                receivedMsgSize = server.receive();
            } while (receivedMsgSize > sizeof(int));

            int totalPack = reinterpret_cast<int*>(server.getBuffer().data())[0];

            std::cout << "Expecting " << totalPack << " packets." << std::endl;

            auto start = std::chrono::high_resolution_clock::now();

            std::vector<unsigned char> buffer(totalPack * 4096);

            for (size_t i = 0; i < totalPack; ++i)
            {
                receivedMsgSize = server.receive();

                std::memcpy(&buffer[i * 4096], server.getBuffer().data(), receivedMsgSize);
            }

            auto received = std::chrono::high_resolution_clock::now();

            cv::Mat rawData = cv::Mat(1, 4096 * totalPack, CV_8UC1, buffer.data());
            cv::Mat frame = cv::imdecode(rawData, cv::IMREAD_COLOR);

            if (frame.size().width == 0)
            {
                std::cerr << "Error decoding frame!" << std::endl;
                continue;
            }

            cv::imshow("recv", frame);

            auto showed = std::chrono::high_resolution_clock::now();

            std::cout << "Time to receive: " << std::chrono::duration_cast<std::chrono::milliseconds>(received - start).count() << " ms" << std::endl;
            std::cout << "Time to process: " << std::chrono::duration_cast<std::chrono::milliseconds>(showed - received).count() << " ms" << std::endl;

            cv::waitKey(1);

            auto nextTick = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(nextTick - lastTick).count() / 1000.0;

            std::cout << "FPS: " << (1 / duration) << "\n";

            lastTick = nextTick;
        }
    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
