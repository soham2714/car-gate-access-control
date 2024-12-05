#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <iostream>

using namespace cv;
using namespace std;


vector<string> db{"Amey wale","9988261601"};

bool validateUser(string inputData){

    for (string x: db){
        if (inputData == x){
            return true;
        }
    }

    return false;
}


int main(int argc, char** argv)
{
    utils::logging::setLogLevel(utils::logging::LOG_LEVEL_SILENT);

    // Create a VideoCapture object to capture images from the camera
    VideoCapture cap("http://192.168.201.92:8080/video");
    // VideoCapture cap("http://192.168.47.249:8080/video");
    
    // VideoCapture cap(0);

    if (!cap.isOpened()) {
        cout << "Error: Unable to open the camera" << endl;
        return -1;
    }

    // Set the camera resolution
    // cap.set(CAP_PROP_FRAME_WIDTH, 640);
    // cap.set(CAP_PROP_FRAME_HEIGHT, 480);
    cap.set(CAP_PROP_FRAME_WIDTH, 1920);
    cap.set(CAP_PROP_FRAME_HEIGHT, 1080);

    // Create a window to display the camera feed
    namedWindow("Camera", WINDOW_NORMAL);

    while (true) {
        // Capture a frame from the camera
        Mat frame;
        cap >> frame;
        
        // Convert the frame to grayscale
        resize(frame,frame,Size(),0.50,0.50);

        Mat gray;
        cvtColor(frame, gray, COLOR_BGR2GRAY);

        // Create a QRCodeDetector object
        QRCodeDetector qrCodeDetector;

        // Detect and decode the QR code
        vector<Point> points;
        string qrCodeData = qrCodeDetector.detectAndDecode(gray, points);

        // Print the QR code data
        if (!qrCodeData.empty()) {

            if (validateUser(qrCodeData)){
                cout << "QR Code detected: " << qrCodeData << endl;
                cout << "Entry allowed, gate opens" << endl;
                break;
            }
            else {  
                cout << "QR Code Invalid" << endl;
                break;
            }
            // Draw the bounding box around the QR code
            for (int i = 0; i < 4; i++) {
                line(frame, points[i], points[(i + 1) % 4], Scalar(0, 0, 255), 2);
            }

        }

        // Show the camera feed with the QR code bounding box
        imshow("Camera", frame);

        // Exit the loop if the 'q' key is pressed
        if (waitKey(1) == 'q') {
            break;
        }
    }

    // Release the camera and close the window
    cap.release();
    destroyAllWindows();

    return 0;
}
