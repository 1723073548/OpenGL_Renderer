#include <iostream>

#include "RendererMrg/RendererMrg.h"

int main(int argc, char* argv[]) {
	try	{
		return RendererMrg::GetInstance().Go();
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	return 0;
}