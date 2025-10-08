
int main(int argc, char* argv[]) {
    neu::file::SetCurrentDirectory("Assets");
    LOG_INFO("current directory {}", neu::file::GetCurrentDirectory());

    // initialize engine
    LOG_INFO("initialize engine...");
    neu::GetEngine().Initialize();

    // initialize scene


    SDL_Event e;
    bool quit = false;
    float angle = 0;
    float scale = 1;

    // MAIN LOOP
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }

        // update
        neu::GetEngine().Update();

        if (neu::GetEngine().GetInput().GetKeyPressed(SDL_SCANCODE_ESCAPE)) quit = true;

        // draw
        neu::vec3 color{ 0, 0, 0 };
        neu::GetEngine().GetRenderer().SetColor(color.r, color.g, color.b);
        neu::GetEngine().GetRenderer().Clear();

        std::vector<neu::vec3> points{ {0.5f, 0.5f, 0}, {0.5f, -0.5f, 0}, {-0.5f, 0, 0} };
        std::vector<neu::vec3> colors{ {1,1,0}, {1,0,1}, {0,1,1} };
        // draw
        glLoadIdentity();
        glPushMatrix();

        glTranslatef(angle, 0, 0);
        glScalef(scale, scale, 0);

        glBegin(GL_TRIANGLES);



        glEnd();


        neu::GetEngine().GetRenderer().Present();
    }

    neu::GetEngine().Shutdown();

    return 0;
}
