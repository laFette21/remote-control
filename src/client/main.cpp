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

        size_t receivedMsgSize;

        client.send(std::string("Hello"));

        while (true)
        {
            do
            {
                receivedMsgSize = client.receive();
            } while (receivedMsgSize > sizeof(int));

            size_t totalPack = reinterpret_cast<int*>(client.getBuffer().data())[0];

            std::cout << "Expecting " << totalPack << " packets." << std::endl;

            auto start = std::chrono::high_resolution_clock::now();

            std::vector<unsigned char> buffer(totalPack * BUFFER_SIZE);

            for (size_t i = 0; i < totalPack; ++i)
            {
                receivedMsgSize = client.receive();

                std::memcpy(&buffer[i * BUFFER_SIZE], client.getBuffer().data(), receivedMsgSize);
            }

            auto received = std::chrono::high_resolution_clock::now();

            cv::Mat rawData = cv::Mat(1, BUFFER_SIZE * totalPack, CV_8UC1, buffer.data());
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
        }
    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
