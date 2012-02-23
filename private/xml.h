
#ifndef FMLL_XML_H
#define FMLL_XML_H

#include "all.h"
#include "lib/memory.h"
#include "lib/exception.h"

#define TYPE_PERCEPTRON "perceptron"
#define TYPE_SOM "som"
#define TYPE_PCA "pca"
#define TYPE_SVM "svm"
#define TYPE_SVM_NET "svm_net"

int xml_create(const char * nn_type, mxml_node_t ** main_node, mxml_node_t ** content_node);
int xml_set_int(mxml_node_t * parent_node, const char * node_name, int value);
int xml_set_text(mxml_node_t * parent_node, const char * node_name, const char * value);
int xml_set_double(mxml_node_t * parent_node, const char * node_name, double value);
int xml_get_int(mxml_node_t * node, const char * node_name, int * var);
int xml_get_text(mxml_node_t * node, const char * node_name, char * var);
int xml_get_double(mxml_node_t * node, const char * node_name, double * var);
void xml_destroy(mxml_node_t * main_node);
int xml_load(const char * fname_prefix, const char * nn_type, mxml_node_t ** main_node, mxml_node_t ** content_node);
int xml_save(const char * fname_prefix, mxml_node_t * main_node);

#endif

