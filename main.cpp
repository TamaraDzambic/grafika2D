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



unsigned int compileShader(GLenum type, const char* source);
unsigned int createShader(const char* vsSource, const char* fsSource); 

static unsigned loadImageToTexture(const char* filePath); 
void initializeTexture(unsigned int VAO, unsigned int VBO, float* vertices, int verticesCount, unsigned indexTexture);

int main(void)
{
    //initialization
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



    //shader and buffers
    Shader unifiedShader("basic.vert", "basic.frag");
    unifiedShader.use();



    unsigned int VAO[9];
    glGenVertexArrays(9, VAO);
    unsigned int VBO[9];
    glGenBuffers(9, VBO);

    //sky
    float skyVerticex[] = {
        -1.0, -1.0, -0.2,
        1.0, -1.0, -0.2,
        -1.0, 1.0, -0.2,
        1.0, 1.0, -0.2,
    }; 
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyVerticex), skyVerticex, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //sun and moon

    float vertices[] =
    {
        //X    Y    Z       R    G    B    A
        0.25, 0.5, 0.75,   1.0, 0.0, 0.0, 0.0, //Crveni trougao - Prednji
       -0.25, 0.5, 0.75,   1.0, 0.0, 0.0, 0.0,
        0.0, -0.5, 0.75,   1.0, 0.0, 0.0, 0.0,

        0.25, -0.5, 0.0,   0.0, 1.0, 1.0, 0.0, //Plavi trougao - Zadnji
       -0.25, -0.5, 0.0,   0.0, 1.0, 1.0, 0.0,
        0.0,   0.5, 0.0,   0.0, 1.0, 1.0, 0.0
    };
    unsigned int stride = (3 + 4) * sizeof(float);

    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // sea
    float seaVerticex[]  = {
    -5.5f,  0.0f, -5.5f,  0.0f,  0.0f,  1.0f,
     5.5f,  0.0f, -5.5f,  0.0f,  0.0f,  1.0f,
     5.5f,  0.0f,  5.5f,  0.0f,  0.0f,  1.0f,
     5.5f,  0.0f,  5.5f,  0.0f,  0.0f,  1.0f,
    -5.5f,  0.0f,  5.5f,  0.0f,  0.0f,  1.0f,
    -5.5f,  0.0f, -5.5f,  0.0f,  0.0f,  1.0f
    };
    glBindVertexArray(VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(seaVerticex), seaVerticex, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // index texture
    float indexVertices[] = {
        1.0f, -1.0f,  1.0f, 0.0f,
        1.0f, -0.85f,  1.0f, 1.0f,
        0.7f, -1.0f,  0.0f, 0.0f,
        0.7f, -0.85f,  0.0f, 1.0f
    };
    unsigned indexTexture = loadImageToTexture("index.png");
    initializeTexture(VAO[3], VBO[3], indexVertices, sizeof(indexVertices), indexTexture);


    // islands


    // palmtree


    // fire


    // clouds

   
    // fishes
    Model fish("res/uploads_files_3373269_shark.obj");
    Model fish2("res/uploads_files_3373269_shark.obj");
    float currentTime = 0.0f;



    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++            UNIFORME            +++++++++++++++++++++++++++++++++++++++++++++++++

    glm::mat4 model = glm::mat4(1.0f); //Matrica transformacija - mat4(1.0f) generise jedinicnu matricu
    unifiedShader.setMat4("uM", model);
    glm::mat4 view; //Matrica pogleda (kamere)

    glm::vec3 camPosition = glm::vec3(1.0f, 1.0f, 8.6f);
    glm::vec3 camOrientation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 camRotation = glm::vec3(0.0f, 1.0f, 0.0f);

    view = glm::lookAt(camPosition, camOrientation, camRotation); // lookAt(Gdje je kamera, u sta kamera gleda, jedinicni vektor pozitivne Y ose svijeta  - ovo rotira kameru)
    
    
    unifiedShader.setMat4("uV", view);


    glm::mat4 projectionP = glm::perspective(glm::radians(45.0f), (float) aspectRatio, 0.1f, 100.0f); //Matrica perspektivne projekcije (FOV, Aspect Ratio, prednja ravan, zadnja ravan)
    glm::mat4 projectionO = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f); //Matrica ortogonalne projekcije (Lijeva, desna, donja, gornja, prednja i zadnja ravan)
    unifiedShader.setMat4("uP", projectionO);




    clock_t lastKeyPressTime = clock();

    unifiedShader.setVec3("uLightPos", 0, 1, 3);
    unifiedShader.setVec3("uViewPos", 0, 0, 5);
    unifiedShader.setVec3("uLightColor", 1, 1, 1);

    float speed = 0.001;
    float r = 1.0;
    unifiedShader.setBool("useTexture", false);
    unifiedShader.setInt("mode", 0);

    while (!glfwWindowShouldClose(window))
    {

        glEnable(GL_DEPTH_TEST);
        glCullFace(GL_BACK);
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Osvjezavamo i Z bafer i bafer boje

        bool speedKeyPressed = false;
       
        bool firstClick = true;
        float sensitivity = 100.0f;



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

            if (!speedKeyPressed && elapsedTime > 0.01) {
                if (speed <= 2.0) {
                    speed += 0.2;
                    speedKeyPressed = true;  // Set the flag to true to indicate key press
                    lastKeyPressTime = currentTime;  // Update the last key press time
                }
            }
   
        }
        if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
        {
            clock_t currentTime = clock();
            double elapsedTime = static_cast<double>(currentTime - lastKeyPressTime) / CLOCKS_PER_SEC;

            if (!speedKeyPressed && elapsedTime > 0.01) {
                if (speed >= 0.2) {
                    speed -= 0.2;
                    speedKeyPressed = true;  // Set the flag to true to indicate key press
                    lastKeyPressTime = currentTime;  // Update the last key press time
                }
            }
        }
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        {
            speed = 1.0;
        }

        if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
        {
            std::cout << camPosition.x << " cam pos x " << camOrientation.x << " cam ori x" << std::endl;
            std::cout << camPosition.y << " cam pos y " << camOrientation.y << " cam ori y" << std::endl;
            std::cout << camPosition.z << " cam pos z " << camOrientation.z << " cam ori z" << std::endl;

        }

        //Mijenjanje projekcija
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
            camPosition = glm::vec3(10.0f,10.0f, 8.6f);
            camOrientation = glm::vec3(0.0f, 0.0f, 0.0f);
            camRotation = glm::vec3(0.0f, 1.0f, 0.0f);

            view = glm::lookAt(camPosition, camOrientation, camRotation); // lookAt(Gdje je kamera, u sta kamera gleda, jedinicni vektor pozitivne Y ose svijeta  - ovo rotira kameru)


            unifiedShader.setMat4("uV", view);
            unifiedShader.setMat4("uP", projectionO);

        }




        ////sky 
        //glBindVertexArray(VAO[0]);
        //glUniform1i(modeLoc, 0);
        //glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        //sun, moon


        //sea 
        glBindVertexArray(VAO[2]);
        unifiedShader.setInt("mode", 2);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        //island1 


        //fishes

        //big fish
        unifiedShader.setInt("mode", 8);
        currentTime += 1;
        float angle = currentTime * speed;
        glm::mat4 translationMatrix = glm::mat4(
            glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
            glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
            glm::vec4(0.0f, 0.0f, 1.0f, 0.0f),
            glm::vec4(1.0f, -0.3f, 1.0f, 1.0f)
        );
        unifiedShader.setMat4("translationMatrix", translationMatrix);
        glm::mat4 rotationMatrix = glm::mat4(
            glm::vec4(cos(angle), 0.0f, sin(angle), 0.0f),
            glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
            glm::vec4(-sin(angle), 0.0f, cos(angle), 0.0f),
            glm::vec4(-sin(angle) * 2.5, 0.0f, cos(angle) * 2.5, 1.0f)
        );
        unifiedShader.setMat4("rotationMatrix", rotationMatrix);
        unifiedShader.setFloat("scale", 0.4);

        fish.Draw(unifiedShader);

        //small fish
        glm::mat4 translationMatrix2 = glm::mat4(
            glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
            glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
            glm::vec4(0.0f, 0.0f, 1.0f, 0.0f),
            glm::vec4(-2.7f, -0.15f, -3.0f, 1.0f)
        );
        unifiedShader.setMat4("translationMatrix", translationMatrix2);
        glm::mat4 rotationMatrix2 = glm::mat4(
            glm::vec4(-cos(angle), 0.0f, -sin(angle), 0.0f),
            glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
            glm::vec4(sin(angle), 0.0f, -cos(angle), 0.0f),
            glm::vec4(sin(angle) * 1.2, 0.0f, -cos(angle) * 1.2, 1.0f)
        );
        unifiedShader.setMat4("rotationMatrix", rotationMatrix2);
        unifiedShader.setFloat("scale", 0.2);

        fish2.Draw(unifiedShader);




        //island2


        //pamltree


        // fire


        // index texture
        unifiedShader.setInt("mode", 3);

        glBindVertexArray(VAO[3]);
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
    glDeleteBuffers(1, VBO);
    glDeleteVertexArrays(1, VAO);
    glDeleteProgram(unifiedShader.ID);
    glfwTerminate();
    return 0;
}

