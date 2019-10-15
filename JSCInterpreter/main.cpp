
#include <stdio.h>
#include <string.h>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>

#include "duktape.h"

const char* InterpretText(const char* variables[], const char* values[], int nbVar, const char* functions[], int nbFunc, const char* textToInterpret)
{
	duk_context* ctx = duk_create_heap_default();
	if (!ctx) {
		printf("Failed to create a Duktape heap.\n");
		return "";
	}

	for (int i = 0; i < nbVar; ++i)
	{
		char szBuf[1024];
		sprintf_s(szBuf, "%s", "var ");
		strcat_s(szBuf, variables[i]);
		strcat_s(szBuf, "=");
		strcat_s(szBuf, values[i]);
		strcat_s(szBuf, ";");

		duk_push_string(ctx, szBuf);
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

	return duk_safe_to_string(ctx, -1);

finished:
	duk_destroy_heap(ctx);
	return "";
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

const char* InterpretTextSimple(const char* variables, const char* values, const char* functions, const char* textToInterpret)
{
	duk_context* ctx = duk_create_heap_default();
	if (!ctx) {
		printf("Failed to create a Duktape heap.\n");
		return "";
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

	return duk_safe_to_string(ctx, -1);

finished:
	duk_destroy_heap(ctx);
	return "";
}

const char* get_InterpretTextSimple()
{
	const char* vars = "nom;age";
	const char* vals = "\"France-Ireland\";18";
	const char* funcs = "function Extract(nom, start, len) { return nom.substring(start, start + len); } function square(n) { return n*n; }";
	const char* textToInterpret = "Extract(nom, 0, 6)";

	return InterpretTextSimple(vars, vals, funcs, textToInterpret);
}

int main(int argc, char* argv[])
{
		
	const char* vars[] = { "nom", "age" };
	const char* vals[] = { "\"Romain\"", "18" };
	const char* funcs[] = { "function Extract(nom, start, len) { return nom.substring(start, start + len); }", "function square(n) { return n*n; }" };
	const char* textToInterpret = "Extract(nom, 0, 2)";
	printf("%s\n", InterpretText(vars, vals, 2, funcs, 2, textToInterpret));

	printf("%s\n", get_InterpretTextSimple());

	return 0;
}

