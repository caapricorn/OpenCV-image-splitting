#include <opencv2/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/core/types_c.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>

#define OFFSET_BETWEEN_SQURES 64

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
    cv::Mat image = imdecode(cv::Mat(1, length, CV_8UC1, buffer), cv::IMREAD_UNCHANGED);

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

	// Define the border size
    int top = 128, bottom = 128, left = 128, right = 128;

	// Define the border type
    int borderType = cv::BORDER_DEFAULT;
	//int borderType = cv::BORDER_REPLICATE;

	// Define the border value
    cv::Scalar value(255, 255, 255);

	// Create a new image with the border
    cv::Mat dst;
    cv::copyMakeBorder(image, dst, top, bottom, left, right, borderType, value);

	imwrite("border.bmp", dst);

	// Define the size of the squares
    int squareSize = 50;

	// Define the number of rows and columns
    int rows = dst.rows / squareSize;
    int cols = dst.cols / squareSize;

	// Loop over the rows and columns
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            // Define the region of interest
			const int rowSize = 
            cv::Rect roi(j * squareSize, i * squareSize, squareSize, squareSize);

            // Extract the square from the image
            cv::Mat square = dst(roi);

            // Do something with the square, for example display it
			char name[80];
			sprintf(name, "square%d-%d.bmp", i, j);
            imwrite(name, square);
        }
    }

    return 0;
}

/*void thresh_callback(cv::Mat img)
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
}*/
