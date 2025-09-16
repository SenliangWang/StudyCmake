#include <GL/glut.h>
#include <cmath>
#include <vector>

struct Vertex3 {
    float x, y, z;
};

static std::vector<Vertex3> spherePositions;
static std::vector<Vertex3> sphereNormals;
static std::vector<unsigned int> sphereIndices;

static float cameraAngleY = 20.0f;
static float cameraAngleX = 20.0f;
static float cameraDistance = 3.0f;

void generateSphere(float radius, int slices, int stacks) {
    spherePositions.clear();
    sphereNormals.clear();
    sphereIndices.clear();

    for (int i = 0; i <= stacks; ++i) {
        float v = static_cast<float>(i) / static_cast<float>(stacks);
        float phi = v * static_cast<float>(M_PI); // 0..PI
        float cosPhi = std::cos(phi);
        float sinPhi = std::sin(phi);

        for (int j = 0; j <= slices; ++j) {
            float u = static_cast<float>(j) / static_cast<float>(slices);
            float theta = u * static_cast<float>(2.0 * M_PI); // 0..2PI
            float cosTheta = std::cos(theta);
            float sinTheta = std::sin(theta);

            float nx = sinPhi * cosTheta;
            float ny = cosPhi;
            float nz = sinPhi * sinTheta;

            Vertex3 normal{nx, ny, nz};
            Vertex3 position{radius * nx, radius * ny, radius * nz};

            sphereNormals.push_back(normal);
            spherePositions.push_back(position);
        }
    }

    // indices (triangle strips between stacks)
    for (int i = 0; i < stacks; ++i) {
        for (int j = 0; j < slices; ++j) {
            int row1 = i * (slices + 1);
            int row2 = (i + 1) * (slices + 1);

            sphereIndices.push_back(row1 + j);
            sphereIndices.push_back(row2 + j);
            sphereIndices.push_back(row2 + j + 1);

            sphereIndices.push_back(row1 + j);
            sphereIndices.push_back(row2 + j + 1);
            sphereIndices.push_back(row1 + j + 1);
        }
    }
}

void setupLightingAndMaterial() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat lightPos[] = {2.0f, 3.0f, 4.0f, 1.0f};
    GLfloat lightAmbient[] = {0.1f, 0.1f, 0.1f, 1.0f};
    GLfloat lightDiffuse[] = {0.9f, 0.9f, 0.9f, 1.0f};
    GLfloat lightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

    GLfloat matAmbient[] = {0.2f, 0.2f, 0.25f, 1.0f};
    GLfloat matDiffuse[] = {0.4f, 0.6f, 0.9f, 1.0f};
    GLfloat matSpecular[] = {0.9f, 0.9f, 0.9f, 1.0f};
    GLfloat matShininess[] = {64.0f};

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);

    glEnable(GL_NORMALIZE); // ensure correct lighting if scaled
}

void onDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Camera transform
    glTranslatef(0.0f, 0.0f, -cameraDistance);
    glRotatef(cameraAngleX, 1.0f, 0.0f, 0.0f);
    glRotatef(cameraAngleY, 0.0f, 1.0f, 0.0f);

    // Light position can be updated relative to current modelview
    GLfloat lightPos[] = {2.0f, 3.0f, 4.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, spherePositions.data());
    glNormalPointer(GL_FLOAT, 0, sphereNormals.data());

    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(sphereIndices.size()), GL_UNSIGNED_INT, sphereIndices.data());

    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    glutSwapBuffers();
}

void onReshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, static_cast<double>(w) / static_cast<double>(h), 0.1, 100.0);
}

void onKeyboard(unsigned char key, int, int) {
    switch (key) {
        case 27: // ESC
            std::exit(0);
            break;
        case '+':
            cameraDistance = std::max(1.2f, cameraDistance - 0.2f);
            glutPostRedisplay();
            break;
        case '-':
            cameraDistance += 0.2f;
            glutPostRedisplay();
            break;
        default:
            break;
    }
}

void onSpecial(int key, int, int) {
    const float step = 5.0f;
    if (key == GLUT_KEY_LEFT) cameraAngleY -= step;
    if (key == GLUT_KEY_RIGHT) cameraAngleY += step;
    if (key == GLUT_KEY_UP) cameraAngleX -= step;
    if (key == GLUT_KEY_DOWN) cameraAngleX += step;
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Fixed-Function Sphere (glVertexPointer/glNormalPointer)");

    glEnable(GL_DEPTH_TEST);
    setupLightingAndMaterial();

    generateSphere(1.0f, 64, 32);

    glutDisplayFunc(onDisplay);
    glutReshapeFunc(onReshape);
    glutKeyboardFunc(onKeyboard);
    glutSpecialFunc(onSpecial);

    glutMainLoop();
    return 0;
}

