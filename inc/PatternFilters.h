#if !defined PATTERN_FILTERS
#define PATTERN_FILTERS

#include <opencv2/opencv.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <iostream>

cv::Mat getStartRegion(const cv::Mat &src)
{
    assert(src.type() == CV_8UC3);

	cv::Mat startRegionMask;

    cv::inRange(src, cv::Scalar(0x97, 0xa1, 0x2e), cv::Scalar(0x97, 0xa1, 0x2e), startRegionMask);
	startRegionMask = startRegionMask > 0;
	cv::erode(startRegionMask, startRegionMask, cv::Mat());

	cv::Rect startRegionArea = cv::boundingRect(startRegionMask);
	cv::Point center = startRegionArea.tl() + cv::Point(startRegionArea.width/2, startRegionArea.height/2);
	circle( startRegionMask, center, startRegionArea.width/2 + 10, cv::Scalar(255,255,255), cv::FILLED, 8, 0 );

	return startRegionMask;
}

cv::Mat getStopRegion(const cv::Mat &src)
{
	assert(src.type() == CV_8UC3);

    cv::Mat stopRegionMask;

    cv::inRange(src, cv::Scalar(0x2e, 0x30, 0xdd), cv::Scalar(0x2e, 0x30, 0xdd), stopRegionMask);
	stopRegionMask = stopRegionMask > 0;
	cv::erode(stopRegionMask, stopRegionMask, cv::Mat());

	cv::Rect stopRegionArea = cv::boundingRect(stopRegionMask);
	cv::Point center = stopRegionArea.tl() + cv::Point(stopRegionArea.width/2, stopRegionArea.height/2);
	circle( stopRegionMask, center, stopRegionArea.width/2 + 10, cv::Scalar(255,255,255), cv::FILLED, 8, 0 );

	return stopRegionMask;
}

cv::Mat getMidPath(const cv::Mat &src)
{
	assert(src.type() == CV_8UC3);

	cv::Mat midPathImage;

	//1) Find start region, calculate bounding circle around:
	cv::Mat startRegionMask = getStartRegion(src);

	//2) Find stop region, calculate bounding circle around:
	cv::Mat stopRegionMask = getStopRegion(src);

	//3) Extract midpath:
	//Color filter to only leave white midpath and circles around start and end point
	cv::inRange(src, cv::Scalar(200, 200, 200), cv::Scalar(255, 255, 255), midPathImage);
	cv::blur( midPathImage, midPathImage, cv::Size(5,5) );
	midPathImage = midPathImage > 0;
	cv::erode(midPathImage, midPathImage, cv::Mat());
	
	//Remove bounding circles from image, using masks
	midPathImage = midPathImage - startRegionMask - stopRegionMask;

	return midPathImage;
}

//Extract mid path points
std::vector<cv::Point2d> extractMidPathPoints(const cv::Mat &src)
{
	assert(src.type() == CV_8UC3);

    std::vector<cv::Point2d> points;

	cv::Mat midPath = getMidPath(src);
	cv::Mat startRegion = getStartRegion(src);
	cv::Mat stopRegion = getStopRegion(src);

	cv::Rect startBox = cv::boundingRect(startRegion);
	cv::Point startPoint = startBox.tl() + cv::Point(startBox.width/2, startBox.height/2);

	cv::Rect stopBox = cv::boundingRect(stopRegion);
	cv::Point stopPoint = stopBox.tl() + cv::Point(stopBox.width/2, stopBox.height/2);

    std::vector<std::vector<cv::Point> > contours;
    findContours(midPath, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
	int j = 0; //index of closest to start
	int k = 0; //index of closest to stop
	for(int i = 0; i < contours[0].size(); i++)
	{
		if(cv::norm(startPoint - contours[0][j]) > cv::norm(startPoint - contours[0][i]))
		{
			j = i;
		}
		if(cv::norm(stopPoint - contours[0][k]) > cv::norm(stopPoint - contours[0][i]))
		{
			k = i;
		}
	}

	points.push_back(startPoint);

	int m = j;
	while(contours[0][m] != contours[0][k])
	{
		points.push_back(contours[0][m]);
		m = (m+1) % contours[0].size();
	}
	points.push_back(contours[0][k]);

	points.push_back(stopPoint);

	return points;
}

cv::Mat getPathRegion(const cv::Mat &src)
{
    assert(src.type() == CV_8UC3);

    cv::Mat polyChanImage;

    //Color filter to only leave path region
    cv::inRange(src, cv::Scalar(0x35, 0x2a, 0x00), cv::Scalar(0x35, 0x2a, 0x00), polyChanImage);

    cv::blur( polyChanImage, polyChanImage, cv::Size(5,5) );
    polyChanImage = polyChanImage <= 0;

    return polyChanImage;
}

//Extract Path Region Points
std::vector<cv::Point2d> extractPathRegionPoints(const cv::Mat &src)
{
    //Assert src is BGR
    assert(src.type() == CV_8UC3);

    //Path Region Points
    std::vector<cv::Point2d> points;

    //Path Region
    cv::Mat pathRegion = getPathRegion(src);

    //Contours vector
    std::vector<std::vector<cv::Point> > contours;
    //Find contours in path region
    findContours(pathRegion, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

    //Resize Path Region Points with contours size
    points.resize(contours[0].size());
    //Copy countours into Path Region Points
    std::copy(contours[0].begin(), contours[0].end(), points.begin());

    //Return Path Region Points
    return points;
}

cv::Mat drawCurve(std::vector<cv::Point2d> curve, int rows, int cols)
{
    cv::Mat output(rows, cols, CV_8UC1);

    for(int i=0; i < curve.size()-1; i++)
    {
      cv::line(output, curve[i], curve[i+1], cv::Scalar(255), 15);
    }

    return output;
}

#endif
