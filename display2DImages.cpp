#include "vtkSmartPointer.h"
#include "vtkPNGReader.h"
#include "vtkImageReader.h"
#include "vtkLookupTable.h" // not needed?
#include <vtkImageMapper3D.h>
#include <vtkImageMapToColors.h>
#include "vtkImageActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkInteractorStyleImage.h"
#include "vtkMetaImageReader.h"

#include "vtkMetaImageSequenceIO.h"
#include "vtkTrackedFrameList.h"
#include "TrackedFrame.h"
#include <itkEuler3DTransform.h>


#include "vtkMatrix4x4.h"
#include "vtkTransform.h"
#include "vtkImageViewer.h"
#include "vtkCamera.h"
#include "vtkImageData.h"


#include "PlusConfigure.h"



int main(int argc, char** argv)
{
  // some additional stuff
  static double XYPlaneElements[16] = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1 };

  static double XZPlaneElements[16] = {
    1, 0, 0, 0,
    0, 0, 1, 0,
    0,-1, 0, 0,
    0, 0, 0, 1 };

  static double YZPlaneElements[16] = {
    0, 0,-1, 0,
    1, 0, 0, 0,
    0,-1, 0, 0,
    0, 0, 0, 1 };


    std::string stylusName("Stylus");
    std::string referenceName("Reference");
    //int verboseLevel=vtkPlusLogger::LOG_LEVEL_UNDEFINED;
    //vtkPlusLogger::Instance()->SetLogLevel(verboseLevel);
    PlusTransformName stylusToReferenceTransformName(stylusName, referenceName);


    // read image
    //	std::string fileName = "D:/Data/ExperimentalData/EPExperiments/EP1_24-Feb-2014/consecutiveInputDataNC/IM_0007/600Images/2DinputImage_0584.png";
    char  fileName[88]; 
    strcpy(fileName,"D:/Data/TrackedImageSequence_20141014_163846.mha");
    vtkSmartPointer<vtkMetaImageSequenceIO> reader = vtkMetaImageSequenceIO::New();
    reader->SetFileName( fileName);
    //reader->
    reader->Read();
    vtkSmartPointer<vtkTrackedFrameList> frames = vtkSmartPointer<vtkTrackedFrameList>::New();
    frames->ReadFromSequenceMetafile( fileName );
    TrackedFrame* frame2 = frames->GetTrackedFrame( 0 );
    typedef itk::Image<unsigned char, 2> ImageType;
    ImageType::Pointer testFrame = ImageType::New();

    PlusVideoFrame* videoFrame = frame2->GetImageData();
    US_IMAGE_TYPE PlusType = videoFrame->GetImageType();
    testFrame = videoFrame->GetDisplayableImage();
    vtkIndent indent;
    frames->PrintSelf( std::cout,  indent );
    vtkSmartPointer<vtkXMLDataElement> xmlData = vtkSmartPointer<vtkXMLDataElement>::New();
    
    std::vector<PlusTransformName> transformNames;
    frame2->GetCustomFrameTransformNameList( transformNames );
    vtkSmartPointer<vtkMatrix4x4> transformMatrix = vtkSmartPointer<vtkMatrix4x4>::New();
    frame2->GetCustomFrameTransform( transformNames[0], transformMatrix);
    
    typedef itk::Euler3DTransform<double> TransformTyp;
    TransformTyp::Pointer transform = TransformTyp::New();
    TransformTyp::MatrixType rotationMatrix;
    rotationMatrix.SetIdentity();
    for ( int row = 0; row < 3; row++ )
    {
      for ( int col = 0; col < 3; col++ )
      {
        rotationMatrix[row][col] = transformMatrix->GetElement( row, col ); 
      }
    }
    transform->SetMatrix( rotationMatrix );

    TransformTyp::OffsetType translation;
    translation.Fill( 0.0 );
    for ( int row = 0; row < 3; row++ )
    {
      translation[row] =  transformMatrix->GetElement( row, 3 ); 
    }
    transform->SetOffset( translation );
    //transform->SetMatrix()
    //transformMatrix->GetElement()
    //videoFrame->GetImage();
    //PlusType* plusFrame = videoFrame->GetImage();

    //videoFrame->DeepCoppyVtkImageToItkImage( videoFrame->GetImage(), testFrame);
    
    // int dim =  reader->NumberOfDimensions();
    //reader->Print();
     strcpy(fileName,"D:/Data/TrackedIma.mhd");
    // reader->SetFileName( fileName);
    //  reader->Modified();
    // reader->WriteImages();
