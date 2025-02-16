#ifndef __METADATA_H__
#define __METADATA_H__


#include <string>
#include <typeinfo>

namespace btrack::nodes {



struct PortMetadata
{
    enum PortType {
        INPUT_VALUE,
        OUTPUT_VALUE,
    };
    const std::string name;
    const std::string description;
    const PortType portType;
    const std::type_index& type;
};

struct NodeMetadata
{
    const std::string name;
    const std::string description;
    const std::string category;
    const std::type_index& nodeType;
    PortMetadata* ports;
};

}

#ifndef BEGIN_METADATA

/**
 * Define start of a Node metadata block 
 * (token)     name: Name (one word)
 * (string) displayName: Display name
 * (string) description: Description of the node
 * (token)     nodeType: Class of the node
 * (token)     metaNodeType: Class of the meta node
 */
#define BEGIN_METADATA(name, displayName, description, nodeType, metaNodeType) const btrack::nodes::NodeMetadata name##_Metadata { \
#name, displayName, description, typeid(nodeType), typeid(metaNodeType), new btrack::nodes::PortMetadata[] {

#define END_METADATA }};

/**
 * Define a metadata input value 
 * (token)     name: Name (one word)
 * (string) displayName: Display name
 * (string) description: Description of the port
 * (token)     type: Value type
 */
#define INPUT_VALUE_META(name, displayName, description, type) \
{#name, displayName, description, btrack::nodes::PortMetadata::INPUT_VALUE, typeid(type)}

/**
 * Define a metadata input array 
 * (token)     name: Name (one word)
 * (string) displayName: Display name
 * (string) description: Description of the port
 * (token)     type: Value type
 */
#define INPUT_ARRAY_META(name, displayName, description, type) \
{#name, displayName, description, btrack::nodes::PortMetadata::INPUT_ARRAY, typeid(type)}

/**
 * Define a metadata output value 
 * (token)     name: Name (one word)
 * (string) displayName: Display name
 * (string) description: Description of the port
 * (token)     type: Value type
 */
#define OUTPUT_VALUE_META(name, displayName, description, type) \
{#name, displayName, description, btrack::nodes::PortMetadata::OUTPUT_VALUE, typeid(type)}

/**
 * Define a metadata output array 
 * (token)     name: Name (one word)
 * (string) displayName: Display name
 * (string) description: Description of the port
 * (token)     type: Value type
 */
#define OUTPUT_ARRAY_META(name, displayName, description, type) \
{#name, displayName, description, btrack::nodes::PortMetadata::OUTPUT_ARRAY, typeid(type)}

#endif // BEGIN_METADATA


#endif // __METADATA_H__