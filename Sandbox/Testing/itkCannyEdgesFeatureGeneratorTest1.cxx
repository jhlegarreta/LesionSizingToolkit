/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkCannyEdgesFeatureGeneratorTest1.cxx

  Copyright (c) Kitware Inc. 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkCannyEdgesFeatureGenerator.h"
#include "itkImage.h"
#include "itkSpatialObject.h"
#include "itkImageSpatialObject.h"
#include "itkImageMaskSpatialObject.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

int main( int argc, char * argv [] )
{

  if( argc < 3 )
    {
    std::cerr << "Missing Arguments" << std::endl;
    std::cerr << argv[0] << " inputImage outputImage [variance upperthreshold lowerthreshold]" << std::endl;
    return EXIT_FAILURE;
    }

  const unsigned int Dimension = 3;
  typedef signed short    InputPixelType;
  typedef float           OutputPixelType;

  typedef itk::Image< InputPixelType,  Dimension >   InputImageType;
  typedef itk::Image< OutputPixelType, Dimension >   OutputImageType;

  typedef itk::ImageFileReader< InputImageType >     ReaderType;
  typedef itk::ImageFileWriter< OutputImageType >    WriterType;

  typedef itk::ImageSpatialObject< Dimension, InputPixelType  > InputImageSpatialObjectType;
  typedef itk::ImageSpatialObject< Dimension, OutputPixelType > OutputImageSpatialObjectType;

  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName( argv[1] );

  try 
    {
    reader->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }

  typedef itk::CannyEdgesFeatureGenerator< Dimension >   CannyEdgesFeatureGeneratorType;
  typedef CannyEdgesFeatureGeneratorType::SpatialObjectType    SpatialObjectType;

  CannyEdgesFeatureGeneratorType::Pointer  featureGenerator = CannyEdgesFeatureGeneratorType::New();
  

  InputImageSpatialObjectType::Pointer inputObject = InputImageSpatialObjectType::New();

  InputImageType::Pointer inputImage = reader->GetOutput();

  inputImage->DisconnectPipeline();

  inputObject->SetImage( inputImage );

  featureGenerator->SetInput( inputObject );

  if( argc > 3 )
    {
    featureGenerator->SetVariance( atof( argv[3] ) );
    }

  if( argc > 4 )
    {
    featureGenerator->SetUpperThreshold( atof( argv[4] ) );
    }

  if( argc > 5 )
    {
    featureGenerator->SetLowerThreshold( atof( argv[4] ) );
    }

  try 
    {
    featureGenerator->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }


  SpatialObjectType::ConstPointer feature = featureGenerator->GetFeature();

  OutputImageSpatialObjectType::ConstPointer outputObject = 
    dynamic_cast< const OutputImageSpatialObjectType * >( feature.GetPointer() );

  OutputImageType::ConstPointer outputImage = outputObject->GetImage();

  WriterType::Pointer writer = WriterType::New();

  writer->SetFileName( argv[2] );
  writer->SetInput( outputImage );
  writer->UseCompressionOn();


  try 
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }


  std::cout << "Name Of Class = " << featureGenerator->GetNameOfClass() << std::endl;

  featureGenerator->Print( std::cout );

 
  return EXIT_SUCCESS;
}
