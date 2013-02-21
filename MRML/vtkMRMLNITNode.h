#ifndef __vtkMRMLNITNode_h
#define __vtkMRMLNITNode_h

#include "vtkSlicerCompathModuleMRMLExport.h"
#include "vtkMRMLAnnotationHierarchyNode.h" 

class vtkMRMLNode;
class vtkMRMLScene;

/// \ingroup Slicer_QtModules_Annotation
class  VTK_SLICER_ANNOTATIONS_MODULE_MRML_EXPORT vtkMRMLNITNode : public vtkMRMLAnnotationHierarchyNode
{
public:
  static vtkMRMLNITNode *New();
  vtkTypeMacro(vtkMRMLNITNode, vtkMRMLAnnotationHierarchyNode);

  //--------------------------------------------------------------------------
  // MRMLNode methods
  //--------------------------------------------------------------------------

  virtual vtkMRMLNode* CreateNodeInstance();
  // Description:
  // Get node XML tag name (like Volume, Model)
  virtual const char* GetNodeTagName() {return "NIT";};

  virtual const char* GetIcon() {return ":/Icons/NIT.png";};

  // Description:
  // Read node attributes from XML file
  virtual void ReadXMLAttributes( const char** atts);
  
  // Description:
  // Write this node's information to a MRML file in XML format.
  virtual void WriteXML(ostream& of, int indent);

  virtual void UpdateScene(vtkMRMLScene *scene);

  // Description:
  // Copy the node's attributes to this object
  virtual void Copy(vtkMRMLNode *node);

  // Description:
  // alternative method to propagate events generated in Display nodes
  virtual void ProcessMRMLEvents ( vtkObject * /*caller*/, 
                                   unsigned long /*event*/, 
                                   void * /*callData*/ );

protected:
  vtkMRMLNITNode();
  ~vtkMRMLNITNode();
  void operator=(const vtkMRMLNITNode&); 
};

#endif
