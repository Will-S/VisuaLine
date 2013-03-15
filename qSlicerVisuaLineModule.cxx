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

// VisuaLine Logic includes
#include <vtkSlicerVisuaLineLogic.h>

// VisuaLine includes
#include "qSlicerVisuaLineModule.h"
#include "qSlicerVisuaLineModuleWidget.h"

//-----------------------------------------------------------------------------
Q_EXPORT_PLUGIN2(qSlicerVisuaLineModule, qSlicerVisuaLineModule);

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerVisuaLineModulePrivate
{
public:
  qSlicerVisuaLineModulePrivate();
};

//-----------------------------------------------------------------------------
// qSlicerVisuaLineModulePrivate methods

//-----------------------------------------------------------------------------
qSlicerVisuaLineModulePrivate
::qSlicerVisuaLineModulePrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerVisuaLineModule methods

//-----------------------------------------------------------------------------
qSlicerVisuaLineModule
::qSlicerVisuaLineModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qSlicerVisuaLineModulePrivate)
{
}

//-----------------------------------------------------------------------------
qSlicerVisuaLineModule::~qSlicerVisuaLineModule()
{
}

//-----------------------------------------------------------------------------
QString qSlicerVisuaLineModule::helpText()const
{
  return "This is a loadable module bundled in an extension";
}

//-----------------------------------------------------------------------------
QString qSlicerVisuaLineModule::acknowledgementText()const
{
  return "This work was was partially funded by NIH grant 3P41RR013218-12S1";
}

//-----------------------------------------------------------------------------
QStringList qSlicerVisuaLineModule::contributors()const
{
  QStringList moduleContributors;
  moduleContributors << QString("Laurent Chauvin (BWH)");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QIcon qSlicerVisuaLineModule::icon()const
{
  return QIcon(":/Icons/VisuaLine.png");
}

//-----------------------------------------------------------------------------
QStringList qSlicerVisuaLineModule::categories() const
{
  return QStringList() << "IGT";
}

//-----------------------------------------------------------------------------
QStringList qSlicerVisuaLineModule::dependencies() const
{
  return QStringList() << "Annotations";
}

//-----------------------------------------------------------------------------
void qSlicerVisuaLineModule::setup()
{
  this->Superclass::setup();
}

//-----------------------------------------------------------------------------
qSlicerAbstractModuleRepresentation * qSlicerVisuaLineModule
::createWidgetRepresentation()
{
  return new qSlicerVisuaLineModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qSlicerVisuaLineModule::createLogic()
{
  return vtkSlicerVisuaLineLogic::New();
}
