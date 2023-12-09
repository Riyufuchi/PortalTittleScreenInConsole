//============================================================================
// Name        : UnixControllerCLI.h
// Author      : Riyufuchi
// Created on  : 22.11.2023
// Last Edit   : 01.12.2023
// Description : This class is Unix CLI controller for the main app
//============================================================================

#ifndef _UnixControllerCLI_H_
#define _UnixControllerCLI_H_

#include "Controller.h"
#include "AsciiPrinter.h"
#include "../imageUtils/ImageToolsCLI.h"

namespace ConsoleArt
{
// TODO: Make this platform independent CLI controller by using ConsoleUtils::IConsole& console; in base class
class UnixControllerCLI : public Controller
{
private:
	ConsoleUtils::UnixConsole unixConsole;
	void convertImage(Images::Image* image) override;
	void confConsoleColor() override;
	void loadAllImages() override;
	std::string inputImageName() override;
	Images::Image* selectImage() override;
	Images::Image* loadImage(std::string path) override;
public:
	UnixControllerCLI();
	UnixControllerCLI(std::string path);
	void run() override;
	~UnixControllerCLI();
};
}
#endif
