// Autor: Tamara Dzambic
// Opis:  projekat 12


#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>

#include <GL/glew.h>   
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"
#include "model.hpp"

#include <cmath>
#define M_PI 3.14159265358979323846
#define _CRT_SECURE_NO_WARNINGS
#define CRES 30 


static unsigned loadImageToTexture(const char* filePath); 
void initializeTexture(unsigned int VAO, unsigned int VBO, float* vertices, int verticesCount, unsigned indexTexture);

int main(void)
{
    // initialization
    if (!glfwInit()){
        std::cout << "GLFW Biblioteka se nije ucitala! :(\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
    const unsigned int width = mode->width;
    const unsigned int height = mode->height;

    GLFWwindow* window = glfwCreateWindow(width,height, "SV43/2020", primaryMonitor, NULL);
    float aspectRatio = static_cast<float>(width) / height;

    if (window == NULL){
        std::cout << "Prozor nije napravljen! :(\n";
        glfwTerminate();
        return 2;
    }
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK){
        std::cout << "GLEW nije mogao da se ucita! :'(\n";
        return 3;
    }

    // shader and buffers
    Shader unifiedShader("basic.vert", "basic.frag");
    unifiedShader.use();

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    // index texture
    float indexVertices[] = {
        1.0f, -1.0f,  1.0f, 0.0f,
        1.0f, -0.85f,  1.0f, 1.0f,
        0.7f, -1.0f,  0.0f, 0.0f,
        0.7f, -0.85f,  0.0f, 1.0f
    };
    unsigned indexTexture = loadImageToTexture("index.png");
    initializeTexture(VAO, VBO, indexVertices, sizeof(indexVertices), indexTexture);

    // fire
    Model fire("res/fire/fire.obj");
    // sea
    Model sea("res/sea/Ocean.obj");
    // island
    Model island("res/island/Base.obj");
    // palmtree
    Model palmtree("res/palm/MY_PALM.obj");
    // clouds
    Model cloud("res/cloud/Cloud_Hipoly.obj");
    // fishes
    Model fish("res/shark/shark.obj");

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++            UNIFORME            +++++++++++++++++++++++++++++++++++++++++++++++++

    glm::mat4 model = glm::mat4(1.0f); //Matrica transformacija - mat4(1.0f) generise jedinicnu matricu
    unifiedShader.setMat4("uM", model);
    
    glm::mat4 view; //Matrica pogleda (kamere)
    glm::vec3 camPosition = glm::vec3(2.69997f, 52.8998f, 68.5003f);
    glm::vec3 camOrientation = glm::vec3(-0.400004f, -17.1f, -22.1f);
    glm::vec3 camRotation = glm::vec3(0.0f, 1.0f, 0.0f);
    view = glm::lookAt(camPosition, camOrientation, camRotation); // lookAt(Gdje je kamera, u sta kamera gleda, jedinicni vektor pozitivne Y ose svijeta  - ovo rotira kameru)
    unifiedShader.setMat4("uV", view);

    glm::mat4 projectionP = glm::perspective(glm::radians(45.0f), (float) aspectRatio, 0.1f, 100.0f); //Matrica perspektivne projekcije (FOV, Aspect Ratio, prednja ravan, zadnja ravan)
    glm::mat4 projectionO = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -15.0f, 100.0f); //Matrica ortogonalne projekcije (Lijeva, desna, donja, gornja, prednja i zadnja ravan)
    unifiedShader.setMat4("uP", projectionO);


    unifiedShader.setVec3("uLightPos", 0, 1, 3);
    unifiedShader.setVec3("uViewPos", 0, 0, 5);
    unifiedShader.setVec3("uLightColor", 1, 1, 1);

    clock_t lastKeyPressTime = clock();
    float currentTime = 0.0f;
    float speed = 0.001;
    float r = 1.0;
    float sunPosition = 1.0f;
    glm::mat4 translationMatrix;

    while (!glfwWindowShouldClose(window))
    {

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE); 
        glCullFace(GL_BACK);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Osvjezavamo i Z bafer i bafer boje

        bool speedKeyPressed = false;
        bool firstClick = true;
        float sensitivity = 100.0f;
        float angle = currentTime * speed;
        currentTime += 1;

        // handle keyboard input: A S D W, UP DOWN LEFT RIGHT,  O P,  + - R,  1 2 3, esc
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            clock_t currentTime = clock();
            double elapsedTime = static_cast<double>(currentTime - lastKeyPressTime) / CLOCKS_PER_SEC;
            if (!speedKeyPressed && elapsedTime > 0.01) {
                speedKeyPressed = true;  // Set the flag to true to indicate key press
                lastKeyPressTime = currentTime;  // Update the last key press time
                camPosition = glm::vec3(camPosition.x - 0.1, camPosition.y, camPosition.z);
                camOrientation = glm::vec3(camOrientation.x - 0.1, camOrientation.y, camOrientation.z);
                view = glm::lookAt(camPosition, camOrientation, camRotation); // lookAt(Gdje je kamera, u sta kamera gleda, jedinicni vektor pozitivne Y ose svijeta  - ovo rotira kameru)
                unifiedShader.setMat4("uV", view);
            }

        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            clock_t currentTime = clock();
            double elapsedTime = static_cast<double>(currentTime - lastKeyPressTime) / CLOCKS_PER_SEC;

            if (!speedKeyPressed && elapsedTime > 0.01) {
                speedKeyPressed = true;  // Set the flag to true to indicate key press
                lastKeyPressTime = currentTime;  // Update the last key press time
                camPosition = glm::vec3(camPosition.x + 0.1, camPosition.y, camPosition.z);
                camOrientation = glm::vec3(camOrientation.x + 0.1, camOrientation.y, camOrientation.z);
                view = glm::lookAt(camPosition, camOrientation, camRotation); // lookAt(Gdje je kamera, u sta kamera gleda, jedinicni vektor pozitivne Y ose svijeta  - ovo rotira kameru)
                unifiedShader.setMat4("uV", view);

            }

        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {


            clock_t currentTime = clock();
            double elapsedTime = static_cast<double>(currentTime - lastKeyPressTime) / CLOCKS_PER_SEC;

            if (!speedKeyPressed && elapsedTime > 0.01) {
                speedKeyPressed = true;  // Set the flag to true to indicate key press
                lastKeyPressTime = currentTime;  // Update the last key press time
                camPosition = glm::vec3(camPosition.x, camPosition.y + 0.1, camPosition.z);
                camOrientation = glm::vec3(camOrientation.x, camOrientation.y + 0.1, camOrientation.z);
                view = glm::lookAt(camPosition, camOrientation, camRotation); // lookAt(Gdje je kamera, u sta kamera gleda, jedinicni vektor pozitivne Y ose svijeta  - ovo rotira kameru)
                unifiedShader.setMat4("uV", view);

            }
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            clock_t currentTime = clock();
            double elapsedTime = static_cast<double>(currentTime - lastKeyPressTime) / CLOCKS_PER_SEC;

            if (!speedKeyPressed && elapsedTime > 0.01) {
                speedKeyPressed = true;  // Set the flag to true to indicate key press
                lastKeyPressTime = currentTime;  // Update the last key press time
                camPosition = glm::vec3(camPosition.x, camPosition.y - 0.1, camPosition.z);
                camOrientation = glm::vec3(camOrientation.x, camOrientation.y - 0.1, camOrientation.z);
                view = glm::lookAt(camPosition, camOrientation, camRotation); // lookAt(Gdje je kamera, u sta kamera gleda, jedinicni vektor pozitivne Y ose svijeta  - ovo rotira kameru)
                unifiedShader.setMat4("uV", view);

            }
        }
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {


            clock_t currentTime = clock();
            double elapsedTime = static_cast<double>(currentTime - lastKeyPressTime) / CLOCKS_PER_SEC;

            if (!speedKeyPressed && elapsedTime > 0.01) {
                speedKeyPressed = true;  // Set the flag to true to indicate key press
                lastKeyPressTime = currentTime;  // Update the last key press time
                camPosition = glm::vec3(camPosition.x, camPosition.y, camPosition.z + 0.1);
                camOrientation = glm::vec3(camOrientation.x, camOrientation.y, camOrientation.z + 0.1);


                view = glm::lookAt(camPosition, camOrientation, camRotation); // lookAt(Gdje je kamera, u sta kamera gleda, jedinicni vektor pozitivne Y ose svijeta  - ovo rotira kameru)
                unifiedShader.setMat4("uV", view);

            }
        }
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
            clock_t currentTime = clock();
            double elapsedTime = static_cast<double>(currentTime - lastKeyPressTime) / CLOCKS_PER_SEC;

            if (!speedKeyPressed && elapsedTime > 0.01) {
                speedKeyPressed = true;  // Set the flag to true to indicate key press
                lastKeyPressTime = currentTime;  // Update the last key press time
                camPosition = glm::vec3(camPosition.x, camPosition.y, camPosition.z - 0.1);
                camOrientation = glm::vec3(camOrientation.x, camOrientation.y, camOrientation.z - 0.1);
                view = glm::lookAt(camPosition, camOrientation, camRotation); // lookAt(Gdje je kamera, u sta kamera gleda, jedinicni vektor pozitivne Y ose svijeta  - ovo rotira kameru)
                unifiedShader.setMat4("uV", view);

            }
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            clock_t currentTime = clock();
            double elapsedTime = static_cast<double>(currentTime - lastKeyPressTime) / CLOCKS_PER_SEC;

            if (!speedKeyPressed && elapsedTime > 0.01) {
                speedKeyPressed = true;  // Set the flag to true to indicate key press
                lastKeyPressTime = currentTime;  // Update the last key press time
                camOrientation = glm::vec3(camOrientation.x - 0.1, camOrientation.y, camOrientation.z);
                view = glm::lookAt(camPosition, camOrientation, camRotation); // lookAt(Gdje je kamera, u sta kamera gleda, jedinicni vektor pozitivne Y ose svijeta  - ovo rotira kameru)
                unifiedShader.setMat4("uV", view);

            }
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            clock_t currentTime = clock();
            double elapsedTime = static_cast<double>(currentTime - lastKeyPressTime) / CLOCKS_PER_SEC;

            if (!speedKeyPressed && elapsedTime > 0.01) {
                speedKeyPressed = true;  // Set the flag to true to indicate key press
                lastKeyPressTime = currentTime;  // Update the last key press time
                camOrientation = glm::vec3(camOrientation.x + 0.1, camOrientation.y, camOrientation.z);
                view = glm::lookAt(camPosition, camOrientation, camRotation); // lookAt(Gdje je kamera, u sta kamera gleda, jedinicni vektor pozitivne Y ose svijeta  - ovo rotira kameru)
                unifiedShader.setMat4("uV", view);

            }
        }
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            clock_t currentTime = clock();
            double elapsedTime = static_cast<double>(currentTime - lastKeyPressTime) / CLOCKS_PER_SEC;

            if (!speedKeyPressed && elapsedTime > 0.01) {
                speedKeyPressed = true;  // Set the flag to true to indicate key press
                lastKeyPressTime = currentTime;  // Update the last key press time
                camOrientation = glm::vec3(camOrientation.x , camOrientation.y + 0.1, camOrientation.z);
                view = glm::lookAt(camPosition, camOrientation, camRotation); // lookAt(Gdje je kamera, u sta kamera gleda, jedinicni vektor pozitivne Y ose svijeta  - ovo rotira kameru)
                unifiedShader.setMat4("uV", view);

            }
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            clock_t currentTime = clock();
            double elapsedTime = static_cast<double>(currentTime - lastKeyPressTime) / CLOCKS_PER_SEC;

            if (!speedKeyPressed && elapsedTime > 0.01) {
                speedKeyPressed = true;  // Set the flag to true to indicate key press
                lastKeyPressTime = currentTime;  // Update the last key press time
                camOrientation = glm::vec3(camOrientation.x , camOrientation.y - 0.1, camOrientation.z);
                view = glm::lookAt(camPosition, camOrientation, camRotation); // lookAt(Gdje je kamera, u sta kamera gleda, jedinicni vektor pozitivne Y ose svijeta  - ovo rotira kameru)
                unifiedShader.setMat4("uV", view);

            }
        }
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            glEnable(GL_PROGRAM_POINT_SIZE);
            glPointSize(4);
        }
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
        {
            clock_t currentTime = clock();
            double elapsedTime = static_cast<double>(currentTime - lastKeyPressTime) / CLOCKS_PER_SEC;

            if (!speedKeyPressed && elapsedTime > 0.2) {
                if (speed <= 0.01) {
                    speed += 0.001;
                    speedKeyPressed = true;  // Set the flag to true to indicate key press
                    lastKeyPressTime = currentTime;  // Update the last key press time
                }
            }
   
        }
        if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
        {
            clock_t currentTime = clock();
            double elapsedTime = static_cast<double>(currentTime - lastKeyPressTime) / CLOCKS_PER_SEC;

            if (!speedKeyPressed && elapsedTime > 0.2) {
                if (speed >= 0.0001) {
                    speed -= 0.0001;
                    speedKeyPressed = true;  // Set the flag to true to indicate key press
                    lastKeyPressTime = currentTime;  // Update the last key press time
                }
            }
        }
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        {
            speed = 0.001;
        }
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        {
            camPosition = glm::vec3(2.8f, 5.6f, 8.6f);
            camOrientation = glm::vec3(2.1f, 4.1f, 6.6f);
            camRotation = glm::vec3(0.0f, 1.0f, 0.0f);

            view = glm::lookAt(camPosition, camOrientation, camRotation); // lookAt(Gdje je kamera, u sta kamera gleda, jedinicni vektor pozitivne Y ose svijeta  - ovo rotira kameru)
            unifiedShader.setMat4("uV", view);
            unifiedShader.setMat4("uP", projectionP);
        }
        if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        {
            camPosition = glm::vec3(2.69997f, 52.8998f, 68.5003f);
            camOrientation = glm::vec3(-0.400004f, -17.1f, -22.1f);
            camRotation = glm::vec3(0.0f, 1.0f, 0.0f);

            view = glm::lookAt(camPosition, camOrientation, camRotation); // lookAt(Gdje je kamera, u sta kamera gleda, jedinicni vektor pozitivne Y ose svijeta  - ovo rotira kameru)
            unifiedShader.setMat4("uV", view);
            unifiedShader.setMat4("uP", projectionO);

        }


        //sky and light
        float t = (sin(glfwGetTime() / 2) + 1.0f) / 2.0f;

        float dirLightPosX = sin(currentTime * sunPosition);
        float dirLightPosY = cos(currentTime * sunPosition);
        float dirLightPosZ = 0.0f;

        glClearColor(0.7, 0.7, 1.0, 1.0);

         //pamltree
        unifiedShader.setBool("model", true);

        unifiedShader.setInt("mode", 1);
        translationMatrix = glm::translate(model, glm::vec3(0.8f, 0.0f, 0.8f));
        unifiedShader.setMat4("translationMatrix", translationMatrix);
        unifiedShader.setFloat("scale", 0.005);
        palmtree.Draw(unifiedShader);

        // sea
        float seaPos = 0.01 * sin(glfwGetTime() * speed * 500);
        unifiedShader.setFloat("seaPos", seaPos);
        translationMatrix = glm::translate(model, glm::vec3(-5.0f, 0.3f, -2.5f));
        unifiedShader.setMat4("translationMatrix", translationMatrix);
        unifiedShader.setFloat("scale", 10.0);
        sea.Draw(unifiedShader);

        // islands 
        unifiedShader.setInt("mode", 2);
        translationMatrix = glm::translate(model, glm::vec3(1.0f, -0.19f, 1.0f));
        unifiedShader.setMat4("translationMatrix", translationMatrix);
        unifiedShader.setFloat("scale", 0.2);
        island.Draw(unifiedShader);

        translationMatrix = glm::translate(model, glm::vec3(-7.7f, -0.19f, -7.0f));
        unifiedShader.setMat4("translationMatrix", translationMatrix);
        unifiedShader.setFloat("scale", 0.08);
        island.Draw(unifiedShader);

        unifiedShader.setBool("model", false);




       // fishes
       unifiedShader.setInt("mode", 3);


        glm::vec3 translation[] = {
            glm::vec3(1.0f, -0.7f , 1.0f),
            glm::vec3(-7.7f, -0.415f, -7.0f),
            glm::vec3(1.0f, -0.4f, 1.0f),
        };
        glm::vec4 rotation[] = {
            glm::vec4(-sin(angle) * 5.0, 0.0f, cos(angle) * 5.0, 1.0f),
            glm::vec4(-sin(angle) * 3.0, 0.0f, cos(angle) * 3.0, 1.0f),
            glm::vec4(-sin(angle) * 7.0, 0.0f, cos(angle) * 7.0, 1.0f)
        };
        float scale[] = { 1.0, 0.5, 0.8 };

        for (int i = 0; i < 3; i++) {
            translationMatrix = glm::translate(model, translation[i]);
            unifiedShader.setMat4("translationMatrix", translationMatrix);
            
            glm::mat4 rotationMatrix = glm::mat4(
                glm::vec4(cos(angle), 0.0f, sin(angle), 0.0f),
                glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
                glm::vec4(-sin(angle), 0.0f, cos(angle), 0.0f),
                rotation[i]
            );
            unifiedShader.setMat4("rotationMatrix", rotationMatrix);
            
            unifiedShader.setFloat("scale",scale[i]);

            fish.Draw(unifiedShader);
        }



        // clouds
        unifiedShader.setInt("mode", 4);

        glm::vec4 cloudVec[] = {
            glm::vec4(-8.0f, 6.5f, -7.0f, 1.0f),
            glm::vec4(-4.0f, 6.5f, 6.0f, 1.0f),
            glm::vec4(-0.5f, 6.0f, 0.5f, 1.0f),
            glm::vec4(4.0f, 6.5f, -4.0f, 1.0f),
            glm::vec4(2.0f, 6.0f, 9.0f, 1.0f)
        };
        for (int i = 0; i < sizeof(cloudVec); ++i) {
            float xTranslation = currentTime * speed;
            xTranslation = fmod(xTranslation, 5.0f);
            float newx = cloudVec[i][0] - 3.0f + xTranslation;

            translationMatrix = glm::translate(model, glm::vec3(newx, cloudVec[i][1], cloudVec[i][2]));
            unifiedShader.setMat4("translationMatrix", translationMatrix);
            unifiedShader.setFloat("scale", 0.4);
            cloud.Draw(unifiedShader);
        }

        // fire

        unifiedShader.setInt("mode", 5);
        unifiedShader.setFloat("firePos", 1.2 + 0.3 * sin(glfwGetTime() * speed * 500));
        translationMatrix = glm::translate(model, glm::vec3(1.3f, 0.0f, 1.3f));
        unifiedShader.setMat4("translationMatrix", translationMatrix);
        unifiedShader.setFloat("scale", 0.2);
        fire.Draw(unifiedShader); 


        // index 
        glBindVertexArray(VAO);
        unifiedShader.setInt("mode", 0);
        unifiedShader.setBool("useTexture", true);
        glBindTexture(GL_TEXTURE_2D, indexTexture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, indexTexture);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        unifiedShader.setBool("useTexture", false);

        //swap
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //empty and end
    glDeleteTextures(1, &indexTexture);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(unifiedShader.ID);
    glfwTerminate();
    return 0;
}

