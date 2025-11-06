#include "EnginePCH.h"

std::string neu::Component::GetClassName()
{
    return typeid(this).name();
}
