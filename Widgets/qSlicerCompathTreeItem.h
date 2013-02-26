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

#include <QList>
#include <QVector>
#include <QVariant>

#include "vtkMRMLAnnotationRulerNode.h"
#include "vtkMRMLAnnotationFiducialNode.h"
#include "vtkMRMLAnnotationLineDisplayNode.h"

#include "vtkMath.h"

class qSlicerCompathTreeItem
{
 public:
  qSlicerCompathTreeItem(const QVector<QVariant> &data, qSlicerCompathTreeItem* parent = 0);
  ~qSlicerCompathTreeItem();
  
  void appendChild(qSlicerCompathTreeItem* child);  
  qSlicerCompathTreeItem* child(int row);
  int childCount() const;
  int columnCount() const;
  QVariant data(int column) const;
  int row() const;
  qSlicerCompathTreeItem* parent();
  Qt::CheckState isChecked() const;
  void setCheckState(Qt::CheckState set);
  void setPathNode(vtkMRMLAnnotationRulerNode* rulerNode);
  vtkMRMLAnnotationRulerNode* getPathNode();
  void setTargetNode(vtkMRMLAnnotationFiducialNode* fiducialNode);
  vtkMRMLAnnotationFiducialNode* getTargetNode();
  void setVirtualOffset(double offset);
  double getVirtualOffset();
  void setVirtualOffsetNode(vtkMRMLAnnotationRulerNode* virtualTip);
  vtkMRMLAnnotationRulerNode* getVirtualOffsetNode();

 private:
  QList<qSlicerCompathTreeItem*> childItems;
  QVector<QVariant> itemData;
  qSlicerCompathTreeItem* parentItem;
  Qt::CheckState checkState;
  vtkMRMLAnnotationRulerNode* PathNode;
  vtkMRMLAnnotationRulerNode* VirtualOffsetNode;
  vtkMRMLAnnotationFiducialNode* TargetNode;
};

#endif
