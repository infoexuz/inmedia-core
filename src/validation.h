// validation.h - checking and cleaning text typed by the user.
// Part of inMedia Core. Written with help from Claude (AI assistant);
// every function was reviewed and can be explained by the team.
#ifndef INMEDIA_VALIDATION_H
#define INMEDIA_VALIDATION_H

#include <string>

// Removes spaces, tabs and newlines from both ends of the text.
std::string trim(const std::string& text);

// Returns a copy with the English letters A-Z changed to a-z.
std::string toLowerAscii(const std::string& text);

// True when the text looks like an e-mail address: one '@', something before
// it, and a domain with a dot that is not at the start or the end.
bool isValidEmail(const std::string& email);

// Returns "" when the password is good enough, otherwise the reason why not
// (at least 8 characters, at least one letter and one digit).
std::string passwordProblem(const std::string& password);

// Turns a title into a short web address: "Silk Road" -> "silk-road",
// "Шёлковый путь" -> "shelkovyy-put". Russian and Uzbek Cyrillic letters are
// written with Latin letters; everything else becomes a single '-'.
std::string slugify(const std::string& text);

// True when `needle` appears inside `haystack`, ignoring capital letters and
// the alphabet (both are compared through slugify, so "шелк" finds "Шёлковый").
bool containsIgnoreCase(const std::string& haystack, const std::string& needle);

#endif
