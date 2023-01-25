
#include "Core/Object.h"

bool Object::IsActive()
{
	return isEnable;
}

void Object::SetActive(bool value)
{
	isEnable = value;
}