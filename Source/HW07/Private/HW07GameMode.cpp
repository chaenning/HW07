#include "HW07GameMode.h"
#include "HW07Character.h"
#include "HW07PlayerController.h"


AHW07GameMode::AHW07GameMode()
{
	DefaultPawnClass = AHW07Character::StaticClass();
	PlayerControllerClass = AHW07PlayerController::StaticClass();
}