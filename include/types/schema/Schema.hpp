//===================================================================================================================================
// @file	Schema.hpp
// @author	Jac Jenkins
// @date	25/09/2025
// 
// @brief	Header file containing definitions for schema 
//===================================================================================================================================

#pragma once

#include <variant>
#include <string>
#include <vector>

/* Provides the default params for a "filed" with accessor methods */
class Field {

private:
	std::variant<std::monostate, int, bool, std::string, std::vector<Field>> value;

public:
	Field() : value(0) {}
	
	template<typename T>
	Field(T fieldValue) : value(fieldValue) {}

	template<typename T>
	T getValue() { return std::get<T>(value); }
};

class IntField : public Field {
	/* Encapsulates the field data and instantiates it as an "int" type */
};

class StringField : public Field {
	/* Encapsulates the field data and instantiates it as an "string" type */
};

class BoolField : public Field {
	/* Encapsulates the field data and instantiates it as an "bool" type */
};

class ListField : public Field {
	/* Creates list member here or uses the variant */
};

/// HIGHER ORDER SCHEMA CLASSES TO BE BUILT OF THESE BUILDING BLOCKS

// Alias for field type
//using FieldType = std::variant<int, bool, std::string, std::vector<Field>>;