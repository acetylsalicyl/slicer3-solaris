#ifndef __vtkIGTNavigationGUI_h
#define __vtkIGTNavigationGUI_h

#include "vtkSlicerModuleGUI.h"
#include "vtkIGTNavigation.h"

class vtkIGTNavigationLogic;
class vtkIGTNavigationMRMLManager;
class vtkKWWizardWidget;
class vtkIGTNavigationInitializationStep;
class vtkIGTNavigationLoadingPreoperativeDataStep;
class vtkIGTNavigationCalibrationStep;
class vtkIGTNavigationRegistrationStep;
class vtkIGTNavigationIntraoperativeProcedureStep;
class vtkMRMLNode;

class VTK_IGT_EXPORT vtkIGTNavigationGUI : 
  public vtkSlicerModuleGUI
{
public:
  static vtkIGTNavigationGUI *New();
  vtkTypeMacro(vtkIGTNavigationGUI,vtkSlicerModuleGUI);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description: 
  // Get the categorization of the module.
  const char *GetCategory() const { return "IGT"; }

  // Description: 
  // Get/Set logic node
  vtkGetObjectMacro(Logic, vtkIGTNavigationLogic);
//  virtual void SetLogic(vtkIGTNavigationLogic*);

  /// Implement setter for vtkLogic* pointer
//  virtual void SetModuleLogic(vtkSlicerLogic*);
  void SetModuleLogic ( vtkSlicerLogic *logic );
 
  // Get/Set mrml manager node
  vtkGetObjectMacro(MRMLManager, vtkIGTNavigationMRMLManager);
  virtual void SetMRMLManager(vtkIGTNavigationMRMLManager*);

  // Description: 
  // Get/Set MRML node
  vtkGetObjectMacro(Node, vtkMRMLNode);
  virtual void SetNode(vtkMRMLNode*);

  // Description: 
  // Get wizard widget
  vtkGetObjectMacro(WizardWidget, vtkKWWizardWidget);

  // Description:
  // Create widgets
  virtual void BuildGUI();

  // Description:
  // Initialize module
  virtual void Init();

  // Description:
  // Delete Widgets
  virtual void TearDownGUI();

  // Description:
  // Add observers to GUI widgets
  virtual void AddGUIObservers();
  
  // Description:
  // Remove observers to GUI widgets
  //virtual void RemoveGUIObservers();

  // Description:
  // Remove observers to MRML node
  //virtual void RemoveMRMLNodeObservers();

  // Description:
  // Remove observers to Logic
  virtual void RemoveLogicObservers();
  
  // Description:
  // Pprocess events generated by Logic
  virtual void ProcessLogicEvents( vtkObject *caller, unsigned long event,
                                   void *callData); 

  // Description:
  // Pprocess events generated by GUI widgets
  /*virtual void ProcessGUIEvents( vtkObject *caller, unsigned long event,
                                 void *callData); */
  // Description:
  // Process events generated by MRML
  virtual void ProcessMRMLEvents( vtkObject *caller, unsigned long event, 
                                  void *callData);

  // Description:
  // Describe behavior at module startup and exit.
  virtual void Enter(){};
  virtual void Exit(){};

  // Description: The name of the Module - this is used to 
  // construct the proc invocations
  vtkGetStringMacro(ModuleName);
  vtkSetStringMacro(ModuleName);

  // Description: set an observer by number (work around
  // limitation in kwwidgets tcl wrapping)
  unsigned long AddObserverByNumber(vtkObject *observee, unsigned long event);

protected:

private:
  vtkIGTNavigationGUI();
  ~vtkIGTNavigationGUI();
  vtkIGTNavigationGUI(const vtkIGTNavigationGUI&);
  void operator=(const vtkIGTNavigationGUI&);

  // Description:
  // Updates GUI widgets based on parameters values in MRML node
  void UpdateGUI();

  // Description:
  // Updates parameters values in MRML node based on GUI widgets 
  void UpdateMRML();
  
  vtkIGTNavigationLogic       *Logic;
  vtkIGTNavigationMRMLManager *MRMLManager;
  vtkMRMLNode       *Node;
  
  char *ModuleName;

  // Description:
  // The wizard widget and steps
  vtkKWWizardWidget                      *WizardWidget;
  vtkIGTNavigationInitializationStep               *InitializationStep;
  vtkIGTNavigationLoadingPreoperativeDataStep      *LoadingPreoperativeDataStep;
  vtkIGTNavigationCalibrationStep                  *CalibrationStep; 
  vtkIGTNavigationRegistrationStep                 *RegistrationStep; 
  vtkIGTNavigationIntraoperativeProcedureStep      *IntraoperativeProcedureStep; 

};

#endif