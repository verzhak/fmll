
#ifndef XML_H
#define XML_H

#include "all.h"
#include "exception.h"

#define TYPE_PERCEPTRON "perceptron"
#define TYPE_SOM "som"
#define TYPE_PCA "pca"

int8_t xml_create(const char * nn_type, mxml_node_t ** main_node, mxml_node_t ** content_node);
int8_t xml_set_int(mxml_node_t * parent_node, const char * node_name, int value);
int8_t xml_set_text(mxml_node_t * parent_node, const char * node_name, const char * value);
int8_t xml_set_double(mxml_node_t * parent_node, const char * node_name, double value);
int8_t xml_get_int(mxml_node_t * node, const char * node_name, int * var);
int8_t xml_get_text(mxml_node_t * node, const char * node_name, char * var);
int8_t xml_get_double(mxml_node_t * node, const char * node_name, double * var);
void xml_destroy(mxml_node_t * main_node);
int8_t xml_load(const char * fname_prefix, const char * nn_type, mxml_node_t ** main_node, mxml_node_t ** content_node);
int8_t xml_save(const char * fname_prefix, mxml_node_t * main_node);

#endif

