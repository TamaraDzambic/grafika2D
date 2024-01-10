// Autor: Tamara Dzambic
// Opis:  projekat 12


#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>

#include <GL/glew.h>   
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cmath>
#define M_PI 3.14159265358979323846
#define _CRT_SECURE_NO_WARNINGS
#define CRES 30 

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned int compileShader(GLenum type, const char* source);
unsigned int createShader(const char* vsSource, const char* fsSource); 
static unsigned loadImageToTexture(const char* filePath); 


void initializeTexture(unsigned int VAO, unsigned int VBO, float* vertices, int verticesCount, unsigned indexTexture) {
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesCount, vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(2);
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
    unsigned int unifiedShader = createShader("basic.vert", "basic.frag");

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
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // sea
    float seaVerticex[]  = {
         1.0, 0.0, 1.0, 
         1.0, 0.0, -1.0, 
        -1.0, 0.0, 1.0,  
        -1.0, 0.0, -1.0, 
    };
    glBindVertexArray(VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(seaVerticex), seaVerticex, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


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




    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++            UNIFORME            +++++++++++++++++++++++++++++++++++++++++++++++++

    glm::mat4 model = glm::mat4(1.0f); //Matrica transformacija - mat4(1.0f) generise jedinicnu matricu
    unsigned int modelLoc = glGetUniformLocation(unifiedShader, "uM");

    glm::mat4 view; //Matrica pogleda (kamere)
    glm::vec3 camPosition = glm::vec3(1.0f, 1.0f, 2.0f);
    glm::vec3 camOrientation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 camRotation = glm::vec3(0.0f, 1.0f, 0.0f);

    view = glm::lookAt(camPosition, camOrientation, camRotation); // lookAt(Gdje je kamera, u sta kamera gleda, jedinicni vektor pozitivne Y ose svijeta  - ovo rotira kameru)
    
    
    unsigned int viewLoc = glGetUniformLocation(unifiedShader, "uV");


    glm::mat4 projectionP = glm::perspective(glm::radians(45.0f), (float)mode -> width / (float)mode ->height, -0.5f, 0.5f); //Matrica perspektivne projekcije (FOV, Aspect Ratio, prednja ravan, zadnja ravan)
    glm::mat4 projectionO = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f); //Matrica ortogonalne projekcije (Lijeva, desna, donja, gornja, prednja i zadnja ravan)
    unsigned int projectionLoc = glGetUniformLocation(unifiedShader, "uP");


    GLuint useTextureLoc = glGetUniformLocation(unifiedShader, "useTexture");
    GLuint modeLoc = glGetUniformLocation(unifiedShader, "mode");
    clock_t lastKeyPressTime = clock();

    glUseProgram(unifiedShader);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); //(Adresa matrice, broj matrica koje saljemo, da li treba da se transponuju, pokazivac do matrica)
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionO));




    float speed = 0.01;
    float r = 1.0;

    while (!glfwWindowShouldClose(window))
    {


        glEnable(GL_DEPTH_TEST);
        glCullFace(GL_BACK);
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Osvjezavamo i Z bafer i bafer boje

        handleInput(window, width, height, camPosition, camPosition, camRotation, view, viewLoc);

        glUniform1i(useTextureLoc, GL_FALSE);


        ////sky 
        //glBindVertexArray(VAO[0]);
        //glUniform1i(modeLoc, 0);
        //glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        //sun, moon
        glBindVertexArray(VAO[1]);
        glUniform1i(modeLoc, 12);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        //sea 
        glBindVertexArray(VAO[2]);
        glUniform1i(modeLoc, 2);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        //island1 


        //fishes


        //island2


        //pamltree


        // fire


        // index texture
        glUniform1i(modeLoc, 3);

        glBindVertexArray(VAO[3]);
        glUniform1i(useTextureLoc, GL_TRUE);
        glBindTexture(GL_TEXTURE_2D, indexTexture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, indexTexture);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glUniform1i(useTextureLoc, GL_FALSE);


        //swap
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //empty and end
    glDeleteTextures(1, &indexTexture);
    glDeleteBuffers(1, VBO);
    glDeleteVertexArrays(1, VAO);
    glDeleteProgram(unifiedShader);
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

void handleInput(GLFWwindow *window, float width, float height, glm::vec3 *camPosition, glm::vec3 *camOrientation, glm::vec3 *camRotation, glm::vec3 *view, int viewLoc) {
    bool speedKeyPressed = false;

    bool firstClick = true;
    float sensitivity = 100.0f;




    // Handles mouse inputs
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        // Hides mouse cursor
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        // Prevents camera from jumping on the first click
        if (firstClick)
        {
            glfwSetCursorPos(window, (width / 2), (height / 2));
            firstClick = false;
        }

        // Stores the coordinates of the cursor
        double mouseX;
        double mouseY;
        // Fetches the coordinates of the cursor
        glfwGetCursorPos(window, &mouseX, &mouseY);

        // Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
        // and then "transforms" them into degrees 
        float rotY = sensitivity * (float)(mouseY - (height / 2)) / height;
        float rotX = sensitivity * (float)(mouseX - (width / 2)) / width;

        camOrientation = glm::vec3(camOrientation.x + rotX, camOrientation.y - rotY, camOrientation.z);
        view = glm::lookAt(camPosition, camOrientation, camRotation); // lookAt(Gdje je kamera, u sta kamera gleda, jedinicni vektor pozitivne Y ose svijeta  - ovo rotira kameru)
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));


        // Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
        glfwSetCursorPos(window, (width / 2), (height / 2));
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        // Unhides cursor since camera is not looking around anymore
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        // Makes sure the next time the camera looks around it doesn't jump
        firstClick = true;
    }




    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {

        clock_t currentTime = clock();
        double elapsedTime = static_cast<double>(currentTime - lastKeyPressTime) / CLOCKS_PER_SEC;

        if (!speedKeyPressed && elapsedTime > 0.2) {
            speedKeyPressed = true;  // Set the flag to true to indicate key press
            lastKeyPressTime = currentTime;  // Update the last key press time
            camPosition = glm::vec3(camPosition.x - 0.1, camPosition.y, camPosition.z);
            camOrientation = glm::vec3(camOrientation.x - 0.1, camOrientation.y, camOrientation.z);
            view = glm::lookAt(camPosition, camOrientation, camRotation); // lookAt(Gdje je kamera, u sta kamera gleda, jedinicni vektor pozitivne Y ose svijeta  - ovo rotira kameru)
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        }

    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        clock_t currentTime = clock();
        double elapsedTime = static_cast<double>(currentTime - lastKeyPressTime) / CLOCKS_PER_SEC;

        if (!speedKeyPressed && elapsedTime > 0.2) {
            speedKeyPressed = true;  // Set the flag to true to indicate key press
            lastKeyPressTime = currentTime;  // Update the last key press time
            camPosition = glm::vec3(camPosition.x + 0.1, camPosition.y, camPosition.z);
            camOrientation = glm::vec3(camOrientation.x + 0.1, camOrientation.y, camOrientation.z);
            view = glm::lookAt(camPosition, camOrientation, camRotation); // lookAt(Gdje je kamera, u sta kamera gleda, jedinicni vektor pozitivne Y ose svijeta  - ovo rotira kameru)
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        }

    }


    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {


        clock_t currentTime = clock();
        double elapsedTime = static_cast<double>(currentTime - lastKeyPressTime) / CLOCKS_PER_SEC;

        if (!speedKeyPressed && elapsedTime > 0.2) {
            speedKeyPressed = true;  // Set the flag to true to indicate key press
            lastKeyPressTime = currentTime;  // Update the last key press time
            camPosition = glm::vec3(camPosition.x, camPosition.y + 0.1, camPosition.z);
            camOrientation = glm::vec3(camOrientation.x, camOrientation.y + 0.1, camOrientation.z);
            view = glm::lookAt(camPosition, camOrientation, camRotation); // lookAt(Gdje je kamera, u sta kamera gleda, jedinicni vektor pozitivne Y ose svijeta  - ovo rotira kameru)
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        }
    }


    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        clock_t currentTime = clock();
        double elapsedTime = static_cast<double>(currentTime - lastKeyPressTime) / CLOCKS_PER_SEC;

        if (!speedKeyPressed && elapsedTime > 0.2) {
            speedKeyPressed = true;  // Set the flag to true to indicate key press
            lastKeyPressTime = currentTime;  // Update the last key press time
            camPosition = glm::vec3(camPosition.x, camPosition.y - 0.1, camPosition.z);
            camOrientation = glm::vec3(camOrientation.x, camOrientation.y - 0.1, camOrientation.z);
            view = glm::lookAt(camPosition, camOrientation, camRotation); // lookAt(Gdje je kamera, u sta kamera gleda, jedinicni vektor pozitivne Y ose svijeta  - ovo rotira kameru)
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        }
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        clock_t currentTime = clock();
        double elapsedTime = static_cast<double>(currentTime - lastKeyPressTime) / CLOCKS_PER_SEC;

        if (!speedKeyPressed && elapsedTime > 0.2) {
            speedKeyPressed = true;  // Set the flag to true to indicate key press
            lastKeyPressTime = currentTime;  // Update the last key press time
            camOrientation = glm::vec3(camOrientation.x - 0.1, camOrientation.y, camOrientation.z);
            view = glm::lookAt(camPosition, camOrientation, camRotation); // lookAt(Gdje je kamera, u sta kamera gleda, jedinicni vektor pozitivne Y ose svijeta  - ovo rotira kameru)
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        }
    }


    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        clock_t currentTime = clock();
        double elapsedTime = static_cast<double>(currentTime - lastKeyPressTime) / CLOCKS_PER_SEC;

        if (!speedKeyPressed && elapsedTime > 0.2) {
            speedKeyPressed = true;  // Set the flag to true to indicate key press
            lastKeyPressTime = currentTime;  // Update the last key press time
            camOrientation = glm::vec3(camOrientation.x + 0.1, camOrientation.y, camOrientation.z);
            view = glm::lookAt(camPosition, camOrientation, camRotation); // lookAt(Gdje je kamera, u sta kamera gleda, jedinicni vektor pozitivne Y ose svijeta  - ovo rotira kameru)
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        }
    }



    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        clock_t currentTime = clock();
        double elapsedTime = static_cast<double>(currentTime - lastKeyPressTime) / CLOCKS_PER_SEC;

        if (!speedKeyPressed && elapsedTime > 0.2) {
            speedKeyPressed = true;  // Set the flag to true to indicate key press
            lastKeyPressTime = currentTime;  // Update the last key press time
            camOrientation = glm::vec3(camOrientation.x, camOrientation.y + 0.1, camOrientation.z);
            view = glm::lookAt(camPosition, camOrientation, camRotation); // lookAt(Gdje je kamera, u sta kamera gleda, jedinicni vektor pozitivne Y ose svijeta  - ovo rotira kameru)
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        }
    }


    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        clock_t currentTime = clock();
        double elapsedTime = static_cast<double>(currentTime - lastKeyPressTime) / CLOCKS_PER_SEC;

        if (!speedKeyPressed && elapsedTime > 0.2) {
            speedKeyPressed = true;  // Set the flag to true to indicate key press
            lastKeyPressTime = currentTime;  // Update the last key press time
            camOrientation = glm::vec3(camOrientation.x, camOrientation.y - 0.1, camOrientation.z);
            view = glm::lookAt(camPosition, camOrientation, camRotation); // lookAt(Gdje je kamera, u sta kamera gleda, jedinicni vektor pozitivne Y ose svijeta  - ovo rotira kameru)
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        }
    }


    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        clock_t currentTime = clock();
        double elapsedTime = static_cast<double>(currentTime - lastKeyPressTime) / CLOCKS_PER_SEC;

        if (!speedKeyPressed && elapsedTime > 0.2) {
            speedKeyPressed = true;  // Set the flag to true to indicate key press
            lastKeyPressTime = currentTime;  // Update the last key press time
            camRotation = glm::vec3(camRotation.x + 0.1, camRotation.y, camRotation.z);
            view = glm::lookAt(camPosition, camOrientation, camRotation); // lookAt(Gdje je kamera, u sta kamera gleda, jedinicni vektor pozitivne Y ose svijeta  - ovo rotira kameru)
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        }
    }



    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        clock_t currentTime = clock();
        double elapsedTime = static_cast<double>(currentTime - lastKeyPressTime) / CLOCKS_PER_SEC;

        if (!speedKeyPressed && elapsedTime > 0.2) {
            speedKeyPressed = true;  // Set the flag to true to indicate key press
            lastKeyPressTime = currentTime;  // Update the last key press time
            camRotation = glm::vec3(camRotation.x - 0.1, camRotation.y, camRotation.z);
            view = glm::lookAt(camPosition, camOrientation, camRotation); // lookAt(Gdje je kamera, u sta kamera gleda, jedinicni vektor pozitivne Y ose svijeta  - ovo rotira kameru)
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            std::cout << camRotation.x << std::endl;
        }
    }





    // tasters
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

        if (!speedKeyPressed && elapsedTime > 0.1) {
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

        if (!speedKeyPressed && elapsedTime > 0.1) {
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
    //Testiranje dubine
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS)
    {
        glEnable(GL_DEPTH_TEST); //Ukljucivanje testiranja Z bafera
    }
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS)
    {
        glDisable(GL_DEPTH_TEST);
    }

    //Odstranjivanje lica (Prethodno smo podesili koje lice uklanjamo sa glCullFace)
    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_7) == GLFW_PRESS)
    {
        glEnable(GL_CULL_FACE);
    }
    if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS)
    {
        glDisable(GL_CULL_FACE);
    }

    //Mijenjanje projekcija
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionP));
    }
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    {
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionO));
    }
}

