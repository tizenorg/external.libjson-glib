#include <tet_api.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glib.h>

#include <json-glib/json-glib.h>

static void startup(void);
static void cleanup(void);

void (*tet_startup)(void) = startup;
void (*tet_cleanup)(void) = cleanup;

static void utc_libjson_json_generator_to_data_func_01(void);
static void utc_libjson_json_generator_to_data_func_02(void);
static void utc_libjson_json_generator_to_data_func_03(void);
static void utc_libjson_json_generator_to_data_func_04(void);
static void utc_libjson_json_generator_to_data_func_05(void);
static void utc_libjson_json_generator_to_data_func_06(void);
static void utc_libjson_json_generator_to_data_func_07(void);


enum {
	POSITIVE_TC_IDX = 0x01,
	NEGATIVE_TC_IDX,
};

struct tet_testlist tet_testlist[] = {
	{ utc_libjson_json_generator_to_data_func_01, POSITIVE_TC_IDX },
	{ utc_libjson_json_generator_to_data_func_02, POSITIVE_TC_IDX },
	{ utc_libjson_json_generator_to_data_func_03, POSITIVE_TC_IDX },
	{ utc_libjson_json_generator_to_data_func_04, POSITIVE_TC_IDX },
	{ utc_libjson_json_generator_to_data_func_05, POSITIVE_TC_IDX },
	{ utc_libjson_json_generator_to_data_func_06, POSITIVE_TC_IDX },
	{ utc_libjson_json_generator_to_data_func_07, POSITIVE_TC_IDX },
	{ NULL, 0 }
};

static const gchar *empty_array  = "[ ]";
static const gchar *empty_object = "{ }";

static const gchar *simple_array = "[ true, false, null, 42, \"foo\" ]"; 
static const gchar *nested_array = "[ true, [ false, null ], 42 ]";

static const gchar *simple_object = "{ \"Bool1\" : true, \"Bool2\" : false, \"Null\" : null, \"Int\" : 42, \"String\" : \"foo\" }";


static void startup(void)
{
	g_type_init();
}

static void cleanup(void)
{
}


/**
 * @brief Positive test case of json_generator_to_data()
 */
static void utc_libjson_json_generator_to_data_func_01(void)
{
	gchar *data = NULL;
	gsize len = 0;

	data = json_generator_to_data (NULL, &len);
	dts_check_eq("json_generator_to_data", len, 0);
	dts_check_eq("json_generator_to_data", data, NULL);
}


/**
 * @brief Positive test case of json_generator_to_data()
 */
static void utc_libjson_json_generator_to_data_func_02(void)
{
	JsonGenerator *gen = NULL;
	gchar *data = NULL;
	gsize len = 0;

	gen = json_generator_new();
	dts_check_ne("json_generator_to_data", gen, NULL);

	data = json_generator_to_data (gen, &len);
	dts_check_eq("json_generator_to_data", len, 0);
	dts_check_eq("json_generator_to_data", data, NULL);

	g_object_unref (gen);
}

static void utc_libjson_json_generator_to_data_func_03(void)
{
	JsonGenerator *gen = NULL;
	JsonNode *root = NULL;
	gchar *data = NULL;
	gsize len = 0;

	gen = json_generator_new();
	dts_check_ne("json_generator_to_data", gen, NULL);

	root = json_node_new (JSON_NODE_ARRAY);
	json_node_take_array (root, json_array_new ());

	json_generator_set_root (gen, root);
	g_object_set (gen, "pretty", FALSE, NULL);

	data = json_generator_to_data (gen, &len);
	dts_check_eq("json_generator_to_data", len, strlen(empty_array));
	dts_check_str_eq("json_generator_to_data", data, empty_array);

	g_free (data);
	json_node_free (root);
	g_object_unref (gen);

}

static void utc_libjson_json_generator_to_data_func_04(void)
{
	JsonGenerator *gen = NULL;
	JsonNode *root = NULL;
	gchar *data = NULL;
	gsize len = 0;

	gen = json_generator_new();
	dts_check_ne("json_generator_to_data", gen, NULL);

	root = json_node_new (JSON_NODE_OBJECT);
	json_node_take_object (root, json_object_new ());
	
	json_generator_set_root (gen, root);
	g_object_set (gen, "pretty", FALSE, NULL);

	data = json_generator_to_data (gen, &len);
	dts_check_eq("json_generator_to_data", len, strlen(empty_object));
	dts_check_str_eq("json_generator_to_data", data, empty_object);

	g_free (data);
	json_node_free (root);
	g_object_unref (gen);
}

