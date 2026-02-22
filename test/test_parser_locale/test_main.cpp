#ifdef ARDUINO
#include <Arduino.h>
#endif

#include <unity.h>
#include <cstring>
#include <stdint.h>

extern "C" {
#include "../../src/parser.h"
#include "../../src/parser.c"
}

#include "../../src/locale/locale_types.h"
#include "../../src/locale/locale_tr.h"
#include "../../src/locale/locale_us.h"

// ─── compare1 ──────────────────────────────────────────────────────────────

void test_compare_case_sensitive_exact_match() {
    TEST_ASSERT_EQUAL(COMPARE_EQUAL,   compare1("US", 2, "US", COMPARE_CASE_SENSETIVE));
    TEST_ASSERT_EQUAL(COMPARE_UNEQUAL, compare1("us", 2, "US", COMPARE_CASE_SENSETIVE));
}

void test_compare_case_insensitive_match() {
    TEST_ASSERT_EQUAL(COMPARE_EQUAL, compare1("tr", 2, "TR", COMPARE_CASE_INSENSETIVE));
    TEST_ASSERT_EQUAL(COMPARE_EQUAL, compare1("TR", 2, "tr", COMPARE_CASE_INSENSETIVE));
    TEST_ASSERT_EQUAL(COMPARE_EQUAL, compare1("Tr", 2, "tR", COMPARE_CASE_INSENSETIVE));
}

void test_compare_length_shorter_than_template() {
    // len=1 but template is "US" (2 chars) — must be UNEQUAL
    TEST_ASSERT_EQUAL(COMPARE_UNEQUAL, compare1("US", 1, "US", COMPARE_CASE_SENSETIVE));
}

void test_compare_length_longer_than_template() {
    // len=3 but template is "US" (2 chars) — must be UNEQUAL
    TEST_ASSERT_EQUAL(COMPARE_UNEQUAL, compare1("USB", 3, "US", COMPARE_CASE_SENSETIVE));
}

void test_compare_empty_string() {
    TEST_ASSERT_EQUAL(COMPARE_UNEQUAL, compare1("", 0, "US", COMPARE_CASE_SENSETIVE));
    TEST_ASSERT_EQUAL(COMPARE_EQUAL,   compare1("", 0, "",   COMPARE_CASE_SENSETIVE));
}

void test_compare_single_char() {
    TEST_ASSERT_EQUAL(COMPARE_EQUAL,   compare1("a", 1, "a", COMPARE_CASE_SENSETIVE));
    TEST_ASSERT_EQUAL(COMPARE_UNEQUAL, compare1("a", 1, "b", COMPARE_CASE_SENSETIVE));
    TEST_ASSERT_EQUAL(COMPARE_EQUAL,   compare1("a", 1, "A", COMPARE_CASE_INSENSETIVE));
}

// ─── parse_lines ───────────────────────────────────────────────────────────

void test_parse_lines_and_words() {
    const char* script = "STRING Hello\nDELAY 100\n";
    line_list* lines = parse_lines1(script, strlen(script));

    TEST_ASSERT_NOT_NULL(lines);
    TEST_ASSERT_EQUAL_UINT32(2, lines->size);

    line_node* first = line_list_get1(lines, 0);
    TEST_ASSERT_NOT_NULL(first);
    TEST_ASSERT_NOT_NULL(first->words);
    TEST_ASSERT_EQUAL_UINT32(2, first->words->size);

    word_node* cmd = word_list_get1(first->words, 0);
    TEST_ASSERT_NOT_NULL(cmd);
    TEST_ASSERT_EQUAL_UINT32(6, cmd->len);
    TEST_ASSERT_EQUAL_STRING_LEN("STRING", cmd->str, cmd->len);

    line_list_destroy1(lines);
}

void test_parse_single_line_no_newline() {
    const char* script = "REM comment";
    line_list* lines = parse_lines1(script, strlen(script));
    TEST_ASSERT_NOT_NULL(lines);
    TEST_ASSERT_EQUAL_UINT32(1, lines->size);

    line_node* n = line_list_get1(lines, 0);
    TEST_ASSERT_NOT_NULL(n);
    TEST_ASSERT_EQUAL_UINT32(1, n->words->size);

    word_node* w = word_list_get1(n->words, 0);
    TEST_ASSERT_EQUAL_STRING_LEN("REM", w->str, w->len);
    line_list_destroy1(lines);
}

