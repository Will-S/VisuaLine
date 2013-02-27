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

#ifndef __qSlicerCompathTreeItem_h
#define __qSlicerCompathTreeItem_h

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

class qSlicerCompathTreeRootItem;
class qSlicerCompathTreeItem : public QObject, public QStandardItem
{
  Q_OBJECT
  QVTK_OBJECT
 public:
    qSlicerCompathTreeItem(const QString& text);
  ~qSlicerCompathTreeItem();
  
  // Qt Tree Items
/*
  qSlicerCompathTreeItem* parent();
  void appendChild(QString child);  
  QString child(int row);
  int childCount() const;
  int columnCount() const;
  int row() const;
  QVariant data(int column) const;
  //bool removeChildren(int position, int count);
  */

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
  
  // Virtual offset
  void setVirtualOffset(double offset);
  double getVirtualOffset();
  
 protected slots:
   void onPathNodeModified();
   void onTargetNodeModified();
  
 private:
/*
  QStringList childItems;
  QVector<QVariant> itemData;
  qSlicerCompathTreeRootItem* parentItem;
*/
  // MRML Nodes
  vtkMRMLAnnotationRulerNode* PathNode;
  vtkMRMLAnnotationRulerNode* VirtualOffsetNode;
  vtkMRMLAnnotationFiducialNode* TargetNode;

  // Virtual offset
  double OffsetValue;
};

//----------------------------------------------------------------------------
void qSlicerCompathTreeItem::
setVisibility(bool visibility)
{
  this->setPathVisibility(visibility);
  this->setTargetVisibility(visibility);
  this->setOffsetVisibility(visibility);
}

//----------------------------------------------------------------------------
void qSlicerCompathTreeItem::
setPathVisibility(bool visibility)
{
  if (this->PathNode)
    {
    this->PathNode->SetDisplayVisibility(visibility);
    }
}

//----------------------------------------------------------------------------
void qSlicerCompathTreeItem::
setTargetVisibility(bool visibility)
{
  if (this->TargetNode)
    {
    this->TargetNode->SetDisplayVisibility(visibility);
    }
}

//----------------------------------------------------------------------------
void qSlicerCompathTreeItem::
setOffsetVisibility(bool visibility)
{
  if (this->VirtualOffsetNode)
    {
    this->VirtualOffsetNode->SetDisplayVisibility(visibility);
    }
}

#endif
