// test_validation.cpp - checks for validation.cpp.
// Written with help from Claude (AI assistant); reviewed by the team.
#include "check.h"
#include "validation.h"

void testValidation() {
    check(trim("  hello \n") == "hello", "trim removes spaces at both ends");
    check(trim("   ") == "", "trim of only spaces is empty");
    check(isValidEmail("demo@inmedia.uz"), "normal e-mail is valid");
    check(!isValidEmail("demo.inmedia.uz"), "e-mail without @ is invalid");
    check(!isValidEmail("demo@inmedia"), "e-mail without a dot in the domain is invalid");
    check(!isValidEmail("a@@b.uz"), "e-mail with two @ is invalid");
    check(!isValidEmail("de mo@inmedia.uz"), "e-mail with a space is invalid");
    check(passwordProblem("demo12345").empty(), "good password is accepted");
    check(!passwordProblem("short1").empty(), "short password is rejected");
    check(!passwordProblem("onlyletters").empty(), "password without a digit is rejected");
    check(slugify("Silk Road") == "silk-road", "slug of an English title");
    check(slugify("Шёлковый путь") == "shelkovyy-put", "slug of a Russian title");
    check(slugify("Oʻzbek kino") == "ozbek-kino", "slug of an Uzbek Latin title");
    check(slugify("!!!") == "video", "slug of punctuation falls back to 'video'");
    check(containsIgnoreCase("Шёлковый путь", "шелк"), "search ignores case and ё/е");
    check(containsIgnoreCase("Tashkent Nights", "NIGHT"), "search ignores capital letters");
    check(!containsIgnoreCase("Tashkent Nights", "!!!"), "search for punctuation finds nothing");
}
