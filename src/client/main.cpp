#include <iostream>

#include <opencv2/opencv.hpp>

#include "../argparse.hpp"
#include "Client.h"

static constexpr size_t PACKET_SIZE = 4096;

int main(int argc, char* argv[])
{
    argparse::ArgumentParser program("client");

    program.add_argument("hostname")
        .help("hostname of the server");
    program.add_argument("port")
        .help("port of the server")
        .scan<'u', unsigned short>();

    try
    {
        program.parse_args(argc, argv);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << program;
        std::exit(1);
    }

    try
    {
        boost::asio::io_service service;
        Client client(service, program.get<std::string>("hostname"), program.get<unsigned short>("port"));

        cv::Mat frame, send;
        std::vector<unsigned char> encoded;
        cv::VideoCapture cap(0);
        cv::namedWindow("send", cv::WINDOW_AUTOSIZE);

        const std::vector<int> compressionParams{
            cv::IMWRITE_JPEG_QUALITY,
            80
        };

        auto lastTick = std::clock();

        while (true)
        {
            auto start = std::chrono::high_resolution_clock::now();
            cap >> frame;

            if (frame.size().width == 0) continue;

            cv::resize(frame, send, cv::Size(1280, 720), 0, 0, cv::INTER_LINEAR);
            cv::imencode(".jpg", send, encoded, compressionParams);
            cv::imshow("send", send);

            int totalPack = 1 + (encoded.size() - 1) / PACKET_SIZE;
            int intBuf[1];
            intBuf[0] = totalPack;

            auto processed = std::chrono::high_resolution_clock::now();

            client.send(intBuf);

            for (size_t i = 0; i < totalPack; ++i)
            {
                auto it = encoded.begin() + (i * PACKET_SIZE);

                if ((i * PACKET_SIZE + PACKET_SIZE) > encoded.size())
                    client.send(std::vector<unsigned char>(it, encoded.end()));
                else
                    client.send(std::vector<unsigned char>(it, it + PACKET_SIZE));
            }

            auto sent = std::chrono::high_resolution_clock::now();
            std::cout << "Time to process: " << std::chrono::duration_cast<std::chrono::milliseconds>(processed - start).count() << " ms" << std::endl;
            std::cout << "Time to send: " << std::chrono::duration_cast<std::chrono::milliseconds>(sent - processed).count() << " ms" << std::endl;

            cv::waitKey(1000 / 30);

            auto nextTick = std::clock();
            double duration = (nextTick - lastTick) / (double)CLOCKS_PER_SEC;

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
