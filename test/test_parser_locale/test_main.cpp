#ifdef ARDUINO
#include <Arduino.h>
#endif

#include <unity.h>
#include <cstring>

extern "C" {
#include "../../src/parser.h"
#include "../../src/parser.c"
}

#include "../../src/locale/locale_types.h"
#include "../../src/locale/locale_tr.h"

void test_compare_case_sensitive_exact_match() {
    TEST_ASSERT_EQUAL(COMPARE_EQUAL, compare1("US", 2, "US", COMPARE_CASE_SENSETIVE));
    TEST_ASSERT_EQUAL(COMPARE_UNEQUAL, compare1("us", 2, "US", COMPARE_CASE_SENSETIVE));
}

void test_compare_case_insensitive_match() {
    TEST_ASSERT_EQUAL(COMPARE_EQUAL, compare1("tr", 2, "TR", COMPARE_CASE_INSENSETIVE));
}

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

void test_locale_tr_basic_mappings() {
    // ascii array stores modifier/key pairs for 0..127
    TEST_ASSERT_EQUAL_UINT32(256, sizeof(ascii_tr));

    // ASCII code 64 is '@' => expected AltGr + Q in locale_tr
    size_t idx = 64 * 2;
    TEST_ASSERT_EQUAL_UINT8(KEY_MOD_RALT, ascii_tr[idx]);
    TEST_ASSERT_EQUAL_UINT8(KEY_Q, ascii_tr[idx + 1]);

    // Locale metadata lengths should stay in sync with table sizes
    TEST_ASSERT_EQUAL_UINT32(128, locale_tr.ascii_len);
    TEST_ASSERT_EQUAL_UINT32(sizeof(utf8_tr) / 6, locale_tr.utf8_len);
}

#ifdef ARDUINO
void setup() {
    delay(1000);
    UNITY_BEGIN();
    RUN_TEST(test_compare_case_sensitive_exact_match);
    RUN_TEST(test_compare_case_insensitive_match);
    RUN_TEST(test_parse_lines_and_words);
    RUN_TEST(test_locale_tr_basic_mappings);
    UNITY_END();
}

void loop() {
}
#else
int main() {
    UNITY_BEGIN();
    RUN_TEST(test_compare_case_sensitive_exact_match);
    RUN_TEST(test_compare_case_insensitive_match);
    RUN_TEST(test_parse_lines_and_words);
    RUN_TEST(test_locale_tr_basic_mappings);
    return UNITY_END();
}
#endif
