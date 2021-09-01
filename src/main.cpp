#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS
#include <iostream>
#include <GL/glew.h>
#include <glm/ext.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/quaternion.hpp>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

#include "particles.h"
#include "random.h"
#include "player.h"
#include "asteroid.h"

using namespace std;
using namespace glm;

//resolution of window in pixels
const int RESX = 900;
const int RESY = 900;

GLFWwindow* window;
bool closed = false;
Player* player;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch(key) {
        case GLFW_KEY_SPACE:
	    	if (action == GLFW_PRESS) particles->toggleGravity();
            break;
	    case GLFW_KEY_ESCAPE:
            closed = true;
            break;
        case GLFW_KEY_LEFT:
            if (action == GLFW_PRESS) player->keyLeft = true;
            else if (action == GLFW_RELEASE) player->keyLeft = false;
            break;
        case GLFW_KEY_RIGHT:
            if (action == GLFW_PRESS) player->keyRight = true;
            else if (action == GLFW_RELEASE) player->keyRight = false;
            break;
        case GLFW_KEY_UP:
            if (action == GLFW_PRESS) player->keyUp = true;
            else if (action == GLFW_RELEASE) player->keyUp = false;
            break;
        case GLFW_KEY_DOWN:
            if (action == GLFW_PRESS) player->keyDown = true;
            else if (action == GLFW_RELEASE) player->keyDown = false;
            break;
        default:
            break;
    }
}

int init() {
	// Initialise GLFW
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(RESX, RESY, "Test", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	// Initialize GLEW
	//glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	//don't need depth test for 2d rendering
	glDisable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	//width of the particles
	glLineWidth(3.f);

	int error = glGetError();

	if (error != GL_NO_ERROR) {
		cout << "OpenGL error: " << error << "\n";
	}

	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, cursorCallback);

	return 0;
}

int main(void) {
	if (init() == -1) return -1;
	ParticleCluster::loadPrograms();
	//argument is number of particles
    rng::srand(1222334);
    vector<Asteroid*> asteroids;
    //for (int i = 0; i < 5; i++) {
    //    asteroids.push_back(new Asteroid());
    //}
    vec2 camera(5.0f);
    player = new Player();

	while (!glfwWindowShouldClose(window) && !closed) {
		glClear(GL_COLOR_BUFFER_BIT);

        for (auto it = asteroids.begin(); it != asteroids.end(); ++it) {
            player->collide(*it);
            //player->gravitate(*it);
            for (auto it2 = it + 1; it2 != asteroids.end(); ++it2) {
                //(*it)->gravitate(*it2);
                (*it)->collide(*it2);
            }
        }

        for (Asteroid* a : asteroids) {
            a->move();
            a->render(camera);
        }
        player->move();
        
        //camera += 0.001f * (player->pos - camera);
        player->render(camera);

		glfwSwapBuffers(window);
		glfwPollEvents();
        //asteroid cleanup
        for (auto it = asteroids.begin(); it != asteroids.end();) {
            Asteroid* a = *it;
            //a->vel -= player->vel;
            if (a->HP <= 0) {
                if (a->points.size() > 7) {
                    Asteroid *a1, *a2;
                    a->split(&a1, &a2);
                    asteroids.erase(it);
                    //it = asteroids.begin();
                    asteroids.push_back(a1);
                    asteroids.push_back(a2);
                    break;
                }
                else {
                    it = asteroids.erase(it);
                }
            } 
            else {
                ++it;
            }
        }
        //player->vel = vec2(0.0f);
        ////asteroid spawn
        while (asteroids.size() < 5) {
            asteroids.push_back(new Asteroid(player->pos));
        }
	}

	glfwTerminate();
	return 0;
}