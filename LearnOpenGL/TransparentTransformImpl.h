#pragma once

#include <map>

#include "InstancedTransformImpl.h"

class TransparentTransformImpl : public InstancedTransformImpl
{
public:
	TransparentTransformImpl();
	~TransparentTransformImpl();

	void refreshModel() override;
};

