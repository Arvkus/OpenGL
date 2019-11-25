#pragma once
#include <json/json.hpp>
#include "common.h"
#include "model.hpp"

// https://wiki.fileformat.com/3d/glb/
// https://github.com/KhronosGroup/glTF/tree/master/specification/2.0
//------------------------------------------------------------------------------

namespace Loader{
    using json = nlohmann::json;
    namespace{ // private members
        // model loading
        json *file_data;
        char *file_buffer;

        //----------------------------------------------------------------------
        // check if accessor data matches and is correct

        template<typename T> 
        void validate_data(unsigned short index_accessor,int *offset, int *length){
            json accessor = (*file_data)["accessors"][index_accessor];

            unsigned short buffer_index = accessor["bufferView"]; // 0
            int buffer_offset = (*file_data)["bufferViews"][buffer_index]["byteOffset"];
            int buffer_length = (*file_data)["bufferViews"][buffer_index]["byteLength"];

            int accessor_offset = 0;
            if(accessor.find("byteOffset") != accessor.end()) accessor_offset = accessor["byteOffset"];

            // dealing with vectors
            // vector value is a float - 5126
            if(typeid(T) == typeid(glm::vec3) 
            && accessor["componentType"] == 5126 // float
            && accessor["type"] == "VEC3"){ 

                (*length) = accessor["count"]; // 24
                (*offset) = accessor_offset + buffer_offset; // bytes

                if(buffer_length != (*length) * 3 * 4){ // 3 - vec3 number, 4 - float size in bytes
                    throw std::runtime_error("glTF invalid buffer data range.");
                }

            }else if(typeid(T) == typeid(glm::vec2) 
            && accessor["componentType"] == 5126 // float
            && accessor["type"] == "VEC2"){

                (*length) = accessor["count"];
                (*offset) = accessor_offset + buffer_offset; // bytes

                if(buffer_length != (*length) * 2 * 4){
                    throw std::runtime_error("glTF invalid buffer data range.");
                }
                
            }else if(typeid(T) == typeid(unsigned short) 
            && accessor["componentType"] == 5123 // unsigned short
            && accessor["type"] == "SCALAR"){

                (*length) = accessor["count"];
                (*offset) = accessor_offset + buffer_offset; // bytes

                if(buffer_length != (*length) * 1 * 2){
                    throw std::runtime_error("glTF invalid buffer data range.");
                }

            }else{
                throw std::runtime_error("Loader doesn't know how to read this.");
            }

            // data is valid if no errors are thrown
        }

        //----------------------------------------------------------------------
        void build_meshes(Model *model){
            // PROCESS ALL THE DATA
            // buffer - binary byte data
            // data - json information

            for(json node : (*Loader::file_data)["nodes"]){ // skipping scene
                if(node.find("mesh") == node.end()) continue; // skip nodes without mesh
                unsigned short index_mesh = node["mesh"];

                json primitives = (*Loader::file_data)["meshes"][index_mesh]["primitives"][0];

                unsigned short index_position = primitives["attributes"]["POSITION"]; // 4
                unsigned short index_normal = primitives["attributes"]["NORMAL"]; // 5
                unsigned short index_texcoord = primitives["attributes"]["TEXCOORD_0"]; // 6
                unsigned short index_indices = primitives["indices"]; // 3
                // TODO: material

                int offset, length;
                Mesh mesh = Mesh();

                std::vector<glm::vec3> positions;
                std::vector<glm::vec3> normals;
                std::vector<glm::vec2> texcoords;

                //----------------------------------------------------
                // construct Mesh/Node information
                //----------------------------------------------------
                if( (*Loader::file_data)["meshes"][index_mesh].find("name") != (*Loader::file_data)["meshes"][index_mesh].end() ){
                    mesh.name = (*Loader::file_data)["meshes"][index_mesh]["name"];
                }

                if(node.find("translation") != node.end()){
                    mesh.position = glm::vec3(
                        node["translation"][0],
                        node["translation"][1],
                        node["translation"][2]
                    );
                }

                if(node.find("scale") != node.end()){
                    mesh.size = glm::vec3(
                        node["scale"][0],
                        node["scale"][1],
                        node["scale"][2]
                    );
                }

                //----------------------------------------------------
                // construct mesh primitives from buffer
                //----------------------------------------------------
                validate_data<glm::vec3>(index_position, &offset, &length);

                for(int i = 0; i < length; i++){
                    glm::vec3 vec(0.0);
                    std::memcpy(&vec, (file_buffer + offset + i*3*4), 3*4);
                    positions.push_back(vec);
                }
                //----------------------------------------------------
                validate_data<glm::vec3>(index_normal, &offset, &length);

                for(int i = 0; i < length; i++){
                    glm::vec3 vec(0.0);
                    std::memcpy(&vec, (file_buffer + offset + i*3*4), 3*4);
                    normals.push_back(vec);
                }
                //----------------------------------------------------
                validate_data<glm::vec2>(index_texcoord, &offset, &length);

                for(int i = 0; i < length; i++){
                    glm::vec3 vec(1.0);
                    std::memcpy(&vec, (file_buffer + offset + i*2*4), 2*4);
                    texcoords.push_back(vec);
                }
                //----------------------------------------------------
                validate_data<unsigned short>(index_indices, &offset, &length);

                for(int i = 0; i < length; i++){
                    unsigned short ind;
                    std::memcpy(&ind, (file_buffer + offset + i*1*2), 1*2);
                    mesh.indices.push_back(ind);
                }
                //----------------------------------------------------
                // debug
                /*
                for(unsigned int i = 0; i < positions.size(); i++) printm(positions[i]);
                std::cout<<std::endl;
                for(unsigned int i = 0; i < normals.size(); i++) printm(normals[i]);
                std::cout<<std::endl;
                for(unsigned int i = 0; i < texcoords.size(); i++) printm(texcoords[i]);
                std::cout<<std::endl;
                for(unsigned int i = 0; i < mesh.indices.size(); i++) std::cout<<mesh.indices[i]<<" ";
                std::cout<<std::endl;
                */
                //----------------------------------------------------
                // construct vertices
                if(positions.size() == normals.size() && normals.size() == texcoords.size()){
                    for(unsigned int i = 0; i < positions.size(); i++){
                        mesh.vertices.push_back( Vertex(positions[i], normals[i], texcoords[i]));
                    }
                }else{
                    throw std::runtime_error("Vertex primitive data length is not equal.");
                }

                //----------------------------------------------------
          
                mesh.setupMesh();
                model->meshes.push_back(mesh);

            }

        }

