/*
form_logger.h - файл, содержащий реализацию класса логирования текущих действий
*/

#pragma once

#include "logger.h"
#include "declaration.h"
#include "clusters_form.h"

class FormLogger : public Logger
{
private:
	clusters_form main_form;
public:
	FormLogger(form & form) : main_form(clusters_form(form)) {};
	virtual void Info(std::wstring str)
	{
		main_form.status(str);
	}
	virtual void Error(std::wstring str)
	{
		main_form.status(str);
	}
};
