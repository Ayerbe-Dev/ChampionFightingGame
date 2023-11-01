#include "HxA.h"

#define READ_UINT32 1



static const std::string hxaLayerDataTypeMap[4] = {
    "HXA_LDT_UINT8",
    "HXA_LDT_INT32",
    "HXA_LDT_FLOAT",
    "HXA_LDT_DOUBLE"
};

static const std::string hxaNodeTypeMap[3] = {
    "HXA_NT_META_ONLY",
    "HXA_NT_GEOMETRY",
    "HXA_NT_IMAGE",
};

static const std::string hxaMetaTypeMap[6] = {
    "HXA_MDT_INT64",
    "HXA_MDT_DOUBLE",
    "HXA_MDT_NODE",
    "HXA_MDT_TEXT",
    "HXA_MDT_BINARY",
    "HXA_MDT_META"
};

struct FileVector {
    std::vector<char> fv {};
    size_t pos { 0 };
};

uint32_t readUint32(FileVector *f) {
    uint32_t o = (f->fv[f->pos + 3] << 24) | (f->fv[f->pos + 2] << 16) | (f->fv[f->pos + 1] << 8) | f->fv[f->pos + 0];
    f->pos = f->pos + 4;
    return o;
}
int32_t readInt32(FileVector *f) {
    int32_t o = (f->fv[f->pos + 3] << 24) | (f->fv[f->pos + 2] << 16) | (f->fv[f->pos + 1] << 8) | f->fv[f->pos + 0];
    f->pos = f->pos + 4;
    return o;
}

double readDouble(FileVector *f) {
    char double_array[8] = {
        f->fv[f->pos + 0],
        f->fv[f->pos + 1],
        f->fv[f->pos + 2],
        f->fv[f->pos + 3],
        f->fv[f->pos + 4],
        f->fv[f->pos + 5],
        f->fv[f->pos + 6],
        f->fv[f->pos + 7],

    };
    double o;
    std::memcpy(&o, double_array, sizeof(double));
    
    f->pos = f->pos + 8;
    return o;
}

float readFloat(FileVector *f) {
    char float_array[4] = {
        f->fv[f->pos + 0],
        f->fv[f->pos + 1],
        f->fv[f->pos + 2],
        f->fv[f->pos + 3]
    };
    float o;
    std::memcpy(&o, float_array, sizeof(float));
    
    f->pos = f->pos + 4;
    return o;
}

int8_t readUint8(FileVector *f) {
    uint8_t o = f->fv[f->pos + 0];
    f->pos = f->pos + 1;
    return o;
}

std::string readStr(FileVector* f) {
    uint8_t name_length = readUint8(f);
    std::string o = "";
    for (uint8_t i = 0; i < name_length; i++) {
        o += f->fv[f->pos + static_cast<size_t>(i)];
    }
    f->pos = f->pos + static_cast<size_t>(name_length);
    return o;
}

std::string readStr(FileVector* f, uint32_t name_length) {
    std::string o = "";
    for (uint8_t i = 0; i < name_length; i++) {
        o += f->fv[f->pos + static_cast<size_t>(i)];
    }
    f->pos = f->pos + static_cast<size_t>(name_length);
    return o;
}

std::vector<char> hxaCopyFile(const char* path)
{
    FILE* fptr = NULL;
    size_t fsize;

    fopen_s(&fptr, path, "rb");
    //fptr = fopen(path, "rb"); // reading in binary mode

    if (fptr == NULL){
        printf("Could not find file '%s'", path);
        exit(1);
    }

    fseek(fptr, 0, SEEK_END);
    fsize = ftell(fptr);  // getting file size
    fseek(fptr, 0, SEEK_SET);

    std::vector<char> memory;
    memory.resize(fsize);

    for (int i = 0; i < fsize; i++) {
        memory[i] = fgetc(fptr); // ! Does not work !
        printf("0x%x\n", memory[i]);
        
    }

    fclose(fptr);
	
    return memory;
}

HXAMeta hxaLoadMeta(FileVector* f) {
    
    HXAMeta meta{};

    meta.name = readStr(f);
    printf("Meta name: `%s`\n", meta.name.c_str());

    meta.type = static_cast<HXAMetaDataType>(readUint8(f));
    printf("Meta type: '%s'\n", hxaMetaTypeMap[meta.type].c_str());

    meta.array_length = readUint32(f);
    printf("Meta length: %u\n",meta.array_length);

    //at this point we need to branch based on the metadata type
    if (meta.type == HXAMetaDataType::HXA_MDT_META) {
        std::vector<HXAMeta> meta_vec(meta.array_length);
        
        
        for (uint32_t i = 0; i < meta.array_length; i++) {
            HXAMeta next = hxaLoadMeta(f);
            meta_vec[i] = next;
        }

        meta.value.array_of_meta = (void*)meta_vec.data();
    }
    else if (meta.type == HXAMetaDataType::HXA_MDT_TEXT) {
        std::string text = readStr(f, meta.array_length);
        printf("meta text: %s\n", text.c_str());
    } 
    else if (meta.type == HXAMetaDataType::HXA_MDT_DOUBLE) {
        std::vector<double> double_vec(meta.array_length);
        
        printf("[ ");
        for (uint32_t i = 0; i < meta.array_length; i++) {
            double_vec[i] = readDouble(f);
            printf(" %f ", double_vec[i]);
        }
        printf(" ]\n");
        
    }
    else
    {
        printf("Meta type '%s' is not currently implemented\n", hxaMetaTypeMap[meta.type].c_str());
        exit(1);
    }
   
    return meta;
}

