#include "Common.h"
#include "lodepng.h"

#include <cmath>
#include <iostream>




int loadPNG(const std::string cFName, std::vector<unsigned char>& rcImgData, unsigned int& ruiWidth, unsigned int& ruiHeight)
{
  //decode
  unsigned int error = lodepng::decode( rcImgData, ruiWidth, ruiHeight, cFName );

  //if there's an error, display it
  if(error) std::cout << "cannot read image " << cFName << " error: " << error << ": " << lodepng_error_text(error) << std::endl;

  return 0;
}


