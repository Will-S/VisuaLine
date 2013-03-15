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

==============================================================================*/

// VisuaLine Logic includes
#include "vtkSlicerVisuaLineLogic.h"

// MRML includes
#include "vtkMRMLNITNode.h"

// VTK includes
#include <vtkNew.h>

// STD includes
#include <cassert>

//----------------------------------------------------------------------------
vtkStandardNewMacro(vtkSlicerVisuaLineLogic);

//----------------------------------------------------------------------------
vtkSlicerVisuaLineLogic::vtkSlicerVisuaLineLogic()
{
}

//----------------------------------------------------------------------------
vtkSlicerVisuaLineLogic::~vtkSlicerVisuaLineLogic()
{
}

//----------------------------------------------------------------------------
void vtkSlicerVisuaLineLogic::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

//---------------------------------------------------------------------------
void vtkSlicerVisuaLineLogic::SetMRMLSceneInternal(vtkMRMLScene * newScene)
{
  vtkNew<vtkIntArray> events;
  events->InsertNextValue(vtkMRMLScene::NodeAddedEvent);
  events->InsertNextValue(vtkMRMLScene::NodeRemovedEvent);
  events->InsertNextValue(vtkMRMLScene::EndBatchProcessEvent);
  this->SetAndObserveMRMLSceneEventsInternal(newScene, events.GetPointer());
}

//-----------------------------------------------------------------------------
void vtkSlicerVisuaLineLogic::RegisterNodes()
{
  assert(this->GetMRMLScene() != 0);

  vtkMRMLNITNode* nitNode = vtkMRMLNITNode::New();
  this->GetMRMLScene()->RegisterNodeClass(nitNode);
  nitNode->Delete();
}

//---------------------------------------------------------------------------
void vtkSlicerVisuaLineLogic::UpdateFromMRMLScene()
{
  assert(this->GetMRMLScene() != 0);
}

//---------------------------------------------------------------------------
void vtkSlicerVisuaLineLogic
::OnMRMLSceneNodeAdded(vtkMRMLNode* vtkNotUsed(node))
{
}

//---------------------------------------------------------------------------
void vtkSlicerVisuaLineLogic
::OnMRMLSceneNodeRemoved(vtkMRMLNode* vtkNotUsed(node))
{
}

