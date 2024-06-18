#include "planar_quadrotor_visualizer.h"

PlanarQuadrotorVisualizer::PlanarQuadrotorVisualizer(PlanarQuadrotor *quadrotor_ptr): quadrotor_ptr(quadrotor_ptr) {}

/**
 * TODO: Improve visualizetion
 * 1. Transform coordinates from quadrotor frame to image frame so the circle is in the middle of the screen
 * 2. Use more shapes to represent quadrotor (e.x. try replicate http://underactuated.mit.edu/acrobot.html#section3 or do something prettier)
 * 3. Animate proppelers
 */
void drawBody(SDL_Renderer* renderer, float x, float y, float width, float height, float angle, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    Eigen::Matrix2f rotation;
    rotation << std::cos(angle), -std::sin(angle),
                std::sin(angle),  std::cos(angle);

    Eigen::Vector2f points[4] = {
        {-width / 2, -height / 2},
        { width / 2, -height / 2},
        { width / 2,  height / 2},
        {-width / 2,  height / 2}
    };

    for (int i = 0; i < 4; ++i) {
        points[i] = rotation * points[i];
        points[i][0] += x;
        points[i][1] += y;
    }

    for (int i = 0; i < 4; ++i) {
        SDL_RenderDrawLine(renderer, points[i][0], points[i][1], points[(i + 1) % 4][0], points[(i + 1) % 4][1]);
    }
}

void PlanarQuadrotorVisualizer::render(std::shared_ptr<SDL_Renderer> &gRenderer) {
    Eigen::VectorXf state = quadrotor_ptr->GetState();
    float q_x, q_y, q_theta;

    /* x, y, theta coordinates */
    q_x = state[0];
    q_y = state[1];
    q_theta = state[2];

    float cen_x = q_x + 640;
    float cen_y = 360 - q_y;

    drawBody(gRenderer.get(), cen_x, cen_y, 80, 10, q_theta, {255, 0, 0, 255});



    drawBody(gRenderer.get(), cen_x - 35 * std::cos(q_theta), cen_y - 35 * std::sin(q_theta) - 5, 1, 15, q_theta, {255, 0, 0, 255});
    drawBody(gRenderer.get(), cen_x + 35 * std::cos(q_theta), cen_y + 35 * std::sin(q_theta) - 5, 1, 15, q_theta, {255, 0, 0, 255});
}