static unsigned loadImageToTexture(const char* filePath) {
    int TextureWidth;
    int TextureHeight;
    int TextureChannels;
    unsigned char* ImageData = stbi_load(filePath, &TextureWidth, &TextureHeight, &TextureChannels, 0);
    if (ImageData != NULL)
    {
        //Slike se osnovno ucitavaju naopako pa se moraju ispraviti da budu uspravne
        stbi__vertical_flip(ImageData, TextureWidth, TextureHeight, TextureChannels);
        // Provjerava koji je format boja ucitane slike
        GLint InternalFormat = -1;
        switch (TextureChannels) {
        case 1: InternalFormat = GL_RED; break;
        case 3: InternalFormat = GL_RGB; break;
        case 4: InternalFormat = GL_RGBA; break;
        default: InternalFormat = GL_RGB; break;
        }

        unsigned int Texture;
        glGenTextures(1, &Texture);
        glBindTexture(GL_TEXTURE_2D, Texture);
        glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, TextureWidth, TextureHeight, 0, InternalFormat, GL_UNSIGNED_BYTE, ImageData);
        glBindTexture(GL_TEXTURE_2D, 0);
        // oslobadjanje memorije zauzete sa stbi_load posto vise nije potrebna
        stbi_image_free(ImageData);
        return Texture;
    }
    else
    {
        std::cout << "Textura nije ucitana! Putanja texture: " << filePath << std::endl;
        stbi_image_free(ImageData);
        return 0;
    }
}

void initializeTexture(unsigned int VAO, unsigned int VBO, float* vertices, int verticesCount, unsigned indexTexture) {
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesCount, vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(4);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, indexTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}
