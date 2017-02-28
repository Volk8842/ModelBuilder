#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

enum class TypeId {
	tBoolean,
	tLong,
	tFloat,
	tString,
	tCustomModel,
	tArrayLong,
	tArrayString,
	tArrayCustomModel
};

struct ObjectType {
	TypeId id;
	string name;
};

struct Field {
	string name;
	ObjectType type;
};

struct Model {
	ObjectType type;
	vector<Field*> fields;
};

TypeId getType(string str) {
	if (str == "boolean") return TypeId::tBoolean;
	if (str == "integer") return TypeId::tLong;
	if (str == "number") return TypeId::tFloat;
	if (str == "string") return TypeId::tString;
	if (str.substr(0, 6) == "Array[") {
		if (str.substr(6, 3) == "int") return TypeId::tArrayLong;
		if (str.substr(6, 3) == "str") return TypeId::tArrayString;
		return TypeId::tArrayCustomModel;
	}
	return TypeId::tCustomModel;
}

string removeArrayText(string str) {
	auto start = str.find_first_of("[");
	auto end = str.length();
	if (start != string::npos) {
		start++;
		end = str.find_first_of("]");
	}
	else {
		start = 0;
	}
	return str.substr(start, end - start);
}

void setObjectType(ObjectType& type, string text) {
	type.id = getType(text);
	size_t toPos;
	switch (type.id) {
	case TypeId::tBoolean:
		type.name = "Boolean";
		break;
	case TypeId::tLong:
		type.name = "Long";
		break;
	case TypeId::tFloat:
		type.name = "Float";
		break;
	case TypeId::tString:
		type.name = "String";
		break;
	case TypeId::tArrayLong:
		type.name = "long[]";
		break;
	case TypeId::tArrayString:
		type.name = "String[]";
		break;
	case TypeId::tCustomModel:
		toPos = text.find("TO");
		if (toPos != string::npos) {
			text.erase(toPos, 2);
		}
		type.name = text + "Model";
		break;
	case TypeId::tArrayCustomModel:
		text = removeArrayText(text);
		toPos = text.find("TO");
		if (toPos != string::npos) {
			text.erase(toPos, 2);
		}
		type.name = "List<" + text + "Model>";
		break;
	}
}

string getJsonConstructorTextline(Field field) {
	switch (field.type.id) {
	case TypeId::tBoolean:
		return "this." + field.name + " = getBooleanField(in, \"" + field.name + "\");";
	case TypeId::tLong:
		return "this." + field.name + " = getLongField(in, \"" + field.name + "\");";
	case TypeId::tFloat:
		return "this." + field.name + " = getFloatField(in, \"" + field.name + "\");";
	case TypeId::tString:
		return "this." + field.name + " = getStringField(in, \"" + field.name + "\", \"\");";
	case TypeId::tCustomModel:
		return "this." + field.name + " = getModel(in, \"" + field.name + "\", " + field.type.name + ".class);";
	case TypeId::tArrayLong:
		return "this." + field.name + " = getLongArray(in, \"" + field.name + "\");";
	case TypeId::tArrayString:
		return "this." + field.name + " = getStringArray(in, \"" + field.name + "\");";
	case TypeId::tArrayCustomModel:
		auto startListType = field.type.name.find('<') + 1;
		auto sizeListType = field.type.name.find('>') - startListType;
		return "this." + field.name + " = getModelList(in, \"" + field.name + "\", " + field.type.name.substr(startListType, sizeListType) + ".class);";
	}
}

string getParcelConstructorTextline(Field field) {
	switch (field.type.id) {
	case TypeId::tBoolean:
		return "if (ParcelUtils.readBooleanFromParcel(in)) this." + field.name + " = in.readByte() != 0;";
	case TypeId::tLong:
		return "if (ParcelUtils.readBooleanFromParcel(in)) this." + field.name + " = in.readLong();";
	case TypeId::tFloat:
		return "if (ParcelUtils.readBooleanFromParcel(in)) this." + field.name + " = in.readFloat();";
	case TypeId::tString:
		return "if (ParcelUtils.readBooleanFromParcel(in)) this." + field.name + " = in.readString();";
	case TypeId::tCustomModel:
		return "if (ParcelUtils.readBooleanFromParcel(in)) this." + field.name + " = in.readParcelable(" + field.type.name + ".class.getClassLoader());";
	case TypeId::tArrayLong:
		return "if (ParcelUtils.readBooleanFromParcel(in)) this." + field.name + " = in.createLongArray();";
	case TypeId::tArrayString:
		return "if (ParcelUtils.readBooleanFromParcel(in)) this." + field.name + " = in.createStringArray();";
	case TypeId::tArrayCustomModel:
		auto startListType = field.type.name.find('<') + 1;
		auto sizeListType = field.type.name.find('>') - startListType;
		return "if (ParcelUtils.readBooleanFromParcel(in)) this." + field.name + " = in.createTypedArrayList(" + field.type.name.substr(startListType, sizeListType) + ".CREATOR);";
	}
}

