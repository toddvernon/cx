# Completer Library Instructions

## Overview
Hierarchical command completion library with literal prefix matching.

## FORBIDDEN: Fuzzy / Dehyphenated Matching

**DO NOT implement, suggest, or reference any of the following:**
- Dehyphenated matching (removing hyphens before comparing)
- Fuzzy matching, abbreviated matching, or command shortening
- Matching "gl" to "goto-line", "sa" to "save-as", "bn" to "buffer-next", etc.
- A `dehyphenate()` function or any hyphen-stripping logic

**All matching is LITERAL PREFIX matching.** The user's input must be an exact prefix of the candidate name, including hyphens. For example:
- "goto" matches "goto-line" (literal prefix) - CORRECT
- "gl" matches "goto-line" (dehyphenated) - WRONG, DO NOT DO THIS
- "save" matches "save-as" (literal prefix) - CORRECT
- "sa" matches "save-as" (dehyphenated) - WRONG, DO NOT DO THIS

## Non-negotiable constraints
- DO NOT use the C++ Standard Library: no `std::` anywhere.
- DO NOT introduce templates.
- Assume older compilers. Avoid C++11+ features.
- See `cx_apps/cm/CLAUDE.md` for full project constraints.
