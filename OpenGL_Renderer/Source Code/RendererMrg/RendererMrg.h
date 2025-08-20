#pragma once
#include "../WindowMrg/WindowMrg.h"
#include "../Tools/SingletonTemplate.h"

class RendererMrg : public Singleton<RendererMrg> {
	friend class Singleton<RendererMrg>;

public:
	RendererMrg();

public:
	int Go();
};