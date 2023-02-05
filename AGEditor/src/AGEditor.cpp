#include "Age/EntryPoint.h"
#include "Editor.h"

AGE::Application* AGE::CreateApplication() {
    return new AGE::Editor;
}
