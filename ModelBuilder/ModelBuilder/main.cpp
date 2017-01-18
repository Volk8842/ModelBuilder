#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

enum ModelTypeId {
	tBoolean,
	tInteger,
	tFloat,
	tString,
	tCustomModel,
	tArrayInteger,
	tArrayString,
	tArrayCustomModel
};

struct Field {
	string name;
	ModelTypeId type;
};

struct Model {
	string name;
	vector<Field*> fields;
};

ModelTypeId getType(string str) {
	if (str == "boolean") return tBoolean;
	if (str == "integer") return tInteger;
	if (str == "number") return tFloat;
	if (str == "string") return tString;
	if (str.substr(0, 6) == "Array[") {
		if (str.substr(6, 3) == "int") return tArrayInteger;
		if (str.substr(6, 3) == "str") return tArrayString;
		return tArrayCustomModel;
	}
	return tCustomModel;
}

string getTypeName(ModelTypeId type) {
	switch (type) {
	case tBoolean:
		return "boolean";
	case tInteger:
		return "int";
	case tFloat:
		return "float";
	case tString:
		return "String";
	case tCustomModel:
		return "tCustomModel";
	case tArrayInteger:
		return "tArrayInteger";
	case tArrayString:
		return "tArrayString";
	case tArrayCustomModel:
		return "tArrayCustomModel";
	default:
		return "PARSE ERROR";
	}
}

void main() {
	vector<Model*> models;
	ifstream source;
	string textLine;
	source.open("SampleSource.txt");
	if (source.is_open()) {
		Model* newModel = nullptr; 
		while (getline(source, textLine)) {
			// new model
			if (textLine[0] >= 'A' && textLine[0] <= 'Z') {
				string name = textLine.substr(0, textLine.find_first_of(" "));
				newModel = new Model();
				newModel->name = name;
				models.push_back(newModel);
			}
			// new field in current model
			if (textLine[0] >= 'a' && textLine[0] <= 'z' && newModel != nullptr) {
				string nameText = textLine.substr(0, textLine.find_first_of(" "));
				int openPos = textLine.find_first_of("(") + 1;
				int closePos = textLine.find_first_of("),");
				string typeText = textLine.substr(openPos, closePos - openPos);
				Field* newField = new Field();
				newField->name = nameText;
				newField->type = getType(typeText);
				newModel->fields.push_back(newField);
			}
		}
		source.close();
	}

	for (const auto& model : models) {
		cout << model->name << endl;
		for (const auto& field : model->fields) {
			cout << " - " << field->name << " (" << getTypeName(field->type) << ")" << endl;
		}
	}

	char a;
	std::cin >> a;
}