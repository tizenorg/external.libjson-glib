#include <tet_api.h>
#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include <json-glib/json-glib.h>

static void startup(void);
static void cleanup(void);

void (*tet_startup)(void) = startup;
void (*tet_cleanup)(void) = cleanup;

static void utc_libjson_json_parser_load_from_data_func_01(void);
static void utc_libjson_json_parser_load_from_data_func_02(void);
static void utc_libjson_json_parser_load_from_data_func_03(void);
static void utc_libjson_json_parser_load_from_data_func_04(void);
static void utc_libjson_json_parser_load_from_data_func_05(void);
static void utc_libjson_json_parser_load_from_data_func_06(void);
static void utc_libjson_json_parser_load_from_data_func_07(void);
static void utc_libjson_json_parser_load_from_data_func_08(void);
static void utc_libjson_json_parser_load_from_data_func_09(void);
static void utc_libjson_json_parser_load_from_data_func_10(void);
static void utc_libjson_json_parser_load_from_data_func_11(void);
static void utc_libjson_json_parser_load_from_data_func_12(void);


enum {
	POSITIVE_TC_IDX = 0x01,
	NEGATIVE_TC_IDX,
};

struct tet_testlist tet_testlist[] = {
	{ utc_libjson_json_parser_load_from_data_func_01, POSITIVE_TC_IDX },
	{ utc_libjson_json_parser_load_from_data_func_02, POSITIVE_TC_IDX },
	{ utc_libjson_json_parser_load_from_data_func_03, POSITIVE_TC_IDX },
	{ utc_libjson_json_parser_load_from_data_func_04, POSITIVE_TC_IDX },
	{ utc_libjson_json_parser_load_from_data_func_05, POSITIVE_TC_IDX },
	{ utc_libjson_json_parser_load_from_data_func_06, POSITIVE_TC_IDX },
	{ utc_libjson_json_parser_load_from_data_func_07, POSITIVE_TC_IDX },
	{ utc_libjson_json_parser_load_from_data_func_08, POSITIVE_TC_IDX },
	{ utc_libjson_json_parser_load_from_data_func_09, POSITIVE_TC_IDX },
	{ utc_libjson_json_parser_load_from_data_func_10, POSITIVE_TC_IDX },
	{ utc_libjson_json_parser_load_from_data_func_11, POSITIVE_TC_IDX },
	{ utc_libjson_json_parser_load_from_data_func_12, POSITIVE_TC_IDX },
	{ NULL, 0 }
};

static const gchar *test_empty_string = "";
static const gchar *test_empty_array_string = "[ ]";
static const gchar *test_empty_object_string = "{ }";

static void
verify_int_value (JsonNode *node)
{
	dts_check_eq("json_parser_load_from_data", 42, json_node_get_int (node));
}

static void
verify_boolean_value (JsonNode *node)
{
	dts_check_eq("json_parser_load_from_data", TRUE, json_node_get_boolean (node));
}

static void
verify_string_value (JsonNode *node)
{
	dts_check_str_eq("json_parser_load_from_data", "string", json_node_get_string (node));
}

static void
verify_double_value (JsonNode *node)
{
	dts_check_eq("json_parser_load_from_data", 10.2e3, json_node_get_double (node));
}

