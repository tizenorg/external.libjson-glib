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

static void utc_libjson_json_array_func_01(void);
static void utc_libjson_json_array_func_02(void);
static void utc_libjson_json_array_func_03(void);
static void utc_libjson_json_array_func_04(void);
static void utc_libjson_json_array_func_05(void);
static void utc_libjson_json_array_func_06(void);


enum {
	POSITIVE_TC_IDX = 0x01,
	NEGATIVE_TC_IDX,
};

struct tet_testlist tet_testlist[] = {
	{ utc_libjson_json_array_func_01, POSITIVE_TC_IDX },
	{ utc_libjson_json_array_func_02, POSITIVE_TC_IDX },
	{ utc_libjson_json_array_func_03, POSITIVE_TC_IDX },
	{ utc_libjson_json_array_func_04, POSITIVE_TC_IDX },
	{ utc_libjson_json_array_func_05, POSITIVE_TC_IDX },
	{ utc_libjson_json_array_func_06, POSITIVE_TC_IDX },
	{ NULL, 0 }
};


typedef struct _TestForeachFixture
{
	gint n_elements;
}TestForeachFixture;

static const struct {
	JsonNodeType element_type;
	GType element_gtype;
}type_verify[] = {
	{ JSON_NODE_VALUE, G_TYPE_INT64 },
	{ JSON_NODE_VALUE, G_TYPE_BOOLEAN },
	{ JSON_NODE_VALUE, G_TYPE_STRING },
	{ JSON_NODE_NULL, G_TYPE_INVALID }
};


static void startup(void)
{  
	g_type_init();
}

static void cleanup(void)
{
}

/**
 * @brief Positive test case of json_array()
 */
static void utc_libjson_json_array_func_01(void)
{
	JsonArray *array = json_array_new ();

	dts_check_eq("json_array_empty", json_array_get_length (array), 0);
	dts_check_eq("json_array_empty", json_array_get_elements (array), NULL);

	json_array_unref (array);
}

/**
 * @brief Negative test case of ug_init json_array()
 */
static void utc_libjson_json_array_func_02(void)
{
	JsonArray *array = json_array_new ();
	JsonNode *node = json_node_new (JSON_NODE_NULL);

	dts_check_eq("json_array_add", json_array_get_length (array), 0);

	json_array_add_element (array, node);
	dts_check_eq("json_array_add", json_array_get_length (array), 1);

	node = json_array_get_element (array, 0);
	dts_check_eq("json_array_add", JSON_NODE_TYPE (node), JSON_NODE_NULL);

	json_array_unref (array);
}

static void utc_libjson_json_array_func_03(void)
{
	JsonArray *array = json_array_new ();
	JsonNode *node = json_node_new (JSON_NODE_NULL);

	json_array_add_element (array, node);	
	json_array_remove_element (array, 0);
	dts_check_eq("json_array_remove", json_array_get_length (array), 0);

	json_array_unref (array);
}

static void
verify_foreach (JsonArray *array,
                guint      index_,
                JsonNode  *element_node,
                gpointer   user_data)
{
	TestForeachFixture *fixture = user_data;

	dts_check_eq("json_array_foreach_element", json_node_get_node_type(element_node), type_verify[index_].element_type);
	dts_check_eq("json_array_foreach_element", json_node_get_value_type(element_node), type_verify[index_].element_gtype);

	fixture->n_elements += 1;
}


static void utc_libjson_json_array_func_04(void)
{
	JsonArray *array = json_array_new ();
	TestForeachFixture fixture = { 0, };

	json_array_add_int_element (array, 42);
	json_array_add_boolean_element (array, TRUE);
	json_array_add_string_element (array, "hello");
	json_array_add_null_element (array);

	json_array_foreach_element (array, verify_foreach, &fixture);

	dts_check_eq("json_array_foreach_element", fixture.n_elements, json_array_get_length (array));

	json_array_unref (array);
}

static void utc_libjson_json_array_func_05(void)
{
	JsonArray *array = json_array_sized_new (3);
	JsonArray *ret_array = NULL;

	ret_array = json_array_ref(array);
	dts_check_eq ("json_object_ref", ret_array, array);

	json_array_unref (array); // for ref
	json_array_unref (array); // for new
}


static void utc_libjson_json_array_func_06(void)
{
	JsonArray* array = NULL;
	JsonArray* array_member = NULL;
	JsonObject * object_member = NULL;
	JsonArray* ret_array = NULL;
	JsonObject * ret_object = NULL;
	JsonNode  * dup_node = NULL;

	array = json_array_new ();

	json_array_add_int_element (array, 42);
	json_array_add_double_element(array, 25.6);
	json_array_add_boolean_element(array, TRUE);
	json_array_add_string_element(array, "foo");
	json_array_add_null_element(array);

	array_member = json_array_new();
	json_array_add_int_element(array_member, 12);
	json_array_add_array_element(array, array_member);

	object_member = json_object_new();
	json_object_set_int_member(object_member, "foo", 89);
	json_array_add_object_element(array, object_member);

	dts_check_eq ("json_array_int_member_get_set", json_array_get_int_element (array, 0), 42);
	dts_check_eq ("json_array_double_member_get_set", json_array_get_double_element (array, 1), 25.6);
	dts_check_eq ("json_array_boolean_member_get_set", json_array_get_boolean_element (array, 2), TRUE);
	dts_check_str_eq ("json_array_string_member_get_set", json_array_get_string_element (array, 3),"foo");
	dts_check_eq ("json_array_null_member_get_set", json_array_get_null_element (array, 4), TRUE);

	ret_array = json_array_get_array_element (array, 5);
	dts_check_eq ("json_array_array_member_get_set", json_array_get_int_element(ret_array, 0), 12);

	ret_object = json_array_get_object_element (array,6);
	dts_check_eq ("json_array_object_member_get_set", json_object_get_int_member (ret_object, "foo"), 89);

	dup_node = json_array_dup_element(array, 0);
	dts_check_eq ("json_array_dup_element", json_node_get_int(dup_node), 42);

	json_object_unref(object_member);
	json_array_unref(array_member);
	json_array_unref(array);
}



