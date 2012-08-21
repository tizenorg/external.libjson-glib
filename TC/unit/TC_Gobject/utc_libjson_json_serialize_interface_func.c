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

static void utc_libjson_json_serialize_full_func_01(void);
static void utc_libjson_json_serialize_full_func_02(void);
static void utc_libjson_json_serialize_full_func_03(void);
static void utc_libjson_json_serialize_full_func_04(void);

enum {
	POSITIVE_TC_IDX = 0x01,
	NEGATIVE_TC_IDX,
};

struct tet_testlist tet_testlist[] = {
	{ utc_libjson_json_serialize_full_func_01, POSITIVE_TC_IDX },
	{ utc_libjson_json_serialize_full_func_02, POSITIVE_TC_IDX },
	{ utc_libjson_json_serialize_full_func_03, POSITIVE_TC_IDX },
	{ utc_libjson_json_serialize_full_func_04, POSITIVE_TC_IDX },
	{ NULL, 0 }
};


#define TEST_TYPE_BOXED                 (test_boxed_get_type ())
#define TEST_TYPE_OBJECT                (test_object_get_type ())
#define TEST_OBJECT(obj)                (G_TYPE_CHECK_INSTANCE_CAST ((obj), TEST_TYPE_OBJECT, TestObject))
#define TEST_IS_OBJECT(obj)             (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TEST_TYPE_OBJECT))
#define TEST_OBJECT_CLASS(klass)        (G_TYPE_CHECK_CLASS_CAST ((klass), TEST_TYPE_OBJECT, TestObjectClass))
#define TEST_IS_OBJECT_CLASS(klass)     (G_TYPE_CHECK_CLASS_TYPE ((klass), TEST_TYPE_OBJECT))
#define TEST_OBJECT_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS ((obj), TEST_TYPE_OBJECT, TestObjectClass))

typedef struct _TestBoxed               TestBoxed;
typedef struct _TestObject              TestObject;
typedef struct _TestObjectClass         TestObjectClass;

struct _TestBoxed
{
	gint foo;
	gboolean bar;
};

struct _TestObject
{
	GObject parent_instance;

	gint foo;
	gboolean bar;
	gchar *baz;
	TestBoxed blah;
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

static TestBoxed *
test_boxed_copy (const TestBoxed *src)
{
	TestBoxed *copy = g_slice_new (TestBoxed);

	*copy = *src;

	return copy;
}

static void
test_boxed_free (TestBoxed *boxed)
{
	if (G_LIKELY (boxed))
	{
		g_slice_free (TestBoxed, boxed);
	}
}

GType
test_boxed_get_type (void)
{
	static GType b_type = 0;

	if (G_UNLIKELY (b_type == 0))
		b_type = g_boxed_type_register_static ("TestBoxed",
											(GBoxedCopyFunc) test_boxed_copy,
											(GBoxedFreeFunc) test_boxed_free);

	return b_type;
}

enum
{
	PROP_0,

	PROP_FOO,
	PROP_BAR,
	PROP_BAZ,
	PROP_BLAH
};

static JsonSerializableIface *serializable_iface = NULL;

static void json_serializable_iface_init (gpointer g_iface);

G_DEFINE_TYPE_WITH_CODE (TestObject, test_object, G_TYPE_OBJECT,
								G_IMPLEMENT_INTERFACE (JSON_TYPE_SERIALIZABLE,
										json_serializable_iface_init));

static JsonNode *
test_object_serialize_property (JsonSerializable *serializable,
								const gchar      *name,
								const GValue     *value,
								GParamSpec       *pspec)
{
	JsonNode *retval = NULL;

	if (strcmp (name, "blah") == 0)
	{
		JsonObject *obj;

		retval = json_node_new (JSON_NODE_OBJECT);
		obj = json_object_new ();

		json_object_set_int_member(obj, "foo", g_value_get_int(value));
		json_node_take_object (retval, obj);
	}
	else
		retval = serializable_iface->serialize_property (serializable,
														name,
														value, pspec);

	return retval;
}


static gboolean
test_object_deserialize_property (JsonSerializable *serializable,
								const gchar      *name,
								GValue     *value,
								GParamSpec       *pspec,
								JsonNode		  *node)
{
	gboolean retval = FALSE;

	if (strcmp (name, "blah") == 0)
	{
		JsonObject *obj;

		obj = json_node_get_object (node);
		g_value_set_int (value, json_object_get_int_member(obj, "foo"));
		retval = TRUE;
	}
	else
		retval = serializable_iface->deserialize_property (serializable,
															name,
															value, pspec, node);
	return retval;
}

static void
json_serializable_iface_init (gpointer g_iface)
{
	JsonSerializableIface *iface = g_iface;

	serializable_iface = g_type_default_interface_peek (JSON_TYPE_SERIALIZABLE);

	iface->serialize_property = test_object_serialize_property;
	iface->deserialize_property= test_object_deserialize_property;
}

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
		case PROP_BLAH:
			g_value_set_boxed (value, &(TEST_OBJECT (gobject)->blah));
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
	g_object_class_install_property (gobject_class,
									PROP_BLAH,
									g_param_spec_boxed ("blah", "Blah", "Blah",
														TEST_TYPE_BOXED,
														G_PARAM_READABLE));
}

