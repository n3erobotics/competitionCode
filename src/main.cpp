#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <cvblob.h>

#include "TrackHandler.h"
#include "Control.h"
#include "SignalInterpretation.h"
#include "Utils.h"
#include "SerialPort.h"

#define NUM_THREADS 2

//blob defines
#define h 480
#define w 640

using namespace std;
using namespace cv;
using namespace cvb;

extern SerialPort serialPort;
extern VideoCapture webcamL, webcamR, webcamT;
extern pthread_mutex_t access_change;
extern bool wanna_change;

CvCapture* capture=cvCreateCameraCapture(0);
CvBlobs blobs;
IplImage *fram=cvCreateImage(cvSize(w,h),8,3);   //Original Image
IplImage *hsvframe=cvCreateImage(cvSize(w,h),8,3);//Image in HSV color space
IplImage *labelImg=cvCreateImage(cvSize(w,h),IPL_DEPTH_LABEL,1);//Image Variable for blobs
IplImage *threshy=cvCreateImage(cvSize(w,h),8,1); //Threshold image of yellow color
IplImage* img=cvCreateImage(cvSize(w,h),8,3);

int blob_tracing(){


	//IplImage* img_bw=cvCreateImage(cvSize(w,h),8,1);
	cvZero(img);
	//Getting the current frame
	IplImage *fram=cvQueryFrame(capture);
	//If failed to get break the loop
	if(!fram){
		return 0;
	}
	//Changing the color space
	cvCvtColor(fram,hsvframe,CV_BGR2HSV);
	//Thresholding the frame for yellow
	cvInRangeS(hsvframe, lowHSV,highHSV,threshy);
	//Filtering the frame
	cvSmooth(threshy,threshy,CV_MEDIAN,5,5);
	//Finding the blobs
	cvLabel(threshy,labelImg,blobs);
	//Filtering the blobs
	cvFilterByArea(blobs, MIN_AREA,999999999);
	//Rendering the blobs
	cvRenderBlobs(labelImg,blobs,img, img);

	return blobs.size();
}

int main()
{
	int rc;
	pthread_t threads[NUM_THREADS];
	string buffer;

	signal (SIGINT, SIG_handler);
	signal (SIGSEGV, SIG_handler);

	int err=serialPort.connect(ARDUINO_CONTROL);

	//	 TODO Uncomment bellow
	if(err==-1){
		cout << "Serial communication not established!" << endl;
		exit(-1);
	}

	cout << "Creating Signal Interpretation thread" << endl;
	rc = pthread_create(&threads[1], NULL, signalInterpretation, (void *)NULL);
	if (rc){
		cout << "ERROR; return code from pthread_create() is "<< rc << endl;
		exit(-1);
	}

	sleep(1);

	cout << "Creating Track Handler thread" << endl;
	rc = pthread_create(&threads[0], NULL, trackHandler, (void *)NULL);
	if (rc){
		cout << "ERROR; return code from pthread_create() is "<< rc << endl;
		exit(-1);
	}

	//	cout << " Waiting ..." << endl;


	//	while(true){
	//		if(blob_tracing()){
	//			pthread_mutex_lock(&access_change);
	//
	//			wanna_change=true;
	//
	//			pthread_mutex_unlock(&access_change);
	//
	//			sleep(7);
	//			pthread_mutex_lock(&access_change);
	//
	//			wanna_change=true;
	//
	//			pthread_mutex_unlock(&access_change);
	//		}
	//	}


	//	sleep(3);
	//	pthread_mutex_lock(&access_change);
	//
	//	wanna_change=true;
	//
	//	pthread_mutex_unlock(&access_change);
	//
	//	sleep(7);
	//	pthread_mutex_lock(&access_change);
	//
	//	wanna_change=true;
	//
	//	pthread_mutex_unlock(&access_change);

	pthread_join(threads[0], NULL);
	pthread_join(threads[1], NULL);

	webcamL.release();
	webcamR.release();
	webcamT.release();

	cout << "Terminating" << endl;
	exit(0);
}
