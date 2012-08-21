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

static void utc_libjson_json_generator_set_root_func_01(void);

enum {
	POSITIVE_TC_IDX = 0x01,
	NEGATIVE_TC_IDX,
};

struct tet_testlist tet_testlist[] = {
	{ utc_libjson_json_generator_set_root_func_01, POSITIVE_TC_IDX },
	{ NULL, 0 }
};

static void startup(void)
{
	g_type_init ();
}

static void cleanup(void)
{
}


static void utc_libjson_json_generator_set_root_func_01(void)
{
	JsonGenerator *gen = NULL;
	JsonParser *parser = NULL;	  
	GError *error = NULL;  

	JsonNode *root = NULL;
	gchar *data = NULL;
	gsize len = 0;

	gen = json_generator_new();
	dts_check_ne("json_generator_set_root", gen, NULL);

	root = json_node_new (JSON_NODE_ARRAY);
	json_node_take_array (root, json_array_new ());

	json_generator_set_root (gen, root);
	g_object_set (gen, "pretty", FALSE, NULL);

	data = json_generator_to_data (gen, &len);
	json_node_free (root);
	g_object_unref (gen);

	parser = json_parser_new ();  
	g_assert(JSON_IS_PARSER (parser));
	if (!json_parser_load_from_data (parser, data, -1, &error))
	{
		g_error_free (error);
	}
	else
	{
		root = json_parser_get_root (parser);
		dts_check_ne("json_generator_set_root", root, NULL);
		dts_check_eq("json_generator_set_root", JSON_NODE_TYPE(root), JSON_NODE_ARRAY);
	}

	dts_pass("json_generator_set_root", "pass");

	g_free (data);
	g_object_unref (parser);

}