static const struct {
	const gchar *str;
	JsonNodeType type;
	GType gtype;
	void (* verify_value) (JsonNode *node);
}test_base_values[] = {
	{ "null",       JSON_NODE_NULL,  G_TYPE_INVALID, NULL, },
	{ "42",         JSON_NODE_VALUE, G_TYPE_INT64,   verify_int_value },
	{ "true",       JSON_NODE_VALUE, G_TYPE_BOOLEAN, verify_boolean_value },
	{ "\"string\"", JSON_NODE_VALUE, G_TYPE_STRING,  verify_string_value },
	{ "10.2e3",     JSON_NODE_VALUE, G_TYPE_DOUBLE,  verify_double_value }
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
static guint n_test_unicode        = G_N_ELEMENTS (test_unicode);
static guint n_test_invalid        = G_N_ELEMENTS (test_invalid);


static void startup(void)
{
	g_type_init();
}

static void cleanup(void)
{
}

/**
 * @brief Positive test case of json_parser_load_from_data()
 */
static void utc_libjson_json_parser_load_from_data_func_01(void)
{
	JsonParser *parser = NULL;    
	GError *error = NULL;  
	gboolean ret = FALSE;

	parser = json_parser_new ();  
	g_assert (JSON_IS_PARSER (parser));

	ret = json_parser_load_from_data(NULL, NULL, -1, &error);
	dts_check_eq("json_parser_load_from_data", ret, FALSE);
	g_error_free (error);	   
	g_object_unref (parser);
}

/**
 * @brief Negative test case of ug_init json_parser_load_from_data()
 */
static void utc_libjson_json_parser_load_from_data_func_02(void)
{
	JsonParser *parser = NULL;    
	GError *error = NULL;  
	gboolean ret = FALSE;

	parser = json_parser_new ();  
	g_assert (JSON_IS_PARSER (parser));

	ret = json_parser_load_from_data(parser, NULL, -1, &error);
	dts_check_eq("json_parser_load_from_data", ret, FALSE);
	g_error_free (error);	   
	g_object_unref (parser);
}

static void utc_libjson_json_parser_load_from_data_func_03(void)
{
	JsonParser *parser = NULL;   
	GError *error = NULL;  
	gboolean ret = FALSE;
	JsonNode *root;

	parser = json_parser_new ();  
	g_assert (JSON_IS_PARSER (parser));

	ret = json_parser_load_from_data(parser, test_empty_string, -1, &error);
	dts_check_eq("json_parser_load_from_data", ret, TRUE);

	root = json_parser_get_root (parser);
	dts_check_eq("json_parser_load_from_data", root, NULL);

	g_error_free (error);	   
	g_object_unref (parser);
}

static void utc_libjson_json_parser_load_from_data_func_04(void)
{
	JsonParser *parser = NULL;  
	GError *error = NULL;  
	gboolean ret = FALSE;
	JsonNode *root = NULL;  
	int i = 0;

	parser = json_parser_new ();  
	g_assert (JSON_IS_PARSER (parser));

	for (i = 0; i < n_test_base_values; i++)
	{
		error = NULL;
		ret = json_parser_load_from_data (parser, test_base_values[i].str, -1, &error);
		dts_check_eq("json_parser_load_from_data", ret, TRUE);
		g_error_free (error);
		
		root = json_parser_get_root (parser);
		dts_check_ne("json_parser_load_from_data", root, NULL);

		dts_check_eq("json_parser_load_from_data", json_node_get_parent(root),  NULL);

		dts_check_eq("json_parser_load_from_data", JSON_NODE_TYPE (root), test_base_values[i].type);
		dts_check_eq("json_parser_load_from_data", json_node_get_value_type (root), test_base_values[i].gtype);

		if (test_base_values[i].verify_value)
			test_base_values[i].verify_value (root);
	}
	dts_pass("json_parser_load_from_data", "pass");
	g_object_unref (parser);

}

static void utc_libjson_json_parser_load_from_data_func_05(void)
{
	JsonParser *parser = NULL;    
	GError *error = NULL;  
	gboolean ret = FALSE;
	JsonNode *root = NULL;  
	JsonArray *array = NULL;  

	parser = json_parser_new ();  
	g_assert (JSON_IS_PARSER (parser));

	ret = json_parser_load_from_data(parser, test_empty_array_string, -1, &error);
	dts_check_eq("json_parser_load_from_data", ret, TRUE);

	root = json_parser_get_root (parser);
	dts_check_ne("json_parser_load_from_data", root, NULL);

	dts_check_eq("json_parser_load_from_data", JSON_NODE_TYPE (root), JSON_NODE_ARRAY);
	dts_check_eq("json_parser_load_from_data", json_node_get_parent (root), NULL);

	array = json_node_get_array (root);
	dts_check_ne("json_parser_load_from_data", array, NULL);

	dts_check_eq("json_parser_load_from_data", json_array_get_length (array), 0);

	g_error_free(error);	   
	g_object_unref(parser);
}


static void utc_libjson_json_parser_load_from_data_func_06(void)
{
	JsonParser *parser = NULL;    
	GError *error = NULL;  
	gboolean ret = FALSE;
	JsonNode *root = NULL; 
	JsonNode *node = NULL; 
	JsonArray *array = NULL;  
	int i = 0;

	parser = json_parser_new ();  
	g_assert (JSON_IS_PARSER (parser));

	for (i = 0; i < n_test_simple_arrays; i++)
	{
		error = NULL;  
		ret = json_parser_load_from_data (parser, test_simple_arrays[i].str, -1, &error);
		dts_check_eq("json_parser_load_from_data", ret, TRUE);
		g_error_free (error);

		root = json_parser_get_root (parser);
		dts_check_ne("json_parser_load_from_data", root, NULL);

		dts_check_eq("json_parser_load_from_data", JSON_NODE_TYPE(root), JSON_NODE_ARRAY);
		dts_check_eq("json_parser_load_from_data", json_node_get_parent(root), NULL);

		array = json_node_get_array (root);
		dts_check_ne("json_parser_load_from_data", array, NULL);

		dts_check_eq("json_parser_load_from_data", json_array_get_length(array), test_simple_arrays[i].len);

		node = json_array_get_element (array, test_simple_arrays[i].element);
		dts_check_ne("json_parser_load_from_data", node, NULL);
		dts_check_eq("json_parser_load_from_data", json_node_get_parent(node), root);
		dts_check_eq("json_parser_load_from_data", JSON_NODE_TYPE(node), test_simple_arrays[i].type);
		dts_check_eq("json_parser_load_from_data", json_node_get_value_type (node), test_simple_arrays[i].gtype);
	}
	dts_pass("json_parser_load_from_data", "pass");
	g_object_unref (parser);
}


static void utc_libjson_json_parser_load_from_data_func_07(void)
{
	JsonParser *parser = NULL;    
	GError *error = NULL;  
	gboolean ret = FALSE;
	JsonNode *root = NULL; 
	JsonArray *array = NULL; 
	int i = 0;

	parser = json_parser_new ();  
	g_assert (JSON_IS_PARSER (parser));

	for (i = 0; i < n_test_nested_arrays; i++)
	{
		error = NULL; 
		ret = json_parser_load_from_data (parser, test_nested_arrays[i], -1, &error);
		dts_check_eq("json_parser_load_from_data", ret, TRUE);
		g_error_free (error);

		root = json_parser_get_root (parser);
		dts_check_ne("json_parser_load_from_data", root, NULL);

		dts_check_eq("json_parser_load_from_data", JSON_NODE_TYPE(root), JSON_NODE_ARRAY);
		dts_check_eq("json_parser_load_from_data", json_node_get_parent(root), NULL);

		array = json_node_get_array (root);
		dts_check_ne("json_parser_load_from_data", array, NULL);

		dts_check_gt("json_parser_load_from_data", json_array_get_length(array), 0);
	}
	dts_pass("json_parser_load_from_data", "pass");
	g_object_unref (parser);
}

static void utc_libjson_json_parser_load_from_data_func_08(void)
{
	JsonParser *parser = NULL;
	GError *error = NULL;
	gboolean ret = FALSE;
	JsonNode *root = NULL;  
	JsonObject *object = NULL;  

	parser = json_parser_new ();  
	g_assert (JSON_IS_PARSER (parser));

	ret = json_parser_load_from_data(parser, test_empty_object_string, -1, &error);
	dts_check_eq("json_parser_load_from_data", ret, TRUE);

	root = json_parser_get_root (parser);
	dts_check_ne("json_parser_load_from_data", root, NULL);

	dts_check_eq("json_parser_load_from_data", json_node_get_parent(root), NULL);
	dts_check_eq("json_parser_load_from_data", JSON_NODE_TYPE(root), JSON_NODE_OBJECT);
	dts_check_eq("json_parser_load_from_data", json_node_get_parent(root), NULL);

	object = json_node_get_object (root);
	dts_check_ne("json_parser_load_from_data", object, NULL);

	dts_check_eq("json_parser_load_from_data", json_object_get_size(object), 0);

	g_error_free (error);	   
	g_object_unref (parser);
}



static void utc_libjson_json_parser_load_from_data_func_09(void)
{
	JsonParser *parser = NULL;
	GError *error = NULL;
	gboolean ret = FALSE;
	JsonNode *root = NULL;  
	JsonNode *node = NULL;  
	JsonObject *object = NULL;  
	int i = 0;

	parser = json_parser_new ();  
	g_assert (JSON_IS_PARSER (parser));

	for (i = 0; i < n_test_simple_objects; i++)
	{
		error = NULL;  
		ret = json_parser_load_from_data (parser, test_simple_objects[i].str, -1, &error);
		dts_check_eq("json_parser_load_from_data", ret, TRUE);
		g_error_free (error);

		root = json_parser_get_root (parser);
		dts_check_ne("json_parser_load_from_data", root, NULL);

		dts_check_eq("json_parser_load_from_data", JSON_NODE_TYPE(root), JSON_NODE_OBJECT);
		dts_check_eq("json_parser_load_from_data", json_node_get_parent(root), NULL);

		object = json_node_get_object (root);
		dts_check_ne("json_parser_load_from_data",object, NULL);
		dts_check_eq("json_parser_load_from_data", json_object_get_size(object), test_simple_objects[i].size);

		node = json_object_get_member (object, test_simple_objects[i].member);
		dts_check_ne("json_parser_load_from_data", node, NULL);
		dts_check_eq("json_parser_load_from_data", json_node_get_parent(node), root);
		dts_check_eq("json_parser_load_from_data", JSON_NODE_TYPE(node), test_simple_objects[i].type);
		dts_check_eq("json_parser_load_from_data", json_node_get_value_type(node), test_simple_objects[i].gtype);
	}
	dts_pass("json_parser_load_from_data", "pass");
	g_object_unref (parser);

}

static void utc_libjson_json_parser_load_from_data_func_10(void)
{
	JsonParser *parser = NULL;    
	GError *error = NULL;  
	gboolean ret = FALSE;
	JsonNode *root = NULL;  
	JsonObject *object = NULL;  
	int i = 0;

	parser = json_parser_new ();  
	g_assert (JSON_IS_PARSER (parser));

	for (i = 0; i < n_test_nested_objects; i++)
	{
		error = NULL;  
		ret = json_parser_load_from_data (parser, test_nested_objects[i], -1, &error);
		dts_check_eq("json_parser_load_from_data", ret, TRUE);
		g_error_free (error);

		root = json_parser_get_root (parser);
		dts_check_ne("json_parser_load_from_data", root, NULL);

		dts_check_eq("json_parser_load_from_data", JSON_NODE_TYPE(root), JSON_NODE_OBJECT);
		dts_check_eq("json_parser_load_from_data", json_node_get_parent(root),  NULL);

		object = json_node_get_object (root);
		dts_check_ne("json_parser_load_from_data", object, NULL);
		dts_check_gt("json_parser_load_from_data", json_object_get_size(object), 0);
	}
	dts_pass("json_parser_load_from_data", "pass");
	g_object_unref (parser);

}


static void utc_libjson_json_parser_load_from_data_func_11(void)
{
	JsonParser *parser = NULL;    
	GError *error = NULL;  
	gboolean ret = FALSE;
	JsonNode *root = NULL;  
	JsonNode *node = NULL; 
    JsonObject *object = NULL; 
	int i = 0;

	parser = json_parser_new ();  
	g_assert (JSON_IS_PARSER (parser));

	for (i = 0; i < n_test_unicode; i++)
	{
		error = NULL;  
		ret = json_parser_load_from_data (parser, test_unicode[i].str, -1, &error);
		dts_check_eq("json_parser_load_from_data", ret, TRUE);
		g_error_free (error);

		root = json_parser_get_root (parser);
		dts_check_ne("json_parser_load_from_data", root, NULL);

		dts_check_eq("json_parser_load_from_data", JSON_NODE_TYPE (root), JSON_NODE_OBJECT);

        object = json_node_get_object (root);
		dts_check_ne("json_parser_load_from_data", object, NULL);
		dts_check_gt("json_parser_load_from_data", json_object_get_size (object), 0);

		node = json_object_get_member(object, test_unicode[i].member);
		dts_check_eq("json_parser_load_from_data", JSON_NODE_TYPE(node), JSON_NODE_VALUE);
		dts_check_str_eq("json_parser_load_from_data", json_node_get_string (node), test_unicode[i].match);
		dts_check_eq("json_parser_load_from_data", g_utf8_validate(json_node_get_string (node), -1, NULL), TRUE);

	}
	dts_pass("json_parser_load_from_data", "pass");
	g_object_unref (parser);
}


static void utc_libjson_json_parser_load_from_data_func_12(void)
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