HXALayer hxaLoadLayer(FileVector* f, uint32_t num_elems) {
    HXALayer layer;
    layer.name = readStr(f);
    layer.components = readUint8(f);
    layer.type = static_cast<HXALayerDataType>(readUint8(f));

    printf("name '%s' type '%s'\n", layer.name.c_str(), hxaLayerDataTypeMap[layer.type].c_str());

    if (layer.type == HXALayerDataType::HXA_LDT_FLOAT) {
        std::vector<float> float_vec(num_elems * layer.components);
        for (uint32_t i = 0; i < num_elems * layer.components; i++) {
            float_vec[i] = readFloat(f);
        }
        layer.data.float_data = float_vec.data();
    } else if (layer.type == HXALayerDataType::HXA_LDT_INT32) {
        std::vector<int32_t> int_vec(num_elems * layer.components);
        for (uint32_t i = 0; i < num_elems * layer.components; i++) {
            int_vec[i] = readInt32(f);
        }
        layer.data.int32_data = int_vec.data();
    }else {
        printf("Layer data type '%s' not supported\n", hxaLayerDataTypeMap[layer.type].c_str());
        exit(1);
    }
    

    return layer;
}

HXALayerStack hxaLoadLayerStack(FileVector* f, uint32_t num_elems) {
    HXALayerStack stack;

    stack.layer_count = readUint32(f);
    std::vector<HXALayer> layers(stack.layer_count);

    for (uint32_t i = 0; i< stack.layer_count; i++){
        layers[i] = hxaLoadLayer(f, num_elems);
    }
    stack.layers = layers.data();
    return stack;
}

HXANode hxaLoadNode(FileVector* f) {
    HXANode node;

    node.type = static_cast<HXANodeType>(readUint8(f));
    printf("Node Type: %s\n", hxaNodeTypeMap[node.type].c_str());

    node.meta_data_count = readUint32(f);
    printf("Meta Data Count: %u\n", node.meta_data_count);

    node.meta_data.resize(node.meta_data_count);
    for (uint32_t i = 0; i < node.meta_data_count; i++) {
        node.meta_data.push_back(hxaLoadMeta(f));
    }

    if (node.type == HXANodeType::HXA_NT_GEOMETRY) {
        node.content.geometry.vertex_count = readUint32(f);
        printf("Loading %u vertices\n", node.content.geometry.vertex_count);
        node.content.geometry.vertex_stack = hxaLoadLayerStack(f, node.content.geometry.vertex_count);
    
        node.content.geometry.edge_corner_count = readUint32(f);
        printf("Loading %u edge corners\n", node.content.geometry.edge_corner_count);
        node.content.geometry.corner_stack = hxaLoadLayerStack(f, node.content.geometry.edge_corner_count);
        node.content.geometry.edge_stack = hxaLoadLayerStack(f, node.content.geometry.edge_corner_count);

        node.content.geometry.face_count = readUint32(f);
        printf("Loading %u faces\n", node.content.geometry.face_count);
        node.content.geometry.face_stack = hxaLoadLayerStack(f, node.content.geometry.face_count);
    }
    else {
        printf("Node type '%s' Not supported \n", hxaNodeTypeMap[node.type].c_str());
        exit(1);
    }

    return node;
}

HXAFile hxaLoad(const char* path)
{
    
    std::vector<char> fv = hxaCopyFile(path);
    FileVector f{ fv,0 };

    uint32_t magicNumber = readUint32(&f);
    if (magicNumber != 4290632) {
        printf("HxA error: not a HxA file\n");
        exit(1);
    }

    uint8_t version = readUint8(&f);
    printf("Version: %u\n",version);

    uint32_t nodeCount = readUint32(&f);
    printf("Nodes: %u\n", nodeCount);

    std::vector<HXANode> nodeArray(nodeCount);
    for (uint32_t i = 0; i < nodeCount; i++) {
        nodeArray.push_back(hxaLoadNode(&f));
    }

    HXAFile fi;
    fi.version = version;
    fi.node_count = nodeCount;
    fi.nodes = nodeArray;
    
    return fi;
}
