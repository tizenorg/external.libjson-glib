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

static void utc_libjson_json_object_func_01(void);
static void utc_libjson_json_object_func_02(void);
static void utc_libjson_json_object_func_03(void);
static void utc_libjson_json_object_func_04(void);
static void utc_libjson_json_object_func_05(void);
static void utc_libjson_json_object_func_06(void);
static void utc_libjson_json_object_func_07(void);


enum {
	POSITIVE_TC_IDX = 0x01,
	NEGATIVE_TC_IDX,
};

struct tet_testlist tet_testlist[] = {
	{ utc_libjson_json_object_func_01, POSITIVE_TC_IDX },
	{ utc_libjson_json_object_func_02, POSITIVE_TC_IDX },
	{ utc_libjson_json_object_func_03, POSITIVE_TC_IDX },
	{ utc_libjson_json_object_func_04, POSITIVE_TC_IDX },
	{ utc_libjson_json_object_func_05, POSITIVE_TC_IDX },
	{ utc_libjson_json_object_func_06, POSITIVE_TC_IDX },
	{ utc_libjson_json_object_func_07, POSITIVE_TC_IDX },
	{ NULL, 0 }
};

typedef struct _TestForeachFixture
{
	gint n_members;
}TestForeachFixture;

static const struct {
	const gchar *member_name;
	JsonNodeType member_type;
	GType member_gtype;
}type_verify[] = {
	{ "integer", JSON_NODE_VALUE, G_TYPE_INT64 },
	{ "boolean", JSON_NODE_VALUE, G_TYPE_BOOLEAN },
	{ "string", JSON_NODE_VALUE, G_TYPE_STRING },
	{ "null", JSON_NODE_NULL, G_TYPE_INVALID }
};

static void startup(void)
{
	g_type_init();
}

static void cleanup(void)
{
}

/**
 * @brief Positive test case of json_object()
 */
static void utc_libjson_json_object_func_01(void)
{
	JsonObject *object = json_object_new ();
	
	dts_check_eq ("json_object_empty", json_object_get_size (object), 0);
	dts_check_eq ("json_object_empty", json_object_get_members (object), NULL);
	
	json_object_unref (object);
}

/**
 * @brief Negative test case of ug_init json_object()
 */
static void utc_libjson_json_object_func_02(void)
{
	JsonObject *object = json_object_new ();
	JsonNode *node = json_node_new (JSON_NODE_NULL);
	JsonNode *dup_node = NULL;

	dts_check_eq ("json_object_add_member", json_object_get_size (object), 0);

	json_object_set_member (object, "Null", node);
	dts_check_eq ("json_object_add_member", json_object_get_size (object), 1);

	dup_node = json_object_dup_member(object, "Null");
	dts_check_eq ("json_object_add_member", JSON_NODE_TYPE (dup_node), JSON_NODE_NULL);

	node = json_object_get_member (object, "Null");
	dts_check_eq ("json_object_add_member", JSON_NODE_TYPE (node), JSON_NODE_NULL);

	json_object_unref (object);
}

static void utc_libjson_json_object_func_03(void)
{
	JsonObject *object = json_object_new ();
	JsonNode *node = json_node_new (JSON_NODE_NULL);

	json_object_set_member (object, "Null", node);

	json_object_remove_member (object, "Null");
	dts_check_eq ("json_object_remove_member", json_object_get_size (object), 0);

	json_object_unref (object);
}

static void
verify_foreach (JsonObject  *object,
                const gchar *member_name,
                JsonNode    *member_node,
                gpointer     user_data)
{
	TestForeachFixture *fixture = user_data;
	gint i = 0;

	for (i = 0; i < G_N_ELEMENTS (type_verify); i++)
	{
		if (strcmp (member_name, type_verify[i].member_name) == 0)
		{
			dts_check_eq ("json_object_foreach_member", json_node_get_node_type (member_node), type_verify[i].member_type);
			dts_check_eq ("json_object_foreach_member", json_node_get_value_type (member_node), type_verify[i].member_gtype);
			break;
		}
	}

	fixture->n_members += 1;
}

static void utc_libjson_json_object_func_04(void)
{
	JsonObject *object = json_object_new ();
	TestForeachFixture fixture = { 0, };

	json_object_set_int_member (object, "integer", 42);
	json_object_set_boolean_member (object, "boolean", TRUE);
	json_object_set_string_member (object, "string", "hello");
	json_object_set_null_member (object, "null");

	json_object_foreach_member (object, verify_foreach, &fixture);

	dts_check_eq ("json_object_foreach_member", fixture.n_members, json_object_get_size (object));

	json_object_unref (object);

}

static void utc_libjson_json_object_func_05(void)
{
	JsonObject *object = json_object_new ();

	json_object_set_string_member (object, "string", "");
	dts_check_eq ("json_object_empty_member", json_object_has_member (object, "string"), TRUE);
	dts_check_str_eq ("json_object_empty_member", json_object_get_string_member (object, "string"), "");

	json_object_unref (object);
}

static void utc_libjson_json_object_func_06(void)
{
	JsonObject *object = json_object_new ();
	JsonObject *ret_object = NULL;

	ret_object = json_object_ref(object);

	json_object_unref (object); // for ref
	json_object_unref (object); // for new

	dts_check_eq ("json_object_ref", ret_object, object);
}

static void utc_libjson_json_object_func_07(void)
{
	JsonObject *object = NULL;
	JsonArray* array = NULL;
	JsonObject * object_member = NULL;
	JsonArray* ret_array = NULL;
	JsonObject * ret_object_member = NULL;

	object = json_object_new ();

	dts_check_eq ("json_object_int_member_get_set", json_object_get_values(object), NULL);

	json_object_set_int_member (object, "Int", 42);
	json_object_set_double_member(object, "Double", 25.6);
	json_object_set_boolean_member (object, "Bool1", TRUE);
	json_object_set_string_member (object, "String", "foo");
	json_object_set_null_member (object, "Null");

	array = json_array_new();
	json_array_add_int_element(array, 12);
	json_object_set_array_member(object, "Array", array);

	object_member = json_object_new();
	json_object_set_int_member(object_member, "foo", 89);
	json_object_set_object_member(object, "Object", object_member);

	dts_check_eq ("json_object_int_member_get_set", json_object_get_int_member (object, "Int"), 42);
	dts_check_eq ("json_object_double_member_get_set", json_object_get_double_member (object, "Double"), 25.6);
	dts_check_eq ("json_object_boolean_member_get_set", json_object_get_boolean_member (object, "Bool1"), TRUE);
	dts_check_str_eq ("json_object_string_member_get_set", json_object_get_string_member (object, "String"),"foo");
	dts_check_eq ("json_object_null_member_get_set", json_object_get_null_member (object, "Null"), TRUE);

	ret_array = json_object_get_array_member (object, "Array");
	dts_check_eq ("json_object_null_member_get_set", json_array_get_int_element(ret_array, 0), 12);

	ret_object_member = json_object_get_object_member (object, "Object");
	dts_check_eq ("json_object_null_member_get_set", json_object_get_int_member (ret_object_member, "foo"), 89);

	dts_check_ne ("json_object_int_member_get_set", json_object_get_values(object), NULL);

	json_object_unref(object_member);
	json_array_unref(array);
	json_object_unref(object);
}



