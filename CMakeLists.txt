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



#LINK_DIRECTORIES ($ENV{BME_BASE_DIR}/Ascension/Lib) 
#LINK_DIRECTORIES ($ENV{BME_BASE_DIR}/NXT++.07/bin32/${OutDir}) 
#LINK_DIRECTORIES ($ENV{BME_BASE_DIR}/NXT++.07/Lib) 

#INCLUDE_DIRECTORIES($ENV{BME_BASE_DIR}/Ascension/Include)
#INCLUDE_DIRECTORIES($ENV{BME_BASE_DIR}/NXT++.07/include)
#INCLUDE_DIRECTORIES($ENV{BME_BASE_DIR}/TiePie/include)

#INCLUDE_DIRECTORIES($ENV{NIG_COMMON_DIR}/../thread)
#INCLUDE_DIRECTORIES($ENV{NIG_COMMON_DIR}/../Timer)
#INCLUDE_DIRECTORIES(${TestMultiThread_SOURCE_DIR})
#INCLUDE_DIRECTORIES(${TestMultiThread_SOURCE_DIR}/Threading)
#INCLUDE_DIRECTORIES(${ITK_DIR}/../Modules/Bridge/VtkGlue/include)
#INCLUDE_DIRECTORIES($ENV{NIG_COMMON_DIR})


#SET (EXT_hdr 
#	$ENV{BME_BASE_DIR}/TiePie/include/TiePie.h
#	$ENV{BME_BASE_DIR}/TiePie/include/TiePieDLL.h
#	$ENV{BME_BASE_DIR}/Ascension/Include/ATC3DG.h
#	$ENV{BME_BASE_DIR}/NXT++.07/include/NXT++.h	 
#	)
#SET (EXT_files  
#	$ENV{BME_BASE_DIR}/TiePie/Source/tiepiedll.cpp
#	$ENV{BME_BASE_DIR}/NXT++.07/src/NXT++.cpp
#	)
   
	  
#SET_SOURCE_FILES_PROPERTIES( ${HW6_hdr} PROPERTIES HEADER_FILE_ONLY TRUE )
#	SOURCE_GROUP( "External headers" FILES ${EXT_hdr} )
#	SOURCE_GROUP( "External sources" FILES ${EXT_files} )


#SET( RobotBaseFiles 
#	$ENV{NIG_COMMON_DIR}/utility.h
#	$ENV{NIG_COMMON_DIR}/../thread//Thread.h
#	Threading/NIGMultiThread.h
#	Threading/NIGReconstructionThread.txx
#	Threading/NIGReconstructionThread.h
#	Threading/NIGTiepieThread.txx
#	Threading/NIGTiepieThread.h
#	Threading/NIGLegoThread.txx
#	Threading/NIGLegoThread.h
#	Threading/NIGOpenCVThread.txx
#	Threading/NIGOpenCVThread.h
#	Threading/NIGViewingThread.txx
#	Threading/NIGViewingThread.h
#	Threading/NIGTrakstarThread.txx
#	MainPage.h
#	$ENV{NIG_COMMON_DIR}/../Timer/Timer.cpp
#	$ENV{NIG_COMMON_DIR}/../Timer/Timer.h
#	${EXT_hdr}
#	${EXT_files}
#	)

		

SET( TestMultiThreadFiles
	mainTestMultiThreading.cpp
 	)
SET( TestMultiThreadSyncFiles 
	mainTestMultiThreadingSync.cpp
	)
SET( TestMultiThreadP2S1Files 
	Part2Section1.cpp
	)
# VXL stuff
#INCLUDE_DIRECTORIES( $ENV{BME_BASE_DIR}/vxl-1.14.0/Bin32/vcl )
#INCLUDE_DIRECTORIES( $ENV{BME_BASE_DIR}/vxl-1.14.0/Bin32/core )
#INCLUDE_DIRECTORIES( $ENV{BME_BASE_DIR}/vxl-1.14.0/core )
#INCLUDE_DIRECTORIES( $ENV{BME_BASE_DIR}/vxl-1.14.0/vcl )
#LINK_DIRECTORIES( $ENV{BME_BASE_DIR}/vxl-1.14.0/Bin32/Lib/${OutDir} )

# common stuff

INCLUDE_DIRECTORIES( $ENV{ALEX_COMMON_DIR} )

ADD_EXECUTABLE( TestMultiThread ${TestMultiThreadFiles})
TARGET_LINK_LIBRARIES ( TestMultiThread ${ITK_LIBRARIES} ${VTK_LIBRARIES} ${OpenCV_LIBS} )

ADD_EXECUTABLE( TestMultiThreadSync ${TestMultiThreadSyncFiles})
TARGET_LINK_LIBRARIES ( TestMultiThreadSync ${ITK_LIBRARIES} ${VTK_LIBRARIES} ${OpenCV_LIBS} )

ADD_EXECUTABLE( TestMultiThreadP2S1 ${TestMultiThreadP2S1Files})
TARGET_LINK_LIBRARIES ( TestMultiThreadP2S1 ${ITK_LIBRARIES} ${VTK_LIBRARIES} ${OpenCV_LIBS} )