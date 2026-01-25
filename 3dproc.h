void command3D(unsigned char newCommand);
void data3D(unsigned char data);
void update3D();
void init3D();

typedef char vec3[3];

typedef struct {
    vec3 positions[3];
    vec3 normals[3];
    unsigned char color;
} Face;

typedef struct {
    Face faces[256];
    unsigned char numFaces;
    vec3 position;
    vec3 rotation;
} Object;