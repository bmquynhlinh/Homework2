//include openCV
#include <opencv2\opencv.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\core.hpp>
#include <iostream>

using namespace cv;
using namespace std;
 
// starting point for the application
void showHistogram(std::vector<cv::Mat>& hists)
{
	// Min/Max computation
	double hmax[3] = { 0,0,0 };
	double min;
	cv::minMaxLoc(hists[0], &min, &hmax[0]);
	cv::minMaxLoc(hists[1], &min, &hmax[1]);
	cv::minMaxLoc(hists[2], &min, &hmax[2]);

	std::string wname[3] = { "blue", "green", "red" };
	cv::Scalar colors[3] = { cv::Scalar(255,0,0), cv::Scalar(0,255,0),
							 cv::Scalar(0,0,255) };

	std::vector<cv::Mat> canvas(hists.size());

	// Display each histogram in a canvas
	for (int i = 0, end = hists.size(); i < end; i++)
	{
		canvas[i] = cv::Mat::ones(125, hists[0].rows, CV_8UC3);

		for (int j = 0, rows = canvas[i].rows; j < hists[0].rows - 1; j++)
		{
			cv::line(
				canvas[i],
				cv::Point(j, rows),
				cv::Point(j, rows - (hists[i].at<float>(j) * rows / hmax[i])),
				hists.size() == 1 ? cv::Scalar(200, 200, 200) : colors[i],
				1, 8, 0
			);
		}

		cv::imshow(hists.size() == 1 ? "value" : wname[i], canvas[i]);
	}
}

void Part11(Mat img)
{
	///////////////////////////////////////////////////
    /////1. SHOWING HISTOGRAM/////////////////////////
    /////////////////////////////////////////////////
	//Split into R, G,B images
	Mat brg_planes[3];
	split(img, brg_planes);


	// Set up value and type
	vector<Mat> hists;
	Mat b_hist, g_hist, r_hist;
	int histSize = 256;
	float hranges[] = { 0, 255 };

	//float sranges[] = { 0, 256 };
	const float* histRange[] = { hranges };

	//Calculate the histogram
	cv::calcHist(&brg_planes[0], 1, 0, Mat(), r_hist, 1, &histSize, histRange, true, false);
	cv::calcHist(&brg_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, histRange, true, false);
	cv::calcHist(&brg_planes[2], 1, 0, Mat(), b_hist, 1, &histSize, histRange, true, false);

	//Add each R,G,B histogram to 1 vector Mat
	hists.push_back(r_hist);
	hists.push_back(g_hist);
	hists.push_back(b_hist);

	//Show histogram
	showHistogram(hists);
}
void Part12(Mat img) {
	//////////////////////////////////////////////
	////2. EQUALIZING THE IMAGE//////////////////
	////////////////////////////////////////////



	// Set up value and type
	vector<Mat> hists, equaliRGB, imageRGB;
	Mat b_hist, g_hist, r_hist, b_dst, g_dst, r_dst;;
	int histSize = 256;
	float hranges[] = { 0, 255 };

	// Split into R,G,B
	Mat brg_planes[3];
	split(img, brg_planes);

	const float* histRange[] = { hranges };


	//  Equalizing each R, G, B images
	cv::equalizeHist(brg_planes[0], r_dst);
	cv::equalizeHist(brg_planes[1], g_dst);
	cv::equalizeHist(brg_planes[2], b_dst);
	//cout << r_dst;

	Mat be_hist, ge_hist, re_hist;

	// Calculate the histogram
	cv::calcHist(&r_dst, 1, 0, Mat(), re_hist, 1, &histSize, histRange, true, false);
	cv::calcHist(&g_dst, 1, 0, Mat(), ge_hist, 1, &histSize, histRange, true, false);
	cv::calcHist(&b_dst, 1, 0, Mat(), be_hist, 1, &histSize, histRange, true, false);

	//Add each R,G,B histogram to 1 vector Mat
	equaliRGB.push_back(re_hist);
	equaliRGB.push_back(ge_hist);
	equaliRGB.push_back(be_hist);

	//Show histogram
	showHistogram(equaliRGB);

	// Show the image after equalizing. 
	Mat image_Equal;

	//Add each R,G,B images to 1 vector Mat
	imageRGB.push_back(r_dst);
	imageRGB.push_back(g_dst);
	imageRGB.push_back(b_dst);

	// Merge into 1 image
	cv::merge(imageRGB, image_Equal);

	// Show the image
	cv::imshow("Equalizes", image_Equal);


}
void Part2(Mat img) {

	///////////////////////////////////////////////////////////
	/////// PART 2: IMAGE FILTERING //////////////////////////
	/////////////////////////////////////////////////////////
	
	int ksize, type;
	double sigma;

	Mat medianImg, gausBlur, bilBlur, dst;
	namedWindow("Filter", WINDOW_AUTOSIZE); // Create a window

	// Create Trackbar
	cv::createTrackbar("Ksize", "Filter", 0, 100);
	cv::createTrackbar("sigma", "Filter", 0, 100);
	cv::createTrackbar("type", "Filter", 0, 2);

	while (1) {
		cv::imshow("image", img);
		ksize = 2 * cv::getTrackbarPos("Ksize", "Filter") + 1; // Kerner size odd
		sigma = cv::getTrackbarPos("sigma", "Filter");
		type = cv::getTrackbarPos("type", "Filter");

		if (type == 0) { //MedianBlur
			cv::medianBlur(img, medianImg, ksize);
			cv::imshow("Filter", medianImg);
		}
		else if (type == 1) //Gaussian Blur
		{
			cv::GaussianBlur(img, gausBlur, Size(ksize, ksize), sigma);
			cv::imshow("Filter", gausBlur);
		}
		else { //Bilateral Filter

			cv::bilateralFilter(img, bilBlur, ksize, sigma, sigma, 4);
			cv::imshow("Filter", bilBlur);

		}
		// Out when pressing ESC
		int outKey = cv::waitKey(25);
		if (outKey == 27)
		{
			break;
		}

	}
}

int main(int argc, char** argv)
{
	std::cout << "Computer_Vision_Lab_2" << std::endl;

	// load an image and place it in the img variable
	cv::Mat img = cv::imread("barbecue.png");
	
	///PART 1.1: SHOWING HISTOGRAM//// 
	// I suggest you to run "Part 11" and "Part12" separately, because there are an overlap in histogram plot. 
	Part11(img);

	///PART 1.2: EQUALIZING THE INPUT IMAGE///
	Part12(img);
	
	///PART 1: IMAGE FILTERING///
	Part2(img);

	/* Parameter explanation: 
	Type == 0: Median Blur
	Type == 1: Gaussian Blur
	Type == 2: Bilateral Filter
	*/

	// Visualize the image
	cv::imshow("img", img);

	

	cv::waitKey(0);
	cv::destroyAllWindows();

	return 0;
};

	