        //----------------------------------------------------------------------

        void build_node_tree(json list){
            

            for(int node_id : list){
                json node = (*Loader::file_data)["nodes"][node_id];

                if(node.find("mesh") != node.end()){
                    json mesh = (*Loader::file_data)["meshes"][ (int)node["mesh"] ];
                    std::cout<<node_id<<" ";

                    if(mesh.find("children") != mesh.end()){
                        build_node_tree(mesh["children"]);
                    }
                }
  
                
            }
            std::cout<<std::endl;

        }

    }//-------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    Model model_from_glb(const char* path){ 
        try{
            // 0x46546C67 - file magic
            // 0x4E4F534A - JSON chunk type
            // 0x004E4942 - Bin chunk type 

            clock_t start_time = clock();
            std::ifstream GLB(path, std::ifstream::binary);
            unsigned int chunk_length;
            unsigned int chunk_type;

            //-------------------------------
            // check if file is valied and read header

            if(!GLB.is_open()){
                throw std::runtime_error(std::string("Failed to open: ") + path);
            }else{
                unsigned int magic;
                unsigned int version;
                unsigned int length;

                GLB.read((char*)&magic, 4);
                GLB.read((char*)&version, 4);
                GLB.read((char*)&length, 4);

                if(0x46546C67 == magic){
                    std::cout<< path << " | 'glb"<< version <<"' file format | " << length << " bytes\n";
                }else{
                    throw std::runtime_error(std::string("file is not .glb or is corrupted: ") + path);
                }
            }
            //-------------------------------
            // read JSON

            GLB.read((char*)&chunk_length, 4);
            GLB.read((char*)&chunk_type, 4);

            if(chunk_type != 0x4E4F534A){
                throw std::runtime_error(std::string("file is corrupted: ") + path);
            }

            json data;
            char *text = new char[chunk_length + 1]; 
            text[chunk_length] = '\0'; // required null terminator to mark the end
            GLB.read(text, chunk_length);
            
            try{
                data = (data).parse(text);
                
            }catch(json::exception& e){
                throw e;
            }

            // output to file for debug
            std::ofstream out("debug.json");
            out << (data).dump(4); out.close();
            //-------------------------------
            // read buffer data

            GLB.read((char*)&chunk_length, 4);
            GLB.read((char*)&chunk_type, 4);

            if(chunk_type != 0x004E4942){
                throw std::runtime_error(std::string("file is corrupted: ") + path);
            }

            char* buffer = new char[chunk_length]; // vertex data & images
            GLB.read( buffer, chunk_length);
            GLB.close();
            //-------------------------------
            // build model from data

            Loader::file_data = &data;
            Loader::file_buffer = buffer;

            Model model = Model();
            Loader::build_meshes(&model);

            // iterate through node list scene is the ROOT node
            Loader::build_node_tree( (*Loader::file_data)["scenes"][0]["nodes"] );

            std::cout<<"Model ("<< path << ") is built successfully. \n";

            // free space
            delete[] buffer;

            std::cout<< "Time to create model: " << (float)(clock() - start_time)/CLOCKS_PER_SEC <<std::endl<<std::endl;
            return model;

        }catch(const json::exception& e){
            std::cout<< e.what() <<std::endl;
            return Model();
        }catch(const std::exception& e){
            std::cout<< e.what() <<std::endl;
            return Model();
        };
        
        
    }
    //--------------------------------------------------------------------------

}