# sheetModel Project Instructions

## Overview
sheetModel is the spreadsheet data model library for the cx framework. It provides the core data structures and computation engine for spreadsheet applications.

## Architecture Principle

**sheetModel is a pure compute structure.** It handles:
- Cell storage (text, numbers, formulas)
- Formula parsing and evaluation
- Dependency tracking and recalculation
- Cell addressing and navigation
- JSON persistence of cell data

**sheetModel does NOT handle visual attributes:**
- Column widths
- Row heights
- Colors, fonts, styles
- Cell alignment
- Any display/rendering concerns

Visual attributes are the responsibility of the application layer (e.g., ss's SheetView). This separation allows sheetModel to be used by non-visual tools like CLI processors, automation scripts, and testing frameworks.

## App Data Mechanism

For apps that need to persist visual attributes alongside cell data:
- `getAppData()` - returns preserved JSON object with unknown top-level keys
- `setAppData()` - sets app-specific data to be saved with the sheet
- Unknown JSON keys (like "columns", "styles") are preserved during load/save
- Apps read/write their visual attributes through this mechanism

Example JSON structure:
```json
{
  "version": 1,
  "currentPosition": "A1",
  "cells": [...],
  "columns": {"A": {"width": 12}, "B": {"width": 25}}
}
```
The "columns" key is preserved by sheetModel but not interpreted - that's the app's job.

## Build Instructions
```bash
make
```

## Do Not Modify
- `darwin_arm64/` - ARM64 build output
- `darwin_x86_64/` - x86_64 build output
- `linux_x86_64/` - Linux build output
