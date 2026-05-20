
#ifndef DA_H
#define DA_H

#define DA_START_SIZE 8
#define DA_GROWTH_RATE 2

// just declare
#define DA_DECLARE(name, type)\
    typedef struct {\
        int capacity;\
        int length;\
        type *data;\
    } name;\
    name name##_new();\
    void name##_destroy(name *f);\
    int name##_resize(name *f);\
    void name##_append(name *f, type val);\
    type name##_get(name *f, int idx);

// define all the functions
#define DA_DEFINE(name, type)\
    name name##_new() {\
        type *data = (type*)malloc(sizeof(type) * DA_START_SIZE);\
        if (data == NULL) {\
            return (name){ .capacity = 0, .length = 0, .data = NULL };\
        }\
        return (name){ .capacity = DA_START_SIZE, .length = 0, .data = data };\
    }\
    void name##_destroy(name *f) {\
        free(f->data);\
        f->data = NULL;\
        f->length = 0;\
        f->capacity = 0;\
    }\
    int name##_resize(name *f) {\
        if (f->length < f->capacity) return 1;\
        type *data = realloc(f->data, sizeof(type) * f->capacity * DA_GROWTH_RATE);\
        if (data == NULL) {\
            free(f->data);\
            f->data = NULL;\
            f->capacity = 0;\
            f->length = 0;\
            return 0;\
        }\
        f->data = data;\
        f->capacity *= DA_GROWTH_RATE;\
        return 1;\
    }\
    void name##_append(name *f, type val) {\
        if (name##_resize(f))\
            f->data[f->length++] = val;\
    }\
    type name##_get(name *f, int idx) {\
        return f->data[idx];\
    }

#endif // DA_H