static void utc_libjson_json_generator_to_data_func_05(void)
{
	JsonGenerator *gen = NULL;
	JsonNode *root = NULL;
	JsonNode *val = NULL;
	JsonArray *array = NULL;
	GValue value = { 0, };
	gchar *data = NULL;
	gsize len = 0;

	gen = json_generator_new();
	dts_check_ne("json_generator_to_data", gen, NULL);

	root = json_node_new (JSON_NODE_ARRAY);
	array = json_array_sized_new (6);

	val = json_node_new (JSON_NODE_VALUE);
	g_value_init (&value, G_TYPE_BOOLEAN);
	g_value_set_boolean (&value, TRUE);
	json_node_set_value (val, &value);
	json_array_add_element (array, val);
	g_value_unset (&value);

	val = json_node_new (JSON_NODE_VALUE);
	json_node_set_boolean (val, FALSE);
	json_array_add_element (array, val);

	val = json_node_new (JSON_NODE_NULL);
	json_array_add_element (array, val);

	json_array_add_int_element (array, 42);

	val = json_node_new (JSON_NODE_VALUE);
	g_value_init (&value, G_TYPE_STRING);
	g_value_set_string (&value, "foo");
	json_node_set_value (val, &value);
	json_array_add_element (array, val);
	g_value_unset (&value);

	json_node_take_array (root, array);
	json_generator_set_root (gen, root);
	g_object_set (gen, "pretty", FALSE, NULL);

	data = json_generator_to_data (gen, &len);
	dts_check_eq("json_generator_to_data", len, strlen(simple_array));
	dts_check_str_eq("json_generator_to_data", data, simple_array);

	g_free (data);
	json_node_free (root);
	g_object_unref (gen);
}

static void utc_libjson_json_generator_to_data_func_06(void)
{
	JsonGenerator *gen = NULL;
	JsonNode *root = NULL;
	JsonNode *val = NULL;
	JsonArray *array = NULL;
	JsonArray *nested = NULL;
	GValue value = { 0, };
	gchar *data = NULL;
	gsize len = 0;

	gen = json_generator_new();
	dts_check_ne("json_generator_to_data", gen, NULL);

	root = json_node_new (JSON_NODE_ARRAY);
	array = json_array_sized_new (3);

	val = json_node_new (JSON_NODE_VALUE);
	g_value_init (&value, G_TYPE_BOOLEAN);
	g_value_set_boolean (&value, TRUE);
	json_node_set_value (val, &value);
	json_array_add_element (array, val);
	g_value_unset (&value);

	nested = json_array_new ();
	json_array_add_boolean_element (nested, FALSE);
	json_array_add_null_element (nested);
	json_array_add_array_element (array, nested);

	val = json_node_new (JSON_NODE_VALUE);
	g_value_init (&value, G_TYPE_INT64);
	g_value_set_int64 (&value, 42);
	json_node_set_value (val, &value);
	json_array_add_element (array, val);
	g_value_unset (&value);

	json_node_take_array (root, array);
	json_generator_set_root (gen, root);
	g_object_set (gen, "pretty", FALSE, NULL);

	data = json_generator_to_data (gen, &len);
	dts_check_eq("json_generator_to_data", len, strlen(nested_array));
	dts_check_str_eq("json_generator_to_data", data, nested_array);

	g_free (data);
	json_node_free (root);
	g_object_unref (gen);
}

static void utc_libjson_json_generator_to_data_func_07(void)
{
	JsonGenerator *gen = NULL;
	JsonNode *root = NULL;
	JsonObject *object = NULL;
	gchar *data = NULL;
	gsize len = 0;

	gen = json_generator_new();
	dts_check_ne("json_generator_to_data", gen, NULL);

	root = json_node_new (JSON_NODE_OBJECT);
	object = json_object_new ();

	json_object_set_boolean_member (object, "Bool1", TRUE);
	json_object_set_boolean_member (object, "Bool2", FALSE);
	json_object_set_null_member (object, "Null");
	json_object_set_int_member (object, "Int", 42);
	json_object_set_string_member (object, "String", "foo");

	json_node_take_object (root, object);
	json_generator_set_root (gen, root);
	g_object_set (gen, "pretty", FALSE, NULL);

	data = json_generator_to_data (gen, &len);
	dts_check_eq("json_generator_to_data", len, strlen(simple_object));
	dts_check_str_eq("json_generator_to_data", data, simple_object);

	g_free (data);
	json_node_free (root);
	g_object_unref (gen);
}


