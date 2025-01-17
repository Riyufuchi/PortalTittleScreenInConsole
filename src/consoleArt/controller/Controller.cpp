﻿//============================================================================
// Name        : Controller.cpp
// Author      : Riyufuchi
// Created on  : Nov 15, 2022
// Last Edit   : Jan 18, 2025
// Description : This class is controller for a main app functionality
//============================================================================

#include "Controller.h"

namespace ConsoleArt
{
Controller::Controller() : Controller("") // Calls constructor with parameter to construct class
{
}
Controller::Controller(std::string path) : workspacePath(path), isRunnable(true), messenger(nullptr)
{
}

bool Controller::applyArgument(int argc, char** argv, int i)
{
	if(!strcmp(argv[i], "-p") || !strcmp(argv[i], "--path"))
	{
		if (argc <= i + 1)
			return true;
		std::string path = reinterpret_cast<const char*>((argv[i + 1])); // or std::string path{argv[2]};
		if((path.substr(path.length() - 1) != "/") && (path.length() > 0)) // if(argv[2][path.length() - 1] == '/')
			path.append("/");
		workspacePath = path;
		messenger->messageUser(Messenger::MessageType::INFO, "Workspace path: " + workspacePath + "\n");
		return false;
	}
	else if (!strcmp(argv[i], "--loadAll"))
	{
		loadAllImagesAsync();
		return false;
	}
	else if (!strcmp(argv[i], "--image"))
	{
		if (!((i + 1) < argc))
		{
			messenger->messageUser(Messenger::MessageType::ERROR, "Missing image parameter\n");
			return false;
		}
		addImage(loadImage(workspacePath + argv[i + 1]));
		if (images.size() > 0)
			convertImage(images.back().get());
		return false;
	}
	else if (!strcmp(argv[i], "--about"))
	{
		GeneralTools::aboutApplication();
		return false;
	}
	return true;
}

void Controller::loadAllImagesAsync()
{
	std::string fne = "";
	std::string itDir = workspacePath;
	if (itDir == "")
		itDir = std::filesystem::current_path().generic_string(); // .generic_string(); is required by Windows
	try
	{
		for (const auto& entry : std::filesystem::directory_iterator(itDir))
		{
			fne = entry.path().extension().generic_string();
			if (fne == ".pcx")
				addImage(new Images::ImagePCX(entry.path().generic_string()));
			else if (fne == ".bmp")
				addImage(new Images::ImageBMP(entry.path().generic_string()));
			else if (fne == ".ppm")
				addImage(new Images::ImagePPM(entry.path().generic_string()));
			else
				messenger->messageUser(Messenger::MessageType::WARNING, "Unsupported format \"" + fne + "\"\n");
		}
	}
	catch (std::runtime_error& e)
	{
		messenger->messageUser(Messenger::MessageType::EXCEPTION, e.what());
		return;
	}
	std::lock_guard<std::mutex> lock(mutexImages);
	std::sort(images.begin(), images.end(), [](const std::unique_ptr<Images::Image>& a, const std::unique_ptr<Images::Image>& b)
	{
		return a->getFilename() < b->getFilename();
	});
	messenger->messageUser(Messenger::MessageType::SUCCESFUL_TASK, "All loaded!\a\n");
	refreshMenu();
}

bool Controller::addImage(Images::Image* image)
{
	if (image == nullptr)
		return false;
	if (!image->isLoaded())
	{
		messenger->messageUser(Messenger::MessageType::ERROR, image->getFileStatus() + "\n");
		delete image;
		image = NULL;
		return false;
	}
	std::lock_guard<std::mutex> lock(mutexImages);
	if(!images.empty())
	{
		for (std::unique_ptr<Images::Image>& existingImage : images)
		{
			if (existingImage->getFilename() == image->getFilename())
			{
				delete image;
				image = NULL;
				return false;
			}
		}
	}
	images.emplace_back(std::unique_ptr<Images::Image>(image));
	return true;
}

Images::Image* Controller::loadImage(std::string path)
{
	std::string ext = "";
	try
	{
		ext = path.substr(path.find_last_of("."));
	}
	catch (std::exception& e)
	{
		messenger->messageUser(Messenger::MessageType::EXCEPTION, e.what());
		std::cout << "\n";
		return nullptr;
	}
	if (ext == ".pcx")
		return new Images::ImagePCX(path);
	else if (ext == ".bmp")
		return new Images::ImageBMP(path);
	else if (ext == ".ppm")
		return new Images::ImagePPM(path);
	else
		messenger->messageUser(Messenger::MessageType::WARNING, ext + " is not supported\n");
	return nullptr;
}

Controller::~Controller()
{
	delete messenger;
	//for(size_t i = 0; i < images.size(); i++)
	//	delete images[i];
	//std::cout << "Controller " << "destructed" << std::endl;
}
}
