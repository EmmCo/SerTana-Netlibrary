#ifndef SerTanaCOPYABLE_H
#define SerTanaCOPYABLE_H


namespace SerTana
{

/// A tag class emphasises the objects are copyable.
/// The empty base class optimization applies.
/// Any derived class of copyable should be a value type.
class copyable
{
};

};



#endif // SerTanaCOPYABLE_H