unsigned int compileShader(GLenum type, const char* source)
{
    std::string content = "";
    std::ifstream file(source);
    std::stringstream ss;
    if (file.is_open())
    {
        ss << file.rdbuf();
        file.close();
        std::cout << "Uspjesno procitao fajl sa putanje \"" << source << "\"!" << std::endl;
    }
    else {
        ss << "";
        std::cout << "Greska pri citanju fajla sa putanje \"" << source << "\"!" << std::endl;
    }
    std::string temp = ss.str();
    const char* sourceCode = temp.c_str();

    int shader = glCreateShader(type);

    int success;
    char infoLog[512];
    glShaderSource(shader, 1, &sourceCode, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        if (type == GL_VERTEX_SHADER)
            printf("VERTEX");
        else if (type == GL_FRAGMENT_SHADER)
            printf("FRAGMENT");
        printf(" sejder ima gresku! Greska: \n");
        printf(infoLog);
    }
    return shader;
}

unsigned int createShader(const char* vsSource, const char* fsSource){

    unsigned int program;
    unsigned int vertexShader;
    unsigned int fragmentShader;

    program = glCreateProgram();

    vertexShader = compileShader(GL_VERTEX_SHADER, vsSource);
    fragmentShader = compileShader(GL_FRAGMENT_SHADER, fsSource);


    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);
    glValidateProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(program, 512, NULL, infoLog);
        std::cout << "Objedinjeni sejder ima gresku! Greska: \n";
        std::cout << infoLog << std::endl;
    }

    glDetachShader(program, vertexShader);
    glDeleteShader(vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(fragmentShader);

    return program;
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
