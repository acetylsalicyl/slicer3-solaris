#include "vtkPichonFastMarching.h"
#include "TestingMacros.h"

int vtkPichonFastMarchingTest(int, char * [] ){
  vtkSmartPointer<vtkPichonFastMarching> filter = 
    vtkSmartPointer<vtkPichonFastMarching>::New();

typedef vtkPichonFastMarching Superclass, MySuperclass;

  EXERCISE_BASIC_OBJECT_METHODS(filter);

  return EXIT_SUCCESS;
}
