#include <tet_api.h>
#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include <json-glib/json-glib.h>

static void startup(void);
static void cleanup(void);

void (*tet_startup)(void) = startup;
void (*tet_cleanup)(void) = cleanup;

static void utc_libjson_json_parser_get_root_func_01(void);
static void utc_libjson_json_parser_get_root_func_02(void);
static void utc_libjson_json_parser_get_root_func_03(void);
static void utc_libjson_json_parser_get_root_func_04(void);
static void utc_libjson_json_parser_get_root_func_05(void);
static void utc_libjson_json_parser_get_root_func_06(void);
static void utc_libjson_json_parser_get_root_func_07(void);
static void utc_libjson_json_parser_get_root_func_08(void);
static void utc_libjson_json_parser_get_root_func_09(void);
static void utc_libjson_json_parser_get_root_func_10(void);
static void utc_libjson_json_parser_get_root_func_11(void);


enum {
	POSITIVE_TC_IDX = 0x01,
	NEGATIVE_TC_IDX,
};

struct tet_testlist tet_testlist[] = {
	{ utc_libjson_json_parser_get_root_func_01, POSITIVE_TC_IDX },
	{ utc_libjson_json_parser_get_root_func_02, POSITIVE_TC_IDX },
	{ utc_libjson_json_parser_get_root_func_03, POSITIVE_TC_IDX },
	{ utc_libjson_json_parser_get_root_func_04, POSITIVE_TC_IDX },
	{ utc_libjson_json_parser_get_root_func_05, POSITIVE_TC_IDX },
	{ utc_libjson_json_parser_get_root_func_06, POSITIVE_TC_IDX },
	{ utc_libjson_json_parser_get_root_func_07, POSITIVE_TC_IDX },
	{ utc_libjson_json_parser_get_root_func_08, POSITIVE_TC_IDX },
	{ utc_libjson_json_parser_get_root_func_09, POSITIVE_TC_IDX },
	{ utc_libjson_json_parser_get_root_func_10, POSITIVE_TC_IDX },
	{ utc_libjson_json_parser_get_root_func_11, POSITIVE_TC_IDX },
	{ NULL, 0 }
};

static const gchar *test_empty_string = "";
static const gchar *test_empty_array_string = "[ ]";
static const gchar *test_empty_object_string = "{ }";

static const struct {
	const gchar *str;
	JsonNodeType type;
	GType gtype;
}test_base_values[] = {
	{ "null",       JSON_NODE_NULL,  G_TYPE_INVALID },
	{ "42",         JSON_NODE_VALUE, G_TYPE_INT64 },
	{ "true",       JSON_NODE_VALUE, G_TYPE_BOOLEAN },
	{ "\"string\"", JSON_NODE_VALUE, G_TYPE_STRING },
	{ "10.2e3",     JSON_NODE_VALUE, G_TYPE_DOUBLE }
};

static const struct {
	const gchar *str;
	gint len;
	gint element;
	JsonNodeType type;
	GType gtype;
}test_simple_arrays[] = {
	{ "[ true ]",                 1, 0, JSON_NODE_VALUE, G_TYPE_BOOLEAN },
	{ "[ true, false, null ]",    3, 2, JSON_NODE_NULL,  G_TYPE_INVALID },
	{ "[ 1, 2, 3.14, \"test\" ]", 4, 3, JSON_NODE_VALUE, G_TYPE_STRING  }
};

static const gchar *test_nested_arrays[] = {
	"[ 42, [ ], null ]",
	"[ [ ], [ true, [ true ] ] ]",
	"[ [ false, true, 42 ], [ true, false, 3.14 ], \"test\" ]",
	"[ true, { } ]",
	"[ false, { \"test\" : 42 } ]",
	"[ { \"test\" : 42 }, null ]",
	"[ true, { \"test\" : 42 }, null ]",
	"[ { \"channel\" : \"/meta/connect\" } ]"
};

static const struct {
	const gchar *str;
	gint size;
	const gchar *member;
	JsonNodeType type;
	GType gtype;
}test_simple_objects[] = {
	{ "{ \"test\" : 42 }", 1, "test", JSON_NODE_VALUE, G_TYPE_INT64 },
	{ "{ \"name\" : \"\", \"state\" : 1 }", 2, "name", JSON_NODE_VALUE, G_TYPE_STRING },
	{ "{ \"foo\" : \"bar\", \"baz\" : null }", 2, "baz", JSON_NODE_NULL, G_TYPE_INVALID },
	{ "{ \"channel\" : \"/meta/connect\" }", 1, "channel", JSON_NODE_VALUE, G_TYPE_STRING }
};

static const gchar *test_nested_objects[] = {
	"{ \"array\" : [ false, \"foo\" ], \"object\" : { \"foo\" : true } }"
};

static const struct {
	const gchar *str;
	const gchar *var;
}test_assignments[] = {
	{ "var foo = [ false, false, true ]", "foo" },
	{ "var bar = [ true, 42 ];", "bar" },
	{ "var baz = { \"foo\" : false }", "baz" }
};