//#if 0		
			
	//int* Dimensions = reader->GetOutputPort()->GetDimensions(); //429 608
	//double * Spacing=reader->GetOutput()->GetSpacing();
	
	double Origin[2];
	//Origin[0]=Dimensions[0]*Spacing[0];
	//Origin[1]=Dimensions[1]*Spacing[1];

  vtkSmartPointer<vtkMatrix4x4> resliceAxes = vtkSmartPointer<vtkMatrix4x4>::New();
  resliceAxes->DeepCopy(XZPlaneElements);
  resliceAxes->SetElement(0, 3, 0.0 );resliceAxes->SetElement(1, 3, Origin[0]/2 );resliceAxes->SetElement(2, 3, 0.0 );
  
  vtkSmartPointer<vtkTransform> resliceAxesTransform = vtkSmartPointer<vtkTransform>::New();
  resliceAxesTransform->SetMatrix( resliceAxes );
  resliceAxesTransform->Update();


  resliceAxes->DeepCopy(YZPlaneElements);
  resliceAxes->SetElement(0, 3, Origin[0]/2);resliceAxes->SetElement(1, 3, 0.0 );resliceAxes->SetElement(2, 3, 0.0 );
  
	vtkSmartPointer<vtkTransform> resliceAxesTransform2 = vtkSmartPointer<vtkTransform>::New();
  resliceAxesTransform2->SetMatrix( resliceAxes );
  resliceAxesTransform2->Update();

	
	resliceAxes->DeepCopy(XYPlaneElements);
	resliceAxes->SetElement(0, 3, 0.0 );resliceAxes->SetElement(1, 3, 0 );resliceAxes->SetElement(2, 3, -Origin[1]/2  );

	vtkSmartPointer<vtkTransform> resliceAxesTransform3 = vtkSmartPointer<vtkTransform>::New();
	resliceAxesTransform3->SetMatrix( resliceAxes );
	resliceAxesTransform3->Update();
	  
  // Create a greyscale lookup table
  vtkSmartPointer<vtkLookupTable> table = vtkSmartPointer<vtkLookupTable>::New();
  table->SetRange(0, 255); // image intensity range
  table->SetValueRange(0.0, 1.0); // from black to white
  table->SetSaturationRange(0.0, 0.0); // no color saturation
  table->SetRampToLinear();
  table->Build();

  // mapper
  /*vtkSmartPointer<vtkImageMapper3D> mapper = vtkSmartPointer<vtkImageMapper3D>::New();
  mapper->SetInputConnection( reader->GetOutputPort() ); 
 */
  // mapper 2
  vtkSmartPointer<vtkImageMapToColors> mapper2 = vtkSmartPointer<vtkImageMapToColors>::New();
  mapper2->SetLookupTable( table );
  

//  mapper2->SetInputConnection( reader->GetOutputData) );
  // actor
  vtkSmartPointer<vtkImageActor> actor = vtkSmartPointer<vtkImageActor>::New();
  actor->SetUserTransform( resliceAxesTransform );
  actor->GetMapper()->SetInputConnection( mapper2->GetOutputPort() );

  
  vtkSmartPointer<vtkImageActor> actor2 = vtkSmartPointer<vtkImageActor>::New();
  actor2->SetUserTransform( resliceAxesTransform2 );
	actor2->GetMapper()->SetInputConnection( mapper2->GetOutputPort() );

	vtkSmartPointer<vtkImageActor> actor3 = vtkSmartPointer<vtkImageActor>::New();
	actor3->SetUserTransform( resliceAxesTransform3 );
	actor3->GetMapper()->SetInputConnection( mapper2->GetOutputPort() );

  // renderer
  vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();


  renderer->AddActor(actor);
	renderer->AddActor(actor2);
	renderer->AddActor(actor3);  //wrong place because image is not the right size

  //render window
  vtkSmartPointer<vtkRenderWindow> window = vtkSmartPointer<vtkRenderWindow>::New();
  window->AddRenderer(renderer);

	// Set up the interaction
  vtkSmartPointer<vtkRenderWindowInteractor> interactor =  vtkSmartPointer<vtkRenderWindowInteractor>::New();



  window->SetInteractor( interactor );

  window->Render();
  window->Start();

  interactor->Start();
//#endif
  std::cin.ignore();
  return 0;
}