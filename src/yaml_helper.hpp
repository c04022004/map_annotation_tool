

#ifndef __YAML_HPP_INCLUDED__
#define __YAML_HPP_INCLUDED__

#include <algorithm>
#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>
#include "alphanum.hpp"

/*
    Presevring reasonable order when saving YAML files.
    Referencing the following resources:
    https://github.com/jbeder/yaml-cpp/issues/169
    http://www.davekoelle.com/alphanum.html

*/

// Recursive helper function that does all the work
void writeNode(const YAML::Node& node, YAML::Emitter& emitter)
{
  switch (node.Type())
  {
    case YAML::NodeType::Sequence:
    {
      emitter << YAML::BeginSeq;
      for (size_t i = 0; i < node.size(); i++)
      {
        writeNode(node[i], emitter);
      }
      emitter << YAML::EndSeq;
      break;
    }
    case YAML::NodeType::Map:
    {
        emitter << YAML::BeginMap;

        // First collect all the keys
        std::vector<std::string> keys(node.size());
        int key_it = 0;
        for (YAML::const_iterator it = node.begin(); it != node.end(); ++it)
        {
            keys[key_it++] = it->first.as<std::string>();
        }

        // Then sort them
        // std::sort(keys.begin(), keys.end());
        std::sort(keys.begin(), keys.end(), doj::alphanum_less<std::string>());

        // Then emit all the entries in sorted order.
        for(size_t i = 0; i < keys.size(); i++)
        {
            emitter << YAML::Key;
            emitter << keys[i];
            emitter << YAML::Value;
            writeNode(node[keys[i]], emitter);
        }
        emitter << YAML::EndMap;
        break;
    }
    default:
        emitter << node;
        break;
  }
}

// Main function that emits a yaml node to an output stream.
void writeYamlOrderedMaps(std::ostream& out, const YAML::Node& node)
{
    YAML::Emitter emitter;
    writeNode(node, emitter);
    out << emitter.c_str() << std::endl;
}

void sortKeyAlphanum(std::vector<std::string>& keys){
    std::sort(keys.begin(), keys.end(), doj::alphanum_less<std::string>());
}

#endif