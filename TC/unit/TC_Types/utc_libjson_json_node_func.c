#include <tet_api.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glib.h>
#include <glib/gstdio.h>

#include <json-glib/json-glib.h>

static void startup(void);
static void cleanup(void);

void (*tet_startup)(void) = startup;
void (*tet_cleanup)(void) = cleanup;

static void utc_libjson_json_node_func_01(void);
static void utc_libjson_json_node_func_02(void);
static void utc_libjson_json_node_func_03(void);
static void utc_libjson_json_node_func_04(void);
static void utc_libjson_json_node_func_05(void);
static void utc_libjson_json_node_func_06(void);
static void utc_libjson_json_node_func_07(void);
static void utc_libjson_json_node_func_08(void);
static void utc_libjson_json_node_func_09(void);
static void utc_libjson_json_node_func_10(void);
static void utc_libjson_json_node_func_11(void);


/*
#define TEST_TYPE_BOXED                 (test_boxed_get_type ())
#define TEST_TYPE_OBJECT                (test_object_get_type ())
#define TEST_OBJECT(obj)                (G_TYPE_CHECK_INSTANCE_CAST ((obj), TEST_TYPE_OBJECT, TestObject))
#define TEST_IS_OBJECT(obj)             (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TEST_TYPE_OBJECT))

*/
#define JSON_TYPE_NODE                 (json_node_get_type ())

#define TEST_IS_JSONNODE(obj)             (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JSON_TYPE_NODE))

GType json_node_get_type (void);

enum {
	POSITIVE_TC_IDX = 0x01,
	NEGATIVE_TC_IDX,
};

struct tet_testlist tet_testlist[] = {
	{ utc_libjson_json_node_func_01, POSITIVE_TC_IDX },
	{ utc_libjson_json_node_func_02, POSITIVE_TC_IDX },
	{ utc_libjson_json_node_func_03, POSITIVE_TC_IDX },
	{ utc_libjson_json_node_func_04, POSITIVE_TC_IDX },
	{ utc_libjson_json_node_func_05, POSITIVE_TC_IDX },
	{ utc_libjson_json_node_func_06, POSITIVE_TC_IDX },
	{ utc_libjson_json_node_func_07, POSITIVE_TC_IDX },
	{ utc_libjson_json_node_func_08, POSITIVE_TC_IDX },
	{ utc_libjson_json_node_func_09, POSITIVE_TC_IDX },
	{ utc_libjson_json_node_func_10, POSITIVE_TC_IDX },
	{ utc_libjson_json_node_func_11, POSITIVE_TC_IDX },
	{ NULL, 0 }
};

static void startup(void)
{
	g_type_init();
}

static void cleanup(void)
{
}

/**
 * @brief Positive test case of json_node()
 */
static void utc_libjson_json_node_func_01(void)
{
	JsonNode *node = json_node_new (JSON_NODE_NULL);
	JsonNode *copy = json_node_copy (node);

	dts_check_eq ("json_node_copy_null", json_node_get_node_type (node), json_node_get_node_type (copy));
	dts_check_eq ("json_node_copy_null", json_node_get_value_type (node), json_node_get_value_type (copy));
	dts_check_eq ("json_node_copy_null", json_node_type_name (node), json_node_type_name (copy));

	json_node_free (copy);
	json_node_free (node);
}

/**
 * @brief Negative test case of ug_init json_node()
 */
static void utc_libjson_json_node_func_02(void)
{
	JsonNode *node = json_node_new (JSON_NODE_VALUE);
	JsonNode *copy;
	
	json_node_set_string (node, "hello");
	
	copy = json_node_copy (node);
	dts_check_eq ("json_node_copy_value", json_node_get_node_type (node), json_node_get_node_type (copy));
	dts_check_str_eq ("json_node_copy_value", json_node_type_name (node), json_node_type_name (copy));
	dts_check_str_eq ("json_node_copy_value", json_node_get_string (node), json_node_get_string (copy));
	
	json_node_free (copy);
	json_node_free (node);
}

static void utc_libjson_json_node_func_03(void)
{
	JsonObject *obj = json_object_new ();
	JsonNode *node = json_node_new (JSON_NODE_OBJECT);
	JsonNode *value = json_node_new (JSON_NODE_VALUE);
	JsonNode *copy;
	
	json_node_set_int (value, 42);
	json_object_set_member (obj, "answer", value);
	
	json_node_take_object (node, obj);
	
	copy = json_node_copy (node);
	
	dts_check_eq ("json_node_copy_object", json_node_get_node_type (node), json_node_get_node_type (copy));
	dts_check_eq ("json_node_copy_object", json_node_get_object (node), json_node_get_object (copy));
	
	json_node_free (copy);
	json_node_free (node);

}


static void utc_libjson_json_node_func_04(void)
{
	JsonNode *node = json_node_new (JSON_NODE_NULL);
	
	g_assert (JSON_NODE_HOLDS_NULL (node));
	dts_check_eq ("json_node_null", json_node_get_value_type (node), G_TYPE_INVALID);
	dts_check_str_eq ("json_node_null", json_node_type_name (node), "NULL");
	
	json_node_free (node);
}

