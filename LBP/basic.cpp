//Original LBP algorithm
#include <iostream>  
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <bitset>
using namespace cv;
using namespace std;


// 计算跳变次数  
int getHopCount(int i)
{
	int a[8] = { 0 };
	int cnt = 0;
	int k = 7;
	while (i)
	{
		a[k] = i & 1;
		i = i >> 1;
		--k;
	}
	for (k = 0; k < 7; k++)
	{
		if (a[k] != a[k + 1])
		{
			++cnt;
		}
	}
	if (a[0] != a[7])
	{
		++cnt;
	}
	return cnt;
}

// 降维数组 由256->59  
void lbp59table(uchar *table)
{
	memset(table, 0, 256);
	uchar temp = 1;
	for (int i = 0; i < 256; i++)
	{
		if (getHopCount(i) <= 2)    // 跳变次数<=2 的为非0值  
		{
			table[i] = temp;
			temp++;
		}
	}
}

void uniformLBP(Mat &image, Mat &result, uchar *table)
{
	for (int y = 1; y < image.rows - 1; y++)
	{
		for (int x = 1; x < image.cols - 1; x++)
		{
			uchar neighbor[8] = { 0 };
			neighbor[0] = image.at<uchar>(y - 1, x - 1);
			neighbor[1] = image.at<uchar>(y - 1, x);
			neighbor[2] = image.at<uchar>(y - 1, x + 1);
			neighbor[3] = image.at<uchar>(y, x + 1);
			neighbor[4] = image.at<uchar>(y + 1, x + 1);
			neighbor[5] = image.at<uchar>(y + 1, x);
			neighbor[6] = image.at<uchar>(y + 1, x - 1);
			neighbor[7] = image.at<uchar>(y, x - 1);
			uchar center = image.at<uchar>(y, x);
			uchar temp = 0;
			for (int k = 0; k < 8; k++)
			{
				temp += (neighbor[k] >= center)* (1 << k);  // 计算LBP的值  
			}
			result.at<uchar>(y, x) = table[temp];   //  降为59维空间  
		}
	}
}

void LBP(Mat &image, Mat &result)
{
	for (int y = 1; y < image.rows - 1; y++)
	{
		for (int x = 1; x < image.cols - 1; x++)
		{
			uchar neighbor[8] = { 0 };
			neighbor[0] = image.at<uchar>(y - 1, x - 1);
			neighbor[1] = image.at<uchar>(y - 1, x);
			neighbor[2] = image.at<uchar>(y - 1, x + 1);
			neighbor[3] = image.at<uchar>(y, x + 1);
			neighbor[4] = image.at<uchar>(y + 1, x + 1);
			neighbor[5] = image.at<uchar>(y + 1, x);
			neighbor[6] = image.at<uchar>(y + 1, x - 1);
			neighbor[7] = image.at<uchar>(y, x - 1);
			uchar center = image.at<uchar>(y, x);
			uchar temp = 0;
			for (int k = 0; k < 8; k++)
			{
				temp += (neighbor[k] >= center)* (1 << k);  // 计算LBP的值  
			}
			result.at<uchar>(y, x) = temp;
		}
	}
}


int main()
{
	uchar table[256];
	lbp59table(table);
	Mat image;
	image = imread("Y:\\Work\\AlgorithmReport\\input.jpg",0);
	namedWindow("Origin", 1);
	imshow("Origin", image);


	
	if (!image.data)
	{
		cout << "Fail to load image" << endl;
		return 0;
	}
	Mat result1;
	result1.create(Size(image.cols, image.rows), image.type());
	LBP(image, result1);

	namedWindow("Result", 1);
	imshow("Result", result1);

	Mat result2;
	result2.create(Size(image.cols, image.rows), image.type()); 
	uniformLBP(image, result2, table);

	namedWindow("uniformResult", 1);
	imshow("uniformResult", result2);
	
	waitKey(0);
	return 0;
	
}
