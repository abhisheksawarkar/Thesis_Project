
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/opencv.hpp"
using namespace cv;
using namespace cv::dnn;
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
using namespace std;
/* Find best class for the blob (i. e. class with maximal probability) */
string ip = "http://192.168.0.100:8080/video?x.mjpeg";
string cascadeName = "haarcascade_frontalface_alt2.xml";

Mat img, frame;
pthread_mutex_t frameLocker1;
dnn::Net net; dnn::Blob inputBlob; dnn::Blob prob;
Ptr<dnn::Importer> importer;
VideoCapture cap;
int counter =0;
double classProb1[5];
int classId1[5];
int size = 5;
Mat image,image_blur,image_gray;
int vrFrameWidth = 1900, vrFrameHeight = 1000;

cv::Mat detectAndDraw( Mat& img, cv::CascadeClassifier& cascade,
                    double scale);
void Gaussian_Blur(int,void*);

int c = 0;

bool terrorist_detected = false;
float probability = 0.0;

bool getMaxClass(dnn::Blob &probBlob, int *classId, double *classProb)
{
    Mat probMat = probBlob.matRefConst().reshape(1, 1); //reshape the blob to 1x1000 matrix
    Point classNumber;

    

    for(int i=0; i<7; i++)
    {
    	minMaxLoc(probMat, NULL, classProb, NULL, &classNumber);
    	classId1[i] = classNumber.x;
    	classProb1[i] = *classProb;
    	probMat.at<int>(0,classNumber.x) = 0;

    	if( (classId1[i]==413|| classId1[i]==764||
			 classId1[i]==499|| classId1[i]==587|| 
			 classId1[i]==597|| classId1[i]==596||
			 classId1[i]==763|| classId1[i]==777))

			{
				cout << "Terrorist!: "<< c <<endl;
				c++;
				//terrorist_detected = 1;
				probability = classProb1[i];
				return true;
			}
		// else
		// {
		// 	terrorist_detected = 0;
		// }

    }

    probability = 0.0;
    return false;
     
    //std::vector<std::vector<double>  out(probMat.begin<double>(), probMat.begin<double>() + probMat.size().width);
    //std::max_element(probMat.begin(),probMat.end());
    
     //std::sort(out.begin(), out.end());
}

std::vector<String> readClassNames(const char *filename = "synset_words.txt")
{
    std::vector<String> classNames;
    std::ifstream fp(filename);
    if (!fp.is_open())
    {
        std::cerr << "File with classes labels not found: " << filename << std::endl;
        exit(-1);
    }
    std::string name;
    while (!fp.eof())
    {
        std::getline(fp, name);
        if (name.length())
            classNames.push_back( name.substr(name.find(' ')+1) );
    }
    fp.close();
    return classNames;
}


void *UpdateFrame1(void *args)
{
	
    for(;;)
    {
         //Mat tempFrame;
        // cap >> tempFrame;
		
         counter++;

         if(counter%5 == 0)
        {
        		 		
        	if(!frame.empty())
        	{
        	pthread_mutex_lock(&frameLocker1);
        	//resize(frame,tempFrame,Size(227, 227));
        	//cvt(tempFrame,tempFrame,CV_BGR2GRAY);

       		inputBlob = dnn::Blob(frame);   //Convert Mat to dnn::Blob image batch
    		net.setBlob(".data", inputBlob);        //set the network input
    		net.forward();                          //compute output
    		prob = net.getBlob("prob");   //gather output of "prob" layer
    		int classId;
    		double classProb;
    		terrorist_detected = getMaxClass(prob, &classId, &classProb);//find the best class//find the best class
     		//std::vector<String> classNames = readClassNames();
    
    		// std::cout << "Best class: #" << classId << " '" << classNames.at(classId) << "'" << std::endl;
   			// std::cout << "Probability: " << classProb * 100 << "%" << std::endl;
     		
    //  		for(int i =0; i<7; i++)
    //  		{
    //  			// std::cout  << "Best class: #" << classId1[i] << " '" << classNames.at(classId1[i]) << "'" << std::endl;
    //  			// std::cout  << "Probability: " << classProb1[i] * 100 << "%" << std::endl;
				// //cout<<"Here!"<<endl;
				
    //  		}
    		
    		//cout<<endl;
     	   	pthread_mutex_unlock(&frameLocker1);

     		}
     		counter = 0;
   		 }
    }
    
}



