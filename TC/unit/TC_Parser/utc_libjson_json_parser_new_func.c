#include <tet_api.h>

#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include <json-glib/json-glib.h>

static void startup(void);
static void cleanup(void);

void (*tet_startup)(void) = startup;
void (*tet_cleanup)(void) = cleanup;

static void utc_libjson_json_parser_new_func_01(void);
static void utc_libjson_json_parser_new_func_02(void);

enum {
	POSITIVE_TC_IDX = 0x01,
	NEGATIVE_TC_IDX,
};

struct tet_testlist tet_testlist[] = {
	{ utc_libjson_json_parser_new_func_01, POSITIVE_TC_IDX },
	{ utc_libjson_json_parser_new_func_02, POSITIVE_TC_IDX },
	{ NULL, 0 }
};

static void startup(void)
{
	g_type_init ();
}

static void cleanup(void)
{
}

/**
 * @brief Positive test case of json_parser_new()
 */
static void utc_libjson_json_parser_new_func_01(void)
{
	JsonParser *parser = NULL;  

  	parser = json_parser_new ();
	dts_check_ne("json_parser_new", parser, NULL);
	g_object_unref (parser);
}

/**
 * @brief Negative test case of ug_init json_parser_new()
 */
static void utc_libjson_json_parser_new_func_02(void)
{
	JsonParser *parser = NULL;  

   	parser = json_parser_new();

	dts_check_eq("json_parser_new", JSON_IS_PARSER(parser), TRUE);
	g_object_unref (parser);
}
