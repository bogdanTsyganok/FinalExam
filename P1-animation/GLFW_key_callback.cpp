#include "globalThings.h"

#include <sstream>
#include <iostream>


// Turns off the: warning C4005: 'APIENTRY': macro redefinition
#pragma warning( disable: 4005)
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
void UseMessageBox(void)
{

    // If you want to do some Win32 calls:
    // Note the "L" which is "wide" or Unicode encoding
    //MessageBox(NULL, L"Moved Camera", L"FYI:", MB_OK);
    int response = MessageBox(NULL, L"Moved Camera", L"FYI:", MB_YESNOCANCEL | MB_ICONWARNING);
    switch (response)
    {
    case IDYES:
        std::cout << "You chose YES" << std::endl;
        break;
    case IDNO:
        std::cout << "You chose NO" << std::endl;
        break;
    case IDCANCEL:
        std::cout << "You chose CANCEL" << std::endl;
        break;
    }

    return;
}

/*static*/ 
void GLFW_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }


    // From the help review session.
    // Move the camera to a location and look somewhere
    if (key == GLFW_KEY_L && action == GLFW_PRESS)
    {
        // Move the eye
        ::g_pFlyCamera->setEye(glm::vec3(8631.0f, -1487.0f, 13010.0f));

        // "look at" something.
        // Since we are using the fly camera, we can't just set the eye.
        //::g_pFlyCamera->setAt(glm::vec3(0.0f, 0.0f, 0.0f));

        ::g_pFlyCamera->Yaw_LeftRight(-80.0f);
        ::g_pFlyCamera->Pitch_UpDown(-10.0f);


        UseMessageBox();
    }

    float cameraSpeed = 1.0f;
    float objectMovementSpeed = 1.0f;
    float lightMovementSpeed = 1.0f;

    bool bShiftDown = false;
    bool bControlDown = false;
    bool bAltDown = false;

    //    // Shift down?
    //    if ( mods == GLFW_MOD_SHIFT )       // 0x0001   0000 0001
    //    {
    //        // ONLY shift is down
    //    }
    //    // Control down?
    //    if ( mods == GLFW_MOD_CONTROL  )    // 0x0002   0000 0010
    //    // Alt down?
    //    if ( mods == GLFW_MOD_ALT   )       // 0x0004   0000 0100

        //   0000 0111 
        // & 0000 0001
        // -----------
        //   0000 0001 --> Same as the shift mask

        // Use bitwise mask to filter out just the shift
    if ((mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT)
    {
        // Shift is down and maybe other things, too
        bShiftDown = true;
    }
    if ((mods & GLFW_MOD_CONTROL) == GLFW_MOD_CONTROL)
    {
        // Shift is down and maybe other things, too
        bControlDown = true;
    }
    if ((mods & GLFW_MOD_ALT) == GLFW_MOD_ALT)
    {
        // Shift is down and maybe other things, too
        bAltDown = true;
    }



    return;
}
