PROJECT( TestMultiThread )
#https://github.com/sintefmath/sist/blob/master/CMakeLists.txt
CMAKE_MINIMUM_REQUIRED(VERSION 2.8 FATAL_ERROR)
IF(COMMAND CMAKE_POLICY)
	CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND CMAKE_POLICY)

INCLUDE ($ENV{BME_BASE_DIR}/Config/BMECMakeLists.txt)

 

#LINK_DIRECTORIES ("C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v6.0/lib/Win32") 


SET( TestMultiThreadFiles
	mainTestMultiThreading.cpp
 	)
SET( TestMultiThreadSyncFiles 
	mainTestMultiThreadingSync.cpp
	)
SET( TestMultiThreadP2S1Files 
	Part2Section1.cpp
	)
SET( TestVideoBufferFiles 
	VideoBuffer.cpp
	)
SET( Test2DimageDisplayFiles 
	display2DImages.cpp
	)
# common stuff

INCLUDE_DIRECTORIES( $ENV{ALEX_COMMON_DIR} )

ADD_EXECUTABLE( TestMultiThread ${TestMultiThreadFiles})
TARGET_LINK_LIBRARIES ( TestMultiThread ${ITK_LIBRARIES} ${VTK_LIBRARIES} ${OpenCV_LIBS} )

ADD_EXECUTABLE( TestMultiThreadSync ${TestMultiThreadSyncFiles})
TARGET_LINK_LIBRARIES ( TestMultiThreadSync ${ITK_LIBRARIES} ${VTK_LIBRARIES} ${OpenCV_LIBS} )

ADD_EXECUTABLE( TestMultiThreadP2S1 ${TestMultiThreadP2S1Files})
TARGET_LINK_LIBRARIES ( TestMultiThreadP2S1 ${ITK_LIBRARIES} ${VTK_LIBRARIES} ${OpenCV_LIBS} )

ADD_EXECUTABLE( TestVideoBuffer ${TestVideoBufferFiles})
TARGET_LINK_LIBRARIES ( TestVideoBuffer ${ITK_LIBRARIES} ${VTK_LIBRARIES} ${OpenCV_LIBS} )

ADD_EXECUTABLE( Test2DimageDisplay ${Test2DimageDisplayFiles})
TARGET_LINK_LIBRARIES ( Test2DimageDisplay ${ITK_LIBRARIES} ${VTK_LIBRARIES} ${OpenCV_LIBS} vtkPlusCommon )