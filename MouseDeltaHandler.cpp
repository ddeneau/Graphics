// Handles mouse actions and their effects on the program (needs to be taken out of main and abstracted in)
#include "MouseDeltaHandler.h"

void MouseDeltaHandler::HandleMouseDelta(Camera& camera, InputManager* inputManager) {

    // We use the variables defined in the header blueprint
    mouseDelta = inputManager->GetMouseDelta();

}