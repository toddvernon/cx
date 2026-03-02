//-------------------------------------------------------------------------------------------------
//
//  sheetCell.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxSheetCell Class Implementation
//
//-------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sheetCell.h"
#include <cx/json/json_member.h>
#include <cx/json/json_string.h>
#include <cx/json/json_number.h>
#include <cx/json/json_boolean.h>


//-------------------------------------------------------------------------
// Static helper: clone a JSON value based on its type
//-------------------------------------------------------------------------
static CxJSONBase*
cloneJSONValue(CxJSONBase* value)
{
    if (value == NULL) return NULL;

    switch (value->type()) {
        case CxJSONBase::STRING:
            return new CxJSONString(((CxJSONString*)value)->get());
        case CxJSONBase::NUMBER:
            return new CxJSONNumber(((CxJSONNumber*)value)->get());
        case CxJSONBase::BOOLEAN:
            return new CxJSONBoolean(((CxJSONBoolean*)value)->get());
        default:
            // For other types (OBJECT, ARRAY), return NULL - not needed for appAttributes
            return NULL;
    }
}


//-------------------------------------------------------------------------
// Static helper: clone a CxJSONObject (shallow copy of members)
//-------------------------------------------------------------------------
static CxJSONObject*
cloneJSONObject(CxJSONObject* src)
{
    if (src == NULL) return NULL;

    CxJSONObject* copy = new CxJSONObject();
    for (int i = 0; i < src->entries(); i++) {
        CxJSONMember* member = src->at(i);
        if (member != NULL) {
            CxJSONBase* valueCopy = cloneJSONValue(member->object());
            if (valueCopy != NULL) {
                copy->append(new CxJSONMember(member->var(), valueCopy));
            }
        }
    }
    return copy;
}


//-------------------------------------------------------------------------
// Static helper: remove a member by key from a CxJSONObject
//-------------------------------------------------------------------------
static void
removeJSONMember(CxJSONObject* obj, const char* key)
{
    if (obj == NULL) return;

    // Find and remove the member with matching key
    for (int i = 0; i < obj->entries(); i++) {
        CxJSONMember* member = obj->at(i);
        if (member != NULL && member->var() == key) {
            CxJSONMember* removed = obj->removeAt(i);
            if (removed != NULL) {
                delete removed;
            }
            return;
        }
    }
}


//-------------------------------------------------------------------------
// CxSheetCell::CxSheetCell
//
// Default constructor - creates empty cell
//-------------------------------------------------------------------------
CxSheetCell::CxSheetCell(void)
: cellType(EMPTY)
, formula(NULL)
, appAttributes(NULL)
{
}


//-------------------------------------------------------------------------
// CxSheetCell::CxSheetCell
//
// Construct as text cell
//-------------------------------------------------------------------------
CxSheetCell::CxSheetCell(CxString textValue)
: cellType(TEXT)
, text(textValue)
, formula(NULL)
, appAttributes(NULL)
{
}


//-------------------------------------------------------------------------
// CxSheetCell::CxSheetCell
//
// Construct as double cell
//-------------------------------------------------------------------------
CxSheetCell::CxSheetCell(CxDouble numericValue)
: cellType(DOUBLE)
, formula(NULL)
, doubleValue(numericValue)
, evaluatedValue(numericValue)
, appAttributes(NULL)
{
}


//-------------------------------------------------------------------------
// CxSheetCell::CxSheetCell
//
// Copy constructor
//-------------------------------------------------------------------------
CxSheetCell::CxSheetCell(const CxSheetCell& other)
: cellType(other.cellType)
, text(other.text)
, formula(NULL)
, doubleValue(other.doubleValue)
, evaluatedValue(other.evaluatedValue)
, appAttributes(NULL)
{
    // Deep copy of formula if present
    if (other.formula != NULL) {
        formula = new CxExpression(other.text);
        formula->Parse();
    }

    // Deep copy of appAttributes if present
    if (other.appAttributes != NULL) {
        appAttributes = cloneJSONObject(other.appAttributes);
    }
}


//-------------------------------------------------------------------------
// CxSheetCell::~CxSheetCell
//
// Destructor
//-------------------------------------------------------------------------
CxSheetCell::~CxSheetCell(void)
{
    if (formula != NULL) {
        delete formula;
        formula = NULL;
    }
    if (appAttributes != NULL) {
        delete appAttributes;
        appAttributes = NULL;
    }
}


