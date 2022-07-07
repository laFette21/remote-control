#include <iostream>

#include <opencv2/opencv.hpp>

#include "../argparse.hpp"
#include "Client.h"

int main(int argc, char *argv[])
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

        cv::namedWindow("recv", cv::WINDOW_AUTOSIZE);

        size_t sizeMsgSize;
        std::pair<size_t, std::vector<uint8_t>> receivedMsg;
        cv::Mat frame;

        client.send(std::string("Hello"));

        while (true)
        {
            do
            {
                sizeMsgSize = client.receive();
            } while (sizeMsgSize > 3 * sizeof(int));

            std::vector<int> size = client.getSizeBuffer();
            // auto size = sizeMsg.second;

            std::cout << "Expecting " << size[2] << " packets." << std::endl;

            auto start = std::chrono::high_resolution_clock::now();

            std::vector<uint8_t> buffer(size[2] * BUFFER_SIZE);

            for (size_t i = 0; i < size[2]; ++i)
            {
                receivedMsg = client.receive<uint8_t>();

                std::memcpy(&buffer[i * BUFFER_SIZE], receivedMsg.second.data(), receivedMsg.first);
            }

            auto received = std::chrono::high_resolution_clock::now();

            // cv::Mat rawData = cv::Mat(1, BUFFER_SIZE * size[2], CV_8UC1, buffer.data());
            // cv::Mat frame = cv::imdecode(rawData, cv::IMREAD_COLOR);

            frame = cv::Mat(size[0], size[1], CV_8UC3, buffer.data());

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
        }
    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
