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
  Hospital. The project was supported by grants 5P01CA067165,
  5R01CA124377, 5R01CA138586, 2R44DE019322, 7R01CA124377,
  5R42CA137886, 5R42CA137886

==============================================================================*/

#ifndef __qSlicerVisuaLineTreeItem_h
#define __qSlicerVisuaLineTreeItem_h

// Standards
#include <sstream>

// Qt includes
#include <QStandardItem>

// MRML includes
#include "vtkMRMLAnnotationFiducialNode.h"
#include "vtkMRMLAnnotationLineDisplayNode.h"
#include "vtkMRMLAnnotationPointDisplayNode.h"
#include "vtkMRMLAnnotationRulerNode.h"
#include "vtkMRMLAnnotationTextDisplayNode.h"

// VTK includes
#include "vtkMath.h"
#include <ctkVTKObject.h>

class qSlicerVisuaLineTreeRootItem;
class qSlicerVisuaLineTreeItem : public QObject, public QStandardItem
{
  Q_OBJECT
  QVTK_OBJECT
 public:
    qSlicerVisuaLineTreeItem(const QString& text);
  ~qSlicerVisuaLineTreeItem();
  
  // MRML Nodes
  void setPathNode(vtkMRMLAnnotationRulerNode* rulerNode);
  vtkMRMLAnnotationRulerNode* getPathNode();
  void setTargetNode(vtkMRMLAnnotationFiducialNode* fiducialNode);
  vtkMRMLAnnotationFiducialNode* getTargetNode();
  void setVirtualOffsetNode(vtkMRMLAnnotationRulerNode* virtualTip);
  vtkMRMLAnnotationRulerNode* getVirtualOffsetNode();
  inline void setVisibility(bool visibility);
  inline void setPathVisibility(bool visibility);
  inline void setTargetVisibility(bool visibility);
  inline void setOffsetVisibility(bool visibility);
  inline void setPathItem(bool isPath);
  inline bool isPathItem();

  // Virtual offset
  void setVirtualOffset(double offset);
  double getVirtualOffset();
  
 protected slots:
   void onPathNodeModified();
   void onTargetNodeModified();
  
 private:

  // MRML Nodes
  vtkMRMLAnnotationRulerNode* PathNode;
  vtkMRMLAnnotationRulerNode* VirtualOffsetNode;
  vtkMRMLAnnotationFiducialNode* TargetNode;

  // Virtual offset
  double OffsetValue;

  // Differentiation between target and path
  bool PathItem;
};

//----------------------------------------------------------------------------
void qSlicerVisuaLineTreeItem::
setVisibility(bool visibility)
{
  this->setPathVisibility(visibility);
  this->setTargetVisibility(visibility);
  this->setOffsetVisibility(visibility);
}

//----------------------------------------------------------------------------
void qSlicerVisuaLineTreeItem::
setPathVisibility(bool visibility)
{
  if (this->PathNode)
    {
    this->PathNode->SetDisplayVisibility(visibility);
    }
}

//----------------------------------------------------------------------------
void qSlicerVisuaLineTreeItem::
setTargetVisibility(bool visibility)
{
  if (this->TargetNode)
    {
    this->TargetNode->SetDisplayVisibility(visibility);
    }
}

//----------------------------------------------------------------------------
void qSlicerVisuaLineTreeItem::
setOffsetVisibility(bool visibility)
{
  if (this->VirtualOffsetNode)
    {
    this->VirtualOffsetNode->SetDisplayVisibility(visibility);
    }
}

//----------------------------------------------------------------------------
void qSlicerVisuaLineTreeItem::
setPathItem(bool isPath)
{
  this->PathItem = isPath;
}

//----------------------------------------------------------------------------
bool qSlicerVisuaLineTreeItem::
isPathItem()
{
  return this->PathItem;
}

#endif
