#include <iostream>

#include <opencv2/opencv.hpp>
#include <pylon/PylonIncludes.h>

#include "../argparse.hpp"
#include "Server.h"
// #include "Stream.h"

int main(int argc, char* argv[])
{
    argparse::ArgumentParser program("server");

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
        Server server(service, program.get<unsigned short>("port"));
        // Stream stream;

        Pylon::PylonInitialize();
        Pylon::CInstantCamera camera(Pylon::CTlFactory::GetInstance().CreateFirstDevice());
        std::cout << "Using device " << camera.GetDeviceInfo().GetModelName() << std::endl;
        camera.MaxNumBuffer = 5;

        Pylon::CImageFormatConverter formatConverter;
		formatConverter.OutputPixelFormat = Pylon::PixelType_BGR8packed;

        Pylon::CGrabResultPtr ptrGrabResult;
        Pylon::CPylonImage pylonImage;
        cv::Mat frame, send;
        std::vector<unsigned char> encoded;
        cv::namedWindow("send", cv::WINDOW_AUTOSIZE);

        const std::vector<int> compressionParams{
            cv::IMWRITE_JPEG_QUALITY,
            80
        };

        server.receive();
        // stream.start();
        camera.StartGrabbing(Pylon::GrabStrategy_LatestImageOnly);

        while (camera.IsGrabbing())
        {
            auto start = std::chrono::high_resolution_clock::now();

            // frame = *stream.read();
            camera.RetrieveResult(5000, ptrGrabResult, Pylon::TimeoutHandling_ThrowException);

            if (ptrGrabResult->GrabSucceeded())
            {
                formatConverter.Convert(pylonImage, ptrGrabResult);
                frame = cv::Mat(ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(), CV_8UC3, (uint8_t *)pylonImage.GetBuffer());

                auto framed = std::chrono::high_resolution_clock::now();

                cv::resize(frame, send, cv::Size(1280, 720), 0, 0, cv::INTER_LINEAR);
                cv::imencode(".jpg", send, encoded, compressionParams);
                cv::imshow("send", send);

                size_t totalPack = 1 + (encoded.size() - 1) / BUFFER_SIZE;
                size_t intBuf[1];
                intBuf[0] = totalPack;

                auto processed = std::chrono::high_resolution_clock::now();

                server.send(intBuf);

                for (size_t i = 0; i < totalPack; ++i)
                {
                    auto it = encoded.begin() + (i * BUFFER_SIZE);

                    if ((i * BUFFER_SIZE + BUFFER_SIZE) > encoded.size())
                        server.send(std::vector<unsigned char>(it, encoded.end()));
                    else
                        server.send(std::vector<unsigned char>(it, it + BUFFER_SIZE));
                }

                auto sent = std::chrono::high_resolution_clock::now();
                std::cout << "Time to read frame: " << std::chrono::duration_cast<std::chrono::milliseconds>(framed - start).count() << " ms" << std::endl;
                std::cout << "Time to process: " << std::chrono::duration_cast<std::chrono::milliseconds>(processed - framed).count() << " ms" << std::endl;
                std::cout << "Time to send: " << std::chrono::duration_cast<std::chrono::milliseconds>(sent - processed).count() << " ms" << std::endl;
            }

            cv::waitKey(1);
        }
    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    Pylon::PylonTerminate();

    return 0;
}
