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
        // camera.ImageCompressionMode.SetValue(Pylon::ImageCompressionMode_BaslerCompressionBeyond);
        // camera.ImageCompressionRateOption.SetValue(Pylon::ImageCompressionRateOption_Lossless);

        Pylon::CImageFormatConverter formatConverter;
		formatConverter.OutputPixelFormat = Pylon::PixelType_BGR8packed;

        Pylon::CGrabResultPtr ptrGrabResult;
        Pylon::CPylonImage pylonImage;
        // cv::Mat frame, send;
        
        // cv::namedWindow("send", cv::WINDOW_AUTOSIZE);

        // const std::vector<int> compressionParams{
        //     cv::IMWRITE_JPEG_QUALITY,
        //     80
        // };

        server.receive<char>();
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
                // frame = cv::Mat(ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(), CV_8UC3, (uint8_t *)pylonImage.GetBuffer());

                auto framed = std::chrono::high_resolution_clock::now();

                // cv::resize(frame, send, cv::Size(1280, 720), 0, 0, cv::INTER_LINEAR);
                // cv::imencode(".jpg", send, encoded, compressionParams);
                // cv::imshow("send", send);

                // Pylon::DisplayImage(1, ptrGrabResult);
                std::vector<int> size{
                    ptrGrabResult->GetHeight(),
                    ptrGrabResult->GetWidth(),
                };

                uint8_t* arr = (uint8_t *)pylonImage.GetBuffer();
                std::vector<uint8_t> data(&arr[0], &arr[size[0] * size[1] * 3]);

                // std::memcpy(data.data(), arr, size[0] * size[1] * 3);

                size.push_back(1 + (data.size() - 1) / BUFFER_SIZE);

                auto processed = std::chrono::high_resolution_clock::now();

                server.send(size);

                for (size_t i = 0; i < size[2]; ++i)
                {
                    auto it = data.begin() + (i * BUFFER_SIZE);

                    if ((i * BUFFER_SIZE + BUFFER_SIZE) > data.size())
                        server.send(std::vector<uint8_t>(it, data.end()));
                    else
                        server.send(std::vector<uint8_t>(it, it + BUFFER_SIZE));
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
