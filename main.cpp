#include <opencv2/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/core/types_c.h>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>

cv::Mat src_gray;
cv::Mat closed;
int thresh = 40;
cv::RNG rng(12345);

void thresh_callback(cv::Mat);

int main( int argc, char** argv )
{
	// Открытие файла в бинарном режиме
	std::ifstream file("Mandrill.bmp", std::ios::binary);

	// Получение размера файла
	file.seekg(0, std::ios::end);
	int length = file.tellg();
	file.seekg(0, std::ios::beg);

	// Выделение памяти для чтения изображения
	char* buffer = new char[length];
	file.read(buffer, length);

	// Закрытие файла
    file.close();

	// Создание объекта Mat из бинарных данных
    cv::Mat image = cv::imdecode(cv::Mat(1, length, cv::CV_8UC1, buffer), cv::IMREAD_UNCHANGED);

	// Освобождение памяти
    delete[] buffer;

	// Проверка, удалось ли прочитать изображение
    if (image.empty())
    {
        std::cout << "Не удалось прочитать изображение" << std::endl;
        return -1;
    }

	// Вывод размеров изображения
    std::cout << "Ширина: " << image.cols << " Высота: " << image.rows << std::endl;

    /*img = cv::imread("Mandrill.bmp",cv::IMREAD_COLOR);
    if(! img.data)
        {
            std::cout<<"Could not open file" << std::endl;
            return -1;
        }  

	int height = img.rows;
	int width = img.cols;
	int max_size = std::max(height, width);
	cv::Mat square = cv::Mat::zeros(max_size, max_size, img.type());
	int x = (max_size - width) / 2;
    int y = (max_size - height) / 2;
	img.copyTo(square(cv::Rect(x, y, width, height)));
	cv::Mat resized;
	cv::resize(square, resized, cv::Size(512, 512));
	imwrite("resize.jpg", resized); 

	cv::Mat image = resized.clone(); 

	for (int i = 0; i < resized.rows; i++)
		for (int j = 0; j < resized.cols; j++)
			if (resized.at<cv::Vec3b>(i, j)[0] > 130 &&
				resized.at<cv::Vec3b>(i, j)[2] > 130 && 
				resized.at<cv::Vec3b>(i, j)[1] > 130)
			{
				resized.at<cv::Vec3b>(i, j)[0] = 0;
				resized.at<cv::Vec3b>(i, j)[1] = 0;
				resized.at<cv::Vec3b>(i, j)[2] = 255;
			}
	//imwrite("cloud_mask.jpg", resized);

	cv::cvtColor(resized, src_gray, cv::COLOR_BGR2GRAY);
	cv::blur(src_gray, src_gray, cv::Size(3, 3));
	//imwrite("img_grey.jpg", src_gray);

	cv::Mat thresh_mat;
	cv::threshold(src_gray, thresh_mat, 100, 255, cv::THRESH_BINARY);

	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
	//cv::Mat closed;
	cv::morphologyEx(thresh_mat, closed, cv::MORPH_CLOSE, kernel);
	
	//imwrite("morph.jpg", closed);
	
	thresh_callback(image);*/

    return 0;
}

void thresh_callback(cv::Mat img)
{
	cv::Mat canny_output;
    cv::Canny(closed, canny_output, thresh, thresh*2);
    std::vector<std::vector<cv::Point> > contours;
    findContours(canny_output, contours, cv::RETR_CCOMP, cv::ContourApproximationModes::CHAIN_APPROX_SIMPLE);

	std::vector<cv::Point> main_contour;
	
	std::vector<cv::Point> hull;
	for (size_t i = 0; i < contours.size(); i++)
		main_contour.insert(main_contour.end(), contours[i].begin(), contours[i].end());


	cv::convexHull(main_contour, hull);
	cv::RotatedRect minRect;
	minRect = cv::minAreaRect(hull);
	
	cv::Point2f rect_points[4];
	minRect.points(rect_points);
	for (int j = 0; j < 4; j++)
		cv::line(img, rect_points[j], rect_points[(j + 1) % 4], cv::Scalar(255, 255, 255));

	cv::Mat M, rotated, cropped;
	float angle = minRect.angle;
	cv::Size rect_size = minRect.size;
	if (minRect.angle < -45.) {
		angle += 90.0;
		cv::swap(rect_size.width, rect_size.height);
	} 
	M = cv::getRotationMatrix2D(minRect.center, angle, 1.0);

	cv::warpAffine(img, rotated, M, img.size(), cv::INTER_CUBIC);
	//imwrite("rotate.jpg", rotated);
	cv::getRectSubPix(rotated, rect_size, minRect.center, cropped);
	//imwrite("crop.jpg", cropped);

	//imwrite("cloud_contour.jpg",  img);
}