void test_parse_three_words_on_one_line() {
    // CTRL ALT DELETE — three words
    const char* script = "CTRL ALT DELETE\n";
    line_list* lines = parse_lines1(script, strlen(script));
    TEST_ASSERT_NOT_NULL(lines);
    TEST_ASSERT_EQUAL_UINT32(1, lines->size);

    line_node* n = line_list_get1(lines, 0);
    TEST_ASSERT_EQUAL_UINT32(3, n->words->size);

    word_node* w0 = word_list_get1(n->words, 0);
    word_node* w1 = word_list_get1(n->words, 1);
    word_node* w2 = word_list_get1(n->words, 2);

    TEST_ASSERT_EQUAL_STRING_LEN("CTRL",   w0->str, w0->len);
    TEST_ASSERT_EQUAL_STRING_LEN("ALT",    w1->str, w1->len);
    TEST_ASSERT_EQUAL_STRING_LEN("DELETE", w2->str, w2->len);

    line_list_destroy1(lines);
}

void test_parse_windows_r_combo() {
    const char* script = "WINDOWS r\n";
    line_list* lines = parse_lines1(script, strlen(script));
    TEST_ASSERT_NOT_NULL(lines);
    TEST_ASSERT_EQUAL_UINT32(1, lines->size);

    line_node* n = line_list_get1(lines, 0);
    TEST_ASSERT_EQUAL_UINT32(2, n->words->size);

    word_node* w0 = word_list_get1(n->words, 0);
    word_node* w1 = word_list_get1(n->words, 1);
    TEST_ASSERT_EQUAL_STRING_LEN("WINDOWS", w0->str, w0->len);
    TEST_ASSERT_EQUAL_STRING_LEN("r",       w1->str, w1->len);

    line_list_destroy1(lines);
}

void test_parse_multiline_script() {
    const char* script =
        "REM Hello World for Windows\n"
        "DEFAULTDELAY 200\n"
        "GUI r\n"
        "STRING notepad\n"
        "ENTER\n";
    line_list* lines = parse_lines1(script, strlen(script));
    TEST_ASSERT_NOT_NULL(lines);
    TEST_ASSERT_EQUAL_UINT32(5, lines->size);
    line_list_destroy1(lines);
}

void test_parse_empty_script() {
    const char* script = "";
    line_list* lines = parse_lines1(script, 0);
    TEST_ASSERT_NOT_NULL(lines);
    TEST_ASSERT_EQUAL_UINT32(0, lines->size);
    line_list_destroy1(lines);
}

void test_parse_crlf_line_endings() {
    const char* script = "DELAY 500\r\nENTER\r\n";
    line_list* lines = parse_lines1(script, strlen(script));
    TEST_ASSERT_NOT_NULL(lines);
    // At least 2 lines produced
    TEST_ASSERT_TRUE(lines->size >= 2);
    line_list_destroy1(lines);
}

// ─── word_list_get index validity ──────────────────────────────────────────

void test_word_list_get_out_of_range_returns_null() {
    const char* script = "ENTER\n";
    line_list* lines = parse_lines1(script, strlen(script));
    line_node* n = line_list_get1(lines, 0);

    // index 0 valid, index 5 is out of range
    TEST_ASSERT_NOT_NULL(word_list_get1(n->words, 0));
    TEST_ASSERT_NULL(word_list_get1(n->words, 5));

    line_list_destroy1(lines);
}

// ─── locale_tr ─────────────────────────────────────────────────────────────

void test_locale_tr_basic_mappings() {
    TEST_ASSERT_EQUAL_UINT32(256, sizeof(ascii_tr));

    // ASCII 64 '@' => AltGr + Q in TR layout
    size_t idx = 64 * 2;
    TEST_ASSERT_EQUAL_UINT8(KEY_MOD_RALT, ascii_tr[idx]);
    TEST_ASSERT_EQUAL_UINT8(KEY_Q,        ascii_tr[idx + 1]);

    TEST_ASSERT_EQUAL_UINT32(128, locale_tr.ascii_len);
    TEST_ASSERT_EQUAL_UINT32(sizeof(utf8_tr) / 6, locale_tr.utf8_len);
}

