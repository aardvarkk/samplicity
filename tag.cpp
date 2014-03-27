#include "tag.h"

Tag::Tag() : id(0), parent_id(0), name("")
{
}

Tag::Tag(int id, int parent_id, QString const& name) :
    id(id),
    parent_id(parent_id),
    name(name)
{
}
