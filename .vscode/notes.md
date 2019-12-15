

```sh

g++ -Wall -std=c++17 src/* -o bin/main.exe -I include -I dependencies -L lib -lSDL2

```

```cpp

template<typename T>
void string_to_vector(std::vector<T> *vec,  xml_node node){
    
    std::stringstream ss( node.text().get() );

    float num;
    while(ss >> num){
        (*vec).push_back(num);
    }

    //std::cout<< node.attribute("id").value() <<" data parsed"<<std::endl;
}

void load_xml(){
    xml_document doc;
    xml_parse_result result = doc.load_file("models/untitled.dae");

    std::cout << "Load result: " << result.description() << std::endl;
    if(result == false) return;

    std::vector<unsigned int> indices;
    std::vector<float> positions;
    std::vector<float> normals;
    std::vector<float> texture;

    xpath_node geometry_node_path = doc.select_node("COLLADA/library_geometries/geometry");
    if(geometry_node_path){
        std::string name = geometry_node_path.node().attribute("name").value();
        std::cout<<"Geometry exist: "<<name<<std::endl;

        xpath_node_set sources = geometry_node_path.node().select_nodes("mesh/source");
        for (xpath_node_set::const_iterator it = sources.begin(); it != sources.end(); ++it){

            xpath_node node_path = *it;
            xml_node array_node = node_path.node().child("float_array");

            if(node_path.node().attribute("id").value() == name + "-mesh-positions") string_to_vector<float>(&positions, array_node);
            if(node_path.node().attribute("id").value() == name + "-mesh-normals") string_to_vector<float>(&normals, array_node);
            if(node_path.node().attribute("id").value() == name + "-mesh-map-0") string_to_vector<float>(&texture, array_node);

        }

        xpath_node triangles_path = geometry_node_path.node().select_node("mesh/triangles/p");
        if(triangles_path){
            //std::cout<< indices_path.node().text().get();
            string_to_vector<unsigned int>(&indices, triangles_path.node());
            std::cout<<indices.size()<<std::endl;
        }

        
        


    }
 
    for(float v : positions)std::cout<<v<<" ";
    std::cout<<std::endl;
    for(float v : normals)std::cout<<v<<" ";
    std::cout<<std::endl;
    for(float v : texture) std::cout<<v<<" ";
    std::cout<<std::endl;


   delete &doc;
    
}

```