//-------------------------------------------------------------------------
// CxSheetCell::operator=
//
// Assignment operator
//-------------------------------------------------------------------------
CxSheetCell&
CxSheetCell::operator=(const CxSheetCell& other)
{
    if (this != &other) {
        // Clean up existing formula
        if (formula != NULL) {
            delete formula;
            formula = NULL;
        }

        // Clean up existing appAttributes
        if (appAttributes != NULL) {
            delete appAttributes;
            appAttributes = NULL;
        }

        cellType = other.cellType;
        text = other.text;
        doubleValue = other.doubleValue;
        evaluatedValue = other.evaluatedValue;

        // Deep copy of formula if present
        if (other.formula != NULL) {
            formula = new CxExpression(other.text);
            formula->Parse();
        }

        // Deep copy of appAttributes if present
        if (other.appAttributes != NULL) {
            appAttributes = cloneJSONObject(other.appAttributes);
        }
    }
    return *this;
}


//-------------------------------------------------------------------------
// CxSheetCell::clear
//
// Reset cell to EMPTY state
// Note: appAttributes are preserved - clearing a cell doesn't remove its formatting
//-------------------------------------------------------------------------
void
CxSheetCell::clear(void)
{
    cellType = EMPTY;
    text = CxString();

    if (formula != NULL) {
        delete formula;
        formula = NULL;
    }

    doubleValue = CxDouble();
    evaluatedValue = CxDouble();

    // Note: appAttributes are intentionally NOT cleared here.
    // Clearing a cell's content doesn't remove its formatting attributes.
    // Use removeAppAttribute() or delete appAttributes explicitly if needed.
}


//-------------------------------------------------------------------------
// CxSheetCell::setText
//
// Set cell as text type
//-------------------------------------------------------------------------
void
CxSheetCell::setText(CxString textValue)
{
    clear();
    cellType = TEXT;
    text = textValue;
}


//-------------------------------------------------------------------------
// CxSheetCell::setDouble
//
// Set cell as double type
//-------------------------------------------------------------------------
void
CxSheetCell::setDouble(CxDouble numericValue)
{
    clear();
    cellType = DOUBLE;
    doubleValue = numericValue;
    evaluatedValue = numericValue;
}


//-------------------------------------------------------------------------
// CxSheetCell::setFormula
//
// Set cell as formula type
// Parses the formula; variable database is set by CxSheetModel before evaluation
//-------------------------------------------------------------------------
void
CxSheetCell::setFormula(CxString formulaText)
{
    clear();
    cellType = FORMULA;
    text = formulaText;

    formula = new CxExpression(formulaText);
    formula->Parse();
}


//-------------------------------------------------------------------------
// CxSheetCell::getType
//
// Get the cell type
//-------------------------------------------------------------------------
CxSheetCell::TYPE
CxSheetCell::getType(void) const
{
    return cellType;
}


//-------------------------------------------------------------------------
// CxSheetCell::getText
//
// Get text value (valid if type is TEXT)
//-------------------------------------------------------------------------
CxString
CxSheetCell::getText(void) const
{
    return text;
}


//-------------------------------------------------------------------------
// CxSheetCell::getDouble
//
// Get double value (valid if type is DOUBLE)
//-------------------------------------------------------------------------
CxDouble
CxSheetCell::getDouble(void) const
{
    return doubleValue;
}


//-------------------------------------------------------------------------
// CxSheetCell::getFormulaText
//
// Get formula as text (valid if type is FORMULA)
//-------------------------------------------------------------------------
CxString
CxSheetCell::getFormulaText(void) const
{
    return text;
}


//-------------------------------------------------------------------------
// CxSheetCell::getEvaluatedValue
//
// Get the evaluated result (valid if type is DOUBLE or FORMULA)
//-------------------------------------------------------------------------
CxDouble
CxSheetCell::getEvaluatedValue(void) const
{
    return evaluatedValue;
}


