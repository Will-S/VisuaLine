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

#include "qSlicerVisuaLineTreeItem.h"

// --------------------------------------------------------------------------
qSlicerVisuaLineTreeItem
::qSlicerVisuaLineTreeItem(const QString& text) : QStandardItem(text)
{
  this->PathNode = NULL;
  this->TargetNode = NULL;
  this->VirtualOffsetNode = NULL;
  this->OffsetValue = 0;
  this->PathItem = false;
}

// --------------------------------------------------------------------------
qSlicerVisuaLineTreeItem::
~qSlicerVisuaLineTreeItem()
{
  this->setVisibility(false);
}

// --------------------------------------------------------------------------
void qSlicerVisuaLineTreeItem::
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
vtkMRMLAnnotationRulerNode* qSlicerVisuaLineTreeItem::
getPathNode()
{
  return this->PathNode;
}

// --------------------------------------------------------------------------
void qSlicerVisuaLineTreeItem::
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
vtkMRMLAnnotationFiducialNode* qSlicerVisuaLineTreeItem::
getTargetNode()
{
  return this->TargetNode;
}

// --------------------------------------------------------------------------
void qSlicerVisuaLineTreeItem::
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
  this->setOffsetVisibility(true);
}

// --------------------------------------------------------------------------
vtkMRMLAnnotationRulerNode* qSlicerVisuaLineTreeItem::
getVirtualOffsetNode()
{
  return this->VirtualOffsetNode;
}

// --------------------------------------------------------------------------
void qSlicerVisuaLineTreeItem::
setVirtualOffset(double offset)
{
  if (!this->VirtualOffsetNode || !this->PathNode)
    {
    return;
    }

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
  this->OffsetValue = offset;

  // Workaround for issue on top
  this->setOffsetVisibility(offset == 0 ? false : true);
  this->VirtualOffsetNode->Modified();
}

// --------------------------------------------------------------------------
double qSlicerVisuaLineTreeItem::
getVirtualOffset()
{

/*
 *  // bug with Annotation ruler. DistanceMeasurement always return 0
 *
 *  if (this->VirtualOffsetNode)
 *   {
 *   return this->VirtualOffsetNode->GetDistanceMeasurement();
 *   }
 *  return 0;
*/
  return this->OffsetValue;
}

// --------------------------------------------------------------------------
void qSlicerVisuaLineTreeItem::
onPathNodeModified()
{
  if (!this->PathNode || !this->TargetNode)
    {
    return;
    }

  // Update target
  double p2[3];
  this->PathNode->GetPosition2(p2);
  this->TargetNode->SetFiducialCoordinates(p2);
  
  // Update virtual offset
  if (this->VirtualOffsetNode && this->OffsetValue != 0)
    {
    this->setVirtualOffset(this->OffsetValue);
    }
}

// --------------------------------------------------------------------------
void qSlicerVisuaLineTreeItem::
onTargetNodeModified()
{
  if (!this->TargetNode || !this->PathNode)
    {
    return;
    }

  double* targetPosition;
  targetPosition = this->TargetNode->GetFiducialCoordinates();

  // Modify ruler target point
  this->PathNode->SetPosition2(targetPosition);

  // Update text
  std::stringstream targetStream;
  targetStream.precision(2);
  targetStream.setf(std::ios::fixed);
  targetStream << "Target (R: " << targetPosition[0]
               << ", A: " << targetPosition[1]
               << ", S: " << targetPosition[2]
               << ")";
  QString targetString = QString(targetStream.str().c_str());

  // Get last child
  if (this->hasChildren())
    {
    qSlicerVisuaLineTreeItem* targetItem
      = dynamic_cast<qSlicerVisuaLineTreeItem*>(this->child(this->rowCount()-1));
    if (targetItem && !targetItem->isPathItem())
      {
      targetItem->setData(targetString, Qt::DisplayRole);
      }
    }

  // Update virtual offset
  if (this->VirtualOffsetNode && this->OffsetValue != 0)
    {
    this->setVirtualOffset(this->OffsetValue);
    }
}

