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

static void utc_libjson_json_generator_to_file_func_01(void);
static void utc_libjson_json_generator_to_file_func_02(void);
static void utc_libjson_json_generator_to_file_func_03(void);
static void utc_libjson_json_generator_to_file_func_04(void);

static const gchar *test_filename = "gen_test.info";

enum {
	POSITIVE_TC_IDX = 0x01,
	NEGATIVE_TC_IDX,
};

struct tet_testlist tet_testlist[] = {
	{ utc_libjson_json_generator_to_file_func_01, POSITIVE_TC_IDX },
	{ utc_libjson_json_generator_to_file_func_02, POSITIVE_TC_IDX },
	{ utc_libjson_json_generator_to_file_func_03, POSITIVE_TC_IDX },
	{ utc_libjson_json_generator_to_file_func_04, POSITIVE_TC_IDX },
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
 * @brief Positive test case of json_generator_to_file()
 */
static void utc_libjson_json_generator_to_file_func_01(void)
{
	GError *error = NULL;  
	gboolean ret = FALSE;

	ret = json_generator_to_file (NULL, NULL, &error);
	dts_check_eq("json_generator_to_data", ret, FALSE);
}


static void utc_libjson_json_generator_to_file_func_02(void)
{
	JsonGenerator *gen = NULL;
	GError *error = NULL;  
	gboolean ret = FALSE;

	gen = json_generator_new();
	dts_check_ne("json_generator_to_data", gen, NULL);

	ret = json_generator_to_file (gen, NULL, &error);
	dts_check_eq("json_generator_to_data", ret, FALSE);

	g_object_unref (gen);
}

static void utc_libjson_json_generator_to_file_func_03(void)
{
	JsonGenerator *gen = NULL;
	GError *error = NULL;  
	gboolean ret = FALSE;

	gen = json_generator_new();
	dts_check_ne("json_generator_to_data", gen, NULL);

	ret = json_generator_to_file (gen, "", &error);
	dts_check_eq("json_generator_to_data", ret, FALSE);

	g_object_unref (gen);
}

static void utc_libjson_json_generator_to_file_func_04(void)
{
	JsonGenerator *gen = NULL;
	GError *error = NULL;  
	gboolean ret = FALSE;

	gen = json_generator_new();
	dts_check_ne("json_generator_to_data", gen, NULL);

	g_remove(test_filename);
	ret = json_generator_to_file (gen, test_filename, &error);
	dts_check_eq("json_generator_to_data", ret, TRUE);

	ret = g_file_test(test_filename, G_FILE_TEST_EXISTS);
	dts_check_eq("json_generator_to_data", ret, TRUE);

	g_object_unref (gen);
}

