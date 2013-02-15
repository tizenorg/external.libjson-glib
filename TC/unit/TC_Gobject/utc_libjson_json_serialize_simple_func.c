#include <tet_api.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glib-object.h>

#include <json-glib/json-glib.h>


static void startup(void);
static void cleanup(void);

void (*tet_startup)(void) = startup;
void (*tet_cleanup)(void) = cleanup;

static void utc_libjson_json_serialize_simple_func_01(void);

enum {
	POSITIVE_TC_IDX = 0x01,
	NEGATIVE_TC_IDX,
};

struct tet_testlist tet_testlist[] = {
	{ utc_libjson_json_serialize_simple_func_01, POSITIVE_TC_IDX },
	{ NULL, 0 }
};

#define TEST_TYPE_OBJECT                (test_object_get_type ())
#define TEST_OBJECT(obj)                (G_TYPE_CHECK_INSTANCE_CAST ((obj), TEST_TYPE_OBJECT, TestObject))
#define TEST_IS_OBJECT(obj)             (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TEST_TYPE_OBJECT))
#define TEST_OBJECT_CLASS(klass)        (G_TYPE_CHECK_CLASS_CAST ((klass), TEST_TYPE_OBJECT, TestObjectClass))
#define TEST_IS_OBJECT_CLASS(klass)     (G_TYPE_CHECK_CLASS_TYPE ((klass), TEST_TYPE_OBJECT))
#define TEST_OBJECT_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS ((obj), TEST_TYPE_OBJECT, TestObjectClass))

typedef struct _TestObject              TestObject;
typedef struct _TestObjectClass         TestObjectClass;

struct _TestObject
{
	GObject parent_instance;

	gint foo;
	gboolean bar;
	gchar *baz;
};

struct _TestObjectClass
{
	GObjectClass parent_class;
};

GType test_object_get_type (void);


static void startup(void)
{
	g_type_init();
}

static void cleanup(void)
{
}


/*** implementation ***/

enum
{
	PROP_0,

	PROP_FOO,
	PROP_BAR,
	PROP_BAZ
};

G_DEFINE_TYPE (TestObject, test_object, G_TYPE_OBJECT);

static void
test_object_finalize (GObject *gobject)
{
	g_free (TEST_OBJECT (gobject)->baz);

	G_OBJECT_CLASS (test_object_parent_class)->finalize (gobject);
}

static void
test_object_set_property (GObject      *gobject,
							guint         prop_id,
							const GValue *value,
							GParamSpec   *pspec)
{
	switch (prop_id)
	{
		case PROP_FOO:
			TEST_OBJECT (gobject)->foo = g_value_get_int (value);
			break;
		case PROP_BAR:
			TEST_OBJECT (gobject)->bar = g_value_get_boolean (value);
			break;
		case PROP_BAZ:
			g_free (TEST_OBJECT (gobject)->baz);
			TEST_OBJECT (gobject)->baz = g_value_dup_string (value);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id, pspec);
	}
}

static void
test_object_get_property (GObject    *gobject,
							guint       prop_id,
							GValue     *value,
							GParamSpec *pspec)
{
	switch (prop_id)
	{
		case PROP_FOO:
			g_value_set_int (value, TEST_OBJECT (gobject)->foo);
			break;
		case PROP_BAR:
			g_value_set_boolean (value, TEST_OBJECT (gobject)->bar);
			break;
		case PROP_BAZ:
			g_value_set_string (value, TEST_OBJECT (gobject)->baz);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id, pspec);
	}
}

static void
test_object_class_init (TestObjectClass *klass)
{
	GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

	gobject_class->set_property = test_object_set_property;
	gobject_class->get_property = test_object_get_property;
	gobject_class->finalize = test_object_finalize;

	g_object_class_install_property (gobject_class,
									PROP_FOO,
									g_param_spec_int ("foo", "Foo", "Foo",
														0, G_MAXINT, 42,
														G_PARAM_READWRITE));
	g_object_class_install_property (gobject_class,
									PROP_BAR,
									g_param_spec_boolean ("bar", "Bar", "Bar",
														FALSE,
														G_PARAM_READWRITE));
	g_object_class_install_property (gobject_class,
									PROP_BAZ,
									g_param_spec_string ("baz", "Baz", "Baz",
														NULL,
														G_PARAM_READWRITE));
}

static void
test_object_init (TestObject *object)
{
	object->foo = 42;
	object->bar = FALSE;
	object->baz = g_strdup ("Test");
}

/**
 * @brief Positive test case of json_serialize_simple()
 */
static void utc_libjson_json_serialize_simple_func_01(void)
{
	TestObject *obj = g_object_new(TEST_TYPE_OBJECT, "bar", TRUE, NULL);
	gchar *data;
	gsize len;

	data = json_gobject_to_data(G_OBJECT(obj), &len);

	dts_check_ne("json_serialize_simple", data, NULL);
	dts_check_gt("json_serialize_simple", len, 0);
	dts_check_eq("json_serialize_simple", len, strlen(data));

	g_free (data);
	g_object_unref (obj);
}


