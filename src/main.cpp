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
const int RESX = 800;
const int RESY = 800;

GLFWwindow* window;
bool closed = false;
ParticleCluster* particles;
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

void cursorCallback(GLFWwindow* window, double xpos, double ypos) {
	float x = float(xpos);
	float y = float(ypos);

	particles->setOrigin(2.f * x / RESX - 1.f, -2.f * y / RESY + 1.f);
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
	particles = new ParticleCluster(2000);
    rng::srand(122234);
    vector<Asteroid*> asteroids;
    //for (int i = 0; i < 5; i++) {
    //    asteroids.push_back(new Asteroid());
    //}
    player = new Player();

	while (!glfwWindowShouldClose(window) && !closed) {
		glClear(GL_COLOR_BUFFER_BIT);

		//particles->compute();
		//particles->render();

        for (auto it = asteroids.begin(); it != asteroids.end(); ++it) {
            player->collide(*it);
            player->gravitate(*it);
            for (auto it2 = it + 1; it2 != asteroids.end(); ++it2) {
                (*it)->gravitate(*it2);
                (*it)->collide(*it2);
            }
        }

        for (Asteroid* a : asteroids) {
            a->move();
            a->render(player->pos);
        }

        player->move();
        player->render(player->pos);

		glfwSwapBuffers(window);
		glfwPollEvents();
        //asteroid cleanup
        for (auto it = asteroids.begin(); it != asteroids.end();) {
            Asteroid* a = *it;
            if (length(a->pos - player->pos) > 7.0f || a->HP <= 0) {
                it = asteroids.erase(it);
            } 
            else {
                ++it;
            }
        }
        ////asteroid spawn
        while (asteroids.size() < 5) {
            asteroids.push_back(new Asteroid(player->pos));
        }
	}

	glfwTerminate();
	return 0;
}