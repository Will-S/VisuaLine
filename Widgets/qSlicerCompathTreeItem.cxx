/*==============================================================================

  Program: 3D Slicer

  Copyright (c) Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

#include "qSlicerCompathTreeItem.h"

// --------------------------------------------------------------------------
qSlicerCompathTreeItem
::qSlicerCompathTreeItem(const QString& text) : QStandardItem(text)
{
  this->PathNode = NULL;
  this->TargetNode = NULL;
  this->VirtualOffsetNode = NULL;
  this->OffsetValue = 0;
}

// --------------------------------------------------------------------------
qSlicerCompathTreeItem::
~qSlicerCompathTreeItem()
{
  this->setVisibility(false);
}

// --------------------------------------------------------------------------
void qSlicerCompathTreeItem::
setPathNode(vtkMRMLAnnotationRulerNode* rulerNode)
{
  if (rulerNode)
    {
    qvtkReconnect(this->PathNode, rulerNode, vtkCommand::ModifiedEvent,
		  this, SLOT(onPathNodeModified()));
    this->PathNode = rulerNode;
    }
}

// --------------------------------------------------------------------------
vtkMRMLAnnotationRulerNode* qSlicerCompathTreeItem::
getPathNode()
{
  return this->PathNode;
}

// --------------------------------------------------------------------------
void qSlicerCompathTreeItem::
setTargetNode(vtkMRMLAnnotationFiducialNode* fiducialNode)
{
  if (fiducialNode)
    {
    qvtkReconnect(this->TargetNode, fiducialNode, vtkCommand::ModifiedEvent,
		  this, SLOT(onTargetNodeModified()));
    this->TargetNode = fiducialNode;
    }
}

// --------------------------------------------------------------------------
vtkMRMLAnnotationFiducialNode* qSlicerCompathTreeItem::
getTargetNode()
{
  return this->TargetNode;
}

// --------------------------------------------------------------------------
void qSlicerCompathTreeItem::
setVirtualOffsetNode(vtkMRMLAnnotationRulerNode* virtualTip)
{
  if (!virtualTip || !this->PathNode)
    {
    return;
    }

  // Set color to green
  if (virtualTip->GetAnnotationLineDisplayNode() &&
      virtualTip->GetAnnotationPointDisplayNode() &&
      virtualTip->GetAnnotationTextDisplayNode())
    {
    virtualTip->GetAnnotationLineDisplayNode()->SetColor(0,1,0);
    virtualTip->GetAnnotationPointDisplayNode()->SetColor(0,1,0);
    virtualTip->GetAnnotationTextDisplayNode()->SetColor(0,1,0);
    }

  // Set points to target point
  double p2[3];
  this->PathNode->GetPosition2(p2);
  virtualTip->SetPosition1(p2);
  virtualTip->SetPosition2(p2);
  virtualTip->SetLocked(1);
  this->VirtualOffsetNode = virtualTip;
  this->setOffsetVisibility(false);
}

// --------------------------------------------------------------------------
vtkMRMLAnnotationRulerNode* qSlicerCompathTreeItem::
getVirtualOffsetNode()
{
  return this->VirtualOffsetNode;
}

// --------------------------------------------------------------------------
void qSlicerCompathTreeItem::
setVirtualOffset(double offset)
{
/*
  if (this->offsetValue == offset)
    {
    return;
    }

  if (this->VirtualOffsetNode && this->PathNode)
    {
    // Only modify offset if ruler visible
    if (this->VirtualOffsetNode->GetDisplayVisibility())
      {
      // Set points position
      double p1[3], p2[3], p2minusp1[3];
      this->PathNode->GetPosition1(p1);
      this->PathNode->GetPosition2(p2);
      vtkMath::Subtract(p2,p1,p2minusp1);
      vtkMath::Normalize(p2minusp1);
      
      double offsetCoordinates[3];
      offsetCoordinates[0] = p2[0] + p2minusp1[0]*offset;
      offsetCoordinates[1] = p2[1] + p2minusp1[1]*offset;
      offsetCoordinates[2] = p2[2] + p2minusp1[2]*offset;

      this->VirtualOffsetNode->SetPosition1(p2);
      this->VirtualOffsetNode->SetPosition2(offsetCoordinates);
      this->offsetValue = offset;
      this->VirtualOffsetNode->Modified();
      }
    }
*/
}

// --------------------------------------------------------------------------
double qSlicerCompathTreeItem::
getVirtualOffset()
{
/*
  if (this->VirtualOffsetNode)
    {
    std::cerr << "Offset for " << this->getPathNode()->GetName() << ": " << this->VirtualOffsetNode->GetDistanceMeasurement() << std::endl;
    return this->VirtualOffsetNode->GetDistanceMeasurement();
    }
  std::cerr << "Fail" << std::endl;
  return 0;
*/
  return this->OffsetValue;
}

// --------------------------------------------------------------------------
void qSlicerCompathTreeItem::
onPathNodeModified()
{
}

// --------------------------------------------------------------------------
void qSlicerCompathTreeItem::
onTargetNodeModified()
{
}

