#include "DEM.h"


DEM<4> Diag;

DEM_event* Temprature_Above_Threshold = Diag.create_event("Temprature_Above_Threshold",3,-3);
DEM_event* Temprature_Below_Threshold = Diag.create_event("Temprature_Below_Threshold",3,-3);
DEM_event* Temprature_Invalid_value   = Diag.create_event("Temprature_Invalid_value",3,-3);
DEM_event* Temprature_DTC             = Diag.create_event("Temprature_DTC",1,-1);