int main(int argc, char **argv)
{
    String modelTxt = "bvlc_googlenet.prototxt";
    String modelBin = "bvlc_googlenet.caffemodel";
    namedWindow("Robot_view", WINDOW_AUTOSIZE);
    //String imageFile = (argc > 1) ? argv[1] : "Image49.jpg";
    cap.open(ip); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    cv::CascadeClassifier cascade;

     if( !cascade.load( cascadeName ) )
    {
        cout << "ERROR: Could not load classifier cascade" << endl;
        return -1;
    }
    
    pthread_mutex_init(&frameLocker1,NULL);
   // pthread_mutex_init(&frameLocker2,NULL);  

    pthread_t UpdThread1;    

    try                                     //Try to import Caffe GoogleNet model
    {
        importer = dnn::createCaffeImporter(modelTxt, modelBin);
    }
    catch (const cv::Exception &err)        //Importer can throw errors, we will catch them
    {
        std::cerr << err.msg << std::endl;
    }
    if (!importer)
    {
        std::cerr << "Can't load network by using the following files: " << std::endl;
        std::cerr << "prototxt:   " << modelTxt << std::endl;
        std::cerr << "caffemodel: " << modelBin << std::endl;
        std::cerr << "bvlc_googlenet.caffemodel can be downloaded here:" << std::endl;
        std::cerr << "http://dl.caffe.berkeleyvision.org/bvlc_googlenet.caffemodel" << std::endl;
        exit(-1);
    }
    
    importer->populateNet(net);
    importer.release();                     //We don't need importer anymore
    //Mat img = imread(imageFile);
           //GoogLeNet accepts only 224x224 RGB-images
     

    //Mat frame;
    int k = pthread_create(&UpdThread1, NULL, UpdateFrame1, NULL);
     	
     	if(k!=0)
    	{
    		std::cerr << "Cannot create thread! " << endl;
    	}

     while(true)
    {

        //Mat currentFrame1;
        cap >> img;
        int co =0;
         if (img.empty())
    	{
        	std::cerr << "Can't read image from camera: " << co++ << endl;
        	continue;
        	if(co==10)
        		return -1;
    	}

    	//currentFrame1 = img;
    	image = img.clone();

    			Gaussian_Blur(0,0);
		cvtColor( image_blur, image_gray, CV_BGR2GRAY );
    	frame = detectAndDraw(image_gray, cascade, 1.3);

    	//imshow("frame", currentFrame1);
        //cap >> img; // get a new frame from camera
        
    
        
    
   
         if(waitKey(30) >= 0) break;
    }

    //imshow("image",img);
    // waitKey(500);
    // if (img.empty())
    // {
    //     std::cerr << "Can't read image from the file: " << imageFile << std::endl;
    //     exit(-1);
    // }
    
    
    return 0;
} //main


cv::Mat detectAndDraw( Mat& img_gray, cv::CascadeClassifier& cascade,
                    double scale)
{
    double t = 0;
    vector<Rect> faces;
    Mat gray, smallImg;
    Mat frameLeft, frameRight, vrFrame(Size(vrFrameWidth, vrFrameHeight), CV_8UC3);
    //cvtColor( img, gray, COLOR_BGR2GRAY );
    std::ostringstream ss;
	ss << probability*100;


    double fx = 1 / scale;
    resize( image_gray, smallImg, Size(), fx, fx, INTER_LINEAR );
    equalizeHist( smallImg, smallImg );

    //t = (double)cvGetTickCount();
    
    cascade.detectMultiScale( smallImg, faces,
        scale, 3, 0
        |CASCADE_FIND_BIGGEST_OBJECT
        //|CASCADE_DO_ROUGH_SEARCH
        |CASCADE_SCALE_IMAGE,
        Size(30, 30) );
    
    Mat cnn_roi;
    Mat temp1 = img.clone();
    Rect person,face;
    int personLeftX_final=0;
    int personLeftY_final=0;
    //t = (double)cvGetTickCount() - t;
    //printf( "detection freq = %g ms\n", ((double)cvGetTickFrequency()*1000.)/t );
    for ( size_t i = 0; i < faces.size(); i++ )
    {
         face = faces[i];
        double aspect_ratio = (double)face.width/face.height;
        if(0.85 < aspect_ratio && aspect_ratio < 1.1 )
        {
        	personLeftX_final = face.x*scale-face.width*2;
            personLeftY_final = face.y*scale-35;

            person = Rect((personLeftX_final),(personLeftY_final),face.width*6,face.height*8);
            
            Rect imgBounds(0,0,image.cols,image.rows);
            person = person & imgBounds;
            cnn_roi = temp1(person);
            resize(cnn_roi,cnn_roi,Size(227, 227));

           rectangle( image, cvPoint(cvRound(face.x*scale), cvRound(face.y*scale)),
                      cvPoint(cvRound((face.x + face.width-1)*scale), cvRound((face.y + face.height-1)*scale)),Scalar(255,0,0), 3, 8, 0);       

     		if(terrorist_detected==true)
     		{
     			rectangle( image, person,Scalar(0,0,255), 6, 8, 0);
     			putText(image,ss.str() + " Terrorist!",Point(personLeftX_final,personLeftY_final),FONT_HERSHEY_SIMPLEX,2,Scalar::all(0),5,8);	
     		}
     		

     		if(terrorist_detected==false)
     		{
				rectangle( image, person,Scalar(0,255,0), 3, 8, 0);
				putText(image,ss.str() ,Point(personLeftX_final,personLeftY_final),FONT_HERSHEY_SIMPLEX,2,Scalar::all(0),5,8);	
     				
     		}
                                                          
         }
      }
    

    	resize(image, frameLeft, Size((vrFrameWidth / 2)-5, vrFrameHeight), 0, 0, INTER_CUBIC);
		resize(image, frameRight, Size((vrFrameWidth / 2)-5, vrFrameHeight), 0, 0, INTER_CUBIC);
		frameLeft.copyTo(vrFrame(Rect(0, 0, (vrFrameWidth / 2)-5, vrFrameHeight)));
		frameRight.copyTo(vrFrame(Rect((vrFrameWidth / 2)+5, 0, (vrFrameWidth / 2)-5, vrFrameHeight)));
    
    
    imshow( "Robot_view", vrFrame);
	return cnn_roi;
    
}

void Gaussian_Blur(int,void*)
{
	cv::GaussianBlur(image,image_blur,Size((2*size)+1,(2*size)+1),1.5,1.5);

}
