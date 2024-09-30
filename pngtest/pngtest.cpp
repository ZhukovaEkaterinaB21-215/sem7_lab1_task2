// pngtest.cpp : Defines the entry point for the console application.
//
// pngtest.cpp : Defines the entry point for the console application.
//

#include "PngProc.h"
#include <string.h>
#include <stdio.h>
#include <iostream>
using namespace std;

void BlendImage(unsigned char* pOut
	, unsigned char* pIn1
	, unsigned char* pIn2
	, unsigned char* pAlpha
	, size_t inWidth1
	, size_t inHeight1
	, size_t inWidth2
	, size_t inHeight2
	, size_t alphaWidth
	, size_t alphaHeight);



int main(int argc, char* argv[])
{

	class CBitsPtrGuard
	{
	public:
		CBitsPtrGuard(unsigned char** pB) : m_ppBits(pB) { }
		~CBitsPtrGuard() { if (*m_ppBits) delete* m_ppBits, * m_ppBits = 0; }
	protected:
		unsigned char** m_ppBits;
	};
	//printf(argv[1]);
	//printf(argv[2]);
	std::cout << argc << std::endl;
	std::cout << argv[1] << std::endl;
	// parse input parameters
	char	szInputFileName1[256];
	char	szInputFileName2[256];
	char	szAlphaFileName[256];
	char    szOutputFileName[256];
	if (argc < 4)
		printf("\nformat: pngtest <input_file> [<output_file>]");
	else
	{
		strcpy(szInputFileName1, argv[1]);
		strcpy(szInputFileName2, argv[2]);
		strcpy(szAlphaFileName, argv[3]);
		if (argc > 4)
			strcpy(szOutputFileName, argv[4]);
		else
		{
			strcpy(szOutputFileName, szInputFileName1);
			strcat(szOutputFileName, "_out.png");
		}
	}

	// Получаем размер изображения
	size_t inReqSize1 = NPngProc::readPngFile(szInputFileName1, 0, 0, 0, 0);
	cout << inReqSize1 << endl;
	size_t inReqSize2 = NPngProc::readPngFile(szInputFileName2, 0, 0, 0, 0);
	cout << inReqSize2 << endl;
	size_t alphaReqSize = NPngProc::readPngFile(szAlphaFileName, 0, 0, 0, 0);
	cout << alphaReqSize << endl;
	if (inReqSize1 == NPngProc::PNG_ERROR || inReqSize2 == NPngProc::PNG_ERROR || alphaReqSize == NPngProc::PNG_ERROR)
	{
		printf("\nError ocured while pngfile was read");
		return -1;
	}


	unsigned char* pInputBits1 = new unsigned char[inReqSize1];
	if (!pInputBits1)
	{
		printf("\nCan't allocate memory for image, required size is %u", inReqSize1);
		return -1;
	}
	CBitsPtrGuard InputBitsPtrGuard1(&pInputBits1);

	unsigned char* pInputBits2 = new unsigned char[inReqSize2];
	if (!pInputBits2)
	{
		printf("\nCan't allocate memory for image, required size is %u", inReqSize2);
		return -1;
	}
	CBitsPtrGuard InputBitsPtrGuard2(&pInputBits2);


	unsigned char* pAlphaBits = new unsigned char[alphaReqSize];
	if (!pAlphaBits)
	{
		printf("\nCan't allocate memory for image, required size is %u", alphaReqSize);
		return -1;
	}
	CBitsPtrGuard AlphaBitsPtrGuard(&pAlphaBits);


	unsigned char* pOutputBits = new unsigned char[inReqSize1];
	if (!pOutputBits)
	{
		printf("\nCan't allocate memory for image, required size is %u", inReqSize1);
		return -1;
	}


	CBitsPtrGuard OutputBitsPtrGuard(&pOutputBits);

	size_t inWidth1, inHeight1, inWidth2, inHeight2, alphaWidth, alphaHeight;
	unsigned int nBPP;

	size_t inRetSize1 = NPngProc::readPngFileGray(szInputFileName1, pInputBits1, &inWidth1, &inHeight1/*, &nBPP*/);
	size_t inRetSize2 = NPngProc::readPngFileGray(szInputFileName2, pInputBits2, &inWidth2, &inHeight2/*, &nBPP*/);
	size_t alphaRetSize = NPngProc::readPngFileGray(szAlphaFileName, pAlphaBits, &alphaWidth, &alphaHeight/*, &nBPP*/);
	nBPP = 8;


	// ASSERT(nRetSize == nReqSize);

	// TODO: image processing 
	//ImageProcessingGray(pOutputBits, pInputBits, nWidth, nHeight);
	BlendImage(pOutputBits, pInputBits1, pInputBits2, pAlphaBits, inWidth1, inHeight1, inWidth2, inHeight2, alphaWidth, alphaHeight);

	if (NPngProc::writePngFile(szOutputFileName, pOutputBits, inWidth1, inHeight1, nBPP) == NPngProc::PNG_ERROR)
	{
		printf("\nError ocuured during png file was written");
		return -1;
	}

	return 0;
}

void BlendImage(unsigned char* pOut
	, unsigned char* pIn1
	, unsigned char* pIn2
	, unsigned char* pAlpha
	, size_t inWidth1
	, size_t inHeight1
	, size_t inWidth2
	, size_t inHeight2
	, size_t alphaWidth
	, size_t alphaHeight)
{
	if (inWidth1 != alphaWidth || alphaWidth != inWidth2 || inHeight1 != alphaHeight || alphaHeight != inHeight2)
	{
		printf("\nError the images are of different sizes");
		return;
	}
	for (size_t y = 0; y < inHeight1; ++y)
	{
		for (size_t x = 0; x < inWidth1; ++x)
		{
			size_t numberPixel = y * inWidth1 + x;
			pOut[numberPixel] = pAlpha[numberPixel] / 255.0 * pIn2[numberPixel] + (1 - pAlpha[numberPixel] / 255.0) * pIn1[numberPixel];
			
		}
	}
	return;
}



