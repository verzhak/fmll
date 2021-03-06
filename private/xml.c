
#include "private/xml.h"

int xml_create(const char * nn_type, mxml_node_t ** main_node, mxml_node_t ** content_node)
{
	int ret = 0;
	mxml_node_t * node, * sub_node;

	fmll_try;

		fmll_throw_null(main_node);
		fmll_throw_null(content_node);
		fmll_throw_null(* main_node = mxmlNewXML(NULL));

			fmll_throw_null(node = mxmlNewElement(* main_node, "fmll"));

				fmll_throw_null(sub_node = mxmlNewElement(node, "library_description"));

					fmll_throw_if(xml_set_text(sub_node, "name", LIB_NAME));
					fmll_throw_if(xml_set_text(sub_node, "id", ID));
					fmll_throw_if(xml_set_text(sub_node, "version", VERSION));
					fmll_throw_if(xml_set_int(sub_node, "file_version", CURRENT_VERSION));

				fmll_throw_if(xml_set_text(node, "ann_type", nn_type));

				fmll_throw_null(* content_node = mxmlNewElement(node, "ann"));

	fmll_catch;

		ret = -1;

		xml_destroy(* main_node);
		* main_node = NULL;
		* content_node = NULL;

	fmll_finally;
	
	return ret;
}

#define BEGIN_XML_SET(type) \
	int ret = 0;\
	mxml_node_t * node;\
\
	fmll_try;\
\
		fmll_throw_null(node = mxmlNewElement(parent_node, node_name));\
		mxmlElementSetAttr(node, "type", type);

#define END_XML_SET \
	fmll_catch;\
\
		ret = -1;\
\
	fmll_finally;\
\
	return ret;

int xml_set_uchar(mxml_node_t * parent_node, const char * node_name, const unsigned char value)
{
	BEGIN_XML_SET("uchar");
	
	mxmlNewInteger(node, value);
	
	END_XML_SET;
}

int xml_set_int(mxml_node_t * parent_node, const char * node_name, const int value)
{
	BEGIN_XML_SET("int");
	
	mxmlNewInteger(node, value);
	
	END_XML_SET;
}

int xml_set_text(mxml_node_t * parent_node, const char * node_name, const char * value)
{
	BEGIN_XML_SET("text");

	mxmlNewText(node, 0, value);

	END_XML_SET;
}

int xml_set_double(mxml_node_t * parent_node, const char * node_name, const double value)
{
	char tvalue[1024];

	BEGIN_XML_SET("double");

	sprintf(tvalue, "%.17f", value);
	mxmlNewText(node, 0, tvalue);

	END_XML_SET;
}

#define BEGIN_XML_GET \
	int ret = 0;\
	mxml_node_t * sub_node;\
\
	fmll_try;\
\
		fmll_throw_null(sub_node = mxmlFindElement(node, node, node_name, NULL, NULL, MXML_DESCEND_FIRST));

#define END_XML_GET \
\
	fmll_catch;\
\
		ret = -1;\
\
	fmll_finally;\
\
	return ret;

int xml_get_uchar(mxml_node_t * node, const char * node_name, unsigned char * var)
{
	BEGIN_XML_GET;
	
	* var = sub_node->child->value.integer;
	
	END_XML_GET;
}

int xml_get_int(mxml_node_t * node, const char * node_name, int * var)
{
	BEGIN_XML_GET;
	
	* var = sub_node->child->value.integer;
	
	END_XML_GET;
}

int xml_get_text(mxml_node_t * node, const char * node_name, char * var)
{
	BEGIN_XML_GET;
	
	strcpy(var, sub_node->child->value.text.string);

	END_XML_GET;
}

int xml_get_double(mxml_node_t * node, const char * node_name, double * var)
{
	BEGIN_XML_GET;
	
	* var = sub_node->child->value.real;
	
	END_XML_GET;
}

void xml_destroy(mxml_node_t * main_node)
{
	if(main_node != NULL)
		mxmlDelete(main_node);
}

mxml_type_t xml_load_callback(mxml_node_t * node)
{
  const char * type;
  
  if((type = mxmlElementGetAttr(node, "type")) == NULL)
	  return MXML_ELEMENT;

  if (! strcmp(type, "double"))
	  return MXML_REAL;
  else if (! strcmp(type, "uchar"))
	  return MXML_INTEGER;
  else if (! strcmp(type, "int"))
	  return MXML_INTEGER;
  else if (! strcmp(type, "text"))
	  return MXML_TEXT;
  else
	  return MXML_OPAQUE;
}

int xml_load(const char * fname_prefix, const char * nn_type, mxml_node_t ** main_node, mxml_node_t ** content_node)
{
	int ret = 0;
	mxml_node_t * node, * sub_node, * sub_sub_node;
	char * fname = NULL;
	FILE * fl = NULL;

	fmll_try;

		fmll_throw_null(fname = fmll_alloc(sizeof(char), 1, strlen(fname_prefix) + 5));
		sprintf(fname, "%s.xml", fname_prefix);

		fmll_throw_null(fl = fopen(fname, "r"));
		fmll_throw_null(* main_node = mxmlLoadFile(NULL, fl, & xml_load_callback));

			fmll_throw_null(node = mxmlFindElement(* main_node, * main_node, "fmll", NULL, NULL, MXML_DESCEND_FIRST));

				fmll_throw_null(sub_node = mxmlFindElement(node, node, "library_description", NULL, NULL, MXML_DESCEND_FIRST));

					fmll_throw_null(sub_sub_node = mxmlFindElement(sub_node, sub_node, "id", NULL, NULL, MXML_DESCEND_FIRST));
					fmll_throw_if(strcmp(sub_sub_node->child->value.text.string, ID));

					fmll_throw_null(sub_sub_node = mxmlFindElement(sub_node, sub_node, "file_version", "type", NULL, MXML_DESCEND_FIRST));
					fmll_throw_if(sub_sub_node->child->value.integer < MIN_VERSION);
					fmll_throw_if(sub_sub_node->child->value.integer > CURRENT_VERSION);

				fmll_throw_null(sub_node = mxmlFindElement(node, node, "ann_type", NULL, NULL, MXML_DESCEND_FIRST));
				fmll_throw_if(strcmp(sub_node->child->value.text.string, nn_type));

				fmll_throw_null(* content_node = mxmlFindElement(node, node, "ann", NULL, NULL, MXML_DESCEND_FIRST));

	fmll_catch;

		xml_destroy(* main_node);
		* main_node = NULL;
		* content_node = NULL;

		ret = -1;

	fmll_finally;

		fmll_free(fname);

		if(fl != NULL)
			fclose(fl);
	
	return ret;
}

int xml_save(const char * fname_prefix, mxml_node_t * main_node)
{
	int ret = 0;
	char * fname = NULL;
	FILE * fl = NULL;

	fmll_try;

		fmll_throw_null(fname = fmll_alloc(sizeof(char), 1, strlen(fname_prefix) + 5));
		sprintf(fname, "%s.xml", fname_prefix);

		fmll_throw_null(fl = fopen(fname, "w"));
		fmll_throw_if(mxmlSaveFile(main_node, fl, MXML_NO_CALLBACK));

	fmll_catch;

		ret = -1;

	fmll_finally;

		fmll_free(fname);

		if(fl != NULL)
			fclose(fl);
	
	return ret;
}

