//-------------------------------------------------------------------------------------------------
//
// CxUTFEditBuffer
//
// CxUTFEditBuffer Class - UTF-8 aware edit buffer
//
//-------------------------------------------------------------------------------------------------
//
// (c) Copyright 2025 T.Vernon
// ALL RIGHTS RESERVED
//
// The software and information contained herein are proprietary to, and
// comprise valuable trade secrets of, T.Vernon , which
// intends to preserve as trade secrets such software and information.
// This software is furnished pursuant to a written license agreement and
// may be used, copied, transmitted, and stored only in accordance with
// the terms of such license and with the inclusion of the above copyright
// notice.  This software and information or any other copies thereof may
// not be provided or otherwise made available to any other person.
//
//
//-------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include <cx/editbuffer/utfeditbuffer.h>

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer:: (constructor)
//
//-------------------------------------------------------------------------------------------------
CxUTFEditBuffer::CxUTFEditBuffer(void)
    : name("")
    , filePath("")
    , cursor(CxEditBufferPosition())
    , lastRequestCol(0)
    , mark(CxEditBufferPosition())
    , markSet(false)
    , _bufferLineList(CxUTFStringList())
    , tabSpaces(4)
    , visualFirstScreenLine(0)
    , visualFirstScreenCol(0)
    , readOnly(false)
    , touched(false)
    , inMemory(false)
    , _killAccumulator("")
    , _lastWasKill(false)
{
    reset();
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer:: (constructor)
//
//-------------------------------------------------------------------------------------------------
CxUTFEditBuffer::CxUTFEditBuffer(int tabspaces)
    : name("")
    , filePath("")
    , cursor(CxEditBufferPosition())
    , lastRequestCol(0)
    , mark(CxEditBufferPosition())
    , markSet(false)
    , _bufferLineList(CxUTFStringList())
    , tabSpaces(4)
    , visualFirstScreenLine(0)
    , visualFirstScreenCol(0)
    , readOnly(false)
    , touched(false)
    , inMemory(false)
    , _killAccumulator("")
    , _lastWasKill(false)
{
    tabSpaces = tabspaces;
    reset();
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer:: (copy constructor)
//
//-------------------------------------------------------------------------------------------------
CxUTFEditBuffer::CxUTFEditBuffer(const CxUTFEditBuffer &c)
{
    if (&c != this) {
        name = c.name;
        filePath = c.filePath;
        cursor = c.cursor;
        markSet = c.markSet;
        mark = c.mark;
        readOnly = c.readOnly;
        touched = c.touched;
        inMemory = c.inMemory;
        visualFirstScreenLine = c.visualFirstScreenLine;
        visualFirstScreenCol = c.visualFirstScreenCol;
        lastRequestCol = c.lastRequestCol;
        tabSpaces = c.tabSpaces;
        _bufferLineList = c._bufferLineList;
        _killAccumulator = c._killAccumulator;
        _lastWasKill = c._lastWasKill;
    }
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer:: (assignment operator)
//
//-------------------------------------------------------------------------------------------------
CxUTFEditBuffer
CxUTFEditBuffer::operator=(const CxUTFEditBuffer &c)
{
    if (&c != this) {
        name = c.name;
        filePath = c.filePath;
        cursor = c.cursor;
        markSet = c.markSet;
        mark = c.mark;
        readOnly = c.readOnly;
        touched = c.touched;
        inMemory = c.inMemory;
        visualFirstScreenLine = c.visualFirstScreenLine;
        visualFirstScreenCol = c.visualFirstScreenCol;
        lastRequestCol = c.lastRequestCol;
        tabSpaces = c.tabSpaces;
        _bufferLineList = c._bufferLineList;
        _killAccumulator = c._killAccumulator;
        _lastWasKill = c._lastWasKill;
    }
    return *this;
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer::reset
//
//-------------------------------------------------------------------------------------------------
void
CxUTFEditBuffer::reset(void)
{
    name = "";
    filePath = "";
    cursor = CxEditBufferPosition();
    lastRequestCol = 0;
    mark = CxEditBufferPosition();
    markSet = false;
    visualFirstScreenLine = 0;
    visualFirstScreenCol = 0;
    readOnly = false;
    touched = false;
    inMemory = false;
    _bufferLineList.clear();
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer::positionString
//
//-------------------------------------------------------------------------------------------------
CxString
CxUTFEditBuffer::positionString(CxUTFEditBuffer::POSITION position)
{
    switch (position) {
        case POS_VALID_INITIAL:    return CxString("POS_VALID_INITIAL");
        case POS_VALID_INSERT:     return CxString("POS_VALID_INSERT");
        case POS_VALID_APPEND_COL: return CxString("POS_VALID_APPEND_COL");
        case POS_INVALID_ROW:      return CxString("POS_INVALID_ROW");
        case POS_INVALID_COL:      return CxString("POS_INVALID_COL");
        case POS_INVALID_MID_TAB:  return CxString("POS_INVALID_MID_TAB");
    }
    return CxString("INVALID_RETURN_CODE");
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer::evaluatePosition
//
// Evaluate a row and col in the buffer.
// NOTE: In UTF version, col is a CHARACTER index, not byte offset.
//
//-------------------------------------------------------------------------------------------------
CxUTFEditBuffer::POSITION
CxUTFEditBuffer::evaluatePosition(unsigned long row_, unsigned long col_)
{
    unsigned long numberOfBufferLines = _bufferLineList.entries();

    // Empty buffer at 0,0
    if ((numberOfBufferLines == 0) && (row_ == 0) && (col_ == 0)) {
        return CxUTFEditBuffer::POS_VALID_INITIAL;
    }

    // Row beyond buffer
    if (row_ >= numberOfBufferLines) {
        return CxUTFEditBuffer::POS_INVALID_ROW;
    }

    CxUTFString *line = _bufferLineList.at(row_);
    unsigned long lineLength = line->charCount();

    // Column beyond line + append position
    if (col_ > lineLength) {
        return CxUTFEditBuffer::POS_INVALID_COL;
    }

    // Append position (one past last char)
    if (col_ == lineLength) {
        return CxUTFEditBuffer::POS_VALID_APPEND_COL;
    }

    // Valid insert position
    return CxUTFEditBuffer::POS_VALID_INSERT;
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer::cursorRightRequest
//
//-------------------------------------------------------------------------------------------------
CxEditHint
CxUTFEditBuffer::cursorRightRequest(void)
{
    // Reset kill accumulation - any non-kill action breaks the sequence
    _lastWasKill = false;

    unsigned long numberOfBufferLines = _bufferLineList.entries();

    if (numberOfBufferLines == 0) {
        return CxEditHint(CxEditHint::UPDATE_HINT_NONE, CxEditHint::CURSOR_HINT_NONE);
    }

    CxUTFString *line = _bufferLineList.at(cursor.row);
    unsigned long lineLength = line->charCount();

    // Can we move right on this line?
    if (cursor.col < lineLength) {
        cursor.col++;
        lastRequestCol = cursor.col;
        return CxEditHint(cursor.row, cursor.col, CxEditHint::UPDATE_HINT_NONE, CxEditHint::CURSOR_HINT_RIGHT);
    }

    // At end of line - try to wrap to next line
    if (cursor.row + 1 < numberOfBufferLines) {
        cursor.row++;
        cursor.col = 0;
        lastRequestCol = cursor.col;
        return CxEditHint(cursor.row, cursor.col, CxEditHint::UPDATE_HINT_NONE, CxEditHint::CURSOR_HINT_WRAP_DOWN);
    }

    // Can't move
    return CxEditHint(cursor.row, cursor.col, CxEditHint::UPDATE_HINT_NONE, CxEditHint::CURSOR_HINT_NONE);
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer::cursorLeftRequest
//
//-------------------------------------------------------------------------------------------------
CxEditHint
CxUTFEditBuffer::cursorLeftRequest(void)
{
    // Reset kill accumulation - any non-kill action breaks the sequence
    _lastWasKill = false;

    // Can we move left on this line?
    if (cursor.col > 0) {
        cursor.col--;
        lastRequestCol = cursor.col;
        return CxEditHint(cursor.row, cursor.col, CxEditHint::UPDATE_HINT_NONE, CxEditHint::CURSOR_HINT_LEFT);
    }

    // At start of line - try to wrap to previous line
    if (cursor.row > 0) {
        cursor.row--;
        CxUTFString *line = _bufferLineList.at(cursor.row);
        cursor.col = line->charCount();
        lastRequestCol = cursor.col;
        return CxEditHint(cursor.row, cursor.col, CxEditHint::UPDATE_HINT_NONE, CxEditHint::CURSOR_HINT_WRAP_UP);
    }

    // Can't move
    return CxEditHint(cursor.row, cursor.col, CxEditHint::UPDATE_HINT_NONE, CxEditHint::CURSOR_HINT_NONE);
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer::cursorUpRequest
//
//-------------------------------------------------------------------------------------------------
CxEditHint
CxUTFEditBuffer::cursorUpRequest(void)
{
    // Reset kill accumulation - any non-kill action breaks the sequence
    _lastWasKill = false;

    if (cursor.row == 0) {
        return CxEditHint(cursor.row, cursor.col, CxEditHint::UPDATE_HINT_NONE, CxEditHint::CURSOR_HINT_NONE);
    }

    cursor.row--;
    cursor.col = lastRequestCol;

    CxUTFString *line = _bufferLineList.at(cursor.row);
    unsigned long lineLength = line->charCount();

    if (cursor.col > lineLength) {
        cursor.col = lineLength;
    }

    return CxEditHint(cursor.row, cursor.col, CxEditHint::UPDATE_HINT_NONE, CxEditHint::CURSOR_HINT_UP);
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer::cursorDownRequest
//
//-------------------------------------------------------------------------------------------------
CxEditHint
CxUTFEditBuffer::cursorDownRequest(void)
{
    // Reset kill accumulation - any non-kill action breaks the sequence
    _lastWasKill = false;

    unsigned long numberOfBufferLines = _bufferLineList.entries();

    if (numberOfBufferLines == 0 || cursor.row >= numberOfBufferLines - 1) {
        return CxEditHint(cursor.row, cursor.col, CxEditHint::UPDATE_HINT_NONE, CxEditHint::CURSOR_HINT_NONE);
    }

    cursor.row++;
    cursor.col = lastRequestCol;

    CxUTFString *line = _bufferLineList.at(cursor.row);
    unsigned long lineLength = line->charCount();

    if (cursor.col > lineLength) {
        cursor.col = lineLength;
    }

    return CxEditHint(cursor.row, cursor.col, CxEditHint::UPDATE_HINT_NONE, CxEditHint::CURSOR_HINT_DOWN);
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer::cursorGotoRequest
//
//-------------------------------------------------------------------------------------------------
CxUTFEditBuffer::POSITION
CxUTFEditBuffer::cursorGotoRequest(unsigned long row_, unsigned long col_)
{
    // Reset kill accumulation - any non-kill action breaks the sequence
    _lastWasKill = false;

    POSITION pos = evaluatePosition(row_, col_);

    if (pos == POS_VALID_INSERT || pos == POS_VALID_APPEND_COL || pos == POS_VALID_INITIAL) {
        cursor.row = row_;
        cursor.col = col_;
        lastRequestCol = col_;
    }

    return pos;
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer::cursorGotoLine
//
//-------------------------------------------------------------------------------------------------
CxUTFEditBuffer::POSITION
CxUTFEditBuffer::cursorGotoLine(unsigned long row_)
{
    unsigned long numberOfBufferLines = _bufferLineList.entries();

    if (numberOfBufferLines == 0) {
        return cursorGotoRequest(0, 0);
    }

    if (row_ >= numberOfBufferLines) {
        row_ = numberOfBufferLines - 1;
    }

    return cursorGotoRequest(row_, 0);
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer::loadText
//
//-------------------------------------------------------------------------------------------------
int
CxUTFEditBuffer::loadText(CxString filepath_, int preload)
{
    CxFile inFile;

    filePath = filepath_;

    if (!inFile.open(filePath, "r")) {
        return FALSE;
    }

    if (preload) {
        struct stat fileStat = inFile.getStat();
        unsigned long fileSize = (unsigned long)fileStat.st_size;

        char *rawBuffer = new char[fileSize + 1];
        size_t bytesRead = inFile.fread(rawBuffer, 1, fileSize);
        rawBuffer[bytesRead] = '\0';

        // Parse buffer into lines
        char *lineStart = rawBuffer;
        char *p = rawBuffer;

        while (*p != '\0') {
            if (*p == '\n') {
                *p = '\0';

                // Remove trailing CR if present (Windows line endings)
                if (p > lineStart && *(p - 1) == '\r') {
                    *(p - 1) = '\0';
                }

                CxUTFString utfLine;
                utfLine.fromBytes(lineStart, p - lineStart, tabSpaces);
                _bufferLineList.append(utfLine);

                lineStart = p + 1;
            }
            p++;
        }

        // Handle last line without newline
        if (p > lineStart) {
            CxUTFString utfLine;
            utfLine.fromBytes(lineStart, p - lineStart, tabSpaces);
            _bufferLineList.append(utfLine);
        }

        delete[] rawBuffer;
        inMemory = TRUE;
    } else {
        inMemory = FALSE;
    }

    inFile.close();
    cursorGotoRequest(0, 0);

    return TRUE;
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer::loadTextFromString
//
//-------------------------------------------------------------------------------------------------
void
CxUTFEditBuffer::loadTextFromString(CxString text)
{
    char *ptr = text.data();
    char *lineStart = ptr;

    while (*ptr != '\0') {
        if (*ptr == '\n') {
            *ptr = '\0';

            CxUTFString utfLine;
            utfLine.fromBytes(lineStart, ptr - lineStart, tabSpaces);
            _bufferLineList.append(utfLine);

            lineStart = ptr + 1;
        }
        ptr++;
    }

    // Handle last line without newline
    if (ptr > lineStart) {
        CxUTFString utfLine;
        utfLine.fromBytes(lineStart, ptr - lineStart, tabSpaces);
        _bufferLineList.append(utfLine);
    }
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer::saveText
//
//-------------------------------------------------------------------------------------------------
void
CxUTFEditBuffer::saveText(CxString filepath_)
{
    CxFile outFile;

    if (!outFile.open(filepath_, "w")) {
        printf("error opening file\n");
        return;
    }

    for (unsigned long row = 0; row < _bufferLineList.entries(); row++) {
        CxUTFString *line = _bufferLineList.at(row);
        CxString bytes = line->toBytes();
        outFile.printf("%s\n", bytes.data());
    }

    outFile.close();
    touched = FALSE;
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer::numberOfLines
//
//-------------------------------------------------------------------------------------------------
unsigned long
CxUTFEditBuffer::numberOfLines(void)
{
    return _bufferLineList.entries();
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer::characterCount
//
//-------------------------------------------------------------------------------------------------
unsigned long
CxUTFEditBuffer::characterCount(void)
{
    unsigned long count = 0;
    for (unsigned long i = 0; i < _bufferLineList.entries(); i++) {
        CxUTFString *line = _bufferLineList.at(i);
        count += line->charCount();
    }
    return count;
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer::line
//
//-------------------------------------------------------------------------------------------------
CxUTFString*
CxUTFEditBuffer::line(unsigned long row)
{
    return _bufferLineList.at(row);
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer::characterAt
//
// Returns ASCII character at position, or 0 if invalid/non-ASCII.
//
//-------------------------------------------------------------------------------------------------
char
CxUTFEditBuffer::characterAt(unsigned long row, unsigned long col)
{
    CxUTFString *line = _bufferLineList.at(row);
    if (line == 0) return 0;

    const CxUTFCharacter *ch = line->at(col);
    if (ch == 0) return 0;

    if (ch->isASCII()) {
        return ch->bytes()[0];
    }

    return 0;
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer::addCharacter (CxString version)
//
// Handle a UTF-8 string, inserting the first character (which may be multi-byte).
// For ASCII characters, delegates to the char version for special handling.
// For multi-byte UTF-8 characters, inserts directly as a single character.
//-------------------------------------------------------------------------------------------------
CxEditHint
CxUTFEditBuffer::addCharacter(CxString c)
{
    if (c.length() == 0) {
        return CxEditHint(CxEditHint::UPDATE_HINT_NONE, CxEditHint::CURSOR_HINT_NONE);
    }

    // Parse the first UTF-8 character
    CxUTFCharacter ch;
    ch.fromUTF8(c.data());

    // For ASCII characters, use the char version which handles newlines, tabs, etc.
    if (ch.isASCII()) {
        return addCharacter((char) ch.bytes()[0]);
    }

    // Handle multi-byte UTF-8 characters directly
    if (readOnly) {
        return CxEditHint(CxEditHint::UPDATE_HINT_NONE, CxEditHint::CURSOR_HINT_NONE);
    }

    // Reset kill accumulation - any non-kill action breaks the sequence
    _lastWasKill = false;
    touched = TRUE;

    // Empty buffer - create first line
    if (_bufferLineList.entries() == 0) {
        CxUTFString newLine;
        newLine.append(ch);
        _bufferLineList.append(newLine);
        cursor.col = 1;
        lastRequestCol = cursor.col;
        return CxEditHint(cursor.row, cursor.col, CxEditHint::UPDATE_HINT_LINE, CxEditHint::CURSOR_HINT_RIGHT);
    }

    CxUTFString *line = _bufferLineList.at(cursor.row);

    // Insert character
    line->insert(cursor.col, ch);

    // Recalculate tabs if any exist after insert point
    line->recalculateTabWidths(tabSpaces);

    cursor.col++;
    lastRequestCol = cursor.col;

    return CxEditHint(cursor.row, cursor.col, CxEditHint::UPDATE_HINT_LINE_PAST_POINT, CxEditHint::CURSOR_HINT_RIGHT);
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer::addCharacter (char version)
//
//-------------------------------------------------------------------------------------------------
CxEditHint
CxUTFEditBuffer::addCharacter(char c)
{
    if (readOnly) {
        return CxEditHint(CxEditHint::UPDATE_HINT_NONE, CxEditHint::CURSOR_HINT_NONE);
    }

    // Reset kill accumulation - any non-kill action breaks the sequence
    _lastWasKill = false;

    // Handle special characters
    if (c == '\n' || c == '\r') {
        return addReturn();
    }

    if (c == '\t') {
        return addTab();
    }

    touched = TRUE;

    // Empty buffer - create first line
    if (_bufferLineList.entries() == 0) {
        CxUTFString newLine;
        newLine.append(CxUTFCharacter::fromASCII(c));
        _bufferLineList.append(newLine);
        cursor.col = 1;
        lastRequestCol = cursor.col;
        return CxEditHint(cursor.row, cursor.col, CxEditHint::UPDATE_HINT_LINE, CxEditHint::CURSOR_HINT_RIGHT);
    }

    CxUTFString *line = _bufferLineList.at(cursor.row);

    // Insert character
    CxUTFCharacter ch = CxUTFCharacter::fromASCII(c);
    line->insert(cursor.col, ch);

    // Recalculate tabs if any exist after insert point
    line->recalculateTabWidths(tabSpaces);

    cursor.col++;
    lastRequestCol = cursor.col;

    return CxEditHint(cursor.row, cursor.col, CxEditHint::UPDATE_HINT_LINE_PAST_POINT, CxEditHint::CURSOR_HINT_RIGHT);
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer::addTab
//
//-------------------------------------------------------------------------------------------------
CxEditHint
CxUTFEditBuffer::addTab(void)
{
    if (readOnly) {
        return CxEditHint(CxEditHint::UPDATE_HINT_NONE, CxEditHint::CURSOR_HINT_NONE);
    }

    touched = TRUE;

    // Empty buffer - create first line with tab
    if (_bufferLineList.entries() == 0) {
        CxUTFString newLine;
        newLine.append(CxUTFCharacter::makeTab(tabSpaces));
        _bufferLineList.append(newLine);
        cursor.col = 1;
        lastRequestCol = cursor.col;
        return CxEditHint(cursor.row, cursor.col, CxEditHint::UPDATE_HINT_LINE, CxEditHint::CURSOR_HINT_RIGHT);
    }

    CxUTFString *line = _bufferLineList.at(cursor.row);

    // Calculate tab width based on cursor display column
    int displayCol = line->displayColumnOfChar(cursor.col);
    int tabWidth = tabSpaces - (displayCol % tabSpaces);
    if (tabWidth == 0) tabWidth = tabSpaces;

    CxUTFCharacter tab = CxUTFCharacter::makeTab(tabWidth);
    line->insert(cursor.col, tab);

    // Recalculate all tabs
    line->recalculateTabWidths(tabSpaces);

    cursor.col++;
    lastRequestCol = cursor.col;

    return CxEditHint(cursor.row, cursor.col, CxEditHint::UPDATE_HINT_LINE_PAST_POINT, CxEditHint::CURSOR_HINT_RIGHT);
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer::addReturn
//
//-------------------------------------------------------------------------------------------------
CxEditHint
CxUTFEditBuffer::addReturn(void)
{
    if (readOnly) {
        return CxEditHint(CxEditHint::UPDATE_HINT_NONE, CxEditHint::CURSOR_HINT_NONE);
    }

    // Reset kill accumulation - any non-kill action breaks the sequence
    _lastWasKill = false;

    touched = TRUE;

    // Empty buffer - create first line
    if (_bufferLineList.entries() == 0) {
        CxUTFString newLine;
        _bufferLineList.append(newLine);
        _bufferLineList.append(newLine);
        cursor.row = 1;
        cursor.col = 0;
        lastRequestCol = 0;
        return CxEditHint(cursor.row, cursor.col, CxEditHint::UPDATE_HINT_SCREEN, CxEditHint::CURSOR_HINT_JUMP);
    }

    CxUTFString *currentLine = _bufferLineList.at(cursor.row);

    // Split the current line
    CxUTFString newLine = currentLine->subString(cursor.col, currentLine->charCount() - cursor.col);
    currentLine->remove(cursor.col, currentLine->charCount() - cursor.col);

    // Recalculate tabs for both lines
    currentLine->recalculateTabWidths(tabSpaces);
    newLine.recalculateTabWidths(tabSpaces);

    // Insert new line after current
    _bufferLineList.insertAfter(cursor.row, newLine);

    // Create hint BEFORE incrementing cursor - hint needs original row
    // so formatMultipleEditorLines redraws the modified original line too
    CxEditHint editHint(cursor.row, cursor.col, CxEditHint::UPDATE_HINT_SCREEN_PAST_POINT, CxEditHint::CURSOR_HINT_JUMP);

    cursor.row++;
    cursor.col = 0;
    lastRequestCol = 0;

    return editHint;
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer::addBackspace
//
//-------------------------------------------------------------------------------------------------
CxEditHint
CxUTFEditBuffer::addBackspace(void)
{
    if (readOnly) {
        return CxEditHint(CxEditHint::UPDATE_HINT_NONE, CxEditHint::CURSOR_HINT_NONE);
    }

    // Reset kill accumulation - any non-kill action breaks the sequence
    _lastWasKill = false;

    // At start of buffer - nothing to delete
    if (cursor.row == 0 && cursor.col == 0) {
        return CxEditHint(CxEditHint::UPDATE_HINT_NONE, CxEditHint::CURSOR_HINT_NONE);
    }

    touched = TRUE;

    // At start of line - join with previous line
    if (cursor.col == 0) {
        return joinLines();
    }

    // Delete character before cursor
    CxUTFString *line = _bufferLineList.at(cursor.row);
    line->remove(cursor.col - 1, 1);
    line->recalculateTabWidths(tabSpaces);

    cursor.col--;
    lastRequestCol = cursor.col;

    return CxEditHint(cursor.row, cursor.col, CxEditHint::UPDATE_HINT_LINE, CxEditHint::CURSOR_HINT_LEFT);
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer::joinLines
//
//-------------------------------------------------------------------------------------------------
CxEditHint
CxUTFEditBuffer::joinLines(void)
{
    if (cursor.row == 0) {
        return CxEditHint(CxEditHint::UPDATE_HINT_NONE, CxEditHint::CURSOR_HINT_NONE);
    }

    CxUTFString *prevLine = _bufferLineList.at(cursor.row - 1);
    CxUTFString *currentLine = _bufferLineList.at(cursor.row);

    unsigned long joinCol = prevLine->charCount();

    // Append current line to previous
    prevLine->append(*currentLine);
    prevLine->recalculateTabWidths(tabSpaces);

    // Remove current line
    _bufferLineList.removeAt(cursor.row);

    cursor.row--;
    cursor.col = joinCol;
    lastRequestCol = cursor.col;

    return CxEditHint(cursor.row, cursor.col, CxEditHint::UPDATE_HINT_SCREEN_PAST_POINT, CxEditHint::CURSOR_HINT_JUMP);
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer::setMark
//
//-------------------------------------------------------------------------------------------------
void
CxUTFEditBuffer::setMark(void)
{
    // Reset kill accumulation - any non-kill action breaks the sequence
    _lastWasKill = false;

    markSet = true;
    mark = cursor;
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer::compareCursorToMark
//
//-------------------------------------------------------------------------------------------------
int
CxUTFEditBuffer::compareCursorToMark(void)
{
    if (cursor.row < mark.row) return -1;
    if (cursor.row > mark.row) return 1;
    if (cursor.col < mark.col) return -1;
    if (cursor.col > mark.col) return 1;
    return 0;
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer::copyText
//
//-------------------------------------------------------------------------------------------------
CxString
CxUTFEditBuffer::copyText(void)
{
    if (!markSet) return CxString("");

    CxEditBufferPosition start = mark;
    CxEditBufferPosition end = cursor;

    // Ensure start is before end
    if (compareCursorToMark() < 0) {
        start = cursor;
        end = mark;
    }

    CxString result;

    if (start.row == end.row) {
        // Single line selection
        CxUTFString *line = _bufferLineList.at(start.row);
        CxUTFString sub = line->subString(start.col, end.col - start.col);
        result = sub.toBytes();
    } else {
        // Multi-line selection
        // First line (from start.col to end)
        CxUTFString *firstLine = _bufferLineList.at(start.row);
        CxUTFString firstSub = firstLine->subString(start.col, firstLine->charCount() - start.col);
        result = firstSub.toBytes();
        result += "\n";

        // Middle lines
        for (unsigned long row = start.row + 1; row < end.row; row++) {
            CxUTFString *midLine = _bufferLineList.at(row);
            result += midLine->toBytes();
            result += "\n";
        }

        // Last line (from start to end.col)
        CxUTFString *lastLine = _bufferLineList.at(end.row);
        CxUTFString lastSub = lastLine->subString(0, end.col);
        result += lastSub.toBytes();
    }

    return result;
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer::deleteText
//
//-------------------------------------------------------------------------------------------------
void
CxUTFEditBuffer::deleteText(void)
{
    if (readOnly || !markSet) return;

    CxEditBufferPosition start = mark;
    CxEditBufferPosition end = cursor;

    if (compareCursorToMark() < 0) {
        start = cursor;
        end = mark;
    }

    touched = TRUE;

    if (start.row == end.row) {
        // Single line deletion
        CxUTFString *line = _bufferLineList.at(start.row);
        line->remove(start.col, end.col - start.col);
        line->recalculateTabWidths(tabSpaces);
    } else {
        // Multi-line deletion
        CxUTFString *firstLine = _bufferLineList.at(start.row);
        CxUTFString *lastLine = _bufferLineList.at(end.row);

        // Keep start of first line + end of last line
        CxUTFString firstPart = firstLine->subString(0, start.col);
        CxUTFString lastPart = lastLine->subString(end.col, lastLine->charCount() - end.col);

        firstPart.append(lastPart);
        firstPart.recalculateTabWidths(tabSpaces);

        // Replace first line
        _bufferLineList.replaceAt(start.row, firstPart);

        // Remove middle and last lines
        for (unsigned long row = end.row; row > start.row; row--) {
            _bufferLineList.removeAt(row);
        }
    }

    cursor = start;
    lastRequestCol = cursor.col;
    markSet = false;
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer::cutToMark
//
//-------------------------------------------------------------------------------------------------
CxString
CxUTFEditBuffer::cutToMark(void)
{
    CxString text = copyText();
    deleteText();
    return text;
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer::cutTextToEndOfLine
//
// Cuts text from cursor to end of line. If cursor is already at end of line, joins with
// the next line. Consecutive calls accumulate into the kill buffer, so a subsequent
// paste will restore all the killed text.
//
//-------------------------------------------------------------------------------------------------
CxString
CxUTFEditBuffer::cutTextToEndOfLine(void)
{
    if (readOnly) return CxString("");

    CxUTFString *line = _bufferLineList.at(cursor.row);
    if (line == 0) return CxString("");

    CxString theCutText;

    // Check if cursor is at or past end of line (nothing to cut on this line)
    if (cursor.col >= line->charCount()) {
        // Join with next line if one exists
        unsigned long numberOfBufferLines = _bufferLineList.entries();
        if (cursor.row + 1 < numberOfBufferLines) {
            CxUTFString *nextLine = _bufferLineList.at(cursor.row + 1);
            if (nextLine != 0) {
                // Append next line to current line
                line->append(*nextLine);
                line->recalculateTabWidths(tabSpaces);

                // Remove the next line
                _bufferLineList.removeAt(cursor.row + 1);

                touched = TRUE;
                theCutText = "\n";
            }
        }
        // If no next line, theCutText remains empty
    } else {
        CxUTFString cut = line->subString(cursor.col, line->charCount() - cursor.col);
        theCutText = cut.toBytes();

        line->remove(cursor.col, line->charCount() - cursor.col);
        touched = TRUE;
    }

    // Accumulate into kill buffer - append if last operation was also a kill
    if (_lastWasKill) {
        _killAccumulator += theCutText;
    } else {
        _killAccumulator = theCutText;
    }
    _lastWasKill = true;

    return _killAccumulator;
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer::insertTextAtCursor
//
// Insert text at cursor, properly handling UTF-8 multi-byte sequences.
//
//-------------------------------------------------------------------------------------------------
void
CxUTFEditBuffer::insertTextAtCursor(CxString text)
{
    if (readOnly) return;

    const char *ptr = text.data();
    const char *end = ptr + text.length();

    while (ptr < end && *ptr != '\0') {
        unsigned char c = (unsigned char)*ptr;

        if (c == '\n') {
            addReturn();
            ptr++;
        } else if (c == '\t') {
            addTab();
            ptr++;
        } else if (c < 0x80) {
            // ASCII character
            addCharacter((char)c);
            ptr++;
        } else {
            // UTF-8 multi-byte sequence - parse as CxUTFCharacter
            CxUTFCharacter ch;
            int consumed = ch.fromUTF8(ptr);
            if (consumed == 0) {
                // Invalid UTF-8, skip byte
                ptr++;
                continue;
            }

            // Insert the UTF-8 character directly
            touched = TRUE;

            if (_bufferLineList.entries() == 0) {
                CxUTFString newLine;
                newLine.append(ch);
                _bufferLineList.append(newLine);
                cursor.col = 1;
            } else {
                CxUTFString *line = _bufferLineList.at(cursor.row);
                line->insert(cursor.col, ch);
                line->recalculateTabWidths(tabSpaces);
                cursor.col++;
            }
            lastRequestCol = cursor.col;

            ptr += consumed;
        }
    }
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer::pasteFromCutBuffer
//
//-------------------------------------------------------------------------------------------------
void
CxUTFEditBuffer::pasteFromCutBuffer(CxString text)
{
    // Reset kill accumulation - any non-kill action breaks the sequence
    _lastWasKill = false;

    insertTextAtCursor(text);
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer::flattenBuffer
//
//-------------------------------------------------------------------------------------------------
CxString
CxUTFEditBuffer::flattenBuffer(void)
{
    CxString result;

    for (unsigned long row = 0; row < _bufferLineList.entries(); row++) {
        CxUTFString *line = _bufferLineList.at(row);
        result += line->toBytes();
        result += "\n";
    }

    return result;
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer::findString
//
//-------------------------------------------------------------------------------------------------
int
CxUTFEditBuffer::findString(CxString findStr)
{
    if (findStr.length() == 0) return FALSE;
    return findAgain(findStr, FALSE);
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer::findAgain
//
//-------------------------------------------------------------------------------------------------
int
CxUTFEditBuffer::findAgain(CxString findStr, int skipIfCurrent)
{
    if (findStr.length() == 0) return FALSE;
    if (cursor.row >= _bufferLineList.entries()) return FALSE;

    if (cursor == lastFindLocation && skipIfCurrent) {
        cursorRightRequest();
    }

    // Search from cursor position
    for (unsigned long row = cursor.row; row < _bufferLineList.entries(); row++) {
        CxUTFString *line = _bufferLineList.at(row);
        CxString lineBytes = line->toBytes();

        unsigned long searchStart = (row == cursor.row) ? cursor.col : 0;

        // Convert character column to byte offset for search
        int byteOffset = 0;
        if (row == cursor.row && searchStart > 0) {
            byteOffset = line->displayColumnOfChar(searchStart);
            // Actually need byte offset, not display column - use toBytes and count
            CxString prefix = line->subString(0, searchStart).toBytes();
            byteOffset = prefix.length();
        }

        int found = lineBytes.index(findStr, byteOffset);
        if (found != -1) {
            // Convert byte offset back to character index
            // Count characters up to found position
            int charIndex = 0;
            int bytes = 0;
            while (charIndex < (int)line->charCount() && bytes < found) {
                const CxUTFCharacter *ch = line->at(charIndex);
                bytes += ch->byteCount();
                charIndex++;
            }

            cursorGotoRequest(row, charIndex);
            lastFindLocation = cursor;
            return TRUE;
        }
    }

    return FALSE;
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer::replaceString
//
//-------------------------------------------------------------------------------------------------
int
CxUTFEditBuffer::replaceString(CxString findStr, CxString replaceStr)
{
    if (readOnly) return FALSE;
    if (findStr.length() == 0) return FALSE;
    return replaceAgain(findStr, replaceStr);
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer::replaceAgain
//
//-------------------------------------------------------------------------------------------------
int
CxUTFEditBuffer::replaceAgain(CxString findStr, CxString replaceStr)
{
    if (readOnly) return FALSE;
    if (findStr.length() == 0) return FALSE;
    if (cursor.row >= _bufferLineList.entries()) return FALSE;

    CxUTFString *line = _bufferLineList.at(cursor.row);
    CxString lineBytes = line->toBytes();

    // Check if findStr is at current position
    CxString prefix = line->subString(0, cursor.col).toBytes();
    int byteOffset = prefix.length();

    int found = lineBytes.index(findStr, byteOffset);

    if (found == byteOffset) {
        // Found at cursor position - do replacement
        touched = TRUE;

        // Remove find string characters
        int findCharCount = 0;
        int findBytes = 0;
        int charIndex = cursor.col;
        while (findBytes < findStr.length() && charIndex < (int)line->charCount()) {
            const CxUTFCharacter *ch = line->at(charIndex);
            findBytes += ch->byteCount();
            findCharCount++;
            charIndex++;
        }

        line->remove(cursor.col, findCharCount);

        // Insert replacement
        CxUTFString replaceUTF;
        replaceUTF.fromBytes(replaceStr.data(), replaceStr.length(), tabSpaces);

        for (int i = replaceUTF.charCount() - 1; i >= 0; i--) {
            line->insert(cursor.col, *replaceUTF.at(i));
        }

        line->recalculateTabWidths(tabSpaces);

        cursor.col += replaceUTF.charCount();
        lastRequestCol = cursor.col;
    }

    return findAgain(findStr, TRUE);
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer::entab
//
//-------------------------------------------------------------------------------------------------
void
CxUTFEditBuffer::entab(void)
{
    // Convert leading spaces to tabs
    for (unsigned long row = 0; row < _bufferLineList.entries(); row++) {
        CxUTFString *line = _bufferLineList.at(row);

        int leadingSpaces = 0;
        for (int i = 0; i < (int)line->charCount(); i++) {
            const CxUTFCharacter *ch = line->at(i);
            if (ch->isASCII() && ch->bytes()[0] == ' ') {
                leadingSpaces++;
            } else {
                break;
            }
        }

        if (leadingSpaces >= tabSpaces) {
            // Remove leading spaces
            line->remove(0, leadingSpaces);

            // Insert tabs
            int numTabs = leadingSpaces / tabSpaces;
            int remainingSpaces = leadingSpaces % tabSpaces;

            for (int t = 0; t < numTabs; t++) {
                line->insert(0, CxUTFCharacter::makeTab(tabSpaces));
            }

            for (int s = 0; s < remainingSpaces; s++) {
                line->insert(numTabs, CxUTFCharacter::fromASCII(' '));
            }

            line->recalculateTabWidths(tabSpaces);
            touched = TRUE;
        }
    }
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer::detab
//
//-------------------------------------------------------------------------------------------------
void
CxUTFEditBuffer::detab(void)
{
    for (unsigned long row = 0; row < _bufferLineList.entries(); row++) {
        CxUTFString *line = _bufferLineList.at(row);

        // Find and replace tabs with spaces
        for (int i = 0; i < (int)line->charCount(); i++) {
            CxUTFCharacter *ch = line->at(i);
            if (ch->isTab()) {
                int width = ch->displayWidth();
                line->remove(i, 1);

                for (int s = 0; s < width; s++) {
                    line->insert(i + s, CxUTFCharacter::fromASCII(' '));
                }

                touched = TRUE;
            }
        }
    }
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer::trimTrailing
//
// Remove trailing whitespace (spaces and tabs) from all lines
// Returns the total number of characters removed
//
//-------------------------------------------------------------------------------------------------
int
CxUTFEditBuffer::trimTrailing(void)
{
    int totalRemoved = 0;

    for (unsigned long row = 0; row < _bufferLineList.entries(); row++) {
        CxUTFString *line = _bufferLineList.at(row);

        // Find last non-whitespace character
        int lastNonSpace = -1;
        for (int i = 0; i < (int)line->charCount(); i++) {
            const CxUTFCharacter *ch = line->at(i);
            if (ch->isASCII()) {
                char c = ch->bytes()[0];
                if (c != ' ' && c != '\t') {
                    lastNonSpace = i;
                }
            } else {
                // Non-ASCII characters are not whitespace
                lastNonSpace = i;
            }
        }

        // Remove trailing whitespace
        int charsToRemove = (int)line->charCount() - (lastNonSpace + 1);
        if (charsToRemove > 0) {
            line->remove(lastNonSpace + 1, charsToRemove);
            totalRemoved += charsToRemove;
            touched = TRUE;
        }
    }

    return totalRemoved;
}


//-------------------------------------------------------------------------------------------------
// State accessors
//-------------------------------------------------------------------------------------------------

int CxUTFEditBuffer::isReadOnly(void) { return readOnly; }
int CxUTFEditBuffer::isTouched(void) { return touched; }
int CxUTFEditBuffer::isInMemory(void) { return inMemory; }
void CxUTFEditBuffer::setReadOnly(int v) { readOnly = v; }

int CxUTFEditBuffer::isCursorFirstRow(void) { return cursor.row == 0; }
int CxUTFEditBuffer::isCursorLastRow(void) {
    unsigned long n = _bufferLineList.entries();
    return n == 0 || cursor.row == n - 1;
}
int CxUTFEditBuffer::isCursorFirstCol(void) { return cursor.col == 0; }
int CxUTFEditBuffer::isCursorLastCol(void) {
    return evaluatePosition(cursor.row, cursor.col) == POS_VALID_APPEND_COL;
}
int CxUTFEditBuffer::isCursorFirstValidPosition(void) {
    return isCursorFirstRow() && isCursorFirstCol();
}
int CxUTFEditBuffer::isCursorLastValidPosition(void) {
    return isCursorLastRow() && isCursorLastCol();
}
int CxUTFEditBuffer::isCursorRowEmpty(void) {
    CxUTFString *line = _bufferLineList.at(cursor.row);
    return line == 0 || line->charCount() == 0;
}

void CxUTFEditBuffer::setVisualFirstScreenLine(int line) { visualFirstScreenLine = line; }
int CxUTFEditBuffer::getVisualFirstScreenLine(void) { return visualFirstScreenLine; }
void CxUTFEditBuffer::setVisualFirstScreenCol(int col) { visualFirstScreenCol = col; }
int CxUTFEditBuffer::getVisualFirstScreenCol(void) { return visualFirstScreenCol; }

CxString CxUTFEditBuffer::getFilePath(void) { return filePath; }
void CxUTFEditBuffer::setFilePath(CxString fp) { filePath = fp; }


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer::cursorDisplayColumn
//
//-------------------------------------------------------------------------------------------------
int
CxUTFEditBuffer::cursorDisplayColumn(void)
{
    CxUTFString *line = _bufferLineList.at(cursor.row);
    if (line == 0) return 0;
    return line->displayColumnOfChar(cursor.col);
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer::displayColumnToCharIndex
//
//-------------------------------------------------------------------------------------------------
int
CxUTFEditBuffer::displayColumnToCharIndex(unsigned long row, int displayCol)
{
    CxUTFString *line = _bufferLineList.at(row);
    if (line == 0) return 0;
    return line->charIndexAtDisplayColumn(displayCol);
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer::getPositionPriorToCursor
//
//-------------------------------------------------------------------------------------------------
CxEditBufferPosition
CxUTFEditBuffer::getPositionPriorToCursor(void)
{
    unsigned long saveRow = cursor.row;
    unsigned long saveCol = cursor.col;

    cursorLeftRequest();

    CxEditBufferPosition result(cursor.row, cursor.col);

    cursor.row = saveRow;
    cursor.col = saveCol;

    return result;
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer::debug_print
//
//-------------------------------------------------------------------------------------------------
void
CxUTFEditBuffer::debug_print(void)
{
    printf("CxUTFEditBuffer: %lu lines\n", _bufferLineList.entries());
    printf("Cursor: row=%lu col=%lu\n", cursor.row, cursor.col);

    for (unsigned long i = 0; i < _bufferLineList.entries() && i < 10; i++) {
        CxUTFString *line = _bufferLineList.at(i);
        CxString bytes = line->toBytes();
        printf("Line %lu (%d chars, %d cols): %s\n",
               i, line->charCount(), line->displayWidth(), bytes.data());
    }
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer::insertCommentBlock
//
// Inserts a comment block and positions the cursor for inserting the comment text.
//
//-------------------------------------------------------------------------------------------------
void
CxUTFEditBuffer::insertCommentBlock(unsigned long lastCol)
{
    if (readOnly) return;

    // Get the current line text as bytes
    CxString text = "";
    if (cursor.row < _bufferLineList.entries()) {
        CxUTFString *currentLine = _bufferLineList.at(cursor.row);
        if (currentLine) {
            text = currentLine->toBytes();
        }
    }

    // Cut the preceding text before the cursor (get leading whitespace)
    text = text.subString(0, cursor.col);

    // Calculate how many remaining positions there are for dashes
    unsigned long remainingCharacterNumber = 0;
    if (lastCol > text.length() + 2) {
        remainingCharacterNumber = lastCol - text.length() - 2;
    }

    // Insert the first line (top border)
    insertTextAtCursor("//");
    for (unsigned long i = 0; i < remainingCharacterNumber; i++) {
        addCharacter('-');
    }
    addCharacter('\n');

    // Insert comment body lines
    insertTextAtCursor(text);
    insertTextAtCursor("// \n");

    insertTextAtCursor(text);
    insertTextAtCursor("// \n");

    insertTextAtCursor(text);
    insertTextAtCursor("// \n");

    insertTextAtCursor(text);
    insertTextAtCursor("// \n");

    // Insert the bottom border
    insertTextAtCursor(text);
    insertTextAtCursor("//");
    for (unsigned long i = 0; i < remainingCharacterNumber; i++) {
        addCharacter('-');
    }
    insertTextAtCursor("\n");

    // Position cursor on the first comment line
    cursorUpRequest();
    cursorUpRequest();
    cursorUpRequest();
    cursorUpRequest();
    cursorUpRequest();
    cursorRightRequest();
    cursorRightRequest();
    cursorRightRequest();
}
