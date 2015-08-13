/*
logger.h - файл, содержащий интерфейс класса логирования текущих действий
*/

#pragma once

#include "declaration.h"

class Logger
{
public:
	virtual void Info(std::wstring str) = 0;
	virtual void Error(std::wstring str) = 0;
};
