#include <iostream>

#include "MainScene/MainScene.h"

int main(int argc, char* argv[]) {
	try	{
		return MainScene::GetInstance().Render();
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	return 0;
}