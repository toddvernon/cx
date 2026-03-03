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
#include <cx/json/json_utf_member.h>
#include <cx/json/json_utf_string.h>
#include <cx/json/json_utf_number.h>
#include <cx/json/json_utf_boolean.h>


//-------------------------------------------------------------------------
// Static helper: clone a JSON value based on its type
//-------------------------------------------------------------------------
static CxJSONUTFBase*
cloneJSONUTFValue(CxJSONUTFBase* value)
{
    if (value == NULL) return NULL;

    switch (value->type()) {
        case CxJSONUTFBase::STRING:
            return new CxJSONUTFString(((CxJSONUTFString*)value)->get());
        case CxJSONUTFBase::NUMBER:
            return new CxJSONUTFNumber(((CxJSONUTFNumber*)value)->get());
        case CxJSONUTFBase::BOOLEAN:
            return new CxJSONUTFBoolean(((CxJSONUTFBoolean*)value)->get());
        default:
            // For other types (OBJECT, ARRAY), return NULL - not needed for appAttributes
            return NULL;
    }
}


//-------------------------------------------------------------------------
// Static helper: clone a CxJSONUTFObject (shallow copy of members)
//-------------------------------------------------------------------------
static CxJSONUTFObject*
cloneJSONUTFObject(CxJSONUTFObject* src)
{
    if (src == NULL) return NULL;

    CxJSONUTFObject* copy = new CxJSONUTFObject();
    for (int i = 0; i < src->entries(); i++) {
        CxJSONUTFMember* member = src->at(i);
        if (member != NULL) {
            CxJSONUTFBase* valueCopy = cloneJSONUTFValue(member->object());
            if (valueCopy != NULL) {
                copy->append(new CxJSONUTFMember(member->var(), valueCopy));
            }
        }
    }
    return copy;
}


//-------------------------------------------------------------------------
// Static helper: remove a member by key from a CxJSONUTFObject
//-------------------------------------------------------------------------
static void
removeJSONUTFMember(CxJSONUTFObject* obj, const char* key)
{
    if (obj == NULL) return;

    // Find and remove the member with matching key
    for (int i = 0; i < obj->entries(); i++) {
        CxJSONUTFMember* member = obj->at(i);
        if (member != NULL && member->var().toBytes() == key) {
            CxJSONUTFMember* removed = obj->removeAt(i);
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
        appAttributes = cloneJSONUTFObject(other.appAttributes);
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
            appAttributes = cloneJSONUTFObject(other.appAttributes);
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
        appAttributes = new CxJSONUTFObject();
    }

    // Remove existing key if present, then add new value
    removeJSONUTFMember(appAttributes, key);
    appAttributes->append(new CxJSONUTFMember(key, new CxJSONUTFString(value)));
}


//-------------------------------------------------------------------------
// CxSheetCell::setAppAttribute (int value)
//-------------------------------------------------------------------------
void
CxSheetCell::setAppAttribute(const char* key, int value)
{
    if (appAttributes == NULL) {
        appAttributes = new CxJSONUTFObject();
    }

    removeJSONUTFMember(appAttributes, key);
    appAttributes->append(new CxJSONUTFMember(key, new CxJSONUTFNumber((double)value)));
}


//-------------------------------------------------------------------------
// CxSheetCell::setAppAttribute (double value)
//-------------------------------------------------------------------------
void
CxSheetCell::setAppAttribute(const char* key, double value)
{
    if (appAttributes == NULL) {
        appAttributes = new CxJSONUTFObject();
    }

    removeJSONUTFMember(appAttributes, key);
    appAttributes->append(new CxJSONUTFMember(key, new CxJSONUTFNumber(value)));
}


//-------------------------------------------------------------------------
// CxSheetCell::setAppAttribute (bool value)
//-------------------------------------------------------------------------
void
CxSheetCell::setAppAttribute(const char* key, bool value)
{
    if (appAttributes == NULL) {
        appAttributes = new CxJSONUTFObject();
    }

    removeJSONUTFMember(appAttributes, key);
    appAttributes->append(new CxJSONUTFMember(key, new CxJSONUTFBoolean(value ? 1 : 0)));
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

    CxJSONUTFMember* member = appAttributes->find(key);
    if (member != NULL && member->object()->type() == CxJSONUTFBase::STRING) {
        return ((CxJSONUTFString*)member->object())->get().toBytes();
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

    CxJSONUTFMember* member = appAttributes->find(key);
    if (member != NULL && member->object()->type() == CxJSONUTFBase::NUMBER) {
        return (int)((CxJSONUTFNumber*)member->object())->get();
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

    CxJSONUTFMember* member = appAttributes->find(key);
    if (member != NULL && member->object()->type() == CxJSONUTFBase::NUMBER) {
        return ((CxJSONUTFNumber*)member->object())->get();
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

    CxJSONUTFMember* member = appAttributes->find(key);
    if (member != NULL && member->object()->type() == CxJSONUTFBase::BOOLEAN) {
        return ((CxJSONUTFBoolean*)member->object())->get() != 0;
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
    removeJSONUTFMember(appAttributes, key);
}
