#pragma once

#ifndef CNET_REGEX_H
#define CNET_REGEX_H

#include "CNetGc.h"
#include "CNetApi.h"

typedef struct sRegexPattern RegexPattern;
typedef struct sRegexMatcher RegexMatcher;

/**
 * Compiles a regular expression (supports literals, '.', character
 * classes "[...]"/"[^...]", shorthand classes \d \D \w \W \s \S, anchors
 * ^ and $, quantifiers * + ? and {n,m}, capturing groups "(...)", and
 * alternation "|"). Matching operates on UTF-16 code units: '.' matches
 * exactly one code unit, so a supplementary-plane character (a surrogate
 * pair) requires two '.' to match in full.
 *
 * @param gc Allocator the compiled pattern (and matchers created from it)
 *           is allocated from; caller owns gc and frees it with gcFree()
 *           when done - there is no separate regex free function.
 * @param pattern NUL-terminated pattern text; borrowed.
 * @return New RegexPattern* allocated from gc. NULL if pattern is
 *         malformed.
 */
CNET_API RegexPattern* regexCompile(Gc* gc, const wchar_t* pattern);

/**
 * Creates a matcher that scans text against a compiled pattern.
 *
 * @param gc Allocator the matcher is allocated from; caller owns gc.
 * @param pattern Compiled pattern to match against; borrowed, must
 *                outlive the matcher.
 * @param text NUL-terminated subject text; borrowed - the matcher stores
 *             this pointer directly (does not copy it), so text must
 *             remain valid for the matcher's lifetime.
 * @return New RegexMatcher* allocated from gc.
 */
CNET_API RegexMatcher* regexMatcher(Gc* gc, RegexPattern* pattern, const wchar_t* text);
/**
 * Resets a matcher's search position and captured groups so regexFind()
 * starts over from the beginning of the text.
 *
 * @param matcher Matcher to reset; borrowed.
 */
CNET_API void regexReset(RegexMatcher* matcher);

/**
 * Tests whether the entire subject text matches the pattern.
 *
 * @param matcher Matcher to run; borrowed.
 * @return 1 if the whole text matches, 0 otherwise.
 */
CNET_API int regexMatches(RegexMatcher* matcher);
/**
 * Tests whether the pattern matches starting at position 0 (a prefix
 * match; need not consume the whole text).
 *
 * @param matcher Matcher to run; borrowed.
 * @return 1 if a match anchored at the start is found, 0 otherwise.
 */
CNET_API int regexLookingAt(RegexMatcher* matcher);
/**
 * Searches for the next match anywhere in the text starting from the
 * matcher's current search position, and advances that position past the
 * match for subsequent calls.
 *
 * @param matcher Matcher to run; borrowed.
 * @return 1 if a match was found (updates group 0 and captures), 0 if no
 *         more matches remain.
 */
CNET_API int regexFind(RegexMatcher* matcher);

/**
 * Returns the number of capturing groups in the compiled pattern (not
 * counting group 0, the whole match).
 *
 * @param matcher Matcher to query; borrowed.
 * @return Capturing group count.
 */
CNET_API int regexGroupCount(RegexMatcher* matcher);
/**
 * Returns the start offset of a group from the most recent successful
 * match.
 *
 * @param matcher Matcher to query; borrowed.
 * @param group 0 for the whole match, or 1..regexGroupCount(matcher) for
 *              a capturing group.
 * @return Code-unit offset into the subject text, or -1 if the group did
 *         not participate in the match or group is out of range.
 */
CNET_API int regexStart(RegexMatcher* matcher, int group);
/**
 * Returns the end offset (exclusive) of a group from the most recent
 * successful match.
 *
 * @param matcher Matcher to query; borrowed.
 * @param group 0 for the whole match, or 1..regexGroupCount(matcher) for
 *              a capturing group.
 * @return Code-unit offset into the subject text, or -1 if the group did
 *         not participate in the match or group is out of range.
 */
CNET_API int regexEnd(RegexMatcher* matcher, int group);
/**
 * Extracts the text captured by a group from the most recent successful
 * match.
 *
 * @param gc Allocator the returned string is allocated from; caller owns
 *           gc.
 * @param matcher Matcher holding the match results; borrowed.
 * @param group 0 for the whole match, or 1..regexGroupCount(matcher) for
 *              a capturing group.
 * @return NUL-terminated copy of the captured text allocated from gc; NULL
 *         if group is out of range or did not participate in the match.
 */
CNET_API const wchar_t* regexGroup(Gc* gc, RegexMatcher* matcher, int group);

/**
 * Convenience one-shot helper that compiles pattern and tests whether text
 * fully matches it (equivalent to regexCompile() + regexMatcher() +
 * regexMatches()).
 *
 * @param gc Allocator used for the temporary compiled pattern and matcher;
 *           caller owns gc.
 * @param pattern Regex pattern; borrowed.
 * @param text Subject text; borrowed.
 * @return 1 if text fully matches pattern, 0 otherwise.
 */
CNET_API int regexQuickMatch(Gc* gc, const wchar_t* pattern, const wchar_t* text);

#endif
