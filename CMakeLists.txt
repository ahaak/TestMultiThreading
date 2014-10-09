PROJECT( TestMultiThread )
#https://github.com/sintefmath/sist/blob/master/CMakeLists.txt
CMAKE_MINIMUM_REQUIRED(VERSION 2.8 FATAL_ERROR)
IF(COMMAND CMAKE_POLICY)
	CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND CMAKE_POLICY)


# Microsoft specific items
IF( MSVC )
 # Kill the anoying MS VS warning about non-safe functions.
 # They hide real warnings.
 ADD_DEFINITIONS(
   /D_SCL_SECURE_NO_DEPRECATE
   /D_CRT_SECURE_NO_DEPRECATE
   /D_CRT_TIME_FUNCTIONS_NO_DEPRECATE
 )
ENDIF()

FIND_PACKAGE (ITK REQUIRED)
INCLUDE( ${USE_ITK_FILE} )

FIND_PACKAGE (VTK REQUIRED)
INCLUDE( ${USE_VTK_FILE} )

FIND_PACKAGE(OpenCV REQUIRED )

#LINK_LIBRARIES(${OpenCV_LIBS} )


#FIND_PACKAGE(CUDA REQUIRED)
 

LINK_DIRECTORIES ("C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v6.0/lib/Win32") 


# Boost stuff

INCLUDE_DIRECTORIES( D:/VisualStudioBaseProjects/boost_1_56_0/bin32/include/boost-1_56)
LINK_DIRECTORIES( D:/VisualStudioBaseProjects/boost_1_56_0/bin32/Lib/${OutDir})

		

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
TARGET_LINK_LIBRARIES ( Test2DimageDisplay ${ITK_LIBRARIES} ${VTK_LIBRARIES} ${OpenCV_LIBS} )