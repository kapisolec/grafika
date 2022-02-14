#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION // dodane do pliku z kursu 
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("7.4.camera.vert", "7.4.camera.frag");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = { //TODO: COLORS
        // positions          // colors        
         /* HOUSE */
         0.5f,  0.5f, 0.0f,  0.3f, 0.3f, 0.6f,  // 0 top right
         0.5f, -0.5f, 0.0f,  0.3f, 0.3f, 0.5f,  // 1 bottom right
        -0.5f, -0.5f, 0.0f,  0.3f, 0.3f, 0.5f,  // 2 bottom left
        -0.5f,  0.5f, 0.0f,  0.3f, 0.3f, 0.6f,  // 3 top left 
         0.5f,  0.5f, -2.0f,  0.4f, 0.4f, 0.6f, // 4 top right Z=1
         0.5f, -0.5f, -2.0f,  0.4f, 0.4f, 0.5f, // 5 bottom right Z=1
        -0.5f, -0.5f, -2.0f,  0.4f, 0.4f, 0.5f, // 6 bottom left Z=1
        -0.5f,  0.5f, -2.0f,  0.4f, 0.4f, 0.6f, // 7 top left Z=1
         0.0f,  1.0f, 0.0f,  0.1f, 0.0f, 0.0f,  // 8 top middle z=0
         0.0f,  1.0f, -2.0f,  0.1f, 0.0f, 0.0f, // 9 top middle z=1

         /* DOORS OUTER WALL*/
         0.5f,  0.0f, 0.0f,  0.4f, 0.4f, 0.5f,     // 10 middle right
         -0.5f,  0.0f, 0.0f,  0.4f, 0.4f, 0.5f,    // 11 middle left 
         0.125f,  0.0f, 0.0f,  0.4f, 0.4f, 0.5f,   // 12 middle centre right
         -0.125f,  0.0f, 0.0f, 0.4f, 0.4f, 0.5f,  // 13 middle centre left
         0.125f,  -0.5f, 0.0f,  0.4f, 0.4f, 0.5f,  // 14 bottom centre right
         -0.125f,  -0.5f, 0.0f,  0.4f, 0.4f, 0.5f, // 15 bottom centre left

         /* DOORS ITSELF */
         0.125f,  0.0f, 0.0f,  1.0f, 1.0f, 0.0f,   // 16 middle centre right
         -0.125f,  0.0f, 0.0f,  1.0f, 1.0f, 0.0f,  // 17 middle centre left
         0.125f,  -0.5f, 0.0f,  1.0f, 1.0f, 0.0f,  // 18 bottom centre right
         -0.125f,  -0.5f, 0.0f,  1.0f, 1.0f, 0.0f, // 19 bottom centre left

         /* WALL BENEATH ROOF */
         0.5f,  0.5f, 0.0f,  0.7f, 0.7f, 0.5f,   // 20 top right
         -0.5f,  0.5f, 0.0f,  0.7f, 0.7f, 0.5f,  // 21 top left 
         0.5f,  0.5f, -2.0f,  0.7f, 0.7f, 0.5f,  // 22 top right Z=1
         -0.5f,  0.5f, -2.0f,  0.7f, 0.7f, 0.5f, // 23 top left Z=1
         0.0f,  1.0f, 0.0f,  0.7f, 0.7f, 0.5f,   // 24 top middle z=0
         0.0f,  1.0f, -2.0f,  0.7f, 0.7f, 0.5f,  // 25 top middle z=1

         /*ROOF SQUARES*/
         0.6f,  0.4f, 0.1f,  0.3f, 0.0f, 0.0f,   // 26 top right
         -0.6f,  0.4f, 0.1f,  0.3f, 0.0f, 0.0f,  // 27 top left 
         0.6f,  0.4f, -2.1f,  0.3f, 0.0f, 0.0f,  // 28 top right Z=1
         -0.6f,  0.4f, -2.1f,  0.3f, 0.0f, 0.0f, // 29 top left Z=1
         0.0f,  1.0f, 0.1f,  0.3f, 0.0f, 0.0f,   // 30 top middle z=0
         0.0f,  1.0f, -2.1f,  0.3f, 0.0f, 0.0f,  // 31 top middle z=1
    };

    unsigned int indices[] = {
        /*HOUSE*/
        0, 1, 5,
        5, 4, 0,
        1, 2, 6,
        6, 5, 1,
        3, 2, 6,
        6, 7, 3,
        5, 6, 7,
        7, 4, 5,

        /*DOORS HOLE*/
        14,12,10,
        10, 1, 14,
        2, 11,13,
        13, 15, 2,
        11, 3, 0,
        0, 10, 11,

        /*DOORS ITSELF*/
        /*16, 18, 19,
        19, 17, 16,*/

        /* WALL BENEATH ROOF */
        20, 21, 24,
        22, 25, 23,

        /*ROOF SQUARES*/
        26, 30, 31,
        31, 28, 26,
        27, 30, 31,
        31, 29, 27,
        
    };

    float verticesD[] = {
        /* DOORS ITSELF */
         0.125f,  0.0f, 0.0f,  1.0f, 1.0f, 0.0f,   // 0/16 middle centre right
         -0.125f,  0.0f, 0.0f,  1.0f, 1.0f, 0.0f,  // 1/17 middle centre left
         0.125f,  -0.5f, 0.0f,  1.0f, 1.0f, 0.0f,  // 2/18 bottom centre right
         -0.125f,  -0.5f, 0.0f,  1.0f, 1.0f, 0.0f, // 3/19 bottom centre left
    };

    unsigned int indicesD[] = {
        /*DOORS ITSELF*/
        0, 2, 3,
        3, 1, 0,
    };


    unsigned int VBO, VAO, EBO, VBO2, VAO2, EBO2;
    /* FIRST BUFFER */
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    
    /* SECOND BUFFER */
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);
    glGenBuffers(1, &EBO2);
    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);

    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesD), verticesD, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesD), indicesD, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // render loop
    // -----------
    glm::mat4 model2 = glm::mat4(1.0f);
    bool clicked = false;

    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        // activate shader
        ourShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);

       // glm::mat4 rotationMatrix = glm::rotate(glm::mat4(), 54.0f, glm::vec3(1.0f, 0.0f, 0.0f));

        // render boxes
       // calculate the model matrix for each object and pass it to shader before drawing
        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        /*
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        */
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, sizeof(vertices) / sizeof(*vertices), GL_UNSIGNED_INT, nullptr);

        // create transformations
        glm::mat4 view2 = glm::mat4(1.0f);
        glm::vec3 translationMatrix = glm::vec3(1.0f);

        glm::mat4 projection2 = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //
        
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && !clicked) {
            clicked = true;
            model2 = glm::translate(model2, glm::vec3(-0.125f, 0.0f, 0.0f)) * glm::rotate(model2, -1.8f, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::translate(model2, glm::vec3(0.125f, 0.0f, 0.0f));
        };
        
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && clicked) {
            model2 = glm::mat4(1.0f);
            clicked = false;
        };

        //view2 = glm::rotate(model2, (float)glfwGetTime(), glm::vec3(0.0f, 0.1f, 0.0f));
        //view2 = glm::translate(view2, glm::vec3(0.0f, 0.0f, 0.0f));
        //projection2 = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        // 
        // retrieve the matrix uniform locations
        unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
        //unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");
        // 
        // pass them to the shaders (3 different ways)
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));
        //glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view2[0][0]);
        // 
        // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        ourShader.setMat4("projection", projection2);

        glBindVertexArray(VAO2);
        glDrawElements(GL_TRIANGLES, sizeof(verticesD) / sizeof(*verticesD), GL_UNSIGNED_INT, nullptr);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
        
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}