void test_locale_tr_ascii_len_matches_table() {
    // ascii_tr table has exactly 128*2 = 256 bytes
    TEST_ASSERT_EQUAL_UINT32(128, locale_tr.ascii_len);
}

void test_locale_tr_has_utf8_table() {
    // utf8 table pointer must be valid and length > 0
    TEST_ASSERT_NOT_NULL(locale_tr.utf8);
    TEST_ASSERT_GREATER_THAN(0, (int)locale_tr.utf8_len);
}

// ─── locale_us ─────────────────────────────────────────────────────────────

void test_locale_us_ascii_len() {
    TEST_ASSERT_EQUAL_UINT32(128, locale_us.ascii_len);
}

void test_locale_us_ascii_table_size() {
    TEST_ASSERT_EQUAL_UINT32(256, sizeof(ascii_us));
}

void test_locale_us_space_mapping() {
    // ASCII 32 is SPACE — no modifier, KEY_SPACE
    size_t idx = 32 * 2;
    TEST_ASSERT_EQUAL_UINT8(KEY_NONE,  ascii_us[idx]);     // no modifier
    TEST_ASSERT_EQUAL_UINT8(KEY_SPACE, ascii_us[idx + 1]);
}

void test_locale_us_uppercase_A_requires_shift() {
    // ASCII 65 'A' => SHIFT + a key (KEY_A)
    size_t idx = 65 * 2;
    TEST_ASSERT_EQUAL_UINT8(KEY_MOD_LSHIFT, ascii_us[idx]);
    TEST_ASSERT_EQUAL_UINT8(KEY_A,          ascii_us[idx + 1]);
}

void test_locale_us_lowercase_a_no_modifier() {
    // ASCII 97 'a' => no modifier, KEY_A
    size_t idx = 97 * 2;
    TEST_ASSERT_EQUAL_UINT8(KEY_NONE, ascii_us[idx]);
    TEST_ASSERT_EQUAL_UINT8(KEY_A,    ascii_us[idx + 1]);
}

void test_locale_us_digit_1() {
    // ASCII 49 '1' => no modifier, KEY_1
    size_t idx = 49 * 2;
    TEST_ASSERT_EQUAL_UINT8(KEY_NONE, ascii_us[idx]);
    TEST_ASSERT_EQUAL_UINT8(KEY_1,    ascii_us[idx + 1]);
}

// ─── Entry point ───────────────────────────────────────────────────────────

#ifdef ARDUINO
void setup() {
    delay(1000);
    UNITY_BEGIN();
#else
int main() {
    UNITY_BEGIN();
#endif

    // compare1
    RUN_TEST(test_compare_case_sensitive_exact_match);
    RUN_TEST(test_compare_case_insensitive_match);
    RUN_TEST(test_compare_length_shorter_than_template);
    RUN_TEST(test_compare_length_longer_than_template);
    RUN_TEST(test_compare_empty_string);
    RUN_TEST(test_compare_single_char);

    // parse_lines1
    RUN_TEST(test_parse_lines_and_words);
    RUN_TEST(test_parse_single_line_no_newline);
    RUN_TEST(test_parse_three_words_on_one_line);
    RUN_TEST(test_parse_windows_r_combo);
    RUN_TEST(test_parse_multiline_script);
    RUN_TEST(test_parse_empty_script);
    RUN_TEST(test_parse_crlf_line_endings);
    RUN_TEST(test_word_list_get_out_of_range_returns_null);

    // locale_tr
    RUN_TEST(test_locale_tr_basic_mappings);
    RUN_TEST(test_locale_tr_ascii_len_matches_table);
    RUN_TEST(test_locale_tr_has_utf8_table);

    // locale_us
    RUN_TEST(test_locale_us_ascii_len);
    RUN_TEST(test_locale_us_ascii_table_size);
    RUN_TEST(test_locale_us_space_mapping);
    RUN_TEST(test_locale_us_uppercase_A_requires_shift);
    RUN_TEST(test_locale_us_lowercase_a_no_modifier);
    RUN_TEST(test_locale_us_digit_1);

#ifdef ARDUINO
    UNITY_END();
}
void loop() {}
#else
    return UNITY_END();
}
#endif
