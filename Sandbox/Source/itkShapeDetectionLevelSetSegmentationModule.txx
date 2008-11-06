/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkShapeDetectionLevelSetSegmentationModule.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkShapeDetectionLevelSetSegmentationModule_txx
#define __itkShapeDetectionLevelSetSegmentationModule_txx

#include "itkShapeDetectionLevelSetSegmentationModule.h"
#include "itkShapeDetectionLevelSetImageFilter.h"


namespace itk
{


/**
 * Constructor
 */
template <unsigned int NDimension>
ShapeDetectionLevelSetSegmentationModule<NDimension>
::ShapeDetectionLevelSetSegmentationModule()
{
}


/**
 * Destructor
 */
template <unsigned int NDimension>
ShapeDetectionLevelSetSegmentationModule<NDimension>
::~ShapeDetectionLevelSetSegmentationModule()
{
}


/**
 * PrintSelf
 */
template <unsigned int NDimension>
void
ShapeDetectionLevelSetSegmentationModule<NDimension>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf( os, indent );
}


/**
 * Generate Data
 */
template <unsigned int NDimension>
void
ShapeDetectionLevelSetSegmentationModule<NDimension>
::GenerateData()
{
  const InputSpatialObjectType * inputObject =
    dynamic_cast< const InputSpatialObjectType * >( this->GetInput() );

  const FeatureSpatialObjectType * featureObject =
    dynamic_cast< const FeatureSpatialObjectType * >( this->GetFeature() );


  const InputImageType * inputImage = inputObject->GetImage();
  const FeatureImageType * featureImage = featureObject->GetImage();

  typedef itk::ShapeDetectionLevelSetImageFilter<
    InputImageType, FeatureImageType, OutputPixelType > FilterType;

  typedef itk::SpatialObject< NDimension >        SpatialObjectType;

  typename FilterType::Pointer filter = FilterType::New();

  filter->SetInput( inputImage );
  filter->SetFeatureImage( featureImage );

  filter->SetMaximumRMSError( this->GetMaximumRMSError() );
  filter->SetNumberOfIterations( this->GetMaximumNumberOfIterations() );
  filter->SetPropagationScaling( this->GetPropagationScaling() );
  filter->SetCurvatureScaling( this->GetCurvatureScaling() );
  filter->SetAdvectionScaling( 0.0 );

  filter->Update();

  std::cout << std::endl;
  std::cout << "Max. no. iterations: " << filter->GetNumberOfIterations() << std::endl;
  std::cout << "Max. RMS error: " << filter->GetMaximumRMSError() << std::endl;
  std::cout << std::endl;
  std::cout << "No. elpased iterations: " << filter->GetElapsedIterations() << std::endl;
  std::cout << "RMS change: " << filter->GetRMSChange() << std::endl;


  typename OutputImageType::Pointer outputImage = filter->GetOutput();

  outputImage->DisconnectPipeline();

  OutputSpatialObjectType * outputObject =
    dynamic_cast< OutputSpatialObjectType * >(this->ProcessObject::GetOutput(0));

  outputObject->SetImage( outputImage );
}

} // end namespace itk

#endif