string getWriteToParcelTextline(Field field) {
	switch (field.type.id) {
	case TypeId::tBoolean:
		return "if (ParcelUtils.writeBooleanToParcel(dest, " + field.name + ")) dest.writeByte(" + field.name + " ? (byte)1 : (byte)0);";
	case TypeId::tLong:
		return "if (ParcelUtils.writeBooleanToParcel(dest, " + field.name + ")) dest.writeLong(" + field.name + ");";
	case TypeId::tFloat:
		return "if (ParcelUtils.writeBooleanToParcel(dest, " + field.name + ")) dest.writeFloat(" + field.name + ");";
	case TypeId::tString:
		return "if (ParcelUtils.writeBooleanToParcel(dest, " + field.name + ")) dest.writeString(" + field.name + ");";
	case TypeId::tCustomModel:
		return "if (ParcelUtils.writeBooleanToParcel(dest, " + field.name + ")) dest.writeParcelable(" + field.name + ", 0);";
	case TypeId::tArrayLong:
		return "if (ParcelUtils.writeBooleanToParcel(dest, " + field.name + ")) dest.writeLongArray(" + field.name + ");";
	case TypeId::tArrayString:
		return "if (ParcelUtils.writeBooleanToParcel(dest, " + field.name + ")) dest.writeStringArray(" + field.name + ");";
	case TypeId::tArrayCustomModel:
		return "if (ParcelUtils.writeBooleanToParcel(dest, " + field.name + ")) dest.writeTypedList(" + field.name + ");";
	}
}

string getGetterTextline(Field field) {
	return "    public " + field.type.name + (field.type.id != TypeId::tBoolean ? " get" : " is") + static_cast<char>(toupper(field.name[0])) + field.name.substr(1, field.name.length() - 1) + "() {\n"
		 + "        return " + field.name + ";\n"
		 + "    }\n";
}

void createModelFile(Model& model) {
	ofstream modelFile;
	string filename = "output\\" + model.type.name + ".java";
	modelFile.open(filename, ios_base::trunc);
	modelFile << "package ru.pochtabank.fakturadbo.api.model.faktura;" << endl;
	modelFile << endl;
	modelFile << "import android.os.Parcel;" << endl;
	modelFile << "import com.google.gson.JsonObject;" << endl;
	modelFile << "import java.util.List;" << endl;
	modelFile << "import ru.pochtabank.dbo.base.model.BaseModel;" << endl;
	modelFile << "import ru.pochtabank.utils.data.ParcelUtils;" << endl;
	modelFile << endl;
	modelFile << "/**" << endl;
	modelFile << "* " << "Autogenerated on " << __DATE__ << endl;
	modelFile << "* " << "Don't place any additional methods here, instead use custom container for this model" << endl;
	modelFile << "*/" << endl;
	modelFile << endl;
	modelFile << "public class " << model.type.name << " extends BaseModel {" << endl;
	modelFile << endl;
	modelFile << "    public static final Creator<" << model.type.name << "> CREATOR = new Creator<" << model.type.name << ">() {" << endl;
	modelFile << "        @Override" << endl;
	modelFile << "        public " << model.type.name << " createFromParcel(Parcel source) {" << endl;
	modelFile << "            return new " << model.type.name << "(source);" << endl;
	modelFile << "        }" << endl;
	modelFile << endl;
	modelFile << "        @Override" << endl;
	modelFile << "        public " << model.type.name << "[] newArray(int size) {" << endl;
	modelFile << "            return new " << model.type.name << "[size];" << endl;
	modelFile << "        }" << endl;
	modelFile << "    };" << endl;
	modelFile << endl;
	for (const auto& field : model.fields) {
		modelFile << "    private " << field->type.name << " " << field->name << ";" << endl;
	}
	modelFile << endl;
	modelFile << "    public " + model.type.name + "(JsonObject in) {" << endl;
	for (const auto& field : model.fields) {
		modelFile << "        " << getJsonConstructorTextline(*field) << endl;
	}
	modelFile << "    }" << endl;
	modelFile << endl;
	modelFile << "    public " + model.type.name + "(Parcel in) {" << endl;
	for (const auto& field : model.fields) {
		modelFile << "        " << getParcelConstructorTextline(*field) << endl;
	}
	modelFile << "    }" << endl;
	modelFile << endl;
	modelFile << "    @Override" << endl;
	modelFile << "    public void writeToParcel(Parcel dest, int flags) {" << endl;
	for (const auto& field : model.fields) {
		modelFile << "        " << getWriteToParcelTextline(*field) << endl;
	}
	modelFile << "    }" << endl;
	modelFile << endl;
	modelFile << "    @Override" << endl;
	modelFile << "    public int describeContents() {" << endl;
	modelFile << "        return 0;" << endl;
	modelFile << "    }" << endl;
	for (const auto& field : model.fields) {
		modelFile << endl << getGetterTextline(*field);
	}
	modelFile << "}" << endl;
	modelFile.close();
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
				setObjectType(newModel->type, name);
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
				setObjectType(newField->type, typeText);
				newModel->fields.push_back(newField);
			}
		}
		source.close();
	}

	for (const auto& model : models) {
		createModelFile(*model);
	}
}