#pragma once
#include "../WindowMrg/WindowMrg.h"
#include "../Scene/Scene.h"
#include "../Tools/SingletonTemplate.h"

class RendererMrg : public Singleton<RendererMrg> {
	friend class Singleton<RendererMrg>;

public:
	RendererMrg();

public:
	Scene m_mainScene;

public:
	int Go();
};