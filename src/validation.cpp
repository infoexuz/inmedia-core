// validation.cpp - see validation.h.
// Written with help from Claude (AI assistant); reviewed by the team.
#include "validation.h"

namespace {

bool isSpace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

bool isAsciiLetterOrDigit(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
}

// One Cyrillic letter (stored as UTF-8 text, 2 bytes) and its Latin spelling.
struct Letter {
    const char* cyrillic;
    const char* latin;
};

// Array of structures: the transliteration table. "ё" is written as "e" so
// that a search for "шелк" also finds "Шёлковый".
const Letter LETTERS[] = {
    {"а", "a"}, {"б", "b"}, {"в", "v"}, {"г", "g"}, {"д", "d"}, {"е", "e"}, {"ё", "e"},
    {"ж", "zh"}, {"з", "z"}, {"и", "i"}, {"й", "y"}, {"к", "k"}, {"л", "l"}, {"м", "m"},
    {"н", "n"}, {"о", "o"}, {"п", "p"}, {"р", "r"}, {"с", "s"}, {"т", "t"}, {"у", "u"},
    {"ф", "f"}, {"х", "kh"}, {"ц", "ts"}, {"ч", "ch"}, {"ш", "sh"}, {"щ", "shch"}, {"ъ", ""},
    {"ы", "y"}, {"ь", ""}, {"э", "e"}, {"ю", "yu"}, {"я", "ya"}, {"ў", "o"}, {"қ", "q"},
    {"ғ", "g"}, {"ҳ", "h"},
    {"А", "a"}, {"Б", "b"}, {"В", "v"}, {"Г", "g"}, {"Д", "d"}, {"Е", "e"}, {"Ё", "e"},
    {"Ж", "zh"}, {"З", "z"}, {"И", "i"}, {"Й", "y"}, {"К", "k"}, {"Л", "l"}, {"М", "m"},
    {"Н", "n"}, {"О", "o"}, {"П", "p"}, {"Р", "r"}, {"С", "s"}, {"Т", "t"}, {"У", "u"},
    {"Ф", "f"}, {"Х", "kh"}, {"Ц", "ts"}, {"Ч", "ch"}, {"Ш", "sh"}, {"Щ", "shch"}, {"Ъ", ""},
    {"Ы", "y"}, {"Ь", ""}, {"Э", "e"}, {"Ю", "yu"}, {"Я", "ya"}, {"Ў", "o"}, {"Қ", "q"},
    {"Ғ", "g"}, {"Ҳ", "h"},
    // Uzbek Latin apostrophes (oʻ, gʻ) are dropped: "Oʻzbek" -> "ozbek".
    {"ʻ", ""}, {"’", ""}, {"‘", ""},
};
const int LETTER_COUNT = sizeof(LETTERS) / sizeof(LETTERS[0]);

const std::size_t MAX_SLUG_LENGTH = 60;

}  // namespace

std::string trim(const std::string& text) {
    std::size_t begin = 0;
    std::size_t end = text.size();
    while (begin < end && isSpace(text[begin])) {
        ++begin;
    }
    while (end > begin && isSpace(text[end - 1])) {
        --end;
    }
    return text.substr(begin, end - begin);
}

std::string toLowerAscii(const std::string& text) {
    std::string result = text;
    for (std::size_t i = 0; i < result.size(); ++i) {
        if (result[i] >= 'A' && result[i] <= 'Z') {
            result[i] = static_cast<char>(result[i] - 'A' + 'a');
        }
    }
    return result;
}

bool isValidEmail(const std::string& email) {
    std::size_t at = email.find('@');
    if (at == std::string::npos || at == 0) {
        return false;  // no '@', or nothing before it
    }
    if (email.find('@', at + 1) != std::string::npos) {
        return false;  // more than one '@'
    }
    for (char c : email) {
        if (isSpace(c)) {
            return false;
        }
    }
    std::string domain = email.substr(at + 1);
    std::size_t dot = domain.find('.');
    return dot != std::string::npos && dot != 0 && domain.back() != '.';
}

std::string passwordProblem(const std::string& password) {
    if (password.size() < 8) {
        return "Password must be at least 8 characters long.";
    }
    bool hasLetter = false;
    bool hasDigit = false;
    for (char c : password) {
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
            hasLetter = true;
        } else if (c >= '0' && c <= '9') {
            hasDigit = true;
        }
    }
    if (!hasLetter || !hasDigit) {
        return "Password must contain at least one letter and one digit.";
    }
    return "";
}

// Transliterates and joins words with '-'. May return "" (for example "!!!").
std::string toSlugText(const std::string& text) {
    std::string slug;
    bool lastWasDash = true;  // true at the start, so the slug never begins with '-'
    std::size_t i = 0;
    while (i < text.size() && slug.size() < MAX_SLUG_LENGTH) {
        char c = text[i];
        if (isAsciiLetterOrDigit(c)) {
            slug += toLowerAscii(std::string(1, c));
            lastWasDash = false;
            ++i;
            continue;
        }
        // Not an English letter or digit: is it a letter from the table?
        bool matched = false;
        for (int k = 0; k < LETTER_COUNT; ++k) {
            std::string from = LETTERS[k].cyrillic;
            if (text.compare(i, from.size(), from) == 0) {
                std::string to = LETTERS[k].latin;
                if (!to.empty()) {
                    slug += to;
                    lastWasDash = false;
                }
                i += from.size();
                matched = true;
                break;
            }
        }
        if (!matched) {
            // Spaces, punctuation and anything unknown become one '-'.
            if (!lastWasDash) {
                slug += '-';
                lastWasDash = true;
            }
            ++i;
        }
    }
    while (!slug.empty() && slug.back() == '-') {
        slug.pop_back();
    }
    return slug;
}

std::string slugify(const std::string& text) {
    std::string slug = toSlugText(text);
    return slug.empty() ? "video" : slug;
}

bool containsIgnoreCase(const std::string& haystack, const std::string& needle) {
    std::string query = toSlugText(needle);
    if (query.empty()) {
        return trim(needle).empty();  // empty search: everything; "!!!": nothing
    }
    return toSlugText(haystack).find(query) != std::string::npos;
}