static void utc_libjson_json_node_func_05(void)
{
	JsonNode *node = json_node_new (JSON_NODE_VALUE);
	GValue value = { 0, };
	GValue check = { 0, };

	dts_check_eq ("json_node_value", JSON_NODE_TYPE (node), JSON_NODE_VALUE);

	g_value_init (&value, G_TYPE_INT64);
	g_value_set_int64 (&value, 42);

	dts_check_eq ("json_node_value", G_VALUE_TYPE (&value), G_TYPE_INT64);
	dts_check_eq ("json_node_value", g_value_get_int64 (&value), 42);

	json_node_set_value (node, &value);
	json_node_get_value (node, &check);

	dts_check_eq ("json_node_value", G_VALUE_TYPE (&value), G_VALUE_TYPE (&check));
	dts_check_eq ("json_node_value", g_value_get_int64 (&value), g_value_get_int64 (&check));
	dts_check_eq ("json_node_value", G_VALUE_TYPE (&check), G_TYPE_INT64);
	dts_check_eq ("json_node_value", g_value_get_int64 (&check), 42);

	g_value_unset (&value);
	g_value_unset (&check);
	json_node_free (node);

}

static void utc_libjson_json_node_func_06(void)
{
	JsonNode *root = json_node_new (JSON_NODE_OBJECT);
	JsonNode *node = json_node_new (JSON_NODE_OBJECT);
	JsonObject * object = NULL;
	JsonObject * ret_object = NULL;

	json_node_set_parent(node, root);
	dts_check_eq("json_node_object", json_node_get_parent(node), root);

	object = json_object_new();
	json_object_set_int_member(object, "foo", 12);

	json_node_set_object(node, object);
	ret_object = json_node_get_object(node);
	dts_check_ne("json_node_object", ret_object, NULL);
	dts_check_eq("json_node_object", json_object_get_int_member(ret_object, "foo"), 12);

	json_node_dup_object(node);
	dts_check_eq("json_node_object", json_object_get_int_member(ret_object, "foo"), 12);

	json_object_unref(object); //for json_node_set_object
	json_object_unref(object); //for json_node_dup_object

	json_node_free(node);
	json_node_free(root);
}

static void utc_libjson_json_node_func_07(void)
{
	JsonNode *node = json_node_new (JSON_NODE_ARRAY);
	JsonArray* array = NULL;
	JsonArray * ret_array = NULL;

	array = json_array_new();
	json_array_add_int_element(array, 12);

	json_node_set_array(node, array);
	ret_array = json_node_get_array(node);
	dts_check_ne("json_node_array", ret_array, NULL);
	dts_check_eq("json_node_array", json_array_get_int_element(ret_array, 0), 12);

	json_node_take_array(node, array);
	ret_array = json_node_get_array(node);
	dts_check_eq("json_node_array", json_array_get_int_element(ret_array, 0), 12);

	json_node_dup_array(node);
	ret_array = json_node_get_array(node);
	dts_check_eq("json_node_array", json_array_get_int_element(ret_array, 0), 12);

	json_array_unref(array); //for json_node_set_array
	json_array_unref(array); //for json_node_dup_array

	json_node_free(node);
}

static void utc_libjson_json_node_func_08(void)
{
	JsonNode *node = json_node_new (JSON_NODE_VALUE);
	char* value = NULL;

	json_node_set_string(node, "foo");
	dts_check_str_eq("json_node_string", json_node_get_string(node), "foo");

	value = json_node_dup_string(node);
	dts_check_str_eq("json_node_string", json_node_get_string(node), "foo");

	json_node_free(node);
}

static void utc_libjson_json_node_func_09(void)
{
	JsonNode *node = json_node_new (JSON_NODE_VALUE);

	json_node_set_double(node, 2.3);
	dts_check_eq("json_node_double", json_node_get_double(node), 2.3);

	json_node_free(node);
}

static void utc_libjson_json_node_func_10(void)
{
	JsonNode *node = json_node_new (JSON_NODE_VALUE);

	json_node_set_boolean(node, TRUE);
	dts_check_eq("json_node_boolean", json_node_get_boolean(node), TRUE);

	json_node_free(node);
}


static void utc_libjson_json_node_func_11(void)
{
	JsonNode *node = NULL;

	//GObject *obj = NULL;
	//obj = g_object_new(JSON_TYPE_NODE, "bar", TRUE, NULL);
	//dts_check_eq("json_node_get_type", TEST_IS_JSONNODE(obj),  TRUE);
	//g_object_unref (obj);

	dts_check_eq("json_node_is_null", json_node_is_null(node), TRUE);

	node = json_node_new (JSON_NODE_OBJECT);
	dts_check_eq("json_node_is_null", json_node_is_null(node), FALSE);

	json_node_free(node);
}


