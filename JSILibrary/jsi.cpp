#include "jsi.h"

#include "duktape.h"
#include <string>
#include <vector>
#include <sstream>
#include <iostream>

void InterpretTextEx(const char* variables[], const char* values[], int nbVar, const char* functions[], int nbFunc, const char* textToInterpret, char* result, int resLength)
{
	strcpy_s(result, resLength, "");

	duk_context* ctx = duk_create_heap_default();
	if (!ctx) {
		printf("Failed to create a Duktape heap.\n");
		return;
	}

	for (int i = 0; i < nbVar; ++i)
	{
		std::string strVar = "var " + std::string(variables[i]) + " = " + values[i];

		duk_push_string(ctx, strVar.c_str());
		if (duk_peval(ctx) != 0) {
			printf("Error: %s\n", duk_safe_to_string(ctx, -1));
			goto finished;
		}
	}

	for (int i = 0; i < nbFunc; ++i)
	{
		duk_push_string(ctx, functions[i]);
		if (duk_peval(ctx) != 0) {
			printf("Error: %s\n", duk_safe_to_string(ctx, -1));
			goto finished;
		}
	}

	duk_eval_string(ctx, textToInterpret);

	strcpy_s(result, resLength, duk_safe_to_string(ctx, -1));

finished:
	duk_destroy_heap(ctx);
}


std::vector<std::string> split(const std::string& str, char delim = ' ')
{
	std::vector<std::string> output;
	std::stringstream ss(str);
	std::string token;
	while (std::getline(ss, token, delim)) {
		output.push_back(token);
	}

	return output;
}

void InterpretText(const char* variables, const char* values, const char* functions, const char* textToInterpret, char* result, int resLength)
{
	strcpy_s(result, resLength, "");

	duk_context* ctx = duk_create_heap_default();
	if (!ctx) {
		printf("Failed to create a Duktape heap.\n");
		return;
	}

	std::string strVars = variables;
	std::string strValues = values;
	std::string strFuncs = functions;
	std::vector<std::string> listVars = split(strVars, ';');
	std::vector<std::string> listValues = split(strValues, ';');

	if (listVars.size() != listValues.size())
	{
		std::cout << "Error: the number of variables is different from the number of values." << std::endl;
		goto finished;
	}

	for (size_t i = 0; i < listVars.size(); ++i)
	{
		std::string strVar = "var " + listVars[i] + " = " + listValues[i] + ";";

		duk_push_string(ctx, strVar.c_str());
		if (duk_peval(ctx) != 0) {
			printf("Error: %s\n", duk_safe_to_string(ctx, -1));
			goto finished;
		}
	}

	duk_push_string(ctx, strFuncs.c_str());
	if (duk_peval(ctx) != 0) {
		printf("Error: %s\n", duk_safe_to_string(ctx, -1));
		goto finished;
	}

	duk_eval_string(ctx, textToInterpret);

	strcpy_s(result, resLength, duk_safe_to_string(ctx, -1));

finished:
	duk_destroy_heap(ctx);
}

