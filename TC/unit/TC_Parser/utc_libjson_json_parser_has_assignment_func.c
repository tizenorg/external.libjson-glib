#include <tet_api.h>
#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include <json-glib/json-glib.h>

static void startup(void);
static void cleanup(void);

void (*tet_startup)(void) = startup;
void (*tet_cleanup)(void) = cleanup;

static void utc_libjson_json_parser_has_assignment_func_01(void);
static void utc_libjson_json_parser_has_assignment_func_02(void);
static void utc_libjson_json_parser_has_assignment_func_03(void);
static void utc_libjson_json_parser_has_assignment_func_04(void);

enum {
	POSITIVE_TC_IDX = 0x01,
	NEGATIVE_TC_IDX,
};

struct tet_testlist tet_testlist[] = {
	{ utc_libjson_json_parser_has_assignment_func_01, POSITIVE_TC_IDX },
	{ utc_libjson_json_parser_has_assignment_func_02, POSITIVE_TC_IDX },
	{ utc_libjson_json_parser_has_assignment_func_03, POSITIVE_TC_IDX },
	{ utc_libjson_json_parser_has_assignment_func_04, POSITIVE_TC_IDX },
	{ NULL, 0 }
};


static const gchar *test_string = "{ \"test\" : 42 }";

static const struct {
	const gchar *str;
	const gchar *var;
}test_assignments[] = {
	{ "var foo = [ false, false, true ]", "foo" },
	{ "var bar = [ true, 42 ];", "bar" },
	{ "var baz = { \"foo\" : false }", "baz" }
};

static guint n_test_assignments    = G_N_ELEMENTS (test_assignments);


static void startup(void)
{
	g_type_init();
}

static void cleanup(void)
{
}



/**
 * @brief Positive test case of json_parser_has_assignment()
 */
static void utc_libjson_json_parser_has_assignment_func_01(void)
{
	gboolean ret = FALSE;

	ret = json_parser_has_assignment (NULL, NULL);
	dts_check_eq("json_parser_has_assignment", ret, FALSE);
}

/**
 * @brief Negative test case of ug_init json_parser_has_assignment()
 */
static void utc_libjson_json_parser_has_assignment_func_02(void)
{
	JsonParser *parser = NULL; 
	gboolean ret = FALSE;

	parser = json_parser_new ();  
	g_assert (JSON_IS_PARSER (parser));

	ret = json_parser_has_assignment(parser, NULL);
	dts_check_eq("json_parser_has_assignment", ret, FALSE);
	g_object_unref (parser);
}

static void utc_libjson_json_parser_has_assignment_func_03(void)
{
	JsonParser *parser = NULL; 
	GError *error = NULL; 
	gboolean ret = FALSE;
	gchar *var = NULL; ;

	parser = json_parser_new ();  
	g_assert (JSON_IS_PARSER (parser));

	if (!json_parser_load_from_data (parser, test_string, -1, &error))
	{
		g_error_free (error);
	}
	else
	{
		ret = json_parser_has_assignment (parser, &var);
		dts_check_eq("json_parser_has_assignment", ret, FALSE);
		dts_check_eq("json_parser_has_assignment", var, NULL);
	}
	g_object_unref (parser);
}

static void utc_libjson_json_parser_has_assignment_func_04(void)
{
	JsonParser *parser = NULL; 
	GError *error = NULL; 
	gboolean ret = FALSE;
	gchar *var = NULL; 
	int i = 0;

	parser = json_parser_new ();  
	g_assert (JSON_IS_PARSER (parser));

	for (i = 0; i < n_test_assignments; i++)
	{
		error = NULL;
		if (!json_parser_load_from_data (parser, test_assignments[i].str, -1, &error))
		{
			g_error_free (error);
		}
		else
		{
			ret = json_parser_has_assignment (parser, &var);
			dts_check_eq("json_parser_has_assignment", ret, TRUE);
			dts_check_ne("json_parser_has_assignment", var, NULL);
			dts_check_str_eq ("json_parser_has_assignment", var, test_assignments[i].var);
		}
	}
	dts_pass("json_parser_has_assignment", "pass");
	g_object_unref (parser);
}