static void
test_object_init (TestObject *object)
{
	object->foo = 42;
	object->bar = TRUE;
	object->baz = g_strdup ("Test");

	object->blah.foo = object->foo;
	object->blah.bar = object->bar;
}


static void utc_libjson_json_serialize_full_func_01(void)
{
	GObject *obj = NULL;
	JsonSerializableIface *iface = NULL;
	JsonSerializable *serializable = NULL;
	GValue value = {0};
	GParamSpec pspecs;
	JsonNode *node = NULL;
	JsonObject *ret_object = NULL;

	obj = g_object_new(TEST_TYPE_OBJECT, "bar", TRUE, NULL);

	serializable = JSON_SERIALIZABLE (obj);
	iface = JSON_SERIALIZABLE_GET_IFACE (serializable);

	g_value_init (&value, G_TYPE_INT);
	g_value_set_int (&value, 42);
	node = json_serializable_serialize_property(serializable, "blah", &value, &pspecs);

	ret_object = json_node_get_object (node);
	dts_check_ne("json_serializable_serialize_property", ret_object, NULL);
	dts_check_eq("json_serializable_serialize_property", json_object_get_int_member(ret_object, "foo"), 42);

	g_object_unref (obj);
	json_node_free (node);
}


static void utc_libjson_json_serialize_full_func_02(void)
{
	JsonObject *object = NULL;
	JsonNode *node = NULL;
	GObject *obj = NULL;
	JsonSerializableIface *iface = NULL;
	JsonSerializable *serializable = NULL;
	GValue value = {0};
	GParamSpec pspecs;
	gboolean ret = FALSE;

	object = json_object_new();
	node = json_node_new(JSON_NODE_OBJECT);
	json_object_set_int_member (object, "foo", 42);
	json_node_take_object (node, object);

	obj = g_object_new(TEST_TYPE_OBJECT, "bar", TRUE, NULL);

	serializable = JSON_SERIALIZABLE (obj);
	iface = JSON_SERIALIZABLE_GET_IFACE (serializable);

	g_value_init (&value, G_TYPE_INT);
	ret = json_serializable_deserialize_property(serializable, "blah", &value, &pspecs, node);
	dts_check_eq("json_serializable_deserialize_property", ret, TRUE);
	dts_check_eq("json_serializable_deserialize_property", g_value_get_int (&value), 42);

	g_object_unref (obj);
	json_object_unref(object);
	json_node_free (node);

}

static void utc_libjson_json_serialize_full_func_03(void)
{
	JsonNode *node = NULL;
	GObject *obj = NULL;
	JsonSerializableIface *iface = NULL;
	JsonSerializable *serializable = NULL;
	GValue value = {0};
	GParamSpec pspecs;

	obj = g_object_new(TEST_TYPE_OBJECT, "bar", TRUE, NULL);
	serializable = JSON_SERIALIZABLE (obj);
	iface = JSON_SERIALIZABLE_GET_IFACE (serializable);

	g_value_init (&value, G_TYPE_INT);
	g_value_set_int (&value, 42);

	node = json_serializable_default_serialize_property(serializable, "blah", &value, &pspecs);
	dts_check_ne("json_serializable_default_serialize_property", node, NULL);
	dts_check_eq("json_serializable_default_serialize_property", json_node_get_int(node), 42);

	g_object_unref (obj);
	json_node_free (node);

}

static void utc_libjson_json_serialize_full_func_04(void)
{
	JsonNode *node = NULL;
	GObject *obj = NULL;
	JsonSerializableIface *iface = NULL;
	JsonSerializable *serializable = NULL;
	GValue value = {0};
	GParamSpec pspecs;
	gboolean ret = FALSE;

	obj = g_object_new(TEST_TYPE_OBJECT, "bar", TRUE, NULL);
	serializable = JSON_SERIALIZABLE (obj);
	iface = JSON_SERIALIZABLE_GET_IFACE (serializable);

	node = json_node_new(JSON_NODE_VALUE);
	json_node_set_int(node, 42);

	g_value_init (&value, G_TYPE_INT);
	ret = json_serializable_default_deserialize_property(serializable, "blah", &value, &pspecs, node);
	dts_check_eq("json_serializable_default_deserialize_property", ret, TRUE);
	dts_check_eq("json_serializable_default_deserialize_property", g_value_get_int (&value), 42);

	g_object_unref (obj);
	json_node_free (node);

}