//-------------------------------------------------------------------------
// App Attributes Helper Methods
//
// These methods provide convenient access to the appAttributes JSON object.
// The appAttributes object is created lazily when first needed.
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
// CxSheetCell::setAppAttribute (string value)
//-------------------------------------------------------------------------
void
CxSheetCell::setAppAttribute(const char* key, const char* value)
{
    if (appAttributes == NULL) {
        appAttributes = new CxJSONObject();
    }

    // Remove existing key if present, then add new value
    removeJSONMember(appAttributes, key);
    appAttributes->append(new CxJSONMember(key, new CxJSONString(value)));
}


//-------------------------------------------------------------------------
// CxSheetCell::setAppAttribute (int value)
//-------------------------------------------------------------------------
void
CxSheetCell::setAppAttribute(const char* key, int value)
{
    if (appAttributes == NULL) {
        appAttributes = new CxJSONObject();
    }

    removeJSONMember(appAttributes, key);
    appAttributes->append(new CxJSONMember(key, new CxJSONNumber((double)value)));
}


//-------------------------------------------------------------------------
// CxSheetCell::setAppAttribute (double value)
//-------------------------------------------------------------------------
void
CxSheetCell::setAppAttribute(const char* key, double value)
{
    if (appAttributes == NULL) {
        appAttributes = new CxJSONObject();
    }

    removeJSONMember(appAttributes, key);
    appAttributes->append(new CxJSONMember(key, new CxJSONNumber(value)));
}


//-------------------------------------------------------------------------
// CxSheetCell::setAppAttribute (bool value)
//-------------------------------------------------------------------------
void
CxSheetCell::setAppAttribute(const char* key, bool value)
{
    if (appAttributes == NULL) {
        appAttributes = new CxJSONObject();
    }

    removeJSONMember(appAttributes, key);
    appAttributes->append(new CxJSONMember(key, new CxJSONBoolean(value ? 1 : 0)));
}


//-------------------------------------------------------------------------
// CxSheetCell::getAppAttributeString
//-------------------------------------------------------------------------
CxString
CxSheetCell::getAppAttributeString(const char* key, const char* defaultValue) const
{
    if (appAttributes == NULL) {
        return CxString(defaultValue);
    }

    CxJSONMember* member = appAttributes->find(key);
    if (member != NULL && member->object()->type() == CxJSONBase::STRING) {
        return ((CxJSONString*)member->object())->get();
    }

    return CxString(defaultValue);
}


//-------------------------------------------------------------------------
// CxSheetCell::getAppAttributeInt
//-------------------------------------------------------------------------
int
CxSheetCell::getAppAttributeInt(const char* key, int defaultValue) const
{
    if (appAttributes == NULL) {
        return defaultValue;
    }

    CxJSONMember* member = appAttributes->find(key);
    if (member != NULL && member->object()->type() == CxJSONBase::NUMBER) {
        return (int)((CxJSONNumber*)member->object())->get();
    }

    return defaultValue;
}


//-------------------------------------------------------------------------
// CxSheetCell::getAppAttributeDouble
//-------------------------------------------------------------------------
double
CxSheetCell::getAppAttributeDouble(const char* key, double defaultValue) const
{
    if (appAttributes == NULL) {
        return defaultValue;
    }

    CxJSONMember* member = appAttributes->find(key);
    if (member != NULL && member->object()->type() == CxJSONBase::NUMBER) {
        return ((CxJSONNumber*)member->object())->get();
    }

    return defaultValue;
}


//-------------------------------------------------------------------------
// CxSheetCell::getAppAttributeBool
//-------------------------------------------------------------------------
bool
CxSheetCell::getAppAttributeBool(const char* key, bool defaultValue) const
{
    if (appAttributes == NULL) {
        return defaultValue;
    }

    CxJSONMember* member = appAttributes->find(key);
    if (member != NULL && member->object()->type() == CxJSONBase::BOOLEAN) {
        return ((CxJSONBoolean*)member->object())->get() != 0;
    }

    return defaultValue;
}


//-------------------------------------------------------------------------
// CxSheetCell::hasAppAttribute
//-------------------------------------------------------------------------
bool
CxSheetCell::hasAppAttribute(const char* key) const
{
    if (appAttributes == NULL) {
        return false;
    }

    return appAttributes->find(key) != NULL;
}


//-------------------------------------------------------------------------
// CxSheetCell::removeAppAttribute
//-------------------------------------------------------------------------
void
CxSheetCell::removeAppAttribute(const char* key)
{
    removeJSONMember(appAttributes, key);
}