static const struct
{
	const gchar *str;
	const gchar *member;
	const gchar *match;
}test_unicode[] = {
	{ "{ \"test\" : \"foo \\u00e8\" }", "test", "foo è" }
};

static const struct
{
	const gchar *str;
}test_invalid[] = {
	{ "test" },
	{ "[ foo, ]" },
	{ "[ true, ]" },
	{ "{ \"foo\" : true \"bar\" : false }" },
	{ "[ true, [ false, ] ]" },
	{ "{ \"foo\" : { \"bar\" : false, } }" },
	{ "[ { }, { }, { }, ]" }
};

static guint n_test_base_values    = G_N_ELEMENTS (test_base_values);
static guint n_test_simple_arrays  = G_N_ELEMENTS (test_simple_arrays);
static guint n_test_nested_arrays  = G_N_ELEMENTS (test_nested_arrays);
static guint n_test_simple_objects = G_N_ELEMENTS (test_simple_objects);
static guint n_test_nested_objects = G_N_ELEMENTS (test_nested_objects);
static guint n_test_assignments    = G_N_ELEMENTS (test_assignments);
static guint n_test_unicode        = G_N_ELEMENTS (test_unicode);
static guint n_test_invalid        = G_N_ELEMENTS (test_invalid);

static void startup(void)
{
	g_type_init ();
}

static void cleanup(void)
{
}

/**
 * @brief Positive test case of json_parser_get_root()
 */
static void utc_libjson_json_parser_get_root_func_01(void)
{
	JsonParser *parser = NULL;    
	GError *error = NULL;  
	gboolean ret = FALSE;
	JsonNode *root = NULL;  

	parser = json_parser_new ();  
	g_assert (JSON_IS_PARSER (parser));

	ret = json_parser_load_from_data(parser, test_empty_string, -1, &error);
	dts_check_eq("json_parser_get_root", ret, TRUE);

	root = json_parser_get_root (parser);
	dts_check_eq("json_parser_get_root", root, NULL);

	g_error_free (error);	   
	g_object_unref (parser);
}

static void utc_libjson_json_parser_get_root_func_02(void)
{
	JsonParser *parser = NULL;    
	GError *error = NULL;  
	JsonNode *root = NULL;  
	int i = 0;

	parser = json_parser_new ();  
	g_assert (JSON_IS_PARSER (parser));

	for (i = 0; i < n_test_base_values; i++)
	{
		error = NULL;
		if (!json_parser_load_from_data (parser, test_base_values[i].str, -1, &error))
		{
			g_error_free (error);
		}
		else
		{
			root = json_parser_get_root (parser);
			dts_check_ne("json_parser_get_root", root, NULL);
			dts_check_eq("json_parser_get_root", JSON_NODE_TYPE(root), test_base_values[i].type);
		}
	}
	dts_pass("json_parser_get_root", "pass");
	g_object_unref (parser);

}

static void utc_libjson_json_parser_get_root_func_03(void)
{
	JsonParser *parser = NULL;    
	GError *error = NULL;  
	gboolean ret = FALSE;
	JsonNode *root = NULL;  

	parser = json_parser_new ();  
	g_assert (JSON_IS_PARSER (parser));

	ret = json_parser_load_from_data(parser, test_empty_array_string, -1, &error);
	dts_check_eq("json_parser_get_root", ret, TRUE);

	root = json_parser_get_root (parser);
	dts_check_ne("json_parser_get_root", root, NULL);
	dts_check_eq("json_parser_get_root", JSON_NODE_TYPE(root), JSON_NODE_ARRAY);
	g_error_free (error);	   
	g_object_unref (parser);
}


static void utc_libjson_json_parser_get_root_func_04(void)
{
	JsonParser *parser = NULL;    
	GError *error = NULL;  
	JsonNode *root = NULL;  
	int i = 0;

	parser = json_parser_new ();  
	g_assert (JSON_IS_PARSER (parser));

	for (i = 0; i < n_test_simple_arrays; i++)
	{
		error = NULL;
		if (!json_parser_load_from_data (parser, test_simple_arrays[i].str, -1, &error))
		{
			g_error_free (error);
		}
		else
		{
			root = json_parser_get_root (parser);
			dts_check_ne("json_parser_get_root", root, NULL);
			dts_check_eq("json_parser_get_root", JSON_NODE_TYPE(root), JSON_NODE_ARRAY);
		}
	}
	dts_pass("json_parser_get_root", "pass");
	g_object_unref (parser);
}


