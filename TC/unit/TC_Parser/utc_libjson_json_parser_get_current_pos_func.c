#include <tet_api.h>
#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include <json-glib/json-glib.h>


static void startup(void);
static void cleanup(void);

void (*tet_startup)(void) = startup;
void (*tet_cleanup)(void) = cleanup;

static void utc_libjson_json_parser_get_current_pos_func_01(void);
static void utc_libjson_json_parser_get_current_pos_func_02(void);
static void utc_libjson_json_parser_get_current_pos_func_03(void);

enum {
	POSITIVE_TC_IDX = 0x01,
	NEGATIVE_TC_IDX,
};

struct tet_testlist tet_testlist[] = {
	{ utc_libjson_json_parser_get_current_pos_func_01, POSITIVE_TC_IDX },
	{ utc_libjson_json_parser_get_current_pos_func_02, POSITIVE_TC_IDX },
	{ utc_libjson_json_parser_get_current_pos_func_03, POSITIVE_TC_IDX },
	{ NULL, 0 }
};

static const gchar *test_string = "{ \"test\" : 42 }";

static void startup(void)
{
	g_type_init();
}

static void cleanup(void)
{
}


/**
 * @brief Positive test case of json_parser_get_current_pos()
 */
static void utc_libjson_json_parser_get_current_pos_func_01(void)
{
	int pos = 0;

   	pos = json_parser_get_current_pos(NULL);

	dts_check_eq("json_parser_get_current_pos", pos, 0);

}

static void utc_libjson_json_parser_get_current_pos_func_02(void)
{
	JsonParser *parser = NULL;    
	int pos = 0;

	parser = json_parser_new ();  
	g_assert (JSON_IS_PARSER (parser));

	pos = json_parser_get_current_pos(parser);
	dts_check_eq("json_parser_get_current_pos", pos, 0);  
	g_object_unref (parser);
}


static void utc_libjson_json_parser_get_current_pos_func_03(void)
{
	JsonParser *parser = NULL;  
	GError *error = NULL;  
	gboolean ret = FALSE;
	int pos = 0;

	parser = json_parser_new ();  
	g_assert (JSON_IS_PARSER (parser));

	ret = json_parser_load_from_data(parser, test_string, -1, &error);
	dts_check_eq("json_parser_get_current_pos", ret, TRUE);

	pos = json_parser_get_current_pos(parser);
	dts_check_eq("json_parser_get_current_pos", pos, 0);
	g_error_free (error);	   
	g_object_unref (parser);
}


