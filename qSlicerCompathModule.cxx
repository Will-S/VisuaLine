/*==============================================================================

  Program: 3D Slicer

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Laurent Chauvin, Brigham and Women's
  Hospital, based on a template developed by Jean-Christophe Fillion-Robin,
  Kitware Inc. The project was supported by grants 5P01CA067165,
  5R01CA124377, 5R01CA138586, 2R44DE019322, 7R01CA124377,
  5R42CA137886, 5R42CA137886

==============================================================================*/

// Qt includes
#include <QtPlugin>

// Compath Logic includes
#include <vtkSlicerCompathLogic.h>

// Compath includes
#include "qSlicerCompathModule.h"
#include "qSlicerCompathModuleWidget.h"

//-----------------------------------------------------------------------------
Q_EXPORT_PLUGIN2(qSlicerCompathModule, qSlicerCompathModule);

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerCompathModulePrivate
{
public:
  qSlicerCompathModulePrivate();
};

//-----------------------------------------------------------------------------
// qSlicerCompathModulePrivate methods

//-----------------------------------------------------------------------------
qSlicerCompathModulePrivate
::qSlicerCompathModulePrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerCompathModule methods

//-----------------------------------------------------------------------------
qSlicerCompathModule
::qSlicerCompathModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qSlicerCompathModulePrivate)
{
}

//-----------------------------------------------------------------------------
qSlicerCompathModule::~qSlicerCompathModule()
{
}

//-----------------------------------------------------------------------------
QString qSlicerCompathModule::helpText()const
{
  return "This is a loadable module bundled in an extension";
}

//-----------------------------------------------------------------------------
QString qSlicerCompathModule::acknowledgementText()const
{
  return "This work was was partially funded by NIH grant 3P41RR013218-12S1";
}

//-----------------------------------------------------------------------------
QStringList qSlicerCompathModule::contributors()const
{
  QStringList moduleContributors;
  moduleContributors << QString("Laurent Chauvin (BWH)");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QIcon qSlicerCompathModule::icon()const
{
  return QIcon(":/Icons/Compath.png");
}

//-----------------------------------------------------------------------------
QStringList qSlicerCompathModule::categories() const
{
  return QStringList() << "IGT";
}

//-----------------------------------------------------------------------------
QStringList qSlicerCompathModule::dependencies() const
{
  return QStringList() << "Annotations";
}

//-----------------------------------------------------------------------------
void qSlicerCompathModule::setup()
{
  this->Superclass::setup();
}

//-----------------------------------------------------------------------------
qSlicerAbstractModuleRepresentation * qSlicerCompathModule
::createWidgetRepresentation()
{
  return new qSlicerCompathModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qSlicerCompathModule::createLogic()
{
  return vtkSlicerCompathLogic::New();
}