static void utc_libjson_json_parser_get_root_func_05(void)
{
	JsonParser *parser = NULL;    
	GError *error = NULL;  
	JsonNode *root = NULL;  
	int i = 0;

	parser = json_parser_new ();  
	g_assert (JSON_IS_PARSER (parser));

	for (i = 0; i < n_test_nested_arrays; i++)
	{
		error = NULL;
		if (!json_parser_load_from_data (parser, test_nested_arrays[i], -1, &error))
		{
			g_error_free (error);
		}
		else
		{
			root = json_parser_get_root (parser);
			dts_check_ne("json_parser_get_root", root, NULL);
			dts_check_eq("json_parser_get_root", JSON_NODE_TYPE(root), JSON_NODE_ARRAY);
		}
	}
	dts_pass("json_parser_get_root", "pass");
	g_object_unref (parser);
}

static void utc_libjson_json_parser_get_root_func_06(void)
{
	JsonParser *parser = NULL;    
	GError *error = NULL;
	gboolean ret = FALSE;
	JsonNode *root = NULL;  

	parser = json_parser_new ();  
	g_assert (JSON_IS_PARSER (parser));

	ret = json_parser_load_from_data(parser, test_empty_object_string, -1, &error);
	dts_check_eq("json_parser_get_root", ret, TRUE);

	root = json_parser_get_root (parser);
	dts_check_ne("json_parser_get_root", root, NULL);
	dts_check_eq("json_parser_get_root", JSON_NODE_TYPE(root), JSON_NODE_OBJECT);
	g_error_free (error);	   
	g_object_unref (parser);
}



static void utc_libjson_json_parser_get_root_func_07(void)
{
	JsonParser *parser = NULL;    
	GError *error = NULL;  
	JsonNode *root = NULL;  
	int i = 0;

	parser = json_parser_new ();  
	g_assert (JSON_IS_PARSER (parser));

	for (i = 0; i < n_test_simple_objects; i++)
	{
		error = NULL;
		if (!json_parser_load_from_data (parser, test_simple_objects[i].str, -1, &error))
		{
			g_error_free (error);
		}
		else
		{
			root = json_parser_get_root (parser);
			dts_check_ne("json_parser_get_root", root, NULL);
			dts_check_eq("json_parser_get_root", JSON_NODE_TYPE(root), JSON_NODE_OBJECT);
		}
	}
	dts_pass("json_parser_get_root", "pass");
	g_object_unref (parser);

}

static void utc_libjson_json_parser_get_root_func_08(void)
{
	JsonParser *parser = NULL;    
	GError *error = NULL;  
	JsonNode *root = NULL;  
	int i = 0;

	parser = json_parser_new ();  
	g_assert (JSON_IS_PARSER (parser));

	for (i = 0; i < n_test_nested_objects; i++)
	{
		error = NULL;
		if (!json_parser_load_from_data (parser, test_nested_objects[i], -1, &error))
		{
			g_error_free (error);
		}
		else
		{
			root = json_parser_get_root (parser);
			dts_check_ne("json_parser_get_root", root, NULL);
			dts_check_eq("json_parser_get_root", JSON_NODE_TYPE(root), JSON_NODE_OBJECT);
		}
	}
	dts_pass("json_parser_get_root", "pass");
	g_object_unref (parser);

}


static void utc_libjson_json_parser_get_root_func_09(void)
{
	JsonParser *parser = NULL; 
	GError *error = NULL; 
	JsonNode *root = NULL;  
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
			root = json_parser_get_root (parser);
			dts_check_ne("json_parser_get_root", root, NULL);
		}
	}
	dts_pass("json_parser_has_assignment", "pass");
	g_object_unref (parser);
}


static void utc_libjson_json_parser_get_root_func_10(void)
{
	JsonParser *parser = NULL;    
	GError *error = NULL;  
	JsonNode *root = NULL;  
	int i = 0;

	parser = json_parser_new ();  
	g_assert (JSON_IS_PARSER (parser));

	for (i = 0; i < n_test_unicode; i++)
	{
		error = NULL;
		if (!json_parser_load_from_data (parser, test_unicode[i].str, -1, &error))
		{
			g_error_free (error);
		}
		else
		{		
			root = json_parser_get_root (parser);
			dts_check_ne("json_parser_load_from_data", root, NULL);
			dts_check_eq("json_parser_load_from_data", JSON_NODE_TYPE (root), JSON_NODE_OBJECT);
		}
	}
	dts_pass("json_parser_load_from_data", "pass");
	g_object_unref (parser);

}


static void utc_libjson_json_parser_get_root_func_11(void)
{
	JsonParser *parser = NULL;    
	GError *error = NULL;  
	gboolean ret = FALSE;
	int i = 0;

	parser = json_parser_new ();  
	g_assert (JSON_IS_PARSER (parser));

	for (i = 0; i < n_test_invalid; i++)
	{
		error = NULL;  
		ret = json_parser_load_from_data (parser, test_invalid[i].str, -1, &error);
		dts_check_eq("json_parser_load_from_data", ret, FALSE);

		dts_check_ne("json_parser_load_from_data", error, NULL);
      	dts_check_eq("json_parser_load_from_data", error->domain, JSON_PARSER_ERROR);
	  
		g_error_free (error);
	}
	dts_pass("json_parser_load_from_data", "pass");
	g_object_unref (parser);

}


