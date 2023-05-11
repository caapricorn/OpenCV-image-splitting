#include <opencv2/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/core/types_c.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>

#define OFFSET_BETWEEN_SQURES 25

int main( int argc, char** argv )
{
	// Открытие файла в бинарном режиме
	std::ifstream file("Mandrill.bmp", std::ios::binary);

	char header[54];
	file.read(header, 54);

	int width = *(int*)&header[18];
	int height = *(int*)&header[22];

	cv::Mat image(height, width, CV_8UC3);

	for (int i = 0; i < height; i++) {
		char* row = (char*)image.ptr(i);
		file.read(row, width * 3);
		int padding = 4 - (width * 3 % 4);
		if (padding == 4) padding = 0;
		file.seekg(padding, std::ios::cur);
	}

	file.close();

	// Получение размера файла
	//file.seekg(0, std::ios::end);
	//int length = file.tellg();
	//file.seekg(0, std::ios::beg);

	// Выделение памяти для чтения изображения
	//char* buffer = new char[length];
	//file.read(buffer, length);

	// Закрытие файла
    //file.close();

	// Создание объекта Mat из бинарных данных
    //cv::Mat image = imdecode(cv::Mat(1, length, CV_8UC1, buffer), cv::IMREAD_UNCHANGED);

	// Освобождение памяти
    //delete[] buffer;

	// Проверка, удалось ли прочитать изображение
    if (image.empty())
    {
        std::cout << "Не удалось прочитать изображение" << std::endl;
        return -1;
    }

	// Вывод размеров изображения
    std::cout << "Ширина: " << image.cols << " Высота: " << image.rows << std::endl;

	//imwrite("test.bmp", image);

	// Define the border size
    int top = 128, bottom = 128, left = 128, right = 128;

	// Define the border type
    int borderType = cv::BORDER_DEFAULT;
	//int borderType = cv::BORDER_REPLICATE;

	// Define the border value
    cv::Scalar value(0, 0, 0);

	// Create a new image with the border
    cv::Mat dst;
    cv::copyMakeBorder(image, dst, top, bottom, left, right, borderType, value);

	image.release();
	
	imwrite("border.bmp", dst);

	// Define the size of the squares
    int squareSize = 200;

	int ir = dst.rows;
	int cr = dst.cols;
	// Loop over the rows and columns
    for (int currentRow = 0; currentRow < ir; currentRow += squareSize)
    {
        for (int currentCol = 0; currentCol < cr; currentCol += squareSize)
        {
			if (currentCol != 0) {
				currentCol -= OFFSET_BETWEEN_SQURES;
			}
			if (currentCol + squareSize > cr && currentRow + squareSize > ir) {
				cv::Rect roi(currentCol, currentRow, cr - currentCol, ir - currentRow);
				cv::Mat square = dst(roi);
				borderType = cv::BORDER_CONSTANT;
				cv::copyMakeBorder(square, square, 0, squareSize - (ir - currentRow), 0, squareSize - (cr - currentCol), borderType, value);
				char name[50];
				sprintf(name, "SQuare%d-%d.bmp", currentRow, currentCol);
            	imwrite(name, square);

				square.release();
				std::cout << "удалось1" << currentRow << currentCol << std::endl;
			}
			if (currentCol + squareSize > cr) {

				cv::Rect roi(currentCol, currentRow, cr - currentCol, squareSize);
				cv::Mat square = dst(roi);
				borderType = cv::BORDER_CONSTANT;
				cv::copyMakeBorder(square, square, 0, 0, 0, squareSize - (cr - currentCol), borderType, value);

				char name[50];
				sprintf(name, "Square%d-%d.bmp", currentRow, currentCol);
            	imwrite(name, square);

				square.release();
				std::cout << "удалось1" << currentRow << currentCol << std::endl;
				//borderType = cv::BORDER_CONSTANT;
				//cv::copyMakeBorder(dst, dst, 0, 0, 0, squareSize, borderType, value);
			}
			if (currentRow + squareSize > ir) {

				cv::Rect roi(currentCol, currentRow, squareSize, ir - currentRow);
				cv::Mat square = dst(roi);
				borderType = cv::BORDER_CONSTANT;
				cv::copyMakeBorder(square, square, 0, squareSize - (ir - currentRow), 0, 0, borderType, value);
				std::cout << "удалось2" << currentRow << currentCol << std::endl;

				char name[50];
				sprintf(name, "sQuare%d-%d.bmp", currentRow, currentCol);
            	imwrite(name, square);

				square.release();

				//borderType = cv::BORDER_CONSTANT;
				//cv::copyMakeBorder(dst, dst, 0, squareSize, 0, 0, borderType, value);
			}
			
			// std::cout << i << j << rowSize << columnSize << std::endl;

            cv::Rect roi(currentCol, currentRow, squareSize, squareSize);

            // Extract the square from the image
            cv::Mat square = dst(roi);
			//std::cout << "удалось3" << std::endl;
			std::cout << currentRow << " " << currentCol << std::endl;

            // Do something with the square, for example display it
			char name[80];
			sprintf(name, "square%d-%d.bmp", currentRow, currentCol);
            imwrite(name, square);
			square.release();
        }
    }

    return